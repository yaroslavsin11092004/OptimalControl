#include "TasksApiRpc.h"
TasksApiRpc::TasksApiRpc(std::string& conf_file)
{
	std::ifstream file(conf_file);
	if (file.is_open())
	{
		std::string buffer = { std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };
		file.close();
		json conf_data = json::parse(buffer);
		std::string target = absl::StrCat(conf_data["TasksServer"]["host"].get<std::string>(), ":", conf_data["TasksServer"]["port"].get<std::string>());
		int thread_num = conf_data["CoreServer"]["tasks_api_worker_threads"].get<int>();
		pool = std::make_unique<net::thread_pool>(thread_num);
		std::shared_ptr<grpc::Channel> channel = grpc::CreateChannel(std::move(target), grpc::InsecureChannelCredentials());
		stub = tasks_api::TasksApiService::NewStub(channel);
		cq = std::make_shared<grpc::CompletionQueue>();
		ioc = std::make_shared<net::io_context>();
		guard.emplace(net::make_work_guard(*ioc));
		for (int i = 0; i < thread_num; i++)
			net::post(*pool, [this]{ioc->run();});
		cq_thread = std::thread([this]{run_completion_queue();});
	}
	else 
	{
		file.close();
		throw std::runtime_error("Error of open config file");
	}
}
TasksApiRpc::~TasksApiRpc()
{
	shutdown = true;
	cq->Shutdown();
	if (cq_thread.joinable())
		cq_thread.join();
	guard->reset();
	ioc->stop();
	pool->stop();
	pool->join();
}
void TasksApiRpc::run_completion_queue()
{
	while(!shutdown)
	{
		void* tag;
		bool ok = false;
		auto deadline = std::chrono::system_clock::now() + std::chrono::milliseconds(100);
		grpc::CompletionQueue::NextStatus status = cq->AsyncNext(&tag, &ok, deadline);
		if (status == grpc::CompletionQueue::NextStatus::GOT_EVENT)
		{
			auto* operation = static_cast<std::function<void(bool)>*>(tag);
			(*operation)(ok);
			delete operation;
		}
		else if (status == grpc::CompletionQueue::NextStatus::SHUTDOWN)
			break;
	}
}
net::awaitable<double> TasksApiRpc::call_equation_async(int id, double arg, matrix<double> params)
{
	auto state = std::make_shared<AsyncCallStateEquation>();	
	state->request.set_id(id);
	state->request.set_arg(arg);
	for (auto val : (*params.store_handle())) state->request.add_params(val);
	state->request.set_size_row(params.size_row());
	state->request.set_size_col(params.size_col());
	auto result_future = state->promise.get_future();
	auto* callback = new std::function<void(bool)>([state](bool ok) mutable 
	{
		if (ok && state->status.ok())
			state->promise.set_value(state->response.result());
		else 
			state->promise.set_exception(std::make_exception_ptr(std::runtime_error(state->status.error_message())));
	});
	state->reader = stub->AsyncCallEquation(&state->context, state->request, cq.get());
	state->reader->Finish(&state->response, &state->status, callback);
	co_await net::dispatch(net::make_strand(*ioc), net::use_awaitable);
	result_future.wait();
	co_return result_future.get();
}
net::awaitable<double> TasksApiRpc::call_linked_async(int id, double arg, matrix<double> params)
{
	auto state = std::make_shared<AsyncCallStateLinked>();
	state->request.set_id(id);
	state->request.set_arg(arg);
	for (auto val : (*params.store_handle())) state->request.add_params(val);
	state->request.set_size_row(params.size_row());
	state->request.set_size_col(params.size_col());
	auto result_future = state->promise.get_future();
	auto* callback = new std::function<void(bool)>([state](bool ok) mutable 
	{
		if (state->status.ok() && ok)
			state->promise.set_value(state->response.result());
		else 
			state->promise.set_exception(std::make_exception_ptr(std::runtime_error(state->status.error_message())));
	});
	state->reader = stub->AsyncCallLinked(&state->context, state->request, cq.get());
	state->reader->Finish(&state->response, &state->status, callback);
	co_await net::dispatch(net::make_strand(*ioc), net::use_awaitable);
	result_future.wait();
	co_return result_future.get();
}
net::awaitable<double> TasksApiRpc::call_param_s_async(matrix<double>& x, matrix<double>& u, matrix<double>& optim_u)
{
	auto state = std::make_shared<AsyncCallStateParamS>();
	for (auto val : (*x.store_handle())) state->request.add_x(val);
	for (auto val : (*u.store_handle())) state->request.add_u(val);
	for (auto val : (*optim_u.store_handle())) state->request.add_optim_u(val);
	state->request.set_x_size_row(x.size_row());
	state->request.set_x_size_col(x.size_col());
	state->request.set_u_size_row(u.size_row());
	state->request.set_u_size_col(u.size_col());
	state->request.set_optim_u_size_row(optim_u.size_row());
	state->request.set_optim_u_size_col(optim_u.size_col());
	auto result_future = state->promise.get_future();
	auto* callback = new std::function<void(bool)>([state](bool ok)
	{
		if (state->status.ok() && ok)
			state->promise.set_value(state->response.result());
		else 
			state->promise.set_exception(std::make_exception_ptr(std::runtime_error(state->status.error_message())));
	});
	state->reader = stub->AsyncCallParamS(&state->context, state->request, cq.get());
	state->reader->Finish(&state->response, &state->status, callback);
	co_await net::dispatch(net::make_strand(*ioc), net::use_awaitable);
	result_future.wait();
	co_return result_future.get();
}
std::shared_ptr<std::vector<Equation>> TasksApiRpc::generate_equations(int dim)
{
	try 
	{
		auto result = std::make_shared<std::vector<Equation>>();
		for (int i = 0; i < dim; i++)
		{
			result->push_back(
			FunctionWrapper<double(double,matrix<double>)>{[self = shared_from_this(), i](double arg, matrix<double> params)
			{
				std::promise<double> result_promise;
				auto result_future = result_promise.get_future();
				net::co_spawn(*self->ioc, 
				[self, result_promise = std::move(result_promise), arg, params,i]() mutable -> net::awaitable<void>
				{
					try 
					{
						double res = co_await self->call_equation_async(i, arg, params);
						result_promise.set_value(res);
					}
					catch(...)
					{
						result_promise.set_exception(std::current_exception());
					}
				},
				net::detached);
				return result_future.get();
			}});
		}
		return result;
	}
	catch(const std::exception& e)
	{
		throw std::runtime_error(e.what());
	}
}
std::shared_ptr<std::vector<Equation>> TasksApiRpc::generate_linked(int dim)
{
	try 
	{
		auto result = std::make_shared<std::vector<Equation>>();
		for (int i = 0; i < dim; i++)
		{
			result->push_back(
			FunctionWrapper<double(double, matrix<double>)>{[self = shared_from_this(),i](double arg, matrix<double> params) 
			{
				std::promise<double> result_promise;
				auto result_future = result_promise.get_future();
				net::co_spawn(*self->ioc,
				[self, result_promise = std::move(result_promise), arg, params, i]() mutable -> net::awaitable<void>
				{
					try 
					{
						double res = co_await self->call_linked_async(i, arg, params);
						result_promise.set_value(res);
					}
					catch(...)
					{
						result_promise.set_exception(std::current_exception());
					}
				},
				net::detached);
				return result_future.get();
			}});
		}
		return result;
	}
	catch(const std::exception& e)
	{
		throw std::runtime_error(e.what());
	}
}
double TasksApiRpc::param_s(matrix<double>& x, matrix<double>& u, matrix<double>& optim_u)
{
	try 
	{
		std::promise<double> result_promise;
		auto result_future = result_promise.get_future();
		net::co_spawn(*ioc, 
		[self = shared_from_this(), result_promise = std::move(result_promise), x,u,optim_u]() mutable -> net::awaitable<void>
		{
			try 
			{
				double res = co_await self->call_param_s_async(x,u,optim_u);
				result_promise.set_value(res);
			}
			catch(...)
			{
				result_promise.set_exception(std::current_exception());
			}
		},
		net::detached);
		return result_future.get();
	}
	catch(const std::exception& e)
	{
		throw std::runtime_error(e.what());
	}
}

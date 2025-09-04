#include "AdamApiRpc.h"
AdamApiRpc::AdamApiRpc(std::string& conf_file)
{
	std::ifstream file(conf_file);
	if (file.is_open())
	{
		std::string buffer = { std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };
		file.close();
		json conf_data = json::parse(buffer);
		std::string target = absl::StrCat(conf_data["AdamServer"]["host"].get<std::string>(), ":", conf_data["AdamServer"]["port"].get<std::string>());
		auto channel = grpc::CreateChannel(target, grpc::InsecureChannelCredentials());
		stub = adam_api::AdamApiService::NewStub(channel);
		cq = std::make_shared<grpc::CompletionQueue>();
		ioc = std::make_shared<net::io_context>();
		guard.emplace(net::make_work_guard(*ioc));
		for (int i = 0; i < 4; i++)
			net::post(pool, [this](){ioc->run();});
		cq_thread = std::thread([this]{run_completion_queue();});
	}
	else 
	{
		file.close();
		throw std::runtime_error("Error of open config file");
	}
}
AdamApiRpc::~AdamApiRpc()
{
	shutdown = true;
	cq->Shutdown();
	if (cq_thread.joinable())
		cq_thread.join();
	guard.reset();
	ioc->stop();
	pool.stop();
	pool.join();
}
void AdamApiRpc::run_completion_queue()
{
	while(!shutdown)
	{
		void* tag;
		bool ok = false;
		auto deadline = std::chrono::system_clock::now() + std::chrono::milliseconds(100);
		grpc::CompletionQueue::NextStatus status = cq->AsyncNext(&tag, &ok, deadline);
		if (status == grpc::CompletionQueue::NextStatus::GOT_EVENT)
		{
			auto* operation = static_cast<std::function<void(bool)>*> (tag);
			(*operation)(ok);
			delete operation;
		}
		else if (status == grpc::CompletionQueue::NextStatus::SHUTDOWN)
			break;
	}
}
net::awaitable<void> AdamApiRpc::set_global_param_async(double learning_rate, double epsilon, int epochs)
{
	auto state = std::make_shared<AsyncCallStateParams>();
	state->request.set_learning_rate(learning_rate);
	state->request.set_epsilon(epsilon);
	state->request.set_epochs(epochs);
	auto* callback = new std::function<void(bool)>([state](bool ok) mutable 
	{
		if (ok && state->status.ok())
			state->promise.set_value();
		else 
			state->promise.set_exception(std::make_exception_ptr(std::runtime_error(state->status.error_message())));
	});
	state->reader = stub->AsyncSetGlobalParams(&state->context, state->request, cq.get());
	state->reader->Finish(&state->response, &state->status, callback);
	std::future<void> result_future = state->promise.get_future();
	co_await net::dispatch(net::make_strand(*ioc),net::use_awaitable);
	result_future.wait();
	result_future.get();
	co_return;
}
net::awaitable<std::vector<double>> AdamApiRpc::adam_async(std::vector<double>& left_edge, std::vector<double>& right_edge, std::vector<double> params)
{
	auto state = std::make_shared<AsyncCallStateAdam>();
	for (double val : left_edge) state->request.add_left_edge(val);
	for (double val : right_edge) state->request.add_right_edge(val);
	for (double val : params) state->request.add_params(val);
	auto* callback = new std::function<void(bool)>([state](bool ok) mutable
	{
		if (ok && state->status.ok())
		{
			std::vector<double> result;
			for (int i = 0; i < state->response.result_size(); i++)
				result.push_back(state->response.result(i));
			state->promise.set_value(std::move(result));
		}
		else 
			state->promise.set_exception(std::make_exception_ptr(std::runtime_error(state->status.error_message())));
	});
	state->reader = stub->AsyncOptimize(&state->context, state->request, cq.get());
	state->reader->Finish(&state->response, &state->status, callback);
	auto result_future = state->promise.get_future();
	co_await net::dispatch(net::make_strand(*ioc),net::use_awaitable);
	result_future.wait();
	co_return result_future.get();
}
void AdamApiRpc::set_global_param(double learning_rate, double epsilon, int epochs)
{
	try 
	{
		std::promise<void> result_promise;
		auto result_future = result_promise.get_future();
		net::co_spawn(*ioc,
		[self = shared_from_this(), result_promise = std::move(result_promise), learning_rate, epsilon, epochs]() mutable -> net::awaitable<void>
		{
			try 
			{
				co_await self->set_global_param_async(learning_rate, epsilon,epochs);
				result_promise.set_value();
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
std::vector<double> AdamApiRpc::adam(std::vector<double>& left_edge, std::vector<double>& right_edge, std::vector<double> params)
{
	try 
	{
		std::promise<std::vector<double>> result_promise;
		auto result_future = result_promise.get_future();
		net::co_spawn(*ioc, 
		[self = shared_from_this(), result_promise = std::move(result_promise), left_edge, right_edge, params = std::move(params)]() mutable -> net::awaitable<void>
		{
			try 
			{
				auto result = co_await self->adam_async(left_edge, right_edge, std::move(params));
				result_promise.set_value(std::move(result));
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

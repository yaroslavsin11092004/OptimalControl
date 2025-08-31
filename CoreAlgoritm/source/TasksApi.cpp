#include "TasksApi.h"
TasksApi::TasksApi(std::reference_wrapper<std::string> conf_file)
{
	std::ifstream file(conf_file.get());
	if (file.is_open())
	{
		std::string buffer = { std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };
		file.close();
		json conf_json = json::parse(buffer);
		this->host = conf_json["TasksServer"]["host"].get<std::string>();
		this->port = conf_json["TasksServer"]["port"].get<std::string>();
		this->ioc = std::make_shared<net::io_context>();
		this->guard.emplace(net::make_work_guard(*this->ioc));
		for (int i = 0; i < 4; i++)
			net::post(this->pool, [self = shared_from_this()](){self->ioc->run();});
	}
	else 
	{
		file.close();
		throw std::runtime_error("Error of open config file");
	}
}
TasksApi::~TasksApi()
{
	this->guard.reset();
	this->ioc->stop();
	this->pool.stop();
	this->pool.join();
}
net::awaitable<double> TasksApi::call_equation(int id, double arg, matrix<double> params)
{
	try 
	{
		json req_json;
		req_json["id"] = id;
		req_json["arg"] = arg;
		req_json["params"] = *params.store_handle();
		req_json["size_row"] = params.size_row();
		req_json["size_col"] = params.size_col();
		auto resolver = tcp::resolver(co_await net::this_coro::executor);
		auto socket = tcp::socket(co_await net::this_coro::executor);
		auto endpoint = co_await resolver.async_resolve(this->host, this->port, net::use_awaitable);
		co_await net::async_connect(socket, endpoint, net::use_awaitable);
		Request req{http::verb::post, "/equation",11};
		req.set(http::field::content_type, "application/json");
		req.body() = req_json.dump();
		req.prepare_payload();
		co_await http::async_write(socket, req, net::use_awaitable);
		http::response<http::string_body> resp;
		beast::flat_buffer buffer;
		co_await http::async_read(socket, buffer, resp, net::use_awaitable);
		json resp_json = json::parse(resp.body());
		co_return resp_json["result"].get<double>();
	}
	catch(const std::exception& e)
	{
		throw std::runtime_error(e.what());
	}
}
net::awaitable<double> TasksApi::call_linked(int id, double arg, matrix<double> params)
{
	try 
	{
		json req_json;
		req_json["id"] = id;
		req_json["arg"] = arg;
		req_json["params"] = *params.store_handle();
		req_json["size_row"] = params.size_row();
		req_json["size_col"] = params.size_col();
		auto resolver = tcp::resolver(co_await net::this_coro::executor);
		auto socket = tcp::socket(co_await net::this_coro::executor);
		auto endpoint = co_await resolver.async_resolve(this->host, this->port, net::use_awaitable);
		co_await net::async_connect(socket, endpoint, net::use_awaitable);
		Request req{http::verb::post, "/linked", 11};
		req.set(http::field::content_type, "application/json");
		req.body() = req_json.dump();
		req.prepare_payload();
		co_await http::async_write(socket, req, net::use_awaitable);
		http::response<http::string_body> resp;
		beast::flat_buffer buffer;
		co_await http::async_read(socket, buffer, resp, net::use_awaitable);
		json resp_json = json::parse(resp.body());
		co_return resp_json["result"].get<double>();
	}
	catch(const std::exception& e)
	{
		throw std::runtime_error(e.what());
	}
}
std::shared_ptr<std::vector<Equation>> TasksApi::generate_equations(int dim)
{
	try 
	{
		auto ans = std::make_shared<std::vector<Equation>>(dim);
		for (int i = 0; i < dim; i++)
		{
			(*ans)[i] = [self = shared_from_this(), i](double arg, matrix<double> params) -> double 
			{
				std::promise<double> result_promise;
				auto result_future = result_promise.get_future();
				net::co_spawn(*self->ioc,
				[self, i, arg, params = std::move(params), result_promise = std::move(result_promise)]() mutable -> net::awaitable<void>
				{
					try 
					{
						double result = co_await self->call_equation(i, arg, std::move(params));
						result_promise.set_value(result);
					}
					catch(...)
					{
						result_promise.set_exception(std::current_exception());
					}
				},
				net::detached);
				return result_future.get();
			};
		}
		return ans;
	}
	catch(const std::exception& e)
	{
		throw std::runtime_error(e.what());
	}
}
std::shared_ptr<std::vector<Equation>> TasksApi::generate_linked(int dim)
{
	try 
	{
		auto ans = std::make_shared<std::vector<Equation>>(dim);
		for (int i = 0; i < dim; i++)
		{
			(*ans)[i] = [self = shared_from_this(), i](double arg, matrix<double> params) -> double 
			{
				std::promise<double> result_promise;
				auto result_future = result_promise.get_future();
				net::co_spawn(*self->ioc,
				[self, i, arg, params = std::move(params), result_promise = std::move(result_promise)]() mutable -> net::awaitable<void>
				{
					try 
					{
						double result = co_await self->call_linked(i, arg, std::move(params));
						result_promise.set_value(result);
					}
					catch(...)
					{
						result_promise.set_exception(std::current_exception());
					}
				},
				net::detached);
				return result_future.get();
			};
		}
		return ans;
	}
	catch(const std::exception& e)
	{
		throw std::runtime_error(e.what());
	}
}

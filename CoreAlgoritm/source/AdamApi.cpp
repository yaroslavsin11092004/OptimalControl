#include "AdamApi.h"
AdamApi::AdamApi(std::string& config_file)
{
	std::fstream file(config_file);
	if (file.is_open())
	{
		std::string buffer = { std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };
		file.close();
		json conf_json = json::parse(buffer);
		try 
		{
			auto self = shared_from_this();
			self->host = conf_json["AdamServer"]["host"].get<std::string>();
			self->port = conf_json["AdamServer"]["port"].get<std::string>();
			self->ioc = std::make_shared<net::io_context>();
			guard.emplace(net::make_work_guard(*self->ioc));
			for (int i = 0; i < 4; i++)
				net::post(self->pool, [self](){self->ioc->run();});
		}
		catch(const std::exception& e)
		{
			throw std::runtime_error("Error of parsing config file to json");
		}
	}
	else 
	{
		file.close();
		throw std::runtime_error("Error of open config file");
	}
}
AdamApi::~AdamApi()
{
	this->guard->reset();
	this->ioc->stop();
	this->pool.stop();
	this->pool.join();
}
net::awaitable<void> AdamApi::call_set_global_params(double learning_rate, double epsilon, int epochs)
{
	try 
	{
		json req_json;
		req_json["learning_rate"] = learning_rate;
		req_json["epsilon"] = epsilon;
		req_json["epochs"] = epochs;
		auto resolver = tcp::resolver(co_await net::this_coro::executor);
		auto socket = tcp::socket(co_await net::this_coro::executor);
		auto endpoint = co_await resolver.async_resolve(this->host, this->port, net::use_awaitable);
		co_await net::async_connect(socket, endpoint, net::use_awaitable);
		Request req{http::verb::post, "/set_params", 11};
		req.body() = req_json.dump();
		req.set(http::field::content_type, "application/json");
		req.prepare_payload();
		co_await http::async_write(socket, req, net::use_awaitable);
		co_return;
	}
	catch(const std::exception& e)
	{
		throw std::runtime_error(e.what());
	}
}
net::awaitable<std::vector<double>> AdamApi::call_adam(std::vector<double>& left_edge, std::vector<double>& right_edge, std::vector<double> params)
{
	try 
	{
		json req_json;
		req_json["left_edge"] = left_edge;
		req_json["right_edge"] = right_edge;
		req_json["params"] = params;
		auto resolver = tcp::resolver(co_await net::this_coro::executor);
		auto socket = tcp::socket(co_await net::this_coro::executor);
		auto endpoint = co_await resolver.async_resolve(this->host, this->port, net::use_awaitable);
		co_await net::async_connect(socket, endpoint, net::use_awaitable);
		Request req{http::verb::post, "/adam",11};
		req.body() = req_json.dump();
		req.set(http::field::content_type, "application/json");
		req.prepare_payload();
		co_await http::async_write(socket, req, net::use_awaitable);
		http::response<http::string_body> resp;
		beast::flat_buffer buffer;
		co_await http::async_read(socket, buffer, resp, net::use_awaitable);
		json resp_json = json::parse(resp.body());
		co_return resp_json["result"].get<std::vector<double>>();
	}
	catch(const std::exception& e)
	{
		throw std::runtime_error(e.what());
	}
}
void AdamApi::set_global_params(double learning_rate, double epsilon, int epochs)
{
	try 
	{
		std::promise<void> result_promise;
		auto result_future = result_promise.get_future();
		net::co_spawn(*this->ioc,
		[self = shared_from_this(), result_promise = std::move(result_promise), learning_rate, epsilon, epochs] mutable -> net::awaitable<void>
		{
			try 
			{
				co_await self->call_set_global_params(learning_rate, epsilon, epochs);
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
std::vector<double> AdamApi::adam(std::vector<double>& left_edge, std::vector<double>& right_edge, std::vector<double> params)
{
	try 
	{
		std::promise<std::vector<double>> result_promise;
		auto result_future = result_promise.get_future();
		net::co_spawn(*this->ioc, 
		[self = shared_from_this(), left_edge, right_edge, params = std::move(params), result_promise = std::move(result_promise)] mutable -> net::awaitable<void>
		{
			try 
			{
				std::vector<double> result = co_await self->call_adam(left_edge, right_edge, std::move(params));
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

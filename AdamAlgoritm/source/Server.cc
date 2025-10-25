#include "Server.h"
void HttpSession::read_request()
{
	parser.reset();
	parser.emplace();
	http::async_read(socket, buffer, *parser, 
	[self = shared_from_this()](beast::error_code ec, size_t)
	{
		if (!ec)
			self->handle_request();
	});
}
void HttpSession::handle_request()
{
	auto self = shared_from_this();
	auto& req = parser->get();
	Response resp;
	resp.keep_alive(req.keep_alive());
	std::cout << "Request:" << std::endl << req.base() << std::endl << "Body:" << std::endl << req.body() << std::endl;
	if (!self->router.handle_request(req, resp))
	{
		resp.result(http::status::not_found);
		beast::ostream(resp.body()) << "{\"result\" : \"request doesn't match for router\"}";
	}
	std::cout << "Response:" << std::endl << resp.base() << std::endl;
	self->send_response(std::make_shared<Response>(resp));
}
void HttpSession::send_response(std::shared_ptr<Response> resp)
{
	auto self = shared_from_this();
	http::async_write(self->socket, *resp,
	[self, resp](beast::error_code ec, size_t)
	{
		if (!resp->keep_alive())
			self->socket.shutdown(tcp::socket::shutdown_send, ec);
	});
}
void HttpServer::do_accepts()
{
	acceptor->async_accept(*socket, 
	[this](beast::error_code ec)
	{
		if (!ec)
			std::make_shared<HttpSession>(std::move(*this->socket), this->router)->start();
		if (ec)
			return;
		do_accepts();
	});
}
void HttpServer::init_routes()
{
	router.add_route(http::verb::post, "/set_params", 
	[this](Request& req, Response& resp, urls::url_view url)
	{
		try 
		{
			resp.set(http::field::content_type, "application/json");
			json req_json = json::parse(req.body());
			std::vector<double> left_edge = req_json["left_edge"].get<std::vector<double>>();
			std::vector<double> right_edge = req_json["right_edge"].get<std::vector<double>>();
			matrix<double> edges(left_edge.size(), 2);
			edges.set_column(0, std::move(left_edge));
			edges.set_column(1, std::move(right_edge));
			adam->set_edges(std::move(edges));
			adam->set_epochs(req_json["epochs"].get<int>());
			adam->set_learning_rate(req_json["learning_rate"].get<double>());
			beast::ostream(resp.body()) << "{\"result\" : \"success\"}";
			resp.result(http::status::ok);
		}
		catch(const std::exception& e)
		{
			resp.result(http::status::bad_request);
			beast::ostream(resp.body()) << "{\"result\" : \"" << e.what() << "\"}";
		}
	});
	router.add_route(http::verb::post, "/optimize_adam",
	[this](Request& req, Response& resp, urls::url_view url)
	{
		try 
		{
			resp.set(http::field::content_type, "application/json");
			json req_json = json::parse(req.body());
			std::vector<double> params = req_json["params"].get<std::vector<double>>();
			adam->set_params(std::move(params));
			auto result = adam->optimize();
			for (auto& i : result) std::cerr << i << " ";
			std::cerr << std::endl;
			json resp_json;
			resp_json["result"] = std::move(result);
			beast::ostream(resp.body()) << resp_json.dump();
			resp.result(http::status::ok);
		}
		catch(const std::exception& e)
		{
			std::cerr << "Error of optimize adam::" << e.what() << std::endl;
			resp.result(http::status::bad_request);
			beast::ostream(resp.body()) << "{\"result\" : \"" << e.what() << "\"}";
		}
	});
	router.add_route(http::verb::post, "/set_hamilton",
	[this](Request& req, Response& resp, urls::url_view url) {
		try {
			resp.set(http::field::content_type, "application/json");
			json req_json = json::parse(req.body());
			auto value = req_json["hamilton"].get<std::string>();
			auto dim = req_json["dim"].get<int>();
			adam->set_hamilton(std::move(value), dim);
			beast::ostream(resp.body()) << "{\"result\" : \"success\"}";
			resp.result(http::status::ok);
		} catch(const std::exception& e) {
			std::cerr << "Error of set hamilton request::" << e.what() << std::endl;
			resp.result(http::status::bad_request);
			beast::ostream(resp.body()) << "{\"result\" : \"" << e.what() << "\"}";
		}
	});
}
HttpServer::HttpServer(std::string& conf_file)
{
	std:: ifstream file(conf_file);
	if (file.is_open())
	{
		try 
		{
			std::string buffer = { std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };
			file.close();
			json conf_json = json::parse(buffer);
			host = conf_json["AdamServer"]["host"].get<std::string>();
			port = conf_json["AdamServer"]["port"].get<std::string>();
			int thread_num = conf_json["AdamServer"]["worker_threads"].get<int>();
			ioc = std::make_shared<net::io_context>();
			guard.emplace(net::make_work_guard(*ioc));
			socket = std::make_shared<tcp::socket>(*ioc);
			auto address = net::ip::make_address(host);
			auto ep = tcp::endpoint(address, std::stoi(port.c_str()));
			acceptor.emplace(tcp::acceptor(*ioc, ep));
			pool = std::make_unique<net::thread_pool>(thread_num);
			adam = std::make_shared<Adam>();
			for (int i = 0; i < thread_num; i++)
				net::post(*pool, [this](){ioc->run();});
		}
		catch(const std::exception& e)
		{
			throw std::runtime_error(e.what());
		}
	}
	else 
	{
		file.close();
		throw std::runtime_error("Error of open config file!");
	}
}
void HttpServer::run()
{
	std::cout << "Adam Server running at http://" << host << ":" << port << std::endl;
	init_routes();
	do_accepts();
	net::signal_set signals(*ioc, SIGINT, SIGTERM);
	signals.async_wait([&](auto, auto)
	{
		guard->reset();
		ioc->stop();
		pool->stop();
	});
	pool->join();
}

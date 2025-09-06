#include "Server.h"
void HttpSession::read_request()
{
	parser.reset();
	parser.emplace();
	http::async_read(this->socket, this->buffer, *this->parser,
	[self = shared_from_this()](beast::error_code ec,size_t)
	{
		if (!ec)
			self->handle_request();
	});
}
void HttpSession::handle_request()
{
	auto self = shared_from_this();
	auto& req = self->parser->get();
	Response resp;
	resp.keep_alive(req.keep_alive());
	std::cout << "Request:\n" << req.base() << std::endl << req.body() << std::endl;
	if (!self->router.handle_request(req, resp))
	{
		resp.result(http::status::not_found);
		resp.set(http::field::content_type, "application/json");
		beast::ostream(resp.body()) << "{\"result\" : \"this request doesn't match for router\"}";
	}
	std::cout << "Response:\n" << resp.base() << std::endl;
	self->send_response(std::make_shared<Response>(resp));
}
void HttpSession::send_response(std::shared_ptr<Response> resp)
{
	auto self = shared_from_this();
	http::async_write(socket, *resp, 
	[self,resp](beast::error_code ec, size_t)
	{
		if (!resp->keep_alive())	
			self->socket.shutdown(tcp::socket::shutdown_send,ec);
	});
}
HttpServer::HttpServer(std::string& conf_file)
{
	std::ifstream file(conf_file);
	if (file.is_open())
	{
		std::string data = { std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };
		file.close();
		this->ioc = std::make_shared<net::io_context>();
		this->guard.emplace(net::make_work_guard(*this->ioc));
		this->socket = std::make_shared<tcp::socket>(*this->ioc);
		json conf_json = json::parse(data);
		this->host = conf_json["CoreServer"]["host"].get<std::string>();
		this->port = conf_json["CoreServer"]["port"].get<std::string>();
		net::ip::address ip_address = net::ip::make_address(this->host);
		tcp::endpoint ep(ip_address, std::stoi(this->port.c_str()));
		this->acceptor.emplace(tcp::acceptor(*this->ioc, ep));
		this->optimal_control_api = std::make_shared<OptimalControl>(conf_file);
	}
	else 
	{
		file.close();
		throw std::runtime_error("Error of open config file");
	}
}
void HttpServer::run()
{
	std::cout << "Server running at http://" + this->host + ":" + this->port << std::endl;
	init_routes();
	do_accept();
	for (int i = 0; i < 4; i++)
		net::post(this->pool, [this](){ this->ioc->run(); });
	net::signal_set signals(*this->ioc, SIGINT, SIGTERM);
	signals.async_wait(
	[&](auto, auto)
	{
		this->guard->reset();
		this->ioc->stop();
	});
	this->pool.join();
}
void HttpServer::do_accept() {
	acceptor->async_accept(*this->socket, [this](beast::error_code ec) {
  	if (!ec)
    	std::make_shared<HttpSession>(std::move(*this->socket), this->router)->start();
		if (ec)
			return;
  		do_accept();
    });
}
void HttpServer::init_routes()
{
	this->router.add_route(http::verb::post, "/set_adam_params", 
	[this](Request& req, Response& resp, urls::url_view url)
	{
		try 
		{
			json req_json = json::parse(req.body());
			this->optimal_control_api->adam_params(req_json["learning_rate"].get<double>(), req_json["epsilon"].get<double>(), req_json["epochs"].get<int>());
			beast::ostream(resp.body()) << "{\"result\" : \"success\"}";
		}
		catch(const std::exception& e)
		{
			beast::ostream(resp.body()) << "\"{result\" : \"" << e.what() << "\"}";
		}
	});
	this->router.add_route(http::verb::post, "/successive_approx",
	[this](Request& req, Response& resp, urls::url_view url)
	{
		try 
		{
			json req_json = json::parse(req.body());
			auto x0 = req_json["x0"].get<std::vector<double>>();
			auto t0 = req_json["t0"].get<double>();
			auto t1 = req_json["t1"].get<double>();
			auto t_step = req_json["t_step"].get<double>();
			auto u_left = req_json["u_left"].get<std::vector<double>>();
			auto u_right = req_json["u_right"].get<std::vector<double>>();
			auto delta = req_json["delta"].get<double>();
			auto result = this->optimal_control_api->successive_approximation(std::move(x0), t0, t1, t_step, std::move(u_left), std::move(u_right), delta);
			json resp_json;
			resp_json["optim_path"] = *result.first.store_handle();
			resp_json["optim_control"] = *result.second.store_handle();
			resp_json["optim_path_size_row"] = result.first.size_row();
			resp_json["optim_path_size_col"] = result.first.size_col();
			resp_json["optim_control_size_row"] = result.second.size_row();
			resp_json["optim_control_size_col"] = result.second.size_col();
			beast::ostream(resp.body()) << resp_json.dump();
		}
		catch(const std::exception& e)
		{
			beast::ostream(resp.body()) << "{\"result\" : \"" << e.what() << "\"}";
		}
	});
}

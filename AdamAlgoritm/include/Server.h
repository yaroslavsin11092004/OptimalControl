#ifndef SERVER_H
#define SERVER_H
#include "Router.h"
#include "Adam.h"
class HttpSession : public std::enable_shared_from_this<HttpSession>
{
	private:
		tcp::socket socket;
		std::optional<http::request_parser<http::string_body>> parser;
		Router& router;
		beast::flat_buffer buffer;
		
		void read_request();
		void handle_request();
		void send_response(std::shared_ptr<Response> resp);
	public:
		HttpSession(tcp::socket socket, Router& router):
			socket(std::move(socket)), router(router) {}
		void start() { read_request(); }
};
class HttpServer 
{
	private:
		std::string host;
		std::string port;
		std::shared_ptr<net::io_context> ioc;
		std::optional<net::executor_work_guard<net::io_context::executor_type>> guard;
		std::unique_ptr<net::thread_pool> pool;
		std::optional<tcp::acceptor> acceptor;
		std::shared_ptr<tcp::socket> socket;
		Router router;
		std::shared_ptr<Adam> adam;
		void do_accepts();
		void init_routes();
	public:
		HttpServer(std::string& config_file);
		~HttpServer() = default;
		void run();
};
#endif

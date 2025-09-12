#ifndef ROUTER_H
#define ROUTER_H
#include "IncludeFiles.h"
class Router 
{
	protected:
		struct Route 
		{
			http::verb method;
			std::string pattern;
			Handler handler;
		};
	private:
		std::vector<Route> routes;
		std::optional<urls::url_view> parse_url(std::string_view target);
		std::string route_pattern_to_regex(std::string& pattern);
		bool match_route(std::string& pattern, urls::url_view url);
	public:
		Router() = default;
		~Router() = default;
		void add_route(http::verb method, std::string pattern, Handler handler);
		bool handle_request(Request& req, Response& resp);
};
#endif

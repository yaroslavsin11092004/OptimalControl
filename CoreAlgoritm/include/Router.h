#ifndef ROUTER_H
#define ROUTER_H
#include "IncludeFiles.h"
class Router 
{
	private:
		struct Route 
		{
			http::verb method;
			std::string pattern;
			Handler handler;
		};
		std::optional<urls::url_view> parse_url(std::string_view target);
		bool match_route(const std::string& pattern, urls::url_view url);
		std::string route_pattern_to_regex(const std::string& pattern);

		std::vector<Route> routes;
	public:
		Router() = default;
		~Router() = default;
		void add_route(http::verb method, std::string pattern, Handler handler);
		bool handle_request(Request& req, Response& resp);
};
#endif

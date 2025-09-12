#include "Router.h"
std::optional<urls::url_view> Router::parse_url(std::string_view target)
{
	if (target[0] != '/' || target.empty())
		return urls::parse_uri(target).value();
	auto result = urls::parse_uri("http://optim.com" + std::string(target));
	if (result.has_value())
		return result.value();
	else return std::nullopt;
}
std::string Router::route_pattern_to_regex(std::string& pattern)
{
	std::string result;
	bool is_param = false;
	for (char c : pattern)
	{
		if (c == '{')
		{
			is_param = true;
			result += "([^/]+)";
		}
		else if (c == '}')
			is_param = false;
		else if (!is_param)
		{
			if (c == '/')
				result += "\\/";
			else 
				result += c;
		}
	}
	return '^' + result + '$';
}
bool Router::match_route(std::string& pattern, urls::url_view url)
{
	std::regex re(pattern);
	return std::regex_match(url.path(), re);
}
void Router::add_route(http::verb method, std::string pattern, Handler handler)
{
	auto re = route_pattern_to_regex(pattern);
	routes.push_back({method, std::move(re), std::move(handler)});
}
bool Router::handle_request(Request& req, Response& resp)
{
	auto url = parse_url(req.target());
	if (!url)
	{
		resp.result(http::status::bad_request);
		return true;
	}
	for (auto route : routes)
	{
		if (req.method() == route.method && match_route(route.pattern, *url))
		{
			route.handler(req, resp, *url);
			return true;
		}
	}
	return false;
}

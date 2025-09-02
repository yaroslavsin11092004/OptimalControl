#include "Router.h"
std::optional<urls::url_view> Router::parse_url(std::string_view target)
{
	if (target.empty() || target[0] != '/')
		return urls::parse_uri(target).value();
	auto result = urls::parse_uri("http://example.com" + std::string(target));
	if (result.has_value())
		return result.value();
	else
	 return std::nullopt;
}
std::string Router::route_pattern_to_regex(const std::string& pattern)
{
	std::string regex_pattern;
	regex_pattern.reserve(pattern.size() * 2);
	bool params = false;
	for (char c : pattern)
	{
		if (c == '{')
		{
			params = true;
			regex_pattern += "([^/]+)";
		}
		else if (c == '}')
			params = false;
		else if (!params)
		{
			if (c == '/')
				regex_pattern += "\\/";
			else
				regex_pattern += c;
		}
	}
	return "^" + regex_pattern + "$";
}
bool Router::match_route(const std::string& pattern, urls::url_view url)
{
	std::regex re(pattern);
	return std::regex_match(url.path(), re);
}
void Router::add_route(http::verb method, std::string pattern, Handler handler)
{
	auto regex_string = route_pattern_to_regex(std::move(pattern));
	this->routes.push_back({method, std::move(regex_string), std::move(handler)});
}
bool Router::handle_request(Request& req, Response& resp)
{
	auto url = parse_url(req.target());
	if (!url)
	{
		resp.result(http::status::bad_request);
		return true;
	}
	for (const auto& route : this->routes)
	{
		if (route.method == req.method() && match_route(route.pattern, *url))
		{
			route.handler(req, resp, *url);
			return true;
		}
	}
	return false;
}

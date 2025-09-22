#ifndef INCLUDE_FILES_H
#define INCLUDE_FILES_H
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <boost/url.hpp>
#include <boost/url/parse.hpp>
#include <boost/url/url_view.hpp>
#include <boost/beast/http.hpp>
#include <MathPlot/RungeKutt.h>
#include <MathPlot/interpolation.h>
#include <MathPlot/MathAnalys.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <cstddef>
#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <regex>
namespace net = boost::asio;
namespace http = boost::beast::http;
namespace urls = boost::urls;
namespace beast = boost::beast;
using json = nlohmann::json;
using tcp = net::ip::tcp;
using Request = http::request<http::string_body>;
using Response = http::response<http::dynamic_body>;
using Handler = std::function<void(Request&, Response&, urls::url_view)>;
using Equation = std::function<double(double, matrix<double>)>;
#endif

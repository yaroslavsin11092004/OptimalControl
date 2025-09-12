#ifndef INCLUDE_FILES_H
#define INCLUDE_FILES_H
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <functional>
#include <MathPlot/matrix.h>
#include <fstream>
#include <regex>
#include <nlohmann/json.hpp>
#include <torch/torch.h>
#include <boost/url.hpp>
#include <boost/beast.hpp>
#include <boost/url/parse.hpp>
#include <boost/asio.hpp>
namespace net = boost::asio;
namespace beast = boost::beast;
namespace urls = boost::urls;
namespace http = boost::beast::http;
using tcp = net::ip::tcp;
using Request = http::request<http::string_body>;
using Response = http::response<http::dynamic_body>;
using Handler = std::function<void(Request&, Response&, urls::url_view)>;
using json = nlohmann::json;
#endif

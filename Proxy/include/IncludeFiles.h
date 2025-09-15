#ifndef INCLUDE_FILES
#define INCLUDE_FILES
#include <grpcpp/grpcpp.h>
#include "adam_api.grpc.pb.h"
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <nlohmann/json.hpp>
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <memory>
namespace net = boost::asio;
namespace beast = boost::beast;
namespace http = boost::beast::http;
using json = nlohmann::json;
using Request = http::request<http::string_body>;
using tcp = net::ip::tcp;
#endif

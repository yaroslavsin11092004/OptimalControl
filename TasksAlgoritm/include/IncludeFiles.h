#ifndef INCLUDE_FILES_H
#define INCLUDE_FILES_H
#include <grpcpp/grpcpp.h>
#include "tasks_api.grpc.pb.h"
#include <MathPlot/MathAnalys.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <cstddef>
#include <functional>
#include <thread>
#include <signal.h>
#include <math.h>
using Equation = std::function<double(double, matrix<double>)>;
using ParamS = std::function<double(matrix<double>, matrix<double>, matrix<double>)>;
using json = nlohmann::json;
#endif

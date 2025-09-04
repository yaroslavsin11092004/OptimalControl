#ifndef TASKS_API_RPC_H
#define TASKS_API_RPC_H
#include "IncludeFiles.h"
#include "tasks_api.grpc.pb.h"
#include <grpcpp/grpcpp.h>
#include <absl/strings/str_cat.h>
class TasksApiRpc : public std::enable_shared_from_this<TasksApiRpc>
{
	protected:
		struct AsyncCallStateEquation 
		{
			grpc::ClientContext context;
			tasks_api::CallEquationRequest request;
			tasks_api::CallEquationResponse response;

		};
		struct AsyncCallStateLinked
		{
		};
		struct AsyncCallStateParamS
		{
		};
};
#endif

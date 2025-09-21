#include "IncludeFiles.h"
#include "Server.h"
using namespace std;
int main()
{
	Hamilton func = [](vector<torch::Tensor> args, vector<torch::Tensor> params) -> torch::Tensor 
	{
		return params[3] * (params[2] + torch::pow(args[0],2)) + params[4] * args[0];
	};
	try 
	{
		string conf_path = "/home/yaroslavsinyakov/source/OptimalControl/config_system.json";
		HttpServer server(conf_path);
		server.set_hamilton_function(func);
		server.run();
	}
	catch(const exception& e)
	{
		cout <<"Error: " << e.what() << endl;
	}
	return 0;
}

#include "Server.h"
using namespace std;
int main()
{
	string conf_file = "/home/yaroslavsinyakov/source/OptimalControl/config_system.json";
	try 
	{
		HttpServer server(conf_file);
		server.run();
	}
	catch(const std::exception& e)
	{
		cout << e.what() << endl;
		exit(-2);
	}
	return 0;	
}

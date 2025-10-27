#include "Server.h"
using namespace std;
int main()
{
	try 
	{
		string conf_path = "./release/config_system.json";
		string task_path = "./release/task.json";
		ifstream task_file(task_path);
		if (task_file.is_open()) {
			string buffer = { istreambuf_iterator<char>(task_file), istreambuf_iterator<char>() };
			task_file.close();
			json task_json = json::parse(buffer);
			int dim = task_json["MoveEquations"].get<vector<string>>().size();
			string func_str = task_json["HamiltonFunction"].get<string>();
			HttpServer server(conf_path);
			server.run();
		}
		else task_file.close();
	}
	catch(const exception& e)
	{
		cout <<"Error: " << e.what() << endl;
	}
	return 0;
}

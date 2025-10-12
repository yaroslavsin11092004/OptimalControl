#include "TasksService.h"
using namespace std;
atomic<bool> stop_requested{false};
void handle_signal(int signal)
{
	std::cout << "\nrecieved signal " << signal << ", shutting down..." << std::endl;
	stop_requested = true;
}
int main()
{
	try 
	{
		string conf_path = "./release/config_system.json";
		signal(SIGINT, handle_signal);
		auto server = std::make_unique<TasksService>(conf_path);
		server->run();
		while(!stop_requested)
			this_thread::sleep_for(chrono::milliseconds(100));
	}
	catch(const exception& e)
	{
		cout << "Error: " << e.what() << endl;
		exit(2);
	}
	return 0;
}

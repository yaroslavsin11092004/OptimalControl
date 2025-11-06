#include "CoreService.h"
#include <signal.h>
#include <thread>
using namespace std;
atomic<bool> stop_requested{false};
void handle_signal(int signal)
{
	std::cout << "\nrecieved signal " << signal << ", shutting down..." << std::endl;
	stop_requested = true;
}
int main()
{
	string conf_file = "./release/config_system.json";
	try 
	{
		signal(SIGINT, handle_signal);
		CoreService service(conf_file);
		service.run();
		while(!stop_requested)
			this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	catch(const std::exception& e)
	{
		cout << e.what() << endl;
		exit(-2);
	}
	return 0;	
}

#include "AdamService.h"
#include <thread>
std::atomic<bool> stop_requested(false);
void handle_signal(int signal)
{
	std::cout << "\nRecieved signal " << signal << ", shutting down..." << std::endl;
	stop_requested = true;
}
int main()
{
	try 
	{
		std::string conf_file = "./release/config_system.json";
		std::signal(SIGINT, handle_signal);
		std::unique_ptr<AdamService> server = std::make_unique<AdamService>(conf_file);
		server->run();
		while(!stop_requested)
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	catch(const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}
	return 0;
}

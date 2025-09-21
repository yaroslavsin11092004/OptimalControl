#include "TasksService.h"
#include "GoldenRatio.h"
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
		string conf_path = "/home/yaroslavsinyakov/source/OptimalControl/config_system.json";
		signal(SIGINT, handle_signal);
		auto server = std::make_unique<TasksService>(conf_path);
		server->add_equation(
		[](double t, matrix<double> params) -> double 
		{
			return params(1,0) + std::pow(params(0,1),2);
		});
		server->add_equation(
		[](double t, matrix<double> params) -> double 
		{
			return params(0,1);
		});
		server->add_linked( 
		[](double t, matrix<double> params) -> double 
		{
			return 0;
		});
		server->add_linked(
		[](double t, matrix<double> params) -> double 
		{
			return -1 * params(0,2);
		});
		server->set_param_s( 
		[](matrix<double> x, matrix<double> u, matrix<double> optim_u) -> double 
		{
			auto functional = [](double s, matrix<double>& x, matrix<double>& u, matrix<double>& optim_u) -> double 
			{
				matrix<double> grid(x.size_row(),2);
				grid.set_column(0, x.make_column_acceptor(0));
				for (int i = 0; i < x.size_row(); i++)
					grid(i,1) = std::pow((s * (optim_u(i,1) - u(i,1)) + u(i,1)),2) + x(i,2);
				return SimpsonIntegral(grid);
			};
			GoldenRatio optimizer(functional, std::move(x), std::move(u), std::move(optim_u));
			return optimizer.minimum(0,1);
		});
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

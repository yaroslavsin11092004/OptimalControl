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
		string conf_path = "/home/yaroslavsinyakov/source/OptimalControl/config_system.json";
		signal(SIGINT, handle_signal);
		auto server = std::make_unique<TasksService>(conf_path);
		server->add_equation(
		[](double t, matrix<double> params) -> double 
		{
			return params(1,0) + pow(params(0,1),2);
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
			matrix<double> num(u.size_row(), 2);
			matrix<double> denum(u.size_row(),2);
			num.set_column(0, u.make_column_acceptor(0));
			denum.set_column(0, u.make_column_acceptor(0));
			for (size_t i = 0; i < u.size_row(); i++)
			{
				num(i,1) = u(i,1) * (optim_u(i,1) - u(i,1));
				denum(i,1) = pow((optim_u(i,1) - u(i,1)),2);
			}
			auto val_num = SimpsonIntegral(num);
			auto val_denum = SimpsonIntegral(denum);
			return -1 * (val_num / val_denum);
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

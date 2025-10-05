#include "TasksService.h"
#include "GoldenRatio.h"
#include "FunctionalBuilder.h"
#include "EquationsBuilder.h"
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
		ifstream task_file("./release/task.json");
		vector<string> equations;
		vector<string> linked;
		string functional;
		if (task_file.is_open()) {
			string buffer = { istreambuf_iterator<char>(task_file), istreambuf_iterator<char>() };
			task_file.close();
			json task_json = json::parse(buffer);
			equations = std::move(task_json["MoveEquations"].get<vector<string>>());
			linked = std::move(task_json["LinkedEquations"].get<vector<string>>());
			functional = std::move(task_json["Functional"].get<string>());
		}
		else task_file.close();
		EquationBuilder eq_builder;
		FunctionalBuilder func_builder;
		signal(SIGINT, handle_signal);
		auto server = std::make_unique<TasksService>(conf_path);
		for (auto& eq : equations)
			server->add_equation(eq_builder.build(eq.c_str()));
		for (auto& ld: linked) 
			server->add_linked(eq_builder.build(ld.c_str()));
		auto functional_val = func_builder.build(functional.c_str());
		server->set_param_s( 
		[func = functional_val](matrix<double> x, matrix<double> u, matrix<double> optim_u) -> double 
		{
			auto functional = [func = func](double s, matrix<double>& x, matrix<double>& u, matrix<double>& optim_u) -> double 
			{
				matrix<double> grid(x.size_row(),2);
				grid.set_column(0, x.make_column_acceptor(0));
				for (int i = 0; i < x.size_row(); i++)
					grid(i,1) = func(i,s,x,u,optim_u);
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

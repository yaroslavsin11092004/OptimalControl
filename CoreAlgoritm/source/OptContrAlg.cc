#include "OptContrAlg.h"
OptimalControl::OptimalControl(std::string& conf_file)
{
	this->tasks_api = std::make_shared<TasksApiRpc>(conf_file);
	this->adam_api = std::make_shared<AdamApiRpc>(conf_file);
}
void OptimalControl::adam_params(double learning_rate, std::vector<double> u_left, std::vector<double> u_right, int epochs)
{
	this->adam_api->set_global_params(learning_rate,std::move(u_left), std::move(u_right), epochs);
}
double OptimalControl::matrix_metrics(matrix<double>& val1, matrix<double>& val2)
{
	std::vector<double> rep;
	for (int i = 0; i < val1.size_row(); i++)
	{
		double sum = 0;
		for (int j = 1; j < val1.size_col(); j++)
			sum += std::abs(val1(i,j) - val2(i,j));
		rep.push_back(sum);
	}
	matrix<double> mesh(val1.size_row(), 2);
	mesh.set_column(0, val1.make_column_acceptor(0));
	mesh.set_column(1, std::move(rep));
	return SimpsonIntegral(mesh);
}
std::pair<matrix<double>, matrix<double>> OptimalControl::successive_approximation(std::vector<double> x0, double t0, double t1,double t_step, double delta, std::vector<double> u0)
{
	std::vector<double> t_grid;
	double cur_t = t0;
	while(cur_t < t1 + t_step)
	{
		t_grid.push_back(cur_t);
		cur_t += t_step;
	}
	matrix<double> cur_u(t_grid.size(), u0.size() + 1);
	cur_u.set_column(0, std::move(t_grid));
	for (size_t i = 0; i < u0.size(); i++)
	{
		std::vector<double> ui(cur_u.size_row(),u0[i]);
		cur_u.set_column(i + 1, std::move(ui));
	}
	matrix<double> last_u(cur_u.size_row(), cur_u.size_col());
	auto equations = this->tasks_api->generate_equations(x0.size());
	auto linked_equations = this->tasks_api->generate_linked(x0.size());
	auto start_value = std::make_shared<std::vector<double>>();
	for (auto val : x0) start_value->push_back(val);
	do
	{
		last_u = cur_u;
		std::vector<std::pair<size_t,size_t>> forward_task_param_idx;
		auto forward_task_param = std::make_shared<std::vector<interpol_param>>();
		for (size_t i = 0; i < last_u.size_col() - 1; i++)
		{
			matrix<double> cur_u(last_u.size_row(),2);
			cur_u.set_column(0, last_u.make_column_acceptor(0));
			cur_u.set_column(1, last_u.make_column_acceptor(i + 1));
			auto ratios = spline_interpolation(cur_u);
			interpol_param ip;
			ip.mesh = std::move(cur_u);
			ip.ratios = std::move(ratios);
			ip.dir = true;
			forward_task_param->push_back(std::move(ip));
			forward_task_param_idx.push_back(std::make_pair(i,1));
		}
		rk_params forward_task;
		forward_task.dir = true;
		forward_task.dim_var = 2;
		forward_task.equations = equations;
		forward_task.index_var = 0;
		forward_task.param_func = forward_task_param;
		forward_task.param_func_indeces = forward_task_param_idx;
		forward_task.step = t_step;
		forward_task.t0 = t0;
		forward_task.t1 = t1;
		forward_task.u0 = start_value;
		auto forward_task_resolve = runge_kutt(std::ref(forward_task));
		for (size_t i = 0; i < forward_task_resolve.size_col() - 1; i++)
			(*start_value)[i] = forward_task_resolve(forward_task_resolve.size_row() - 1, i + 1);
		rk_params back_task;
		back_task.t0 = t1;
		back_task.t1 = t0;
		back_task.step = t_step;
		back_task.param_func = forward_task_param;
		back_task.param_func_indeces = forward_task_param_idx;
		back_task.u0 = start_value;
		back_task.index_var = 0;
		back_task.dim_var = 2;
		back_task.equations = equations;
		back_task.dir = false;
		auto back_task_resolve = runge_kutt(std::ref(back_task));
		std::vector<interpol_param> linked_task_param;
		std::vector<std::pair<size_t,size_t>> linked_task_param_idx;
		for (size_t i = 0; i < back_task_resolve.size_col() - 1; i++)
		{
			matrix<double> cur_x(back_task_resolve.size_row(), 2);
			cur_x.set_column(0, back_task_resolve.make_column_acceptor(0));
			cur_x.set_column(1, back_task_resolve.make_column_acceptor(i + 1));
			auto ratios = spline_interpolation(cur_x);
			interpol_param ip;
			ip.mesh = std::move(cur_x);
			ip.ratios = std::move(ratios);
			ip.dir = false;
			linked_task_param.push_back(std::move(ip));
			linked_task_param_idx.push_back(std::make_pair(i, 0));
		}
		for (size_t i = 0; i < forward_task_param->size(); i++)
		{
			linked_task_param.push_back((*forward_task_param)[i]);
			linked_task_param_idx.push_back(std::make_pair(i, 1));
		}
		std::transform(start_value->begin(), start_value->end(), start_value->begin(), [](double x){return 0.0;});
		(*start_value)[0] = 0.0;
		rk_params linked_task;
		linked_task.equations = linked_equations;
		linked_task.t0 = t1;
		linked_task.t1 = t0;
		linked_task.dim_var = 3;
		linked_task.index_var = 2;
		linked_task.step = t_step;
		linked_task.dir = false;
		linked_task.param_func = std::make_shared<std::vector<interpol_param>>(std::move(linked_task_param));
		linked_task.param_func_indeces = std::move(linked_task_param_idx);
		linked_task.u0 = start_value;
		auto linked_task_resolve = runge_kutt(std::ref(linked_task));
		matrix<double> optim_u(cur_u.size_row(), cur_u.size_col());
		for (size_t i = linked_task_resolve.size_row() - 1; i >= 1; i--)
		{
			std::vector<double> params;
			params.push_back(linked_task_resolve(i,0));
			for (size_t k = 0; k < back_task_resolve.size_col() - 1; k++)
				params.push_back(back_task_resolve(i,k + 1));
			for (size_t k = 0; k < linked_task_resolve.size_col() - 1; k++)
				params.push_back(linked_task_resolve(i,k + 1));
			auto optim_ui = this->adam_api->adam(std::move(params));
			optim_u(linked_task_resolve.size_row() - 1 - i, 0) = linked_task_resolve(i,0);
			for (size_t k = 0; k < optim_ui.size(); k++)
				optim_u(linked_task_resolve.size_row() - 1 - i, k + 1) = optim_ui[k];
		}
		std::vector<interpol_param> opt_x_task_params;
		std::vector<std::pair<size_t,size_t>> opt_x_task_params_idx;
		for (size_t i = 0; i < optim_u.size_col() - 1; i++)
		{
			matrix<double> cur_opt_u(optim_u.size_row(),2);
			cur_opt_u.set_column(0,optim_u.make_column_acceptor(0));
			cur_opt_u.set_column(1, optim_u.make_column_acceptor(i + 1));
			auto ratios = spline_interpolation(cur_opt_u);
			interpol_param ip;
			ip.mesh = std::move(cur_opt_u);
			ip.ratios = std::move(ratios);
			ip.dir = true;
			opt_x_task_params.push_back(std::move(ip));
			opt_x_task_params_idx.push_back(std::make_pair(i,1));
		}
		std::copy(x0.begin(), x0.end(), start_value->begin());
		rk_params opt_x_task;
		opt_x_task.t0 = t0;
		opt_x_task.t1 = t1;
		opt_x_task.param_func_indeces = std::move(opt_x_task_params_idx);
		opt_x_task.param_func = std::make_shared<std::vector<interpol_param>>(std::move(opt_x_task_params));
		opt_x_task.dir = true;
		opt_x_task.u0 = start_value;
		opt_x_task.step = t_step;
		opt_x_task.dim_var = 2;
		opt_x_task.index_var = 0;
		opt_x_task.equations = equations;
		auto opt_x_task_resolve = runge_kutt(std::ref(opt_x_task));
		double s = this->tasks_api->param_s(opt_x_task_resolve, last_u, optim_u);
		std::cerr << s << std::endl;
		for (size_t i = 0; i < last_u.size_col() - 1; i++)
		{
			for (size_t j = 0; j < last_u.size_row(); j++)
				cur_u(j,i + 1) = last_u(j,i + 1) + s * (optim_u(j, i + 1) - last_u(j, i + 1));
		}
		if (matrix_metrics(cur_u,last_u) < delta) 
		{
			std::vector<interpol_param> optim_x_task_param;
			std::vector<std::pair<size_t,size_t>> optim_x_task_param_idx;
			for (size_t i = 0; i < cur_u.size_col() - 1; i++)
			{
				matrix<double> ui(cur_u.size_row(), 2);
				ui.set_column(0,cur_u.make_column_acceptor(0));
				ui.set_column(1, cur_u.make_column_acceptor(i + 1));
				auto ratios = spline_interpolation(ui);
				interpol_param ip;
				ip.mesh = std::move(ui);
				ip.ratios = std::move(ratios);
				ip.dir = true;
				optim_x_task_param.push_back(std::move(ip));
				optim_x_task_param_idx.push_back(std::make_pair(i,1));
			}
			opt_x_task.param_func = std::make_shared<std::vector<interpol_param>>(std::move(optim_x_task_param));
			opt_x_task.param_func_indeces = std::move(optim_x_task_param_idx);
			auto optim_x_task_resolve = runge_kutt(std::ref(opt_x_task));
			return std::make_pair(std::move(optim_x_task_resolve), std::move(cur_u));
		}
	} while(true);
}

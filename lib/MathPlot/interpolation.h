#ifndef INTERPOLATION_H
#define INTERPOLATION_H
#include "matrix.h"
struct interpol_param
{
	matrix<double> ratios;
	matrix<double> mesh;
	bool dir;
	interpol_param() = default;
	~interpol_param() = default;
};
matrix<double> running(matrix<double>&, matrix<double>&);
matrix<double> spline_interpolation(matrix<double>&);
size_t get_range(matrix<double>&, double, bool);
double value(matrix<double>&, matrix<double>&, double, bool);
matrix<double> make_draw_matrix(matrix<double>&, matrix<double>&, double, bool);
#endif

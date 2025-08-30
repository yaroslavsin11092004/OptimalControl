#ifndef MATHANALYS_H
#define MATHANALYS_H
#include "matrix.h"
double DownRectIntegral(matrix<double>& mesh);
double UpRectIntegral(matrix<double>& mesh);
double CentrRectIntegral(matrix<double>& mesh);
double TrapezeIntegral(matrix<double>& mesh);
double SimpsonIntegral(matrix<double>& mesh);
double MonteCarloIntegral(matrix<double>& mesh);
size_t BinarySearch(double value, matrix<double>& mesh);
double DifferentSubtract(matrix<double>& mesh);
int Factorial(int n);
double Differential(double x, int order, int offset, matrix<double>& mesh);
double ExactDifferential(double x,int order, matrix<double>& mesh);
double DifferentFroward(double x, matrix<double>& mesh);
double DifferentBack(double x, matrix<double>& mesh);
double DifferentCenter(double x, matrix<double>& mesh);
#endif

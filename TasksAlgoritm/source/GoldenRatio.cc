#include "GoldenRatio.h"
int GoldenRatio::far_point(std::vector<double>& points, int min_idx)
{
	double cur = -1;
	double ans = -1;
	for (int i = 0; i < points.size(); i++)
	{
		if (i != min_idx)
		{
			double val = std::abs(points[i] - points[min_idx]);
			if (cur < val)
			{
				cur = val;
				ans = i;
			}
		}
	}
	return ans;
}
int GoldenRatio::min_idx(std::vector<double>& points)
{
	double cur = function(points[0], x, u, optim_u);
	int ans = 0;
	for (int i = 1; i < points.size(); i++)
	{
		double val = function(points[i],x,u,optim_u);
		if (val < cur)
		{
			cur = val;
			ans = i;
		}
	}
	return ans;
}
double GoldenRatio::minimum(double left, double right)
{
	std::vector<double> points;
	double golden_ratio = 2.0 / (1 + std::sqrt(5));
	points.push_back(left);
	points.push_back(right);
	points.push_back(left + golden_ratio*(right-left));
	points.push_back(right - golden_ratio*(right-left));
	double x0 = left;
	double x1 = right;
	double ans = left;
	do 
	{
		int index_min = min_idx(points);
		int remove_index = far_point(points, index_min);
		points.erase(points.begin() + remove_index);
		std::sort(points.begin(), points.end());
		x0 = *points.begin();
		x1 = *(points.end() - 1);
		ans = (x0 + x1) / 2.0;
		points.push_back(*points.begin() + *(points.end() - 1) - *(points.begin() + 1));
	}while(x1 - x0 > 1.0e-8);
	return ans;
}

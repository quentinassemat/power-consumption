#include "point.hpp"

#include <cmath> // for sqrt, pow, etc
#include <cassert>
#include <cfloat>  // for DBL_MAX
#include <cstdlib> // for rand, srand
#include <iostream>

int point::d = 0; // has to be defined;

bool point::set_dim(int _d)
{
	if (d == 0)
	{
		d = _d;
		return true;
	}
	return false;
}

int point::get_dim()
{
	return point::d;
}

point::point()
{
	coords = new double[d];
	for (int m = 0; m < d; m++)
	{
		coords[m] = 0.0;
	}
	label = 0;
	date = std::string();
	time = std::string();
}

point::~point()
{
	delete[] coords;
	date.erase();
	time.erase();
}

void point::print()
{
	for (int i(0); i < d; i++)
	{
		std::cout << coords[i];
		if (i < d - 1)
		{
			std::cout << '\t';
		}
	}
	std::cout << std::endl;
}

double point::squared_dist(point &q)
{
	double res = 0;
	for (int i(0); i < d; i++)
	{
		res += pow((q.coords[i] - coords[i]), 2);
	}
	return res;
}
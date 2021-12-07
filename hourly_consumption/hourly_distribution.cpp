#include <iostream>
#include <cassert>
#include <cmath>   // for sqrt, fabs
#include <cfloat>  // for DBL_MAX
#include <cstdlib> // for rand, srand
#include <ctime>   // for rand seed
#include <fstream>
#include <cstdio> // for EOF
#include <string>
#include <algorithm> // for count
#include <vector>
#include <cstring>

#include "hourly_distribution.hpp"

//calcul de la p valeur pour le test de kolmogorov
double p_value(double d, long double threshold)
{
	long double res = 0;
	int i = 1;
	long double temp = 2 * std::pow(-1, i + 1) * std::exp(-2 * i * i * d * d);
	while (std::abs(temp) > threshold)
	{
		res += temp;
		i++;
		temp = 2 * std::pow(-1, i + 1) * std::exp(-2 * i * i * d * d);
	}
	return res;
}

// Prints correct usage
void msgleave(char *argv[])
{
	std::cout << "Usage: " << argv[0] << " <file csv trié selon la première colonne> " << std::endl;
}

int main(int argc, char *argv[])
{

	// ./hourly_distribution ../../p1-power-weather/household/preprocessed/hours_sorted_power/preprocessed_household_sorted_hour_19.csv
	// ./hourly_distribution ../../p1-power-weather/household/preprocessed/hours_season/hour_18_saison_0.csv

	// Tests correct usage
	if (argc != 2)
	{
		msgleave(argv);
		return 1;
	}

	int pow_eps = -15;
	long double eps = std::pow(10, pow_eps);
	printf("eps : 10^(%d)\n", pow_eps);

	cloud cloud_dataset(7, 3000, 1);

	cloud_dataset.fill(argv[1]);
	std::cout << "Nous venons de construire notre cloud avec les points du csv." << std::endl;

	std::cout << "test de kolmogorov : " << cloud_dataset.kolmogorov_test(0) << std::endl;
	std::cout << "p_value : " << p_value(cloud_dataset.kolmogorov_test(0), eps) << std::endl;

	return 0;
}
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

#include "seasonality_detection.hpp"

int silouhette_method(int p, char *file)
{
	std::string const file_name("../../p1-power-weather/household/processed/silouhette_method.csv");

	std::ofstream stream(file_name.c_str());

	if (stream.fail())
	{
		std::cout << "Cannot write into file " << file_name << " !" << std::endl;
		exit(1);
	}
	//on met les titres :
	stream << "k,silouhette" << std::endl;


	cloud **clouds = new cloud *[p - 1];
	double silouhette[p - 1];
	for (int i = 2; i < p + 1; i++)
	{
		silouhette[i - 2] = 0;
		double sil = 0;
		for (int p = 0; p < N_SIL; p++)
		{
			clouds[i - 2] = new cloud(7, 1500, i);
			clouds[i - 2]->fill(file);
			clouds[i - 2]->kmeans();
			silouhette[i - 2] += clouds[i - 2]->avg_silouhette();
		}
		silouhette[i-2] = silouhette[i-2] / ((double) N_SIL);
		printf("average silouhette on %d iteration: %f		and		k : %d\n",N_SIL ,silouhette[i - 2], clouds[i - 2]->get_k());
	}
	for (int i = 2; i < p; i++)
	{
		stream << i << ",";
		stream << silouhette[i-2] << std::endl;
	}
	int i_min = 2;
	double temp = silouhette[0];
	for (int i = 3; i < p + 1; i++)
	{
		if (silouhette[i - 2] > temp)
		{
			temp = silouhette[i - 2];
			i_min = i;
		}
	}
	return i_min;
}

void elbow_method(int p, char *file)
{

	cloud **clouds = new cloud *[p - 1];
	double elbow[p-1];

	std::string const file_name("../../p1-power-weather/household/processed/elbow_method.csv");

	std::ofstream stream(file_name.c_str());

	if (stream.fail())
	{
		std::cout << "Cannot write into file " << file_name << " !" << std::endl;
		exit(1);
	}
	//on met les titres :
	stream << "k,variance" << std::endl;
	for (int i = 2; i < p+1; i++)
	{
		elbow[i-2] = 0;
		for (int p = 0; p < N_ELB; p++)
		{
			clouds[i - 2] = new cloud(7, 1500, i);
			clouds[i - 2]->fill(file);
			clouds[i - 2]->kmeans();
			elbow[i - 2] += clouds[i - 2]->intracluster_variance();
		}
		elbow[i - 2] = elbow[i - 2] / ((double) N_ELB);
	}
	for (int i = 2; i < p; i++)
	{
		stream << i << ",";
		stream << elbow[i-2] << std::endl;
	}
}

// Prints correct usage
void msgleave(char *argv[])
{
	std::cout << "Usage: " << argv[0] << " <k> <file(csv)>  <0 ou 1>" << std::endl;
}

int main(int argc, char *argv[])
{

	// donc pour tester : ./seasonality_detection 4 ../../p1-power-weather/household/preprocessed/preprocessed_household_day.csv (0 ou 1)

	// Tests correct usage
	if ((argc < 3) || (argc > 4))
	{
		msgleave(argv);
		return 1;
	}

	// arguments du main à prendre en compte :
	int k = atoi(argv[1]);
	if (k < 1)
	{
		std::cout << "Invalid value of k." << std::endl;
		msgleave(argv);
		return 1;
	}

	int opt_k = 0;
	if (argc == 4)
	{
		opt_k = atoi(argv[3]);
	}

	cloud cloud_dataset(7, 3000, k);

	cloud_dataset.fill(argv[2]);
	std::cout << "Nous venons de construire notre cloud avec les points du csv." << std::endl;

	std::cout << "Variance avant kmeans : " << cloud_dataset.intracluster_variance() << std::endl;
	cloud_dataset.kmeans();
	std::cout << "Variance après kmeans : " << cloud_dataset.intracluster_variance() << std::endl;
	cloud_dataset.to_csv();
	std::cout << "Exportation en csv réussi dans le folder 'processed' " << std::endl;
	if (opt_k)
	{
		std::cout << "Vous avez choisi de trouver le k optimal. Nous allons executer la méthode silouhette et elbow :" << std::endl;
		std::cout << "Execution de la méthode silouhette ... \n"
				  << std::endl;
		opt_k = silouhette_method(K_MAX_SILOUHETTE, argv[2]);
		std::cout << "Nous avons trouvé : " << opt_k << std::endl;
		std::cout << "Execution de la méthode elbow ... \n"
				  << std::endl;
		elbow_method(K_MAX_ELBOW, argv[2]);
		std::cout << "Nous avons exporté en csv afin d'afficher la courbe pour la méthode elbow" << std::endl;
	}

	return 0;
}
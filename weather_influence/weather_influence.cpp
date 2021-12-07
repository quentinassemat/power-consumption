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

#include "weather_influence.hpp"

#define DIM 666 // nombre de colonne utile du csv pour la regression (correspond à d dans le cours) (sera égale au nombre de colonne utile de weather_preproceessed)
#define K 4    // valeur de K pour le knn

// Prints correct usage
void msgleave(char *argv[])
{
    std::cout << "Usage: " << argv[0] << " <train_file> <test_file> " << std::endl;
}

// ./weather_influence ../../p1-power-weather/weather/preprocessed_weather_norm_day.csv ../../p1-power-weather/weather/preprocessed_weather_norm_day.csv
// ./weather_influence ../../p1-power-weather/weather/trainset.csv ../../p1-power-weather/weather/testset.csv

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        msgleave(argv);
        exit(-1);
    }

    std::string train_dataset_name("train_dataset");
    std::string test_dataset_name("test_dataset");

    //création des datasets :
    Dataset train_dataset(argv[1], DIM, DIM +6, train_dataset_name);
    Dataset test_dataset1(argv[2], DIM, DIM +6, test_dataset_name);

    train_dataset.Show(false);
    test_dataset1.Show(false);

    assert(train_dataset.GetDim() == test_dataset1.GetDim());

    int col_regr = test_dataset1.GetDim() - 1;

    std::cout << "Linear regression on dataset 1: " << std::endl;

    LinearRegression linear(&train_dataset, col_regr);
    linear.ShowCoefficients();

    double l_ess, l_rss, l_tss;

    linear.SumsOfSquares(&test_dataset1, l_ess, l_rss, l_tss);

    std::cout << "Sums of Squares:" << std::endl
              << " ESS=" << l_ess << std::endl
              << " RSS=" << l_rss << std::endl
              << " TSS=" << l_tss << std::endl
              << " R2=" << l_ess / l_tss << std::endl
              << " MSE=" << l_rss / train_dataset.GetNbrSamples() << std::endl
              << std::endl;

    std::cout << "KNN regression on dataset 1: " << std::endl;

    KnnRegression knn(K, &train_dataset, col_regr);

    double mse = 0;
    for (int i = 0; i < test_dataset1.GetNbrSamples(); i++)
    {
        std::vector<double> sample = test_dataset1.GetInstance(i);
        
        Eigen::VectorXd query(test_dataset1.GetDim() - 1);

        for (int j = 0; j < train_dataset.GetDim() - 1; j++)
        {
            query[j] = sample[j]; // simplified as our regression column is at the end of the dataset;
        }
        double estim = knn.Estimate(query);
        mse += (estim - sample[col_regr]) * (estim - sample[col_regr]) / test_dataset1.GetNbrSamples();
    }

    std::cout << "Mean Square(d) Error (MSE) over test set: " << mse << std::endl
              << std::endl;

    std::cout << "Extraction des données des regressions linéaire et knn vers des csv :" << std::endl;

    linear.to_csv();
    knn.to_csv(&test_dataset1);

    return 0;
}
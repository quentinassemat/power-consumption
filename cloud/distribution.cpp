#include <cfloat>  // for DBL_MAX
#include <cmath>
#include "distribution.hpp"

double gaussian(double mu, double sigma,double x) { //fonction de répartition d'une gausienne
    return (0.5) * (1 + std::erf((x - mu)/(sigma * sqrt(2))));
}
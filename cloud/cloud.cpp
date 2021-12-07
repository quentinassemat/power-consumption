#include <cassert>
#include <cfloat>  // for DBL_MAX
#include <cstdlib> // for rand, srand
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>

#include "cloud.hpp"

cloud::cloud(int _d, int _nmax, int _k)
{
    d = _d;
    // To be used once set_dim is implemented
    point::set_dim(_d);
    n = 0;
    k = _k;

    nmax = _nmax;

    points = new point[nmax];
    centers = new point[k];
}

void cloud::fill(const char *file)
{
    std::ifstream fin(file);

    if (fin.fail())
    {
        std::cout << "Cannot read from file " << file << " !" << std::endl;
        exit(1);
    }

    std::vector<double> row;
    std::string line, word, temp_time, temp_date;

    point p;

    bool not_first_row = false;
    while (getline(fin, line))
    {
        if (not_first_row)
        {
            row.clear();
            std::stringstream s(line);
            int ncols = 0;
            int first_col = 0;
            while (getline(s, word, ','))
            {
                //manage the first column
                if (first_col==0)
                {
                    temp_date = word.c_str();
                    first_col++;
                }
                else if (first_col==1)
                {
                    temp_time = word.c_str();
                    first_col++;
                }
                else
                {
                    // add all the column data
                    // of a row to a vector
                    double val = std::atof(word.c_str());
                    row.push_back(val);
                    ncols++;
                }
            }
            if (d == -1)
            {
                d = ncols;
                point::set_dim(d);
            }
            else if (d != ncols)
            {
                printf("n cols : %d\n", ncols);
                std::cerr << "ERROR, inconsistent dataset" << std::endl;
                exit(-1);
            }
            for (int i = 0; i < d; i++)
            {
                p.coords[i] = row[i];
            }
            p.date = temp_date;
            p.time = temp_time;
            add_point(p, 0);
        }
        else
        {
            not_first_row = true;
        }
    }
    fin.close();
}

void cloud::show()
{
    std::cout << "Date - Label" << std::endl;
    for (int i = 0; i < get_n(); i++)
    {
        std::cout << points[i].time << " - " << points[i].label << std::endl;
    }
}

void cloud::show(int p)
{
    std::cout << "Date - Label" << std::endl;
    for (int i = 0; i < std::min(p, get_n()); i++)
    {
        std::cout << points[i].time << " - " << points[i].label << std::endl;
    }
}

void cloud::to_csv()
{
    std::string const file_name("../../p1-power-weather/household/processed/output.csv");

    std::ofstream stream(file_name.c_str());

    if (stream.fail())
    {
        std::cout << "Cannot write into file " << file_name << " !" << std::endl;
        exit(1);
    }
    //on remet les titres : 
    stream << "date,global_active_power,globale_reactive_power,voltage,global_intensity,sub_metering_1,sub_metering_2,sub_metering_3,label" << std::endl;
    for (int i = 0; i<n; i++) {
        stream << points[i].date << ",";
        for (int j = 0; j<d; j++) {
            stream << points[i].coords[j] << ","; 
        }
        stream << points[i].label << std::endl; 
    }
}

double cloud::mean(int k)
{
    double mean = 0;
    for (int i = 0; i < n; i++)
    {
        mean += points[i].coords[k];
    }
    return mean / n;
}

double cloud::var(int k)
{
    double var = 0;
    double mean = this->mean(k);
    for (int i = 0; i < n; i++)
    {
        var += points[i].coords[k] * points[i].coords[k];
    }
    var = var / ((double)n);
    var = var - (mean * mean);
    return var;
}

double cloud::silouhette(int i)
{
    point p = get_point(i);
    double dist[k];
    int count[k];
    for (int j = 0; j<k; j++) {
        dist[j] = 0;
        count[j] = 0;
    }
    for (int j = 0; j < n; j++)
    {
        dist[get_point(j).label] += sqrt(p.squared_dist(get_point(j)));
        count[get_point(j).label]++;
    }
    for (int j = 0; j < k; j++)
    {
        dist[j] = dist[j] / count[j];
    }
    double b = dist[0];
    double a = dist[p.label];
    for (int j = 1; j < k; j++)
    {
        if (dist[j] < b && (p.label != j))
        {
            b = dist[j];
        }
    }
    double max = std::max(a, b);
    return (b - a) / max;
}

double cloud::avg_silouhette()
{
    double res = 0;
    for (int i = 0; i < n; i++)
    {
        res += silouhette(i);
    }
    return (res / n);
}


double cloud::kolmogorov_test(int k){
    double sigma = std::sqrt(this->var(k));
    double mu = this->mean(k);
    double res = std::max(std::abs(gaussian(mu, sigma, points[0].coords[k]) - 0/n),std::abs(gaussian(mu, sigma, points[0].coords[k]) - 1/n ));
    double temp;
    for (int i = 1; i<n; i++) {
        temp = std::max(std::abs(gaussian(mu, sigma, points[i].coords[k]) - i/((double) n)),std::abs(gaussian(mu, sigma, points[i].coords[k]) - (i+1)/((double) n) ));
        if (temp > res) {
            res = temp;
        }
    }
    return res * std::sqrt(n);
}

cloud::~cloud()
{
    delete[] centers;
    delete[] points;
}

void cloud::add_point(point &p, int label)
{
    assert(n < nmax);

    for (int m = 0; m < d; m++)
    {
        points[n].coords[m] = p.coords[m];
    }
    points[n].label = label;
    points[n].date = p.date;
    points[n].time = p.time;
    n++;
}

int cloud::get_d()
{
    return d;
}

int cloud::get_n()
{
    return n;
}

int cloud::get_k()
{
    return k;
}

void cloud::set_k(int _k)
{
    k = _k;
}

point &cloud::get_point(int i)
{
    return points[i];
}

point &cloud::get_center(int j)
{
    return centers[j];
}

void cloud::set_center(point &p, int j)
{
    for (int m = 0; m < d; m++)
        centers[j].coords[m] = p.coords[m];
}

double cloud::intracluster_variance()
{
    double sum = 0.0;
    for (int i = 0; i < n; i++)
        sum += points[i].squared_dist(centers[points[i].label]);

    return sum / n;
}

int cloud::set_voronoi_labels()
{
    int num_changes = 0;
    for (int i = 0; i < n; i++)
    {
        double min_dist = DBL_MAX;
        int min_ind = -1;

        for (int j = 0; j < k; j++)
        {
            if (points[i].squared_dist(centers[j]) < min_dist)
            {
                min_dist = points[i].squared_dist(centers[j]);
                min_ind = j;
            }
        }

        if (min_ind != points[i].label)
        {
            ++num_changes;
            points[i].label = min_ind;
        }
    }
    return num_changes;
}

void cloud::set_centroid_centers()
{
    int *counts = new int[k];
    for (int j = 0; j < k; j++)
        counts[j] = 0;
    for (int i = 0; i < n; i++)
        counts[points[i].label]++;
    for (int j = 0; j < k; j++)
    {
        if (counts[j] != 0)
        {
            for (int m = 0; m < d; m++)
            {
                centers[j].coords[m] = 0.0;
            }
        }
    }
    for (int i = 0; i < n; i++)
    {
        for (int m = 0; m < d; m++)
            centers[points[i].label].coords[m] += points[i].coords[m];
    }
    for (int j = 0; j < k; j++)
    {
        if (counts[j] != 0)
        {
            for (int m = 0; m < d; m++)
                centers[j].coords[m] /= counts[j];
        }
    }
    delete[] counts;
}

void cloud::kmeans()
{
    // init_forgy();
    init_plusplus();
    // init_random_partition();

    bool stabilized = false;

    while (true)
    {
        int num_changed = set_voronoi_labels();
        set_centroid_centers();
        if (num_changed == 0)
            break;
    }
}

void cloud::init_forgy()
{
    int *already_chosen = new int[n];

    for (int j = 0; j < k; j++)
    {
        // choose index i different from those already chosen
        int i;
        bool new_index = false;
        while (!new_index)
        {
            i = rand() % n;
            // check whether i was already chosen
            new_index = true;
            for (int r = 0; r < j; r++)
            {
                if (already_chosen[r] == i)
                {
                    new_index = false;
                    break;
                }
            }
        }
        already_chosen[j] = i;

        for (int m = 0; m < d; m++)
            centers[j].coords[m] = points[i].coords[m];
    }

    delete[] already_chosen;
}

void cloud::init_plusplus()
{
    // choose first center
    int i = rand() % n;
    for (int m = 0; m < d; m++)
        centers[0].coords[m] = points[i].coords[m];

    // number of centers already chosen
    int nb_already_chosen = 1;

    // will hold squared distances to nearest already chosen center
    double *distances = new double[n];

    while (nb_already_chosen < k)
    {
        double sum_distances = 0.0;
        // calculate squared distance to nearest already chosen center
        for (i = 0; i < n; i++)
        {
            distances[i] = DBL_MAX;

            for (int j = 0; j < nb_already_chosen; j++)
            {
                if (points[i].squared_dist(centers[j]) < distances[i])
                    distances[i] = points[i].squared_dist(centers[j]);
            }
            sum_distances += distances[i];
        }

        // choose random point proportional to square distance
        double random = ((double)rand() / RAND_MAX) * sum_distances;
        double prob_sum = 0.0;
        i = 0;
        while (prob_sum <= random && i < n)
        {
            prob_sum += distances[i];
            i++;
        }

        // i-1 is now the index of the chosen point
        for (int m = 0; m < d; m++)
            centers[nb_already_chosen].coords[m] = points[i - 1].coords[m];
        nb_already_chosen++;
    }

    delete[] distances;
}

void cloud::init_random_partition()
{
    for (int i = 0; i < n; i++)
    {
        points[i].label = rand() % k;
    }
    set_centroid_centers();
}

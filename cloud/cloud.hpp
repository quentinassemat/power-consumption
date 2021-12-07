#pragma once // ensures that this header file is only included once

#include "point.hpp" // include the header file with the declaration
					 // of the class point, so that it can be used
					 // here

#include "distribution.hpp"

/**
 * @brief The class cloud to store the data points
*/
class cloud // this is just a declaration of the class
			// implementation is in a separate file:
			// cloud.cpp
{
private:
	/**
     * @brief The dimension of the cloud
    */
	int d;

	/**
     * @brief the number of point of the cloud
    */
	int n;

	/**
     * @brief the value of k for k-means
    */
	int k;

	/**
     * @brief the name of the cloud
    */
	std::string name;

	/**
     * @brief the maximum number of points
    */
	int nmax;

	/**
     * @brief the array of points 
    */
	point *points;

	/**
     * @brief the array of the centers (k-means)
    */
	point *centers;

public:
	/**
     * @brief a constructor of the cloud
	 * @param _d the futur dimension
	 * @param _nmax the maximum number of points
	 * @param _k the futur value of k
    */
	cloud(int _d, int _nmax, int _k);

	/**
     * @brief the destructor of the cloud
    */
	~cloud();

	// Getters

	/**
     * @brief the getter of d
    */
	int get_d();

	/**
     * @brief the getter of n
    */
	int get_n();

	/**
     * @brief the getter of k
    */
	int get_k();

	/**
     * @brief the getter of the point i
	 * @param i
    */
	point &get_point(int i);

	/**
     * @brief the getter of center of the point i
	 * @param i
    */
	point &get_center(int j);

	// Setters

	/**
     * @brief To set the center of the point j
	 * @param p the futur center
	 * @param j
    */
	void set_center(point &p, int j);

	/**
     * @brief To set the value of k
	 * @param _k the futur value of k
    */
	void set_k(int _k);

	// Helper methods

	/**
     * @brief To add a point to the cloud
	 * @param p the point to add
	 * @param the label of the point
    */
	void add_point(point &p, int label);

	/**
     * @brief compute the intracluster_variance
	 * @return the intracluster_variance
    */
	double intracluster_variance();

	/**
     * @brief To set the voronoi labels
	 * @return the number of label changed
    */
	int set_voronoi_labels();

	/**
     * @brief To set the centroid centers
    */
	void set_centroid_centers();

	/**
     * @brief To use the kmeans algorithm
    */
	void kmeans();

	/**
     * @brief To fill the cloud from a csv
	 * @param file the path of the .csv
    */
	void fill(const char *file);

	/**
     * @brief To print the cloud
    */
	void show(); // pour afficher le cloud

	/**
     * @brief To print the cloud (only first points)
	 * @param p the number of points to print
    */
	void show(int p);

	/**
     * @brief To export into .csv
    */
	void to_csv();

	/**
     * @brief To do the gaussian kolmogorov_test
	 * @param k coordonnée sur laquelle vérifier la distribution
	 * @return the kolmogorov heuristic
    */
	double kolmogorov_test(int k); // coordonnée sur laquelle vérifier la distribution

	// pour simplifier quelques calculs :

	/**
     * @brief to compute the mean of the coordinate k in the cloud 
	 * @param k the coordinate to consider
	 * @return the mean
    */
	double mean(int k);

	/**
     * @brief to compute the mean of the coordinate k in the cloud 
	 * @param k the coordinate to consider
	 * @return the variance
    */
	double var(int k);

	// pour la méthode silouhette :

	/**
     * @brief to compute the silouhette of the point i
	 * @param i the index of the point of which we compute the silouhette
	 * @return the silouhette
    */
	double silouhette(int i);

	/**
     * @brief To export into .csv
	 * @return the average silouhette
    */
	double avg_silouhette();

	// pour la méthode elbow :

	/**
     * @brief compute the elbow heuristic
	 * @return the elbow heuristic
    */
	double elbow_heuristic();

	// Cluster initialisation options

	/**
     * @brief Init forgy of knn
    */
	void init_forgy();
	/**
     * @brief Init plusplus of knn
    */
	void init_plusplus();
	/**
     * @brief Random init of knn
    */
	void init_random_partition();
};

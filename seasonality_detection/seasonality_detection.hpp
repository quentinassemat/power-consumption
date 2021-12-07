#pragma once

#include "../cloud/cloud.hpp"
#include "../cloud/point.hpp"
#include "test.hpp"

#define K_MAX_SILOUHETTE 20 // ici le paramètre maximum pour la recherche du k_optimal avec la méthode silouhette et elbow
#define K_MAX_ELBOW 20 
#define N_SIL 20
#define N_ELB 20

/** 
	@brief The implementation of the silouhette method
    @param p the bounds of the search for the silouhette method
    @param the file of the .csv for the method
    @return the optimal value of k for k-means algorithm on the .csv
*/
int silouhette_method(int p, char *file);

/** 
	@brief The implementation of the elbow method
    @param p the bounds of the search for the elbow method
    @param the file of the .csv for the method
    @return write on a output file to anlyse graphically
*/
void elbow_method(int p, char *file);

/**
 * @brief to print the usage 
 */
void msgleave(char *argv[]);
#include <ANN/ANN.h>
#include <Eigen/Dense>
#include <Eigen/Core>
#include "Dataset.hpp"
#include "Regression.hpp"

#ifndef KNNREGRESSION_HPP
#define KNNRREGRESSION_HPP

/**
  The KnnRegression class inherits from the Regression class, adds the number of neighbours k, the kdTree obtained from the ANN library, and a bunch of methods.
*/
class KnnRegression : public Regression
{
private:
	/**
      The number of neighbours k to compute.
    */
	int m_k;
	/**
      The kdTree obtained from the ANN library.
    */
	ANNkd_tree *m_kdTree;
	// This is needed to properly deallocate the array
	/**
      The array of data points from the ANN library
    */
	ANNpointArray m_dataPts;

public:
	/**
      @brief The constructor needs:
     @param k the number of neighbours
     @param dataset the pointer to a dataset of class Dataset
     @param col_regr the integer that defines the column index that is to be regressed against
    */
	KnnRegression(int k, Dataset *dataset, int col_regr);
	/**
      @brief The standard destructor.
    */
	~KnnRegression();
	/**
      @brief The predicted output for a new instance:
     @param x the new instance which output we wish to predict, as a VectorXd of class Eigen.
     @returns the prediction as a double float
    */
	double Estimate(const Eigen::VectorXd &x) const;
	/**
      @brief The getter for m_k
    */
	int GetK() const;
	/**
      @brief The kdTree getter
    */
	ANNkd_tree *GetKdTree() const;

	/**
	 * @brief turn to csv the dataset with the knn prediction
	 * @param dataset the test dataset
	 * @return write on a file in the weather folder 
	*/
	void to_csv(Dataset * dataset);
};

#endif //KNNREGRESSION_HPP

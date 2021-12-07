#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>

#ifndef DATASET_HPP
#define DATASET_HPP

/**
  The Dataset class encapsulates a dataset in a vector of vectors and provides a kind of interface to manipulate it.
*/
class Dataset
{
public:
	/**
     * @brief The constructor changed to apprehend several columns of the csv as potential regression column (column >= dim) 
	 * @param file the path of the .cs
	 * @param dim the number of column to use for the prediction (d in the course). The dataset will be of dimension dim+1
	 * @param col_regr is the regression column chosen
	 * @param dataset_name is the name of the dataset
    */
	Dataset(const char *file, int dim, int col_regr, std::string dataset_name);
	/**
          @brief Standard destructor
        */
	~Dataset();

	/**
          @brief The Show method displays the number of instances and columns of the Dataset.
          @param verbose If set to True, the Dataset is also printed.
        */
	void Show(bool verbose) const;
	/**
         @brief Returns a copy of an instance.
        @param i Instance number (= row) to get.
        */
	const std::vector<double> &GetInstance(int i) const;
	/**
          @brief The getter to the number of instances / samples.
        */
	int GetNbrSamples() const;
	/**
          @brief The getter to the dimension of the dataset.
        */
	int GetDim() const;

	/**
	 *  @brief the name of the columns of the dataset
	*/
	std::vector<std::string> columns;

	/**
	 *  @brief the name of the dataset
	*/
	std::string m_dataset_name;

private:
	/**
          @brief The dimension of the dataset.
        */
	int m_dim;
	/**
          @brief The number of instances / samples.
        */
	int m_nsamples;
	/**
          @brief The dataset is stored as a vector of double vectors.
        */
	std::vector<std::vector<double>> m_instances;
};
#endif //DATASET_HPP

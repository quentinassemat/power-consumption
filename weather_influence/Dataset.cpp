#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include "Dataset.hpp"

int Dataset::GetNbrSamples() const
{
	return m_nsamples;
}

int Dataset::GetDim() const
{
	return m_dim;
}

Dataset::~Dataset()
{
}

void Dataset::Show(bool verbose) const
{
	std::cout << "Affichage du dataset : " << std::endl;
	std::cout << "Dataset with " << m_nsamples << " samples, and " << m_dim << " dimensions." << std::endl;

	std::cout << "Columns used to predict : " << std::endl;
	for (int i = 0; i < m_dim - 1; i++)
	{
		std::cout << columns[i] << ",";
	}
	std::cout << std::endl
			  << "Regression column : ";
	std::cout << columns[m_dim - 1] << std::endl;

	if (verbose)
	{
		for (int i = 0; i < m_nsamples; i++)
		{
			for (int j = 0; j < m_dim; j++)
			{
				std::cout << m_instances[i][j] << " ";
			}
			std::cout << std::endl;
		}
	}
	std::cout << std::endl;
}

Dataset::Dataset(const char *file, int dim, int col_regr, std::string dataset_name) // les dim premières colonnes sont le train_dataset, et la col regr est censé être une après : col_regr >= dim
{
	m_dataset_name = dataset_name;
	m_nsamples = 0;
	m_dim = -1;

	std::ifstream fin(file);

	if (fin.fail())
	{
		std::cout << "Cannot read from file " << file << " !" << std::endl;
		exit(1);
	}

	std::vector<double> row;
	std::string line, word, temp;

	if (col_regr < dim)
	{
		std::cout << "Unvalid input (dim, col_regr)" << std::endl;
		exit(1);
	}

	bool is_first_column = true;

	int index_col;	   // permet de situer dans quelle colonne on regarde dans le dataset
	int index_col_csv; // permet de situer dans quelle colonne on regarde dans le csv

	while (getline(fin, line))
	{
		if (is_first_column)
		{
			row.clear();
			index_col = 0;
			index_col_csv = 0;
			std::stringstream s(line);
			while (getline(s, word, ','))
			{
				if ((index_col_csv < dim) || (index_col_csv == col_regr))
				{ // attention les indices de colonnes commençeront à 0
					columns.push_back(word.c_str());
					index_col++;
				}
				index_col_csv++;
			}
			if (index_col != dim + 1)
			{
				std::cout << "Unvalid input (dim, csv)" << std::endl;
				exit(1);
			}
			m_dim = index_col;
			is_first_column = false;
		}
		else
		{
			row.clear();
			index_col = 0;
			index_col_csv = 0;
			std::stringstream s(line);
			while (getline(s, word, ','))
			{
				if ((index_col_csv < dim) || (index_col_csv == col_regr))
				{
					double val = std::atof(word.c_str());
					row.push_back(val);
					index_col++;
				}
				index_col_csv++;
			}
			m_instances.push_back(row);
			if (m_dim != index_col)
			{
				std::cerr << "ERROR, inconsistent dataset" << std::endl;
				exit(-1);
			}
			m_nsamples++;
		}
	}
	std::cout << "Dataset on the " << dim << " first columns of the csv built." << std::endl;
	std::cout << "The regression column is the " << col_regr + 1 << "-th column of the csv" << std::endl
			  << std::endl;
	fin.close();
}

const std::vector<double> &Dataset::GetInstance(int i) const
{
	return m_instances[i];
}

#include "LinearRegression.hpp"
#include "Dataset.hpp"
#include "Regression.hpp"
#include <iostream>
#include <cassert>
#include <fstream>
#include <sstream>

const double LAMDA = 0; // à choisir ce qu'il y a de mieux

LinearRegression::LinearRegression(Dataset *dataset, int col_regr)
	: Regression(dataset, col_regr)
{
	SetCoefficients(LAMDA);
}

LinearRegression::~LinearRegression()
{
	m_beta->resize(0);
	delete m_beta;
}

void LinearRegression::SetCoefficients(double lamda)
{
	//on commence par créer la matrice X, le vecteur y et le vecteur résultat
	Eigen::VectorXd *beta = new Eigen::VectorXd(m_dataset->GetDim());
	Eigen::VectorXd y(m_dataset->GetNbrSamples());
	Eigen::MatrixXd X(m_dataset->GetNbrSamples(), m_dataset->GetDim());
	Eigen::MatrixXd Lamda(m_dataset->GetDim(), m_dataset->GetDim());

	//on rempli les deux
	for (int i = 0; i < m_dataset->GetNbrSamples(); i++)
	{
		X(i, 0) = 1;
	}
	for (int j = 0; j < m_dataset->GetDim(); j++)
	{
		for (int i = 0; i < m_dataset->GetNbrSamples(); i++)
		{
			if (j < m_col_regr)
			{
				X(i, j + 1) = m_dataset->GetInstance(i)[j];
			}
			else if (j == m_col_regr)
			{
				y(i) = m_dataset->GetInstance(i)[j];
			}
			else
			{
				X(i, j) = m_dataset->GetInstance(i)[j];
			}
		}
	}
	for (int i = 0; i < m_dataset->GetDim(); i++)
	{
		for (int j = 0; j < m_dataset->GetDim(); j++)
		{
			if (i != j)
			{
				Lamda(i, j) = 0;
			}
			else
			{
				Lamda(i, j) = lamda;
			}
		}
	}

	*beta = (X.transpose() * X + Lamda).colPivHouseholderQr().solve(X.transpose() * y);

	m_beta = beta;
}

const Eigen::VectorXd *LinearRegression::GetCoefficients() const
{
	if (!m_beta)
	{
		std::cout << "Coefficients have not been allocated." << std::endl;
		return NULL;
	}
	return m_beta;
}

void LinearRegression::ShowCoefficients() const
{
	if (!m_beta)
	{
		std::cout << "Coefficients have not been allocated." << std::endl;
		return;
	}

	if (m_beta->size() != m_dataset->GetDim())
	{ // ( beta_0 beta_1 ... beta_{d} )
		std::cout << "Warning, unexpected size of coefficients vector: " << m_beta->size() << std::endl;
	}

	std::cout << "beta = (";
	for (int i = 0; i < m_beta->size(); i++)
	{
		std::cout << " " << (*m_beta)[i];
	}
	std::cout << " )" << std::endl;
}

void LinearRegression::SumsOfSquares(Dataset *dataset, double &ess, double &rss, double &tss) const
{
	assert(dataset->GetDim() == m_dataset->GetDim());
	int d = dataset->GetDim() - 1;
	int n = dataset->GetNbrSamples();
	//on commence par créer la matrice X, le vecteur y et le vecteur résultat
	Eigen::VectorXd y(n);
	Eigen::MatrixXd X(n, d);

	//on rempli les deux
	for (int j = 0; j < d + 1; j++)
	{
		for (int i = 0; i < n; i++)
		{
			if (j < m_col_regr)
			{
				X(i, j) = dataset->GetInstance(i)[j];
			}
			else if (j == m_col_regr)
			{
				y[i] = dataset->GetInstance(i)[j];
			}
			else
			{
				X(i, j - 1) = dataset->GetInstance(i)[j];
			}
		}
	}

	double beta0 = (*GetCoefficients())(0);
	Eigen::VectorXd beta2 = GetCoefficients()->tail(d);

	Eigen::VectorXd *temp = new Eigen::VectorXd(n);
	Eigen::VectorXd *pred = new Eigen::VectorXd(n);
	Eigen::VectorXd *ybarre = new Eigen::VectorXd(n);
	*temp = X * (beta2);

	for (int i = 0; i < n; i++)
	{
		(*pred)(i) = (*temp)(i) + beta0;
	}

	double somme = 0;
	for (int i = 0; i < n; i++)
	{
		somme += y(i);
	}
	for (int i = 0; i < n; i++)
	{
		(*ybarre)(i) = somme / ((double)n);
	}

	tss = (y - *ybarre).squaredNorm();
	ess = (*pred - *ybarre).squaredNorm();
	rss = (y - *pred).squaredNorm();
}

double LinearRegression::Estimate(const Eigen::VectorXd &x) const
{
	const Eigen::VectorXd *beta = GetCoefficients();
	double est = (*beta)(0);
	for (int i = 0; i < m_dataset->GetDim() - 1; i++)
	{
		est += x(i) * (*beta)(i + 1);
	}
	return est;
}

void LinearRegression::to_csv()
{
	int d = this->GetDataset()->GetDim();
	int n = this->GetDataset()->GetNbrSamples();

	std::string file_name("../../p1-power-weather/weather/output_linear_");
	file_name.append(this->GetDataset()->m_dataset_name);
	file_name.append(std::string(".csv"));

	std::ofstream stream(file_name.c_str());

	if (stream.fail())
	{
		std::cout << "Cannot write into file " << file_name << " !" << std::endl;
		exit(1);
	}
	// on remet les titres :
	for (int i = 0; i < d - 1; i++)
	{
		stream << this->GetDataset()->columns[i] << ",";
	}
	stream << "(predicted)" << this->GetDataset()->columns[d - 1] << std::endl;

	//on fait la prédiction

	Eigen::MatrixXd X(n, d - 1);

	//on rempli les deux
	for (int j = 0; j < d - 1; j++)
	{
		for (int i = 0; i < n; i++)
		{
			X(i, j) = this->GetDataset()->GetInstance(i)[j]; // simplified because regression column is the last
		}
	}

	double beta0 = (*GetCoefficients())(0);
	Eigen::VectorXd beta2 = GetCoefficients()->tail(d - 1);

	Eigen::VectorXd *temp = new Eigen::VectorXd(n);
	Eigen::VectorXd *pred = new Eigen::VectorXd(n);
	*temp = X * (beta2);

	for (int i = 0; i < n; i++)
	{
		(*pred)(i) = (*temp)(i) + beta0;
	}

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < d - 1; j++)
		{
			stream << this->GetDataset()->GetInstance(i)[j] << ",";
		}
		stream << (*pred)(i) << std::endl;
	}
}

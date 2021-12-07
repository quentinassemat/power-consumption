#include "KnnRegression.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

KnnRegression::KnnRegression(int k, Dataset *dataset, int col_regr)
    : Regression(dataset, col_regr)
{
    m_k = k;
    int d = dataset->GetDim() - 1;
    int nPts = dataset->GetNbrSamples();

    ANNpointArray dataPts;
    dataPts = annAllocPts(nPts, d); // on met -1 car une colonne du dataset (col_class) sert de label et non de coordonnées

    //remplissage du tableau :
    for (int i = 0; i < nPts; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if (j < col_regr)
            {
                dataPts[i][j] = dataset->GetInstance(i)[j];
            }
            else
            {
                dataPts[i][j] = dataset->GetInstance(i)[j + 1];
            }
        }
    }

    //construction de l'arbre :
    ANNkd_tree *kdTree = new ANNkd_tree(dataPts, nPts, d);

    this->m_kdTree = kdTree;
    this->m_dataPts = dataPts;
}

KnnRegression::~KnnRegression()
{
    annDeallocPts(this->m_dataPts);
    delete m_kdTree;
    annClose();
}

double KnnRegression::Estimate(const Eigen::VectorXd &x) const
{
    assert(x.size() == m_dataset->GetDim() - 1);
    int d = m_dataset->GetDim() - 1;
    ANNidxArray nnIdx = new ANNidx[this->m_k];
    ANNdistArray dists = new ANNdist[this->m_k];
    ANNpoint queryPt = annAllocPt(d);
    for (int i = 0; i < d; i++)
    {
        queryPt[i] = x[i];
    }

    this->m_kdTree->annkSearch(queryPt, this->m_k, nnIdx, dists); //dist are squared at that time

    double temp = 0;
    for (int i = 0; i < this->m_k; i++)
    {
        temp += this->m_dataset->GetInstance(nnIdx[i])[this->m_col_regr];
    }
    temp = temp / (this->m_k);

    return temp;
}

int KnnRegression::GetK() const
{
    return m_k;
}

ANNkd_tree *KnnRegression::GetKdTree() const
{
    return m_kdTree;
}

void KnnRegression::to_csv(Dataset * dataset)
{
    int d = this->GetDataset()->GetDim();
    int n = dataset->GetNbrSamples();

    std::string file_name("../../p1-power-weather/weather/output_knn_");
    file_name.append(dataset->m_dataset_name);
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

    // on fait la prédiction

    //Eigen::VectorXd *pred = new Eigen::VectorXd(n);
    std::vector<double> * pred_knn = new std::vector<double>;
    pred_knn->resize(n);

    for (int i = 0; i < n; i++)
    {
        //std::vector<double> sample = dataset.GetInstance(i);
        // extract column for regression
        Eigen::VectorXd query(dataset->GetDim() - 1);

        for (int j = 0; j < dataset->GetDim() - 1; j++)
        {
            query(j) = dataset->GetInstance(i)[j]; // dataset.GetInstance(i)[j]; // simplified as our regression column is at the end of the dataset;
        }
        (*pred_knn)[i] = this->Estimate(query);
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < d - 1; j++)
        {
            stream << this->GetDataset()->GetInstance(i)[j] << ",";
        }
        stream << (*pred_knn)[i] << std::endl;
    }
}

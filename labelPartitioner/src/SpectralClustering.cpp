//
// Created by matteo on 27/02/19.
//

#include <SpectralClustering.h>
#include <eigen3/Eigen/Dense>
#include <iostream>
#include <opencv2/core/eigen.hpp>
#include <opencv2/core/types_c.h>

std::vector<std::vector<gNodeDsc>> SpectralClustering::spectralClustering(const Eigen::MatrixXd &simMatrix, unsigned short k)
{
    (simMatrix.array() == 0).rowwise().all();

    Eigen::MatrixXd normalizedLaplacian = computeNormLaplacian(simMatrix);
    Eigen::MatrixXd reducedEigenvectors = computeKOrderedEigenvectors(normalizedLaplacian,k);
    return clusterKMeans(reducedEigenvectors,k);
}

Eigen::MatrixXd SpectralClustering::computeNormLaplacian(const Eigen::MatrixXd &simMatrix)
{
    Eigen::MatrixXd degrees = Eigen::MatrixXd::Zero(simMatrix.rows(),simMatrix.cols());
    Eigen::MatrixXd ones = Eigen::MatrixXd::Identity(simMatrix.rows(),simMatrix.cols());

    for (unsigned int i = 0; i < simMatrix.cols(); ++i)
    {
        degrees(i,i) = 1.0/(sqrt(simMatrix.row(i).sum()));
    }

    Eigen::MatrixXd symmLaplacian = ones - degrees*simMatrix*degrees;
    std::cout << symmLaplacian << std::endl;

    return symmLaplacian;
}

Eigen::MatrixXd SpectralClustering::computeKOrderedEigenvectors(const Eigen::MatrixXd &normLaplacian, unsigned short k)
{
    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> solver(normLaplacian);
    Eigen::MatrixXd eigenvectors = solver.eigenvectors();

    Eigen::MatrixXd cutEigenvectors;
    if(eigenvectors.cols() > k)
    {
        cutEigenvectors = eigenvectors.block(0,0,eigenvectors.rows(),k);
    }
    else
    {
        cutEigenvectors = eigenvectors;
    }

    cutEigenvectors.rowwise().normalize();
    return cutEigenvectors;
}

std::vector<std::vector<gNodeDsc>> SpectralClustering::clusterKMeans(const Eigen::MatrixXd &reducedEigenvec, unsigned short k)
{
    unsigned int numElements = reducedEigenvec.rows();
    unsigned short numFeatures = reducedEigenvec.cols();

    cv::Mat eigenvectors(numElements, numFeatures, CV_32F);
    cv::eigen2cv(reducedEigenvec,eigenvectors);

    cv::Mat labels, centers;
    unsigned int attempts = 50;
    eigenvectors.convertTo(eigenvectors,CV_32F);
    cv::kmeans(eigenvectors,k,labels,cv::TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 10000, 0.0001), attempts, cv::KMEANS_PP_CENTERS, centers);

    std::vector<std::vector<gNodeDsc>> newClusters(k);
    for(unsigned int i = 0; i < numElements; ++i)
    {
        newClusters[labels.at<int>(i)].emplace_back(i);
    }

    for(unsigned int i = 0; i < k; ++i)
    {
       std::cout << std::endl;
       for(auto dsc : newClusters[i])
           std::cout << dsc << " ";
    }

    return newClusters;
}

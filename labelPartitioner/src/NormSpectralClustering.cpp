//
// Created by matteo on 27/02/19.
//

#include <NormSpectralClustering.h>
#include <eigen3/Eigen/Dense>
#include <iostream>
#include <opencv2/core/eigen.hpp>
#include <opencv2/core/types_c.h>

void NormSpectralClustering::normalizedSpectralClustering(const Eigen::MatrixXd &simMatrix, unsigned short k)
{
    Eigen::MatrixXd normalizedLaplacian = computeNormLaplacian(simMatrix);
    Eigen::MatrixXd reducedEigenvectors = computeKOrderedEigenvectors(normalizedLaplacian,k);
    clusterKMeans(reducedEigenvectors,k);
}

Eigen::MatrixXd NormSpectralClustering::computeNormLaplacian(const Eigen::MatrixXd &simMatrix)
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

Eigen::MatrixXd NormSpectralClustering::computeKOrderedEigenvectors(const Eigen::MatrixXd &normLaplacian, unsigned short k)
{
    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> solver(normLaplacian);

    Eigen::VectorXd eigenvalues = solver.eigenvalues();
    Eigen::MatrixXd eigenvectors = solver.eigenvectors();
    std::cout << eigenvalues << std::endl;
    std::cout << eigenvectors << std::endl << std::endl;

    /*for (unsigned short i = 0; i < eigenvalues.cols() - 1; ++i)
    {
        unsigned short maxEigenvalueIdx;
        eigenvalues.segment(i,eigenvalues.cols() - i).maxCoeff(&maxEigenvalueIdx);

        if(maxEigenvalueIdx > 0)
        {
            std::swap(eigenvalues[i],eigenvalues[maxEigenvalueIdx+i]);
            eigenvectors.col(i).swap(eigenvectors.col(maxEigenvalueIdx+i));
        }
    }*/

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
    std::cout << cutEigenvectors << std::endl;
    return cutEigenvectors;
}

void NormSpectralClustering::clusterKMeans(const Eigen::MatrixXd &reducedEigenvec, unsigned short k)
{
    unsigned int numElements = reducedEigenvec.rows();
    unsigned short numFeatures = reducedEigenvec.cols();

    cv::Mat eigenvectors(numElements, numFeatures, CV_32F);
    cv::eigen2cv(reducedEigenvec,eigenvectors);
    std::cout << eigenvectors << std::endl;

    cv::Mat labels, centers;
    unsigned int attempts = 50;
    eigenvectors.convertTo(eigenvectors,CV_32F);
    cv::kmeans(eigenvectors,k,labels,cv::TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 10000, 0.0001), attempts, cv::KMEANS_PP_CENTERS, centers);

    std::cout << std::endl;
    std::cout << "LABELS: " << std::endl;
    std::cout << labels << std::endl;
}

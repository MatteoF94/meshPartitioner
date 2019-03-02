//
// Created by matteo on 27/02/19.
//

#ifndef MESHPARTITIONER_NORMSPECTRALCLUSTERING_H
#define MESHPARTITIONER_NORMSPECTRALCLUSTERING_H

#include <eigen3/Eigen/Core>
#include <types.h>

class SpectralClustering {
public:
    std::vector<std::vector<gNodeDsc>> spectralClustering(const Eigen::MatrixXd &simMatrix, unsigned short k);

private:
    Eigen::MatrixXd computeNormLaplacian(const Eigen::MatrixXd &simMatrix);
    Eigen::MatrixXd computeKOrderedEigenvectors(const Eigen::MatrixXd &normLaplacian, unsigned short k);
    std::vector<std::vector<gNodeDsc>> clusterKMeans(const Eigen::MatrixXd &reducedEigenvec, unsigned short k);
};


#endif //MESHPARTITIONER_NORMSPECTRALCLUSTERING_H

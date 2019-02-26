/********************************************
 *
 * Copyright (C) 2018 Matteo Frosi
 *
 ********************************************/

/**
 * @file SimMatrixHandler.h
 * @author Matteo Frosi
 * @date 26 feb 2019
 */

#ifndef MESHPARTITIONER_SIMMATRIXHANDLER_H
#define MESHPARTITIONER_SIMMATRIXHANDLER_H

#include <types.h>
#include <eigen3/Eigen/Core>

/**
 * @class SimMatrixHandler
 * @details Performs all the operations needed to handle similarity matrices and to associate them to a graph.
 */
class SimMatrixHandler {
public:

    /**
     * @param filename Name of the file to read, containing a similarity matrix between labels.
     * @return
     */
    Eigen::MatrixXd readMatrix(std::string &filename);

    /**
     * @brief Builds the similarity matrix of a graph using the labels of its nodes and a similarity matrix for the labels.
     *        Different labels similarity matrices produces different graph similarity matrices from the same graph.
     * @param g The considered graph.
     * @param nodeLabels The labels associated to the nodes of the graph.
     * @param labelsSimMatrix Similarity matrix that describes similarities between labels.
     * @return The similarity matrix of the graph.
     */
    Eigen::MatrixXd buildGraphSimMatrix(const Graph &g, const std::vector<unsigned int> &nodeLabels, const Eigen::MatrixXd &labelsSimMatrix);
};


#endif //MESHPARTITIONER_SIMMATRIXHANDLER_H

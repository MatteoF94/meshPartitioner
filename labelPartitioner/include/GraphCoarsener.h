//
// Created by matteo on 28/02/19.
//

#ifndef MESHPARTITIONER_GRAPHCOARSENER_H
#define MESHPARTITIONER_GRAPHCOARSENER_H

#include <types.h>
#include <eigen3/Eigen/Core>

class GraphCoarsener {
public:
    std::vector<std::vector<gNodeDsc>> coarsen(const Graph &g, unsigned int maxNumNodes, Graph &coarsenedGraph);
    void similarEdgeMatching(Graph &tmpGraph, std::vector<std::vector<gNodeDsc>> &clusters, Graph &coarsenedGraph);
private:
    void createCoarseGraph(Graph &tmpGraph, std::vector<unsigned int> &perm, const std::vector<std::vector<gNodeDsc>> newNodes, Graph &coarsenedGraph);
    void populateClusters(std::vector<std::vector<gNodeDsc>> &clusters, std::vector<std::vector<gNodeDsc>> &newClusters);
};


#endif //MESHPARTITIONER_GRAPHCOARSENER_H

/********************************************
 *
 * Copyright (C) 2018 Matteo Frosi
 *
 ********************************************/

/**
 * @file GraphReducer.h
 * @author Matteo Frosi
 * @date 22 feb 2019
 */

#ifndef MESHPARTITIONER_GRAPHREDUCER_H
#define MESHPARTITIONER_GRAPHREDUCER_H

#include <types.h>
#include <deque>

/**
 * @class GraphReducer
 * @details Given an undirected labeled graph, we can reduce it to a graph where every node is connected
 *          to only nodes corresponding to different labels. The new nodes correspond to connected nodes
 *          with the same label of the original graph.
 * @example Consider the undirected linear graph 1-1-0-2-1-1, where the numbers represent the labels of the
 *          nodes, the algorithm produces a new labeled graph 0-1-2-3 (0 corresponding to original label 1,
 *          1 to original label 0, 2 to 2 and 3 to 1 again).
 */
class GraphReducer {
public:

    /**
     * @brief Class constructor.
     */
    GraphReducer();

    /**
     * @brief Class constructor
     * @param isPartitionIterative Value that tells if the algorithm should run recursively or iteratively.
     */
    explicit GraphReducer(bool isPartitionIterative);

    /**
     * @brief Prepares the graph to be reduced and selects the algorithm to execute (iterative or recursive).
     * @param g Graph to reduce.
     * @param labels Labels associated to the graph nodes.
     * @param nodesGroup Correspondences between the graph node descriptors and the reduced graph descriptors.
     * @return The reduced graph.
     */
    Graph reduceLabeledGraph(const Graph &g, std::vector<unsigned int> &labels, std::vector<unsigned int> &nodesGroup);

private:
    /**
     * Structure to represent the labeled graph concept.
     */
    struct labeledGraphToReduce {
        const Graph &g;                         /**< the graph to reduce */
        std::vector<unsigned int> &labels;      /**< the labels of the graph nodes */
        std::vector<unsigned int> &nodesGroup;  /**< the assignments of nodes to the reduced graph */
        unsigned int currGroup;                 /**< current node of the reduced graph, used in the algorithms */

        labeledGraphToReduce(const Graph &gr,std::vector<unsigned int> &gLabels, std::vector<unsigned int> &nodesGroup) :
                g(gr),
                labels(gLabels),
                nodesGroup(nodesGroup),
                currGroup(0) {}
    };

    /**
     * The enumeration relative to the way the reducing algorithm executes.
     */
    enum class ReductionMode {RECURSIVE, ITERATIVE};

    /**
     * @brief Detects the connected components of graph g and associates to each component a map
     *        node descriptor - boolean to be used in the reduction algorithms to verify nodes expansion.
     * @param g The graph to be reduced.
     * @return A vector of maps, each to be used in the reduction algorithm to check if a node is expanded.
     */
    std::vector<std::unordered_map<gNodeDsc,bool>> prepareConnectedComponents(const Graph &g);

    /**
     * @brief Recursive algorithm to reduce the graph.
     * @param lgr Structure containing graph, labels and the assignments of nodes to the reduced graph.
     * @param nodeDsc Descriptor of the current node to explore.
     * @param reducedGraph The reduced graph.
     * @param expanded List of flags expanded/not expanded associated to the graph nodes.
     */
    void reduceRecursively(labeledGraphToReduce &lgr, gNodeDsc nodeDsc, Graph &reducedGraph,
                           std::unordered_map<gNodeDsc,bool> &expanded);

    /**
     * @brief Iterative algorithm to reduce the graph.
     * @param lgr Structure containing graph, labels and the assignments of nodes to the reduced graph.
     * @param nodeDsc Descriptor of the first explored node.
     * @param reducedGraph The reduced graph.
     * @param expanded List of flags expanded/not expanded associated to the graph nodes.
     */
    void reduceIteratively(labeledGraphToReduce &lgr, gNodeDsc nodeDsc, Graph &reducedGraph,
                           std::unordered_map<gNodeDsc,bool> &expanded);

    ReductionMode reductionMode; /**< the way the algorithm reduces a graph*/
};


#endif //MESHPARTITIONER_GRAPHREDUCER_H

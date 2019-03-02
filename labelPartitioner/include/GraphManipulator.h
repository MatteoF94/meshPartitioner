/********************************************
 *
 * Copyright (C) 2018 Matteo Frosi
 *
 ********************************************/

/**
 * @file GraphManipulator.h
 * @author Matteo Frosi
 * @date 26 feb 2019
 */

#ifndef MESHPARTITIONER_GRAPHMANIPULATOR_H
#define MESHPARTITIONER_GRAPHMANIPULATOR_H

#include <types.h>
#include <unordered_map>

class GraphManipulator
{
public:

    std::vector<std::vector<gNodeDsc>> mapElementsToReducedGraph(const Graph &g, const std::vector<unsigned int> &groups);
    std::vector<unsigned int> mapReducedGraphToLabels(const std::unordered_map<gNodeDsc,std::deque<unsigned int>> &reducedToOriginalMap, const std::vector<unsigned int> &labels);
    std::vector<unsigned int> findReducedGraphLabels(const std::vector<std::vector<gNodeDsc>> &reducedToOriginalNodes,
                                                     const std::vector<unsigned int> &labels);
};


#endif //MESHPARTITIONER_GRAPHMANIPULATOR_H

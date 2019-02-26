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

    std::unordered_map<gNodeDsc,std::deque<unsigned int>> mapElementsToReducedGraph(const Graph &g, const std::vector<unsigned int> &groups);
    std::vector<unsigned int> mapReducedGraphToLabels(const std::unordered_map<gNodeDsc,std::deque<unsigned int>> &reducedToOriginalMap, const std::vector<unsigned int> &labels);
};


#endif //MESHPARTITIONER_GRAPHMANIPULATOR_H

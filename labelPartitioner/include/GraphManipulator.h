//
// Created by matteo on 25/02/19.
//

#ifndef MESHPARTITIONER_GRAPHMANIPULATOR_H
#define MESHPARTITIONER_GRAPHMANIPULATOR_H

#include <types.h>
#include <unordered_map>

class GraphManipulator
{
public:


    std::unordered_map<gNodeDsc,std::deque<unsigned int>> mapElementsToReducedGraph(const Graph &g, const std::vector<unsigned int> &groups);

};


#endif //MESHPARTITIONER_GRAPHMANIPULATOR_H

//
// Created by matteo on 25/02/19.
//

#include <GraphManipulator.h>

std::unordered_map<gNodeDsc,std::deque<unsigned int>> GraphManipulator::mapElementsToReducedGraph(const Graph &g, const std::vector<unsigned int> &groups)
{
    std::unordered_map<gNodeDsc,std::deque<unsigned int>> reducedToOriginalMap;

    for(size_t i = 0; i < boost::num_vertices(g); ++i)
    {
        std::deque<unsigned int> newDeque;
        reducedToOriginalMap.insert({i,newDeque});
    }

    for(unsigned int i = 0; i < groups.size(); ++i)
    {
        unsigned int currGroup = groups[i];
        std::deque<unsigned int>* originalElements = &reducedToOriginalMap[currGroup];
        originalElements->emplace_back(i);
    }

    for(size_t i = 0; i < boost::num_vertices(g); ++i)
    {
        std::deque<unsigned int> originalElements = reducedToOriginalMap[i];
        std::cout << "Reduced node " << i << " has original elements: ";
        for(auto element : originalElements)
        {
            std::cout << element << " ";
        }
        std::cout << std::endl;
    }

    return reducedToOriginalMap;
}

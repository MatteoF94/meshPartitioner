//
// Created by matteo on 25/02/19.
//

#include <GraphManipulator.h>

std::vector<std::vector<gNodeDsc>> GraphManipulator::mapElementsToReducedGraph(const Graph &g, const std::vector<unsigned int> &groups)
{
    std::vector<std::vector<gNodeDsc>> reducedToOriginalNodes(boost::num_vertices(g));
    for(unsigned int i = 0; i < groups.size(); ++i)
    {
        reducedToOriginalNodes[groups[i]].emplace_back(i);
    }

    std::cout << "going to return" << std::endl;
    return reducedToOriginalNodes;
    
    /*std::unordered_map<gNodeDsc,std::deque<unsigned int>> reducedToOriginalMap;
    
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
    }*/
}

std::vector<unsigned int> GraphManipulator::mapReducedGraphToLabels(
        const std::unordered_map<gNodeDsc, std::deque<unsigned int>> &reducedToOriginalMap,
        const std::vector<unsigned int> &labels)
{
    std::vector<unsigned int> reducedNodesLabels(reducedToOriginalMap.size(),0);

    for (auto &mapIt : reducedToOriginalMap)
    {
        gNodeDsc currReducedNode = mapIt.first;
        std::deque<unsigned int> currNodes = mapIt.second;
        unsigned int currLabel = labels[currNodes.back()];
        reducedNodesLabels[currReducedNode] = currLabel;
    }

    return reducedNodesLabels;
}

std::vector<unsigned int> GraphManipulator::findReducedGraphLabels(const std::vector<std::vector<gNodeDsc>> &reducedToOriginalNodes,
                                                 const std::vector<unsigned int> &labels)
{
    std::vector<unsigned int> reducedGraphLabels(reducedToOriginalNodes.size());
    
    for(unsigned int i = 0; i < reducedToOriginalNodes.size(); ++i)
    {
        gNodeDsc oneNode = reducedToOriginalNodes[i].back();
        unsigned int reducedNodeLabel = labels[oneNode];
        reducedGraphLabels[i] = reducedNodeLabel;
    }
    
    return reducedGraphLabels;
}


//
// Created by matteo on 28/02/19.
//

#include <GraphCoarsener.h>
#include <boost/graph/graph_utility.hpp>

std::vector<std::vector<gNodeDsc>> GraphCoarsener::coarsen(const Graph &g, unsigned int maxNumNodes, Graph &coarsenedGraph)
{
    Graph tmpGraph = g;

    std::vector<std::vector<gNodeDsc>> clusters;

    while(boost::num_vertices(tmpGraph) > maxNumNodes)
    {
        std::vector<std::vector<gNodeDsc>> currClusters;

        similarEdgeMatching(tmpGraph,currClusters,coarsenedGraph);

        tmpGraph = coarsenedGraph;
        if(!clusters.empty())
        {
            populateClusters(clusters,currClusters);
        }
        else
        {
            clusters = currClusters;
        }
    }

    return clusters;
}

void
GraphCoarsener::similarEdgeMatching(Graph &tmpGraph, std::vector<std::vector<gNodeDsc>> &clusters, Graph &coarsenedGraph)
{
    unsigned int numNodes = boost::num_vertices(tmpGraph);
    std::vector<bool> isMatched(numNodes,false);
    std::vector<gNodeDsc> nodeDscs(numNodes);
    std::iota(nodeDscs.begin(),nodeDscs.end(),1);
    std::vector<unsigned int> perm(numNodes);

    // Shuffle the graph nodes
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(nodeDscs.begin(),nodeDscs.end(),g);

    // Reduce the graph pairing a node with an adjacent node such that the edge connecting them has max weight
    std::vector<std::pair<gNodeDsc,gNodeDsc>> matchedEdges;
    for(unsigned int i = 0; i < numNodes; ++i)
    {
        gNodeDsc currNodeDsc = nodeDscs[i] - 1;

        if(!isMatched[currNodeDsc])
        {
            gNodeDsc maxWeightedNode = 0;
            double maxWeight = 0;
            unsigned short numAdjacentNodes = 0;

            boost::graph_traits<Graph>::adjacency_iterator aib,aie;
            for(boost::tie(aib,aie) = boost::adjacent_vertices(currNodeDsc,tmpGraph); aib != aie; ++aib)
            {
                if(!isMatched[*aib])
                {
                    std::pair<Graph::edge_descriptor, bool> edge = boost::edge(currNodeDsc, *aib, tmpGraph);
                    double weight = boost::get(boost::edge_weight_t(), tmpGraph, edge.first);
                    if (weight > maxWeight) {
                        maxWeight = weight;
                        maxWeightedNode = *aib;
                    }
                    ++numAdjacentNodes;
                }
            }

            if(numAdjacentNodes != 0)
            {
                isMatched[currNodeDsc] = true;
                isMatched[maxWeightedNode] = true;
                matchedEdges.emplace_back(std::make_pair(currNodeDsc,maxWeightedNode));
            }
        }
    }

    // Build the new clusters considering also the not-reduced vertices
    unsigned int reducedNodeCurrDsc = 0;
    for(auto it : matchedEdges)
    {
        perm[it.first] = reducedNodeCurrDsc;
        perm[it.second] = reducedNodeCurrDsc;

        std::vector<gNodeDsc> currMatch = {it.first,it.second};
        clusters.emplace_back(currMatch);

        reducedNodeCurrDsc++;
    }

    for(unsigned int i = 0; i < isMatched.size(); ++i)
    {
        if(!isMatched[i])
        {
            perm[i] = reducedNodeCurrDsc;

            std::vector<gNodeDsc> currMatch = {i};
            clusters.emplace_back(currMatch);

            reducedNodeCurrDsc++;
        }
    }

    Graph newGraph(reducedNodeCurrDsc);
    coarsenedGraph.swap(newGraph);
    createCoarseGraph(tmpGraph,perm,clusters,coarsenedGraph);
}

void GraphCoarsener::createCoarseGraph(Graph &tmpGraph, std::vector<unsigned int> &perm, const std::vector<std::vector<gNodeDsc>> newNodes, Graph &coarsenedGraph)
{
    for(unsigned int coarseNodeIdx = 0; coarseNodeIdx < newNodes.size(); ++coarseNodeIdx)
    {
        std::vector<gNodeDsc> coarseNodeClusts = newNodes[coarseNodeIdx];

        for(auto nodeDsc : coarseNodeClusts)
        {
            boost::graph_traits<Graph>::adjacency_iterator aib,aie;

            for(boost::tie(aib,aie) = boost::adjacent_vertices(nodeDsc,tmpGraph); aib != aie; ++aib)
            {
                if (coarseNodeIdx != perm[*aib])
                {
                    std::pair<Graph::edge_descriptor, bool> edge = boost::edge(coarseNodeIdx, perm[*aib], coarsenedGraph);
                    std::pair<Graph::edge_descriptor, bool> orEdge = boost::edge(nodeDsc, *aib, tmpGraph);
                    double weightToMul = boost::get(boost::edge_weight_t(), tmpGraph, orEdge.first);

                    if (edge.second)
                    {
                        double oldWeight = boost::get(boost::edge_weight_t(),coarsenedGraph, edge.first);
                        boost::put(boost::edge_weight_t(), coarsenedGraph, edge.first, oldWeight*weightToMul);
                    }
                    else
                    {
                        boost::add_edge(coarseNodeIdx, perm[*aib], weightToMul, coarsenedGraph);
                    }
                }
            }
        }
    }

    boost::graph_traits<Graph>::edge_iterator eib,eie;
    for(boost::tie(eib,eie) = boost::edges(coarsenedGraph); eib != eie; ++eib)
    {
        double currWeight = boost::get(boost::edge_weight_t(),coarsenedGraph,*eib);
        boost::put(boost::edge_weight_t(), coarsenedGraph, *eib, sqrt(currWeight));
        std::cout << boost::get(boost::edge_weight_t(),coarsenedGraph,*eib) << std::endl;
    }

    /*boost::property_map<Graph,boost::edge_weight_t>::type EdgeWeightMap = boost::get(boost::edge_weight_t(),coarsenedGraph);
    std::pair<boost::graph_traits<Graph>::edge_iterator,boost::graph_traits<Graph>::edge_iterator> edgePair;
    for(edgePair = boost::edges(coarsenedGraph);edgePair.first != edgePair.second; ++edgePair.first)
        std::cout << *edgePair.first << " " << EdgeWeightMap[*edgePair.first] << std::endl;*/
}

void GraphCoarsener::populateClusters(std::vector<std::vector<gNodeDsc>> &clusters, std::vector<std::vector<gNodeDsc>> &newClusters)
{
    for(unsigned int i = 0; i < newClusters.size(); ++i)
    {
        std::vector<gNodeDsc > tmpNodes = newClusters[i];
        newClusters[i].clear();

        for(auto tmpNode : tmpNodes)
        {
            newClusters[i].insert(newClusters[i].end(),clusters[tmpNode].begin(),clusters[tmpNode].end());
        }
    }

    clusters = newClusters;
}

//
// Created by matteo on 22/02/19.
//

#include <GraphReducer.h>
#include <stack>
#include <boost/graph/connected_components.hpp>

GraphReducer::GraphReducer() : reductionMode(ReductionMode::ITERATIVE) {}

GraphReducer::GraphReducer(bool isPartitionIterative)
{
    if(isPartitionIterative)
    {
        reductionMode = ReductionMode::ITERATIVE;
    }
    else
    {
        reductionMode = ReductionMode::RECURSIVE;
    }
}

Graph GraphReducer::reduceLabeledGraph(const Graph &g, std::vector<unsigned int> &labels,
                                       std::vector<unsigned int> &nodesGroup)
{
    labeledGraphToReduce labeledGraph(g,labels,nodesGroup);
    Graph reducedGraph;
    std::vector<std::unordered_map<gNodeDsc,bool>> mapsExpandedNodes = prepareConnectedComponents(g);

    if(reductionMode == ReductionMode::RECURSIVE)
    {
        for(auto expanded : mapsExpandedNodes)
        {
            gNodeDsc initNodeDsc = expanded.begin()->first;
            expanded[initNodeDsc] = true;
            labeledGraph.nodesGroup[initNodeDsc] = labeledGraph.currGroup;
            reduceRecursively(labeledGraph,initNodeDsc,reducedGraph,expanded);
            labeledGraph.currGroup++;
        }
    }
    else
    {
        for(auto expanded : mapsExpandedNodes)
        {
            gNodeDsc initNodeDsc = expanded.begin()->first;
            expanded[initNodeDsc] = true;
            labeledGraph.nodesGroup[initNodeDsc] = labeledGraph.currGroup;
            reduceIteratively(labeledGraph,initNodeDsc,reducedGraph,expanded);
            labeledGraph.currGroup++;
        }
    }

    return reducedGraph;
}

std::vector<std::unordered_map<gNodeDsc,bool>> GraphReducer::prepareConnectedComponents(const Graph &g)
{
    std::vector<unsigned int> components(boost::num_vertices(g));
    unsigned int numComponents = boost::connected_components(g, &components[0]);

    std::vector<std::unordered_map<gNodeDsc, bool>> mapsExploredNodes(numComponents);
    for (size_t i = 0; i < boost::num_vertices(g); ++i)
    {
        unsigned int currComponent = components[i];
        mapsExploredNodes[currComponent].insert({i,false});
    }

    return mapsExploredNodes;
}

void GraphReducer::reduceRecursively(GraphReducer::labeledGraphToReduce &lpg, gNodeDsc nodeDsc, Graph &reducedGraph,
                                     std::unordered_map<gNodeDsc,bool> &expanded)
{
    std::list<boost::graph_traits<Graph>::vertex_descriptor> nodesSameLabel, nodesDiffLabel;
    boost::graph_traits<Graph>::adjacency_iterator aib,aie;

    for(boost::tie(aib,aie) = boost::adjacent_vertices(nodeDsc,lpg.g); aib != aie; ++aib)
    {
        if(!expanded[*aib])
        {
            if(lpg.labels[*aib] == lpg.labels[nodeDsc])
            {
                nodesSameLabel.emplace_back(*aib);
                lpg.nodesGroup[*aib] = lpg.nodesGroup[nodeDsc];
            }
            else
            {
               nodesDiffLabel.emplace_back(*aib);
            }
        }
    }

    for(auto dscSameLabel : nodesSameLabel)
    {
        if(!expanded[dscSameLabel])
        {
            expanded[dscSameLabel] = true;
            reduceRecursively(lpg, dscSameLabel, reducedGraph, expanded);
        }
    }

    for(auto dscDiffLabel : nodesDiffLabel)
    {
        if(!expanded[dscDiffLabel])
        {
            ++lpg.currGroup;
            boost::add_vertex(reducedGraph);
            lpg.nodesGroup[dscDiffLabel] = lpg.currGroup;
            expanded[dscDiffLabel] = true;
            reduceRecursively(lpg, dscDiffLabel, reducedGraph, expanded);
        }
        boost::add_edge(lpg.nodesGroup[nodeDsc],lpg.nodesGroup[dscDiffLabel],reducedGraph);
    }
}

/*void GraphReducer::reduceIteratively(GraphReducer::labeledGraphToReduce &lpg, gNodeDsc nodeDsc, Graph &reducedGraph,
                                     std::unordered_map<gNodeDsc,bool> &expanded)
{
    struct SnapshotIteration {
        gNodeDsc currNodeDsc;
        std::stack<gNodeDsc> nodesSameLabel;
        std::stack<gNodeDsc> nodesDiffLabel;
        unsigned short stage;

        explicit SnapshotIteration(gNodeDsc dsc) : currNodeDsc(dsc), stage(0) {}
    };

    std::stack<SnapshotIteration> snapshotsStack;
    SnapshotIteration firstSnapshot(nodeDsc);
    snapshotsStack.emplace(firstSnapshot);

    lpg.nodesGroup[nodeDsc] = lpg.currGroup;

    while(!snapshotsStack.empty())
    {
        SnapshotIteration *currSnapshot = &snapshotsStack.top();
        expanded[currSnapshot->currNodeDsc] = true;
        boost::graph_traits<Graph>::adjacency_iterator aib,aie;

        switch (currSnapshot->stage)
        {
            case 0:
                for(boost::tie(aib,aie) = boost::adjacent_vertices(currSnapshot->currNodeDsc,lpg.g); aib != aie; ++aib)
                {
                    if(!expanded[*aib])
                    {
                        if(lpg.labels[*aib] == lpg.labels[currSnapshot->currNodeDsc])
                        {
                            currSnapshot->nodesSameLabel.emplace(*aib);
                            lpg.nodesGroup[*aib] = lpg.nodesGroup[currSnapshot->currNodeDsc];
                        }
                        else
                        {
                            currSnapshot->nodesDiffLabel.emplace(*aib);
                        }
                    }
                }

                currSnapshot->stage = 1;
                break;

            case 1:
                if(!currSnapshot->nodesSameLabel.empty())
                {
                    gNodeDsc currDsc = currSnapshot->nodesSameLabel.top();
                    if(!expanded[currDsc])
                    {
                            SnapshotIteration newSnapshot(currDsc);
                            snapshotsStack.emplace(newSnapshot);
                    }
                    currSnapshot->nodesSameLabel.pop();
                }
                else
                {
                    currSnapshot->stage = 2;
                }

                break;

            case 2:
                if(!currSnapshot->nodesDiffLabel.empty())
                {
                    gNodeDsc currDsc = currSnapshot->nodesDiffLabel.top();
                    if(!expanded[currDsc])
                    {
                        ++lpg.currGroup;
                        boost::add_vertex(reducedGraph);
                        lpg.nodesGroup[currDsc] = lpg.currGroup;
                        expanded[currDsc] = true;

                        SnapshotIteration newSnapshot(currDsc);
                        snapshotsStack.emplace(newSnapshot);
                    }

                    boost::add_edge(lpg.nodesGroup[currSnapshot->currNodeDsc],lpg.nodesGroup[currDsc],reducedGraph);
                    currSnapshot->nodesDiffLabel.pop();
                }
                else
                {
                   currSnapshot->stage = 3;
                }

                break;

            case 3:

                snapshotsStack.pop();
                break;

            default:
                reducedGraph.clear();
                return;
        }
    }
}*/

void GraphReducer::reduceIteratively(GraphReducer::labeledGraphToReduce &lpg, gNodeDsc nodeDsc, Graph &reducedGraph,
                                     std::unordered_map<gNodeDsc,bool> &expanded)
{
    struct SnapshotIteration {
        gNodeDsc currNodeDsc;
        std::stack<gNodeDsc> nodesSameLabel;
        std::list<gNodeDsc> nodesDiffLabel;
        std::list<gNodeDsc>::iterator nodesDiffLabelIt;
        unsigned short stage;

        explicit SnapshotIteration(gNodeDsc dsc) : currNodeDsc(dsc), stage(0) {}
    };

    std::list<SnapshotIteration> snapshotList;
    SnapshotIteration firstSnapshot(nodeDsc);
    snapshotList.emplace_back(firstSnapshot);
    std::list<SnapshotIteration>::iterator snapIt = snapshotList.begin();

    lpg.nodesGroup[nodeDsc] = lpg.currGroup;

    while(!snapshotList.empty())
    {
        boost::graph_traits<Graph>::adjacency_iterator aib,aie;

        switch (snapIt->stage)
        {
            case 0:
                for(boost::tie(aib,aie) = boost::adjacent_vertices(snapIt->currNodeDsc,lpg.g); aib != aie; ++aib)
                {
                    if(!expanded[*aib])
                    {
                        if(lpg.labels[*aib] == lpg.labels[snapIt->currNodeDsc])
                        {
                            snapIt->nodesSameLabel.emplace(*aib);
                            lpg.nodesGroup[*aib] = lpg.nodesGroup[snapIt->currNodeDsc];
                            expanded[*aib] = true;
                        }
                        else
                        {
                            snapIt->nodesDiffLabel.emplace_back(*aib);
                        }
                    }
                }

                if(!snapIt->nodesDiffLabel.empty())
                {
                    snapIt->nodesDiffLabelIt = snapIt->nodesDiffLabel.begin();
                }

                snapIt->stage = 1;
                break;

            case 1:
                if(!snapIt->nodesSameLabel.empty())
                {
                    while(!snapIt->nodesSameLabel.empty())
                    {
                        gNodeDsc currDsc = snapIt->nodesSameLabel.top();

                        SnapshotIteration newSnapshot(currDsc);
                        snapshotList.emplace_back(newSnapshot);

                        snapIt->nodesSameLabel.pop();
                    }
                }

                snapIt->stage = 2;
                if(std::next(snapIt,1) != snapshotList.end())
                    snapIt++;

                break;

            case 2:
                if(!snapIt->nodesDiffLabel.empty())
                {
                    if(snapIt->nodesDiffLabelIt == snapIt->nodesDiffLabel.end())
                    {
                        snapIt->stage = 3;
                        break;
                    }

                    gNodeDsc currDsc = *snapIt->nodesDiffLabelIt;
                    boost::add_edge(lpg.nodesGroup[snapIt->currNodeDsc],lpg.nodesGroup[currDsc],reducedGraph);
                    snapIt->nodesDiffLabelIt++;

                    if(!expanded[currDsc])
                    {
                        ++lpg.currGroup;
                        boost::add_vertex(reducedGraph);
                        lpg.nodesGroup[currDsc] = lpg.currGroup;
                        expanded[currDsc] = true;

                        SnapshotIteration newSnapshot(currDsc);
                        snapshotList.emplace_back(newSnapshot);

                        snapIt++;
                    }
                }
                else
                {
                    snapIt->stage = 3;
                }

                break;

            case 3:
                if(snapIt != snapshotList.begin())
                    snapIt--;
                snapshotList.pop_back();
                break;

            default:
                reducedGraph.clear();
                return;
        }
    }
}
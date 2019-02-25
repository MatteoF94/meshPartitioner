//
// Created by matteo on 25/02/19.
//

#include "MeshReducer.h"
#include <boost/property_map/property_map.hpp>
#include <CGAL/Polygon_mesh_processing/connected_components.h>
#include <stack>

MeshReducer::MeshReducer() : reductionMode(ReductionMode::ITERATIVE) {}

MeshReducer::MeshReducer(bool isPartitionIterative)
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

Graph MeshReducer::reduceLabeledMesh(const Mesh &m, std::vector<unsigned int> &labels,
                                     std::vector<unsigned int> &facesGroups)
{
    labeledMeshToReduce labeledMesh(m,labels,facesGroups);
    Graph reducedGraph;
    std::vector<std::unordered_map<mFaceDsc,bool>> mapsExpandedNodes = prepareConnectedComponents(m);

    if(reductionMode == ReductionMode::RECURSIVE)
    {
        for(auto expanded : mapsExpandedNodes)
        {
            mFaceDsc initFaceDsc = expanded.begin()->first;
            expanded[initFaceDsc] = true;
            labeledMesh.facesGroup[initFaceDsc] = labeledMesh.currGroup;
            reduceRecursively(labeledMesh, initFaceDsc, reducedGraph, expanded);
            labeledMesh.currGroup++;
        }
    }
    else
    {
        for(auto expanded : mapsExpandedNodes)
        {
            mFaceDsc initFaceDsc = expanded.begin()->first;
            expanded[initFaceDsc] = true;
            labeledMesh.facesGroup[initFaceDsc] = labeledMesh.currGroup;
            reduceIteratively(labeledMesh, initFaceDsc, reducedGraph, expanded);
            labeledMesh.currGroup++;
        }
    }

    return reducedGraph;
}

std::vector<std::unordered_map<mFaceDsc,bool>> MeshReducer::prepareConnectedComponents(const Mesh &m)
{
    Mesh::Property_map <Mesh::face_index, int64_t> fccmap;
    Mesh meshCopy = m;
    fccmap = meshCopy.add_property_map<Mesh::face_index, int64_t>("f:CC").first;
    unsigned int numComponents = CGAL::Polygon_mesh_processing::connected_components(meshCopy, fccmap);

    std::vector<std::unordered_map<mFaceDsc,bool>> mapsExploredFaces(numComponents);
    for (auto faceDsc : m.faces())
    {
        unsigned int currComponent = fccmap[faceDsc];
        mapsExploredFaces[currComponent].insert({faceDsc,false});
    }

    return mapsExploredFaces;
};

void MeshReducer::reduceRecursively(MeshReducer::labeledMeshToReduce &lmr, mFaceDsc faceDsc, Graph &reducedGraph,
                                    std::unordered_map<mFaceDsc,bool> &expanded)
{
    std::list<mFaceDsc> facesSameLabel, facesDiffLabel;
    CGAL::Face_around_face_iterator<Mesh> fafib,fafie;

    for(boost::tie(fafib,fafie) = CGAL::faces_around_face(lmr.m.halfedge(faceDsc),lmr.m); fafib != fafie; ++fafib)
    {
        if(*fafib != Mesh::null_face())
        {
            if (!expanded[*fafib])
            {
                if (lmr.labels[*fafib] == lmr.labels[faceDsc])
                {
                    facesSameLabel.emplace_back(*fafib);
                    lmr.facesGroup[*fafib] = lmr.facesGroup[faceDsc];
                }
                else
                {
                    facesDiffLabel.emplace_back(*fafib);
                }
            }
        }
    }

    for(auto dscSameLabel : facesSameLabel)
    {
        if(!expanded[dscSameLabel])
        {
            expanded[dscSameLabel] = true;
            reduceRecursively(lmr, dscSameLabel, reducedGraph, expanded);
        }
    }

    for(auto dscDiffLabel : facesDiffLabel)
    {
        if(!expanded[dscDiffLabel])
        {
            ++lmr.currGroup;
            boost::add_vertex(reducedGraph);
            lmr.facesGroup[dscDiffLabel] = lmr.currGroup;
            expanded[dscDiffLabel] = true;
            reduceRecursively(lmr, dscDiffLabel, reducedGraph, expanded);
        }
        boost::add_edge(lmr.facesGroup[faceDsc],lmr.facesGroup[dscDiffLabel],reducedGraph);
    }
}

void MeshReducer::reduceIteratively(MeshReducer::labeledMeshToReduce &lmr, mFaceDsc faceDsc, Graph &reducedGraph,
                                    std::unordered_map<mFaceDsc,bool> &expanded)
{
    struct SnapshotIteration {
        mFaceDsc currFaceDsc;
        std::stack<gNodeDsc> facesSameLabel;
        std::stack<gNodeDsc> facesDiffLabel;
        unsigned short stage;

        explicit SnapshotIteration(mFaceDsc dsc) : currFaceDsc(dsc), stage(0) {}
    };

    std::stack<SnapshotIteration> snapshotsStack;
    SnapshotIteration firstSnapshot(faceDsc);
    snapshotsStack.emplace(firstSnapshot);

    while(!snapshotsStack.empty())
    {
        SnapshotIteration *currSnapshot = &snapshotsStack.top();
        expanded[currSnapshot->currFaceDsc] = true;
        CGAL::Face_around_face_iterator<Mesh> fafib,fafie;

        switch (currSnapshot->stage)
        {
            case 0:
                for(boost::tie(fafib,fafie) = CGAL::faces_around_face(lmr.m.halfedge(currSnapshot->currFaceDsc),lmr.m); fafib != fafie; ++fafib)
                {
                    if(*fafib != Mesh::null_face())
                    {
                        if (!expanded[*fafib])
                        {
                            if (lmr.labels[*fafib] == lmr.labels[currSnapshot->currFaceDsc])
                            {
                                currSnapshot->facesSameLabel.emplace(*fafib);
                                lmr.facesGroup[*fafib] = lmr.facesGroup[currSnapshot->currFaceDsc];
                            }
                            else
                                {
                                currSnapshot->facesDiffLabel.emplace(*fafib);
                            }
                        }
                    }
                }

                currSnapshot->stage = 1;
                break;

            case 1:
                if(!currSnapshot->facesSameLabel.empty())
                {
                    auto currDsc = Mesh::face_index(static_cast<unsigned int>(currSnapshot->facesSameLabel.top()));
                    if(!expanded[currDsc])
                    {
                        SnapshotIteration newSnapshot(currDsc);
                        snapshotsStack.emplace(newSnapshot);
                    }
                    currSnapshot->facesSameLabel.pop();
                }
                else
                {
                    currSnapshot->stage = 2;
                }

                break;

            case 2:
                if(!currSnapshot->facesDiffLabel.empty())
                {
                    auto currDsc = Mesh::face_index(static_cast<unsigned int>(currSnapshot->facesDiffLabel.top()));
                    if(!expanded[currDsc])
                    {
                        ++lmr.currGroup;
                        boost::add_vertex(reducedGraph);
                        lmr.facesGroup[currDsc] = lmr.currGroup;
                        expanded[currDsc] = true;

                        SnapshotIteration newSnapshot(currDsc);
                        snapshotsStack.emplace(newSnapshot);
                    }

                    boost::add_edge(lmr.facesGroup[currSnapshot->currFaceDsc],lmr.facesGroup[currDsc],reducedGraph);
                    currSnapshot->facesDiffLabel.pop();
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
}
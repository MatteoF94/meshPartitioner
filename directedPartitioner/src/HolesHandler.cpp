//
// Created by matteo on 03/03/19.
//

#include <HolesHandler.h>
#include <stack>
#include <boost/circular_buffer.hpp>
#include <partitionTypes.h>

void HolesHandler::findHolesDsc(DirectedPartition &dirPart)
{
    std::stack<Mesh::face_index> faceDscStack;
    std::unordered_map<Mesh::face_index,Mesh::face_index> originOfFaceDsc;
    std::unordered_set<Mesh::face_index> isFaceVisited;
    bool exitCond = false;
    Mesh::face_index otherPartFaceDsc;

    faceDscStack.emplace(dirPart.lastFaceDsc);
    originOfFaceDsc.emplace({dirPart.lastFaceDsc, dirPart.lastFaceDsc});

    CGAL::Face_around_face_iterator<Mesh> fafib, fafie;
    while(!exitCond)
    {
        Mesh::face_index currFaceDsc = faceDscStack.top();
        faceDscStack.pop();
        isFaceVisited.insert(currFaceDsc);

        std::vector<Mesh::face_index> currFaceNeighs;
        for (boost::tie(fafib,fafie) = CGAL::faces_around_face(dirPart.mesh.halfedge(currFaceDsc),dirPart.mesh); fafib != fafie; ++fafib)
        {
            if(*fafib != Mesh::null_face() && dirPart.isFaceInPartition->operator[](*fafib))
            {
                currFaceNeighs.emplace_back(*fafib);
            }
        }

        boost::circular_buffer<Mesh::face_index> currFaceCircularNeighs(currFaceNeighs.size());
        currFaceCircularNeighs.assign(currFaceNeighs.begin(),currFaceNeighs.end());

        Mesh::face_index currFaceOriginDsc = originOfFaceDsc[currFaceDsc];
        while(*currFaceCircularNeighs.begin() != currFaceOriginDsc)
        {
            currFaceCircularNeighs.rotate(currFaceCircularNeighs.begin() + 1);
        }

        for(unsigned int i = 0; i < currFaceCircularNeighs.size(); ++i)
        {
            if(dirPart.isFaceExplored->operator[](currFaceCircularNeighs[i]))
            {
                if(isFaceVisited.find(currFaceCircularNeighs[i]).operator->() == isFaceVisited.end().operator->())
                {
                    faceDscStack.emplace(currFaceCircularNeighs[i]);
                }
            }
            else
            {
                exitCond = true;
                otherPartFaceDsc = currFaceCircularNeighs[i];
                break;
            }

            originOfFaceDsc[currFaceCircularNeighs[i]] = currFaceDsc;
        }
    }
}

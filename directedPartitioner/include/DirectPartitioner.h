//
// Created by matteo on 01/03/19.
//

#ifndef MESHPARTITIONER_DIRECTPARTITIONER_H
#define MESHPARTITIONER_DIRECTPARTITIONER_H

#include <types.h>

class DirectPartitioner {
public:
    std::vector<bool> bisectMesh(const Mesh &mesh, const Mesh::face_index initDsc, const std::vector<bool> &isNodeInSubMesh);
private:
    Mesh::face_index findOuterBorderInit(const Mesh::face_index lastFaceDsc,
                                         const Mesh::face_index lastButOneFaceDsc,
                                         const std::vector<bool> &isNodeInSubMesh,
                                         const std::vector<bool> &isFaceExplored,
                                         const Mesh &mesh);
    std::vector<Mesh::face_index> findOuterBorder(const Mesh::face_index initFaceDsc,
                                                  const std::vector<bool> &isNodeInSubMesh,
                                                  const std::vector<bool> &isFaceExplored,
                                                  const Mesh &mesh);
    std::vector<Mesh::face_index> findHoles(const std::vector<bool> &visitedNodes, const std::vector<bool> &expandedNodes, const std::vector<Mesh::face_index> &outerBorder);
    void fillHoles(const Mesh &mesh, const std::vector<Mesh::face_index> &holesDscs, std::vector<bool> &insertedFaces, const std::vector<bool> &isNodeInSubMesh);
};


#endif //MESHPARTITIONER_DIRECTPARTITIONER_H

//
// Created by matteo on 03/03/19.
//

#ifndef MESHPARTITIONER_PARTITIONTYPES_H
#define MESHPARTITIONER_PARTITIONTYPES_H

#include <types.h>

typedef struct
{
    Mesh mesh;

    std::vector<bool> *isFaceInPartition;
    std::vector<bool> *isFaceExplored;
    std::vector<bool> *isFaceVisited;

    Mesh::face_index initDsc;
    Mesh::face_index lastFaceDsc;
    Mesh::face_index lastButOneFaceDsc;

    unsigned int cutThresh;
} DirectedPartition;

#endif //MESHPARTITIONER_PARTITIONTYPES_H

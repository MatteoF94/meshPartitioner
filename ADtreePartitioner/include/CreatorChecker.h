//
// Created by matteo on 29/04/19.
//

#ifndef MESHPARTITIONER_CREATORCHECKER_H
#define MESHPARTITIONER_CREATORCHECKER_H

#include "ADtypes.h"

class CreatorChecker {
public:

    bool checkTreeIntegrity(const ADnode *const &root, unsigned int numNodes);

private:

    bool checkTreeConcatenation(const ADnode *const &root, unsigned int numNodes);
    bool checkTreeStructure(const ADnode *const &root, unsigned int numNodes);
    bool checkTreeSemantic(const ADnode *const &root);

};


#endif //MESHPARTITIONER_CREATORCHECKER_H

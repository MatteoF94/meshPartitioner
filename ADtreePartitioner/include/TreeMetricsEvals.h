//
// Created by matteo on 29/04/19.
//

#ifndef MESHPARTITIONER_TREEMETRICSEVALS_H
#define MESHPARTITIONER_TREEMETRICSEVALS_H

#include "ADtypes.h"

class TreeMetricsEvals
{
public:

    void evaluateTree(const ADnode *const &root, const std::string &filename);

private:

    void evaluateDepth(const ADnode *const &root);
    void evaluateAvgFanOut(const ADnode *const &root);
    void evaluateTreeMaxLevelSize(const ADnode *const &root);
};


#endif //MESHPARTITIONER_TREEMETRICSEVALS_H

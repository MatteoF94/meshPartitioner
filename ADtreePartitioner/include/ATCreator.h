//
// Created by matteo on 29/04/19.
//

#ifndef MESHPARTITIONER_ATCREATOR_H
#define MESHPARTITIONER_ATCREATOR_H

#include <types.h>
#include "ADtypes.h"

class ATCreator {
public:
/**
     * @brief Class constructor.
     */
    ATCreator();

    /**
     * @brief Class destructor.
     */
    ~ATCreator();

    /**
     * @brief Set the tree concatenation type between nodes.
     * @param concatenationType
     */
    void configCreator(AriadneTreeVariant aVariant);

    /**
     * @brief Selects which algorithm variant is used to build the tree, depending on the way the nodes should be linked.
     * @param mesh The mesh to be converted.
     * @param root Root of the tree representing the mesh.
     */
    void createAriadneTree(ADnode *const &root);

private:

    void createTreeLtR(ADnode *const &root);
    void createTreeRtL(ADnode *const &root);
    void createTreeAlternated(ADnode *const &root);
    void createTreeFlippant(ADnode *const &root);

    void concatenateNode(ADnode *&cursor, ADnode *const &node);

    AriadneTreeVariant aVariant_;
};


#endif //MESHPARTITIONER_ATCREATOR_H

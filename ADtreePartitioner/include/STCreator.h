//
// Created by matteo on 29/04/19.
//

#ifndef MESHPARTITIONER_STCREATOR_H
#define MESHPARTITIONER_STCREATOR_H

#include <types.h>
#include "ADtypes.h"

class STCreator {
public:
/**
     * @brief Class constructor.
     */
    STCreator();

    /**
     * @brief Class destructor.
     */
    virtual ~STCreator();

    /**
     * @brief Set the tree concatenation type between nodes.
     * @param concatenationType
     */
    void configCreator(SpanningTreeVariant sVariant);

    /**
     * @brief Selects which algorithm variant is used to build the tree, depending on the way the nodes should be linked.
     * @param mesh The mesh to be converted.
     * @param root Root of the tree representing the mesh.
     */
    void createSpanningTree(const Mesh &mesh, ADnode *const &root);
    void createSpanningTree(const Graph &graph, ADnode *const &root);

protected:

    /**
     * @brief Builds the tree expanding neighbouring nodes left to right breadth first.
     * @param mesh The mesh to be converted.
     * @param root The root of the tree representing the mesh.
     */
    virtual void createTreeLtR(const Mesh &mesh, ADnode *const &root) = 0;
    virtual void createTreeLtR(const Graph &graph, ADnode *const &root) = 0;

    /**
     * @brief Builds the tree expanding neighbouring nodes right to left breadth first.
     * @param mesh The mesh to be converted.
     * @param root The root of the tree representing the mesh.
     */
    virtual void createTreeRtL(const Mesh &mesh, ADnode *const &root) = 0;
    virtual void createTreeRtL(const Graph &graph, ADnode *const &root) = 0;

    /**
     * @brief Builds the tree expanding neighbouring nodes alternating left to right and right to left  breadth first
     *        at each tree level.
     * @param mesh The mesh to be converted.
     * @param root The root of the tree representing the mesh.
     */
    virtual void createTreeAlternated(const Mesh &mesh, ADnode *const &root) = 0;
    virtual void createTreeAlternated(const Graph &graph, ADnode *const &root) = 0;

    /**
     * @brief Builds the tree expanding neighbouring nodes alternating left to right and right to left  breadth first
     *        at each node expansion.
     * @param mesh The mesh to be converted.
     * @param root The root of the tree representing the mesh.
     */
    virtual void createTreeFlippant(const Mesh &mesh, ADnode *const &root) = 0;
    virtual void createTreeFlippant(const Graph &graph, ADnode *const &root) = 0;

    /**
     * @brief Set one of the current node descendant or relative, depending on the position.
     * @param node Current node in the tree.
     * @param neighbour Index of the already inserted tree.
     * @param treeNodes List of all the nodes inserted in the tree (some also not inserted).
     */
    void insertNodeDescAndRels(ADnode *const &node, unsigned int neighbour, const std::vector<ADnode*> &treeNodes);


    SpanningTreeVariant sVariant_;
};


#endif //MESHPARTITIONER_STCREATOR_H

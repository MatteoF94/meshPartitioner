//
// Created by matteo on 25/02/19.
//

#ifndef MESHPARTITIONER_MESHREDUCER_H
#define MESHPARTITIONER_MESHREDUCER_H

#include <types.h>

/**
 * @class MeshReducer
 * @details Given a manifold labeled mesh, we can reduce it to a graph where every node is connected
 *          to only nodes corresponding to different labels. The new nodes correspond to adjacent faces
 *          with the same label of the mesh.
 * @example Consider the mesh representing a stripe 1-1-0-2-1-1, where the numbers represent the labels of the
 *          faces, the algorithm produces a labeled graph 0-1-2-3 (0 corresponding to original label 1,
 *          1 to original label 0, 2 to 2 and 3 to 1 again).
 */
class MeshReducer {
public:

    /**
     * @brief Class constructor.
     */
    MeshReducer();

    /**
     * @brief Class constructor
     * @param isPartitionIterative Value that tells if the algorithm should run recursively or iteratively.
     */
    explicit MeshReducer(bool isPartitionIterative);

    /**
     * @brief Prepares the graph to be reduced and selects the algorithm to execute (iterative or recursive).
     * @param m Mesh to reduce.
     * @param labels Labels associated to the graph nodes.
     * @param facesGroups Correspondences between the mesh faces descriptors and the reduced graph descriptors.
     * @return The reduced graph.
     */
    Graph reduceLabeledMesh(const Mesh &m, std::vector<unsigned int> &labels, std::vector<unsigned int> &facesGroups);

private:
    /**
     * Structure to represent the labeled graph concept.
     */
    struct labeledMeshToReduce {
        const Mesh &m;                         /**< the mesh to reduce */
        std::vector<unsigned int> &labels;      /**< the labels of the graph nodes */
        std::vector<unsigned int> &facesGroup;  /**< the assignments of faces to the reduced graph */
        unsigned int currGroup;                 /**< current node of the reduced graph, used in the algorithms */

        labeledMeshToReduce(const Mesh &mr,std::vector<unsigned int> &mLabels, std::vector<unsigned int> &fGroup) :
                m(mr),
                labels(mLabels),
                facesGroup(fGroup),
                currGroup(0) {}
    };

    /**
     * The enumeration relative to the way the reducing algorithm executes.
     */
    enum class ReductionMode {RECURSIVE, ITERATIVE};

    /**
     * @brief Detects the connected components of mesh m and associates to each component a map
     *        node descriptor - boolean to be used in the reduction algorithms to verify nodes expansion.
     * @param m The mesh to be reduced.
     * @return A vector of maps, each to be used in the reduction algorithm to check if a face is expanded.
     */
    std::vector<std::unordered_map<mFaceDsc,bool>> prepareConnectedComponents(const Mesh &m);

    /**
     * @brief Recursive algorithm to reduce the mesh.
     * @param lmr Structure containing mesh, labels and the assignments of faces to the reduced graph.
     * @param faceDsc Descriptor of the current face to explore.
     * @param reducedGraph The reduced graph.
     * @param expanded List of flags expanded/not expanded associated to the mesh faces.
     */
    void reduceRecursively(labeledMeshToReduce &lmr, mFaceDsc faceDsc, Graph &reducedGraph,
                           std::unordered_map<mFaceDsc,bool> &expanded);

    /**
     * @brief Iterative algorithm to reduce the mesh.
     * @param lmr Structure containing mesh, labels and the assignments of faces to the reduced graph.
     * @param faceDsc Descriptor of the first explored face.
     * @param reducedGraph The reduced graph.
     * @param expanded List of flags expanded/not expanded associated to the mesh.
     */
    void reduceIteratively(labeledMeshToReduce &lmr, mFaceDsc faceDsc, Graph &reducedGraph,
                           std::unordered_map<mFaceDsc,bool> &expanded);

    ReductionMode reductionMode; /**< the way the algorithm reduces a mesh*/
};


#endif //MESHPARTITIONER_MESHREDUCER_H

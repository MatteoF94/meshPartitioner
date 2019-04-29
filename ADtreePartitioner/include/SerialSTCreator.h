//
// Created by matteo on 29/04/19.
//

#ifndef MESHPARTITIONER_SERIALTREECREATOR_H
#define MESHPARTITIONER_SERIALTREECREATOR_H

#include "STCreator.h"

class SerialSTCreator : public STCreator {
public:

    /**
     * @brief Class constructor.
     */
    SerialSTCreator();

    /**
     * @brief Class destructor.
     */
    ~SerialSTCreator() override;

protected:

    /**
     * Overrides base class function.
     */
    void createTreeLtR(const Mesh &mesh, ADnode *const &root) override;
    void createTreeLtR(const Graph &graph, ADnode *const &root) override;

    /**
     * Overrides base class function.
     */
    void createTreeRtL(const Mesh &mesh, ADnode *const &root) override;
    void createTreeRtL(const Graph &graph, ADnode *const &root) override;

    /**
     * Overrides base class function.
     */
    void createTreeAlternated(const Mesh &mesh, ADnode *const &root) override;
    void createTreeAlternated(const Graph &graph, ADnode *const &root) override;

    /**
     * Overrides base class function.
     */
    void createTreeFlippant(const Mesh &mesh, ADnode *const &root) override;
    void createTreeFlippant(const Graph &graph, ADnode *const &root) override;

};


#endif //MESHPARTITIONER_SERIALTREECREATOR_H

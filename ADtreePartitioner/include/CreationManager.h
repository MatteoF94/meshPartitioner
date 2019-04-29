//
// Created by matteo on 29/04/19.
//

#ifndef MESHPARTITIONER_CREATIONMANAGER_H
#define MESHPARTITIONER_CREATIONMANAGER_H

#include <types.h>
#include <ADtypes.h>
#include "STCreator.h"
#include "ATCreator.h"

class CreationManager {
public:
    CreationManager();
    ~CreationManager();

    void setCreationMode(CreationMode creationMode);
    //void setIfAdoptionEnabled(bool isAdoptionEnabled);
    void setSTCreatorVariant(SpanningTreeVariant sVariant);
    void setATCreatorVariant(AriadneTreeVariant aVariant);

    void createFromMesh(Mesh &mesh, ADnode *const &root);
    void createFromGraph(Graph &graph, ADnode *const &root);

private:

    std::unique_ptr<STCreator> STcreator_;
    std::unique_ptr<ATCreator> ATcreator_;

    CreationMode creationMode_;

    SpanningTreeVariant STvariant_;
    AriadneTreeVariant ATvariant_;
};


#endif //MESHPARTITIONER_CREATIONMANAGER_H

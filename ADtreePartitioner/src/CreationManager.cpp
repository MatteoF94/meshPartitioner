//
// Created by matteo on 29/04/19.
//

#include <CreationManager.h>
#include <SerialSTCreator.h>

CreationManager::CreationManager()  : STcreator_(new SerialSTCreator),
                                      ATcreator_(new ATCreator),
                                      creationMode_(CreationMode::Serial),
                                      STvariant_(SpanningTreeVariant::LtR),
                                      ATvariant_(AriadneTreeVariant::LtR)
{}

CreationManager::~CreationManager() {}

void CreationManager::setCreationMode(CreationMode creationMode)
{
    switch (creationMode)
    {
        case CreationMode::Serial :
            STcreator_ = std::make_unique<SerialSTCreator>();
            creationMode_ = CreationMode::Serial;
            break;
        case CreationMode::Parallel:
            //TODO
            break;
    }
}

void CreationManager::setSTCreatorVariant(SpanningTreeVariant sVariant)
{
    STvariant_ = sVariant;
    STcreator_->configCreator(sVariant);
}

void CreationManager::setATCreatorVariant(AriadneTreeVariant aVariant)
{
    ATvariant_ = aVariant;
    ATcreator_->configCreator(aVariant);
}

void CreationManager::createFromMesh(Mesh &mesh, ADnode *const &root)
{
    STcreator_->createSpanningTree(mesh,root);

    if(ATvariant_ != AriadneTreeVariant::LtR)
        ATcreator_->createAriadneTree(root);
}

void CreationManager::createFromGraph(Graph &graph, ADnode *const &root)
{
    STcreator_->createSpanningTree(graph,root);

    if(ATvariant_ != AriadneTreeVariant::LtR)
        ATcreator_->createAriadneTree(root);
}

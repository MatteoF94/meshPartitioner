//
// Created by matteo on 29/04/19.
//

#include <STCreator.h>

STCreator::STCreator() : sVariant_(SpanningTreeVariant::LtR)
{}

STCreator::~STCreator()
{}

void STCreator::configCreator(SpanningTreeVariant sVariant)
{
    sVariant_ = sVariant;
}

void STCreator::createSpanningTree(const Mesh &mesh, ADnode *const &root)
{
    switch (sVariant_)
    {
        case SpanningTreeVariant::LtR :
            createTreeLtR(mesh,root);
            break;
        case SpanningTreeVariant::RtL :
            createTreeRtL(mesh,root);
            break;
        case SpanningTreeVariant::Alternated :
            createTreeAlternated(mesh,root);
            break;
        case SpanningTreeVariant::Flippant :
            createTreeFlippant(mesh,root);
            break;
    }
}

void STCreator::createSpanningTree(const Graph &graph, ADnode *const &root)
{
    switch (sVariant_)
    {
        case SpanningTreeVariant::LtR :
            createTreeLtR(graph,root);
            break;
        case SpanningTreeVariant::RtL :
            createTreeRtL(graph,root);
            break;
        case SpanningTreeVariant::Alternated :
            createTreeAlternated(graph,root);
            break;
        case SpanningTreeVariant::Flippant :
            createTreeFlippant(graph,root);
            break;
    }
}

/**
 * @brief Set one of the current node descendant or relative, depending on the position.
 */
void STCreator::insertNodeDescAndRels(ADnode *const &node, unsigned int neighbour, const std::vector<ADnode*> &treeNodes)
{
    if (node->parent_->id_ != neighbour)
    {
        ADnode *relativeNode = treeNodes[neighbour];

        if(node->level_ > relativeNode->level_)
        {
            relativeNode->descendants_.emplace_back(node);
            node->relatives_.emplace_back(relativeNode);
        }

        if(node->level_ == relativeNode->level_)
        {
            relativeNode->descendants_.emplace_back(node);

            if (std::find(relativeNode->descendants_.begin(), relativeNode->descendants_.end(), node) ==
                relativeNode->descendants_.end())
            {
                node->descendants_.emplace_back(relativeNode);
                relativeNode->relatives_.emplace_back(node);
            }
        }
    }
}
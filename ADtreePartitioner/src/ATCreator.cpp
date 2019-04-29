//
// Created by matteo on 29/04/19.
//

#include <ATCreator.h>

ATCreator::ATCreator() : aVariant_(AriadneTreeVariant::LtR)
{}

ATCreator::~ATCreator()
{}

void ATCreator::configCreator(AriadneTreeVariant aVariant)
{
    aVariant_ = aVariant;
}

void ATCreator::createAriadneTree(ADnode *const &root)
{
    switch (aVariant_)
    {
        case AriadneTreeVariant::LtR :
            createTreeLtR(root);
            break;
        case AriadneTreeVariant::RtL :
            createTreeRtL(root);
            break;
        case AriadneTreeVariant::Alternated :
            createTreeAlternated(root);
            break;
        case AriadneTreeVariant::Flippant :
            createTreeFlippant(root);
            break;
    }
}

void ATCreator::createTreeLtR(ADnode *const &root)
{
    ADnode *cursor(root);
    ADnode *currNode(root);

    while(currNode)
    {
        for(auto &child : currNode->children_)
        {
            concatenateNode(cursor,child);
        }

        currNode = currNode->next_;
    }
}

void ATCreator::createTreeRtL(ADnode *const &root)
{
    ADnode *cursor(root);
    ADnode *currNode(root);

    while(currNode)
    {
        std::vector<ADnode*> children(currNode->children_);
        std::reverse(children.begin(),children.end());

        for(auto &child : children)
        {
            concatenateNode(cursor,child);
        }

        currNode = currNode->next_;
    }
}

void ATCreator::createTreeAlternated(ADnode *const &root)
{
    ADnode *cursor(root);
    ADnode *currNode(root);
    root->next_ = nullptr;

    bool isLexOrderInverse = false;
    bool inverseFlag = false;
    unsigned int currLevel = 1;
    std::vector<ADnode*> currLevelNodes;

    for(auto &child : currNode->children_)
    {
        concatenateNode(cursor,child);
    }
    currNode = currNode->next_;

    while(currNode)
    {
        std::vector<ADnode*> children(currNode->children_);

        if(currNode->level_ != currLevel)
        {
            for(unsigned int i = 0; i < currLevelNodes.size() - 1; ++i)
            {
                concatenateNode(cursor,currLevelNodes[i+1]);
            }

            currLevel = currNode->level_;
            isLexOrderInverse = !isLexOrderInverse;
            currLevelNodes.clear();
        }

        if(isLexOrderInverse)
            std::reverse(children.begin(),children.end());

        for(auto &child : children)
        {
            currLevelNodes.emplace_back(child);
        }

        if(!currNode->next_)
        {
            if (!currLevelNodes.empty())
            {
                std::reverse(currLevelNodes.begin(), currLevelNodes.end());
                concatenateNode(cursor, currLevelNodes.front());
            }
        }

        currNode = currNode->next_;
    }
}

void ATCreator::createTreeFlippant(ADnode *const &root)
{
    ADnode *cursor(root);
    ADnode *currNode(root);

    bool isLexOrderInverse = true;

    while(currNode)
    {
        std::vector<ADnode*> children(currNode->children_);
        isLexOrderInverse = !isLexOrderInverse;

        if(isLexOrderInverse)
            std::reverse(children.begin(),children.end());

        for(auto &child : children)
        {
            concatenateNode(cursor,child);
        }

        currNode = currNode->next_;
    }
}

/**
 * Concatenates one node to the last one chained to the tree.
 */
void ATCreator::concatenateNode(ADnode *&cursor, ADnode *const &node)
{
    cursor->next_ = node;
    node->prev_ = cursor;
    cursor = node;
    cursor->next_ = nullptr;
}
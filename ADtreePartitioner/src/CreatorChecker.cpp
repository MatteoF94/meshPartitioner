//
// Created by matteo on 29/04/19.
//

#include <CreatorChecker.h>
#include <queue>

bool CreatorChecker::checkTreeIntegrity(const ADnode *const &root, unsigned int numNodes)
{
    bool isConcatenationOk = checkTreeConcatenation(root, numNodes);
    bool isStructureOk = checkTreeStructure(root,numNodes);
    return isConcatenationOk && isStructureOk;
}

bool CreatorChecker::checkTreeConcatenation(const ADnode *const &root, unsigned int numNodes)
{
    std::vector<bool> areNodesInserted(numNodes,false);
    const ADnode *currNode = root;

    while(currNode->next_)
    {
        if(areNodesInserted[currNode->id_])
            return false;
        else
            areNodesInserted[currNode->id_] = true;

        if(currNode->next_->prev_ != currNode)
            return false;

        currNode = currNode->next_;
    }

    if(areNodesInserted[currNode->id_])
        return false;
    else
        areNodesInserted[currNode->id_] = true; // insert last node

    for(auto isIn : areNodesInserted)
        if(!isIn)
            return false;

    return true;
}

bool CreatorChecker::checkTreeStructure(const ADnode *const &root, unsigned int numNodes)
{
    std::vector<bool> areNodesInserted(numNodes,false);
    std::queue<const ADnode*> nodeQueue;
    const ADnode *currNode;
    nodeQueue.emplace(root);

    if(!root->descendants_.empty() || !root->relatives_.empty())
        return false;

    while(!nodeQueue.empty())
    {
        currNode = nodeQueue.front();

        if(areNodesInserted[currNode->id_])
            return false;
        else
            areNodesInserted[currNode->id_] = true;

        for(auto &child : currNode->children_)
        {
            if(child->level_ != currNode->level_ + 1)
                return false;
            if(child->parent_ != currNode)
                return false;

            nodeQueue.emplace(child);
        }

        nodeQueue.pop();
    }

    for(auto isIn : areNodesInserted)
        if(!isIn)
            return false;

    return true;
}

bool CreatorChecker::checkTreeSemantic(const ADnode *const &root)
{
    const ADnode *currNode = root->next_;



    while(currNode->next_)
    {
        unsigned int numChildren = currNode->children_.size();
        unsigned int numDescendants = currNode->descendants_.size();
        unsigned int numRelatives = currNode->relatives_.size();

        if(numChildren + numDescendants + numRelatives > 2)
            return false;

        currNode = currNode->next_;
    }

    return true;
}

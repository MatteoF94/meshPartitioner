//
// Created by matteo on 29/04/19.
//

#include <TreeMetricsEvals.h>

void TreeMetricsEvals::evaluateTree(const ADnode *const &root, const std::string &filename)
{
    evaluateDepth(root);
    evaluateAvgFanOut(root);
    evaluateTreeMaxLevelSize(root);
}

void TreeMetricsEvals::evaluateDepth(const ADnode *const &root)
{
    const ADnode *currNode(root);
    unsigned int depth = 0;

    while(currNode)
    {
        if(currNode->level_ != depth)
            depth = currNode->level_;
        currNode = currNode->next_;
    }

    std::cout << "Depth " << depth << std::endl;
}

void TreeMetricsEvals::evaluateAvgFanOut(const ADnode *const &root)
{
    const ADnode *currNode(root);
    unsigned int numLeaves = 0;
    std::vector<unsigned int> numChildren;

    while(currNode)
    {
        if(!currNode->children_.empty())
             numChildren.emplace_back(currNode->children_.size());
        else
            numLeaves++;

        currNode = currNode->next_;
    }

    double mean = std::accumulate(numChildren.begin(),numChildren.end(),0.0)/numChildren.size();
    std::cout << "Avg fan out " << mean << std::endl;
    std::vector<double> diff(numChildren.size());
    std::transform(numChildren.begin(),numChildren.end(),diff.begin(),[mean](double x) {return x - mean; });
    double sqSum = std::inner_product(diff.begin(),diff.end(),diff.begin(),0.0);
    double stdev = std::sqrt(sqSum / numChildren.size());
    std::cout << "Stdev fan out " << stdev << std::endl;

    std::cout << "Num leaves " << numLeaves << std::endl;
}

void TreeMetricsEvals::evaluateTreeMaxLevelSize(const ADnode *const &root)
{
    const ADnode *currNode(root);
    unsigned int depth = 0;

    while(currNode)
    {
        if(currNode->level_ != depth)
            depth = currNode->level_;
        currNode = currNode->next_;
    }

    std::vector<unsigned int> levSize(depth,0);

    currNode = root;
    while(currNode)
    {
        levSize[currNode->level_]++;
        currNode = currNode->next_;
    }

    std::cout << "Max level size " << *std::max_element(levSize.begin(),levSize.end()) << std::endl;
}
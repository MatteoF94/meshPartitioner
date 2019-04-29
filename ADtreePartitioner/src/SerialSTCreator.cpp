//
// Created by matteo on 29/04/19.
//

#include <SerialSTCreator.h>

/**
  * @brief Class constructor.
  */
SerialSTCreator::SerialSTCreator() {}

/**
  * @brief Class destructor.
  */
SerialSTCreator::~SerialSTCreator() {}

/**
  * Overrides base class function.
  */
void SerialSTCreator::createTreeLtR(const Mesh &mesh, ADnode *const &root)
{
    std::vector<ADnode*> treeNodes(mesh.num_faces(),nullptr);
    treeNodes[root->id_] = root;

    ADnode* cursor = root;
    ADnode* currNode = root;

    Mesh::face_index nullFace = Mesh::null_face();

    while (currNode)
    {
        CGAL::Face_around_face_iterator<Mesh> fafBegin,fafEnd;

        std::vector<unsigned int> sortedNeighbours;
        for (boost::tie(fafBegin,fafEnd)=CGAL::faces_around_face(mesh.halfedge(Mesh::face_index(currNode->id_)),mesh); fafBegin != fafEnd; ++fafBegin)
        {
            if (*fafBegin != nullFace)
            {
                if (!treeNodes[*fafBegin])
                {
                    sortedNeighbours.emplace_back(*fafBegin);
                }
                else
                {
                    insertNodeDescAndRels(currNode, *fafBegin, treeNodes);
                }
            }
        }
        std::sort(sortedNeighbours.begin(),sortedNeighbours.end());

        for(auto &neigh : sortedNeighbours)
        {
            currNode->children_.emplace_back(new ADnode(neigh));
            ADnode *currChildren = currNode->children_.back();
            currChildren->parent_ = currNode;
            currChildren->level_ = currChildren->parent_->level_ + 1;

            treeNodes[neigh] = currChildren;

            currChildren->prev_ = cursor;
            cursor->next_ = currChildren;
            cursor = currChildren;
        }

        currNode = currNode->next_;
    }
}

void SerialSTCreator::createTreeLtR(const Graph &graph, ADnode *const &root)
{
    std::vector<ADnode*> treeNodes(boost::num_vertices(graph),nullptr);
    treeNodes[root->id_] = root;

    ADnode* cursor = root;
    ADnode* currNode = root;

    Mesh::face_index nullFace = Mesh::null_face();

    while (currNode)
    {
        boost::graph_traits<Graph>::adjacency_iterator aiBegin,aiEnd;

        std::vector<unsigned int> sortedNeighbours;
        for(boost::tie(aiBegin,aiEnd) = boost::adjacent_vertices(currNode->id_,graph); aiBegin != aiEnd; ++aiBegin)
        {
            if (!treeNodes[*aiBegin])
            {
                sortedNeighbours.emplace_back(*aiBegin);
            }
            else
            {
                insertNodeDescAndRels(currNode, *aiBegin, treeNodes);
            }
        }
        std::sort(sortedNeighbours.begin(),sortedNeighbours.end());

        for(auto &neigh : sortedNeighbours)
        {
            currNode->children_.emplace_back(new ADnode(neigh));
            ADnode *currChildren = currNode->children_.back();
            currChildren->parent_ = currNode;
            currChildren->level_ = currChildren->parent_->level_ + 1;

            treeNodes[neigh] = currChildren;

            currChildren->prev_ = cursor;
            cursor->next_ = currChildren;
            cursor = currChildren;
        }

        currNode = currNode->next_;
    }
}

void SerialSTCreator::createTreeRtL(const Mesh &mesh, ADnode *const &root)
{
    std::vector<ADnode*> treeNodes(mesh.num_faces(),nullptr);
    treeNodes[root->id_] = root;

    ADnode* cursor = root;
    ADnode* currNode = root;

    Mesh::face_index nullFace = Mesh::null_face();

    while (currNode)
    {
        CGAL::Face_around_face_iterator<Mesh> fafBegin,fafEnd;

        std::vector<unsigned int> sortedNeighbours;
        for (boost::tie(fafBegin,fafEnd)=CGAL::faces_around_face(mesh.halfedge(Mesh::face_index(currNode->id_)),mesh); fafBegin != fafEnd; ++fafBegin)
        {
            if (*fafBegin != nullFace)
            {
                if (!treeNodes[*fafBegin])
                {
                    sortedNeighbours.emplace_back(*fafBegin);
                }
                else
                {
                    insertNodeDescAndRels(currNode, *fafBegin, treeNodes);
                }
            }
        }
        std::sort(sortedNeighbours.begin(),sortedNeighbours.end(),std::greater<>());

        for(auto &neigh : sortedNeighbours)
        {
            currNode->children_.emplace_back(new ADnode(neigh));
            ADnode *currChildren = currNode->children_.back();
            currChildren->parent_ = currNode;
            currChildren->level_ = currChildren->parent_->level_ + 1;

            treeNodes[neigh] = currChildren;

            currChildren->prev_ = cursor;
            cursor->next_ = currChildren;
            cursor = currChildren;
        }

        currNode = currNode->next_;
    }
}

void SerialSTCreator::createTreeRtL(const Graph &graph, ADnode *const &root)
{
    std::vector<ADnode*> treeNodes(boost::num_vertices(graph),nullptr);
    treeNodes[root->id_] = root;

    ADnode* cursor = root;
    ADnode* currNode = root;

    Mesh::face_index nullFace = Mesh::null_face();

    while (currNode)
    {
        boost::graph_traits<Graph>::adjacency_iterator aiBegin,aiEnd;

        std::vector<unsigned int> sortedNeighbours;
        for(boost::tie(aiBegin,aiEnd) = boost::adjacent_vertices(currNode->id_,graph); aiBegin != aiEnd; ++aiBegin)
        {
            if (!treeNodes[*aiBegin])
            {
                sortedNeighbours.emplace_back(*aiBegin);
            }
            else
            {
                insertNodeDescAndRels(currNode, *aiBegin, treeNodes);
            }
        }
        std::sort(sortedNeighbours.begin(),sortedNeighbours.end(),std::greater<>());

        for(auto &neigh : sortedNeighbours)
        {
            currNode->children_.emplace_back(new ADnode(neigh));
            ADnode *currChildren = currNode->children_.back();
            currChildren->parent_ = currNode;
            currChildren->level_ = currChildren->parent_->level_ + 1;

            treeNodes[neigh] = currChildren;

            currChildren->prev_ = cursor;
            cursor->next_ = currChildren;
            cursor = currChildren;
        }

        currNode = currNode->next_;
    }
}

void SerialSTCreator::createTreeAlternated(const Mesh &mesh, ADnode *const &root)
{
    std::vector<ADnode*> treeNodes(mesh.num_faces(),nullptr);
    treeNodes[root->id_] = root;

    ADnode* cursor = root;
    ADnode* currNode = root;

    Mesh::face_index nullFace = Mesh::null_face();

    bool isLexOrderInverse = false;
    unsigned int currLevel = 0;

    while (currNode)
    {
        CGAL::Face_around_face_iterator<Mesh> fafBegin,fafEnd;
        if(currNode->level_ != currLevel)
        {
            currLevel = currNode->level_;
            isLexOrderInverse = !isLexOrderInverse;
        }

        std::vector<unsigned int> sortedNeighbours;
        for (boost::tie(fafBegin,fafEnd)=CGAL::faces_around_face(mesh.halfedge(Mesh::face_index(currNode->id_)),mesh); fafBegin != fafEnd; ++fafBegin)
        {
            if (*fafBegin != nullFace)
            {
                if (!treeNodes[*fafBegin])
                {
                    sortedNeighbours.emplace_back(*fafBegin);
                }
                else
                {
                    insertNodeDescAndRels(currNode, *fafBegin, treeNodes);
                }
            }
        }

        if(!isLexOrderInverse)
            std::sort(sortedNeighbours.begin(),sortedNeighbours.end());
        else
            std::sort(sortedNeighbours.begin(),sortedNeighbours.end(),std::greater<>());

        for(auto &neigh : sortedNeighbours)
        {
            currNode->children_.emplace_back(new ADnode(neigh));
            ADnode *currChildren = currNode->children_.back();
            currChildren->parent_ = currNode;
            currChildren->level_ = currChildren->parent_->level_ + 1;

            treeNodes[neigh] = currChildren;

            currChildren->prev_ = cursor;
            cursor->next_ = currChildren;
            cursor = currChildren;
        }

        currNode = currNode->next_;
    }
}

void SerialSTCreator::createTreeAlternated(const Graph &graph, ADnode *const &root)
{
    std::vector<ADnode*> treeNodes(boost::num_vertices(graph),nullptr);
    treeNodes[root->id_] = root;

    ADnode* cursor = root;
    ADnode* currNode = root;

    Mesh::face_index nullFace = Mesh::null_face();

    bool isLexOrderInverse = false;
    unsigned int currLevel = 0;

    while (currNode)
    {
        boost::graph_traits<Graph>::adjacency_iterator aiBegin,aiEnd;
        if(currNode->level_ != currLevel)
        {
            currLevel = currNode->level_;
            isLexOrderInverse = !isLexOrderInverse;
        }

        std::vector<unsigned int> sortedNeighbours;
        for(boost::tie(aiBegin,aiEnd) = boost::adjacent_vertices(currNode->id_,graph); aiBegin != aiEnd; ++aiBegin)
        {
            if (!treeNodes[*aiBegin])
            {
                sortedNeighbours.emplace_back(*aiBegin);
            }
            else
            {
                insertNodeDescAndRels(currNode, *aiBegin, treeNodes);
            }
        }
        if(!isLexOrderInverse)
            std::sort(sortedNeighbours.begin(),sortedNeighbours.end());
        else
            std::sort(sortedNeighbours.begin(),sortedNeighbours.end(),std::greater<>());

        for(auto &neigh : sortedNeighbours)
        {
            currNode->children_.emplace_back(new ADnode(neigh));
            ADnode *currChildren = currNode->children_.back();
            currChildren->parent_ = currNode;
            currChildren->level_ = currChildren->parent_->level_ + 1;

            treeNodes[neigh] = currChildren;

            currChildren->prev_ = cursor;
            cursor->next_ = currChildren;
            cursor = currChildren;
        }

        currNode = currNode->next_;
    }
}

void SerialSTCreator::createTreeFlippant(const Mesh &mesh, ADnode *const &root)
{
    std::vector<ADnode*> treeNodes(mesh.num_faces(),nullptr);
    treeNodes[root->id_] = root;

    ADnode* cursor = root;
    ADnode* currNode = root;

    Mesh::face_index nullFace = Mesh::null_face();

    bool isLexOrderInverse = true;

    while (currNode)
    {
        CGAL::Face_around_face_iterator<Mesh> fafBegin,fafEnd;
        isLexOrderInverse = !isLexOrderInverse;

        std::vector<unsigned int> sortedNeighbours;
        for (boost::tie(fafBegin,fafEnd)=CGAL::faces_around_face(mesh.halfedge(Mesh::face_index(currNode->id_)),mesh); fafBegin != fafEnd; ++fafBegin)
        {
            if (*fafBegin != nullFace)
            {
                if (!treeNodes[*fafBegin])
                {
                    sortedNeighbours.emplace_back(*fafBegin);
                }
                else
                {
                    insertNodeDescAndRels(currNode, *fafBegin, treeNodes);
                }
            }
        }

        if(!isLexOrderInverse)
            std::sort(sortedNeighbours.begin(),sortedNeighbours.end());
        else
            std::sort(sortedNeighbours.begin(),sortedNeighbours.end(),std::greater<>());

        for(auto &neigh : sortedNeighbours)
        {
            currNode->children_.emplace_back(new ADnode(neigh));
            ADnode *currChildren = currNode->children_.back();
            currChildren->parent_ = currNode;
            currChildren->level_ = currChildren->parent_->level_ + 1;

            treeNodes[neigh] = currChildren;

            currChildren->prev_ = cursor;
            cursor->next_ = currChildren;
            cursor = currChildren;
        }

        currNode = currNode->next_;
    }
}

void SerialSTCreator::createTreeFlippant(const Graph &graph, ADnode *const &root)
{
    std::vector<ADnode*> treeNodes(boost::num_vertices(graph),nullptr);
    treeNodes[root->id_] = root;

    ADnode* cursor = root;
    ADnode* currNode = root;

    Mesh::face_index nullFace = Mesh::null_face();

    bool isLexOrderInverse = true;

    while (currNode)
    {
        boost::graph_traits<Graph>::adjacency_iterator aiBegin,aiEnd;
        isLexOrderInverse = !isLexOrderInverse;

        std::vector<unsigned int> sortedNeighbours;
        for(boost::tie(aiBegin,aiEnd) = boost::adjacent_vertices(currNode->id_,graph); aiBegin != aiEnd; ++aiBegin)
        {
            if (!treeNodes[*aiBegin])
            {
                sortedNeighbours.emplace_back(*aiBegin);
            }
            else
            {
                insertNodeDescAndRels(currNode, *aiBegin, treeNodes);
            }
        }
        if(!isLexOrderInverse)
            std::sort(sortedNeighbours.begin(),sortedNeighbours.end());
        else
            std::sort(sortedNeighbours.begin(),sortedNeighbours.end(),std::greater<>());

        for(auto &neigh : sortedNeighbours)
        {
            currNode->children_.emplace_back(new ADnode(neigh));
            ADnode *currChildren = currNode->children_.back();
            currChildren->parent_ = currNode;
            currChildren->level_ = currChildren->parent_->level_ + 1;

            treeNodes[neigh] = currChildren;

            currChildren->prev_ = cursor;
            cursor->next_ = currChildren;
            cursor = currChildren;
        }

        currNode = currNode->next_;
    }
}
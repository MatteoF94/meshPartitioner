//
// Created by matteo on 14/02/19.
//

#include <fstream>
#include "InputManager.h"
#include <boost/graph/graph_utility.hpp>

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}

void InputManager::readMeshFromOff(const std::string &offFilename, Mesh &mesh)
{
    std::ifstream offFile(offFilename);

    if (!offFile) {
        std::cout << "Wrong format" << std::endl;
    }
    else
    {
        offFile >> mesh;
    }
    offFile.close();
}

void InputManager::readGraphFromEdgeList(const std::string &graphFilename, Graph &graph)
{
    std::ifstream graphFile(graphFilename);

    std::string line;
    std::istringstream iss(line);

    while(std::getline(graphFile,line))
    {
        std::istringstream is(line);
        unsigned int a,b;
        is >> a >> b;
        boost::add_edge(a,b,graph);
    }

    boost::print_graph(graph);
}
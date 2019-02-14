//
// Created by matteo on 14/02/19.
//

#include <fstream>
#include "InputManager.h"

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

void InputManager::readGraphFromGraph(const std::string &graphFilename, Graph &graph)
{
    std::ifstream graphFile(graphFilename);

    int numVertices;
    std::string line;
    std::getline(graphFile,line);
    std::istringstream iss(line);

    if(!(iss >> numVertices)) std::cout << "Error" << std::endl;

    for(unsigned int i = 0; i < numVertices; ++i)
        boost::add_vertex(graph);

    while(std::getline(graphFile,line))
    {
        std::istringstream is(line);
        //std::
    }
}
//
// Created by matteo on 14/02/19.
//

#ifndef MESHPARTITIONER_INPUTMANAGER_H
#define MESHPARTITIONER_INPUTMANAGER_H

#include <types.h>
#include <string>

class InputManager {
public:

    InputManager();
    ~InputManager();

    void readMeshFromOff(const std::string &offFilename, Mesh &mesh);
    void readGraphFromEdgeList(const std::string &graphFilename, Graph &graph);
};


#endif //MESHPARTITIONER_INPUTMANAGER_H

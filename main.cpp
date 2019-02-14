#include <iostream>
#include <InputManager.h>

int main() {

    InputManager inputManager;
    Mesh mesh;
    inputManager.readMeshFromOff("../data/bunny.off", mesh);
    std::cout << mesh.num_faces() << std::endl;
    return 0;
}
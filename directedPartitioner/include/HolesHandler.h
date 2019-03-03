//
// Created by matteo on 03/03/19.
//

#ifndef MESHPARTITIONER_HOLESHANDLER_H
#define MESHPARTITIONER_HOLESHANDLER_H

#include "partitionTypes.h"

class HolesHandler {
public:
    void findAndFillHoles(DirectedPartition &dirPart);
private:
    void findHolesDsc(DirectedPartition &dirPart);
};


#endif //MESHPARTITIONER_HOLESHANDLER_H

#ifndef _GRID_CELL_H
#define _GRID_CELL_H

#include "common.h"

struct GridCell {
    int counter = 0;
    Vector3 average = Vector3(0,0,0);
    void addVertex(const Vector3 &newVertex) {
        average += newVertex;
        counter++;
    }
    Vector3 averageVertices() {
        return average / counter;
    }
};

#endif
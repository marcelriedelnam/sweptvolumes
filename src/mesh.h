#ifndef _MESH_H
#define _MESH_H

#include <memory>

#include "common.h"

struct Mesh  {

    std::vector<Vector3, allocV> vertices;
    std::vector<std::tuple<IndexType,IndexType,IndexType>> faces;
};


#endif
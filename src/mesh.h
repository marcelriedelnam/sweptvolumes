#ifndef _MESH_H
#define _MESH_H

#include <memory>
#include <unordered_set>
#include <tuple>

#include "common.h"

struct Mesh  {

    std::vector<Vector3, allocV> vertices;
    std::vector<Vector3, allocV> normals;
    // tuple(vertice, normal)
    std::vector<IndexTypePair> vertexNormalPairs;
	Float longestEdge;

};


#endif
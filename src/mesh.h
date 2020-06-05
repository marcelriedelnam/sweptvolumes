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
    std::vector<std::tuple<IndexType, IndexType, IndexType>> faces;
	Float longestMeshEdge;
    Float longestTemporalEdge;

    static std::unique_ptr<Mesh> copyEmptyFrom(const Mesh &mesh) {
        auto newMesh = std::make_unique<Mesh>();
        newMesh->longestMeshEdge = mesh.longestMeshEdge;
        newMesh->longestTemporalEdge = mesh.longestTemporalEdge;
        return newMesh;
    }
};


#endif
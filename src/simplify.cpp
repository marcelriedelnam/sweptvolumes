#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <unordered_map>

#include "simplify.h"

using std::cout;
using std::endl;
using Eigen::Vector3i;

std::unique_ptr<Mesh> Simplify::simplify(const Mesh &inputMesh) {
    auto outputMesh = Mesh::copyEmptyFrom(inputMesh);

    std::unordered_map<Vector3i, GridCell, Vector3iHasher> voxelGrid;
    Float longestEdge = std::max(inputMesh.longestMeshEdge, inputMesh.longestTemporalEdge);
    Float gridCellLength = longestEdge; //PARAMETER

    for (int i = 0; i < inputMesh.vertices.size(); ++i) {
        Vector3 vertex = inputMesh.vertices[i];
        Vector3i coord = (vertex / gridCellLength).cast<int>();
        voxelGrid[coord].addVertex(vertex);
    }

    for (auto& n : voxelGrid) {
        outputMesh->vertices.push_back(n.second.averageVertices());
    }

    cout << "After sweep:" << endl;
    cout << "Resulting vertices: " << outputMesh->vertices.size() << endl;
    cout << endl;

    return outputMesh;
}
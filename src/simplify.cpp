#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <unordered_map>

#include "simplify.h"
#include "config.h"

using std::cout;
using std::endl;
using Eigen::Vector3i;


std::unique_ptr<Mesh> Simplify::simplify(const Mesh &inputMesh) {
    auto outputMesh = Mesh::copyEmptyFrom(inputMesh);

    std::unordered_map<Vector3i, std::vector<Vector3, allocV>, Vector3iHasher, std::equal_to<Vector3i>, allocMap> voxelGrid;
    Float longestEdge = std::max(inputMesh.longestMeshEdge, inputMesh.longestTemporalEdge);
    Float gridCellLength = ParameterConfig::gridCellLength;
	Float radius = ParameterConfig::simplifyRadius;

    for (int i = 0; i < inputMesh.vertices.size(); ++i) {
        Vector3 vertex = inputMesh.vertices[i];
        Vector3i coord = (vertex / gridCellLength).cast<int>();
        voxelGrid[coord].push_back(vertex);
    }

	while (!voxelGrid.empty()) {
		Vector3 currSample = voxelGrid.begin()->second.at(0);
		Vector3i voxelCoord = voxelGrid.begin()->first;
		int nrNeighborBlocks = std::ceil(radius/gridCellLength);

		for (int x = voxelCoord[0] - nrNeighborBlocks; x <= voxelCoord[0] + nrNeighborBlocks; ++x) {
			for (int y = voxelCoord[1] - nrNeighborBlocks; y <= voxelCoord[1] + nrNeighborBlocks; ++y) {
				for (int z = voxelCoord[2] - nrNeighborBlocks; z <= voxelCoord[2] + nrNeighborBlocks; ++z) {
					auto& chunk = voxelGrid[Vector3i(x,y,z)];
					chunk.erase(std::remove_if(chunk.begin(), chunk.end(), [&](const Vector3& vec){
						return (currSample - vec).norm() <= radius;
					}), chunk.end());
					if (chunk.empty()) voxelGrid.erase(Vector3i(x,y,z));
				}
			}
		}
		outputMesh->vertices.push_back(currSample);
	}

    cout << "After sweep:" << endl;
    cout << "Resulting vertices: " << outputMesh->vertices.size() << endl;
    cout << endl;

    return outputMesh;
}
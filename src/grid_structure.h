#ifndef _GRID_STRUCTURE_H
#define _GRID_STRUCTURE_H

#include "common.h"
#include <vector>
#include <algorithm>
#include <unordered_map>

template<typename T>
class GridStructure {
private: 
    Float cellSize;
    std::unordered_map<Vector3i, 
                       std::pair<std::vector<Vector3, allocV>, std::vector<T>>,
                       Vector3iHasher, 
                       std::equal_to<Vector3i>, 
                       allocMap> voxelGrid;

public:
    void setCellSize(Float value) {
        cellSize = value;
    }

    bool contains(const Vector3 &pos, T &outValue) {
        Vector3i coord = (pos / cellSize).cast<int>();

        for (int x = -1; x <= 1; ++x) {
            for (int y = -1; y <= 1; ++y) {
                for(int z = -1; z <= 1; ++z) {
                    
                    auto it = voxelGrid.find(coord + Vector3i(x,y,z));
                    if (it != voxelGrid.end()) {
                        
                        auto &cellPositions = it->second.first;
                        auto iter = std::find_if(cellPositions.begin(), cellPositions.end(), 
                            [&pos](const Vector3 &vec){return (pos - vec).squaredNorm() < 1e-8;});
                        
                        if (iter != cellPositions.end()) {
                            auto offset = std::distance(cellPositions.begin(), iter); 
                            outValue = it->second.second[offset];
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    }

    void insert(const Vector3 &vec, const T &value) {
        Vector3i coord = (vec / cellSize).cast<int>();
        auto& cell = voxelGrid[coord];
        cell.first.push_back(vec);
        cell.second.push_back(value);
    }
    
};


#endif
#ifndef _SIMPLIFY_H
#define _SIMPLIFY_H

#include "common.h"
#include "grid_cell.h"
#include "transform.h"

class Simplify {

public:
    static std::unique_ptr<Mesh> simplify(const Mesh &inputMesh);
private:
    struct Vector3iHasher {
        std::size_t operator()(const Eigen::Vector3i &key) const {
            std::size_t hash = 0;
            hash ^= key[0] + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            hash ^= key[1] + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            hash ^= key[2] + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            return hash;
        }
    };
    

};

#endif
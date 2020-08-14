#ifndef _COMMON_H
#define _COMMON_H

#include <cstdint>
#include <Eigen/Core>
#include <Eigen/StdVector>
#include <Eigen/Dense>
#include <unsupported/Eigen/MatrixFunctions>

using Float = double;
using IndexType = uint64_t;

using Vector3 = Eigen::Matrix<Float, 3, 1>;
using Eigen::Vector3i;
using Vector4 = Eigen::Matrix<Float, 4, 1>;
using Matrix4 = Eigen::Matrix<Float, 4, 4>;
using Matrix3 = Eigen::Matrix<Float, 3, 3>;
using allocV = Eigen::aligned_allocator<Vector3>;
using allocM = Eigen::aligned_allocator<Matrix4>;
using IndexTypePair = std::tuple<IndexType, IndexType>;
using Quaternion = Eigen::Quaternion<Float>;
using allocMap = Eigen::aligned_allocator<std::pair<Eigen::Vector3i, std::vector<Vector3, allocV>>>;

struct Vector3iHasher {
    std::size_t operator()(const Eigen::Vector3i &key) const {
        std::size_t hash = 0;
        hash ^= key[0] + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        hash ^= key[1] + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        hash ^= key[2] + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        return hash;
    }
};

struct IndexTypePairHasher {
    std::size_t operator()(const IndexTypePair &key) const {
        std::size_t hash = 0;
        hash ^= std::get<0>(key) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        hash ^= std::get<1>(key) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        return hash;
    }
};

#endif
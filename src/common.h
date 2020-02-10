#ifndef _COMMON_H
#define _COMMON_H

#include <cstdint>
#include <Eigen/Core>
#include <Eigen/StdVector>
#include <Eigen/Dense>

using Float = double;
using IndexType = uint64_t;

using Vector3 = Eigen::Matrix<Float, 3, 1>;
using Vector4 = Eigen::Matrix<Float, 4, 1>;
using Matrix4 = Eigen::Matrix<Float, 4, 4>;
using allocV = Eigen::aligned_allocator<Vector3>;
using allocM = Eigen::aligned_allocator<Matrix4>;

#endif
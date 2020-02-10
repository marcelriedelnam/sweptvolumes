#include <vector>
#include <string>
#include <iostream>

#include "tranform.h"

void Transform::transformVertices(std::vector<Vector3, allocV> vertices,
                                  std::vector<std::tuple<IndexType,IndexType,IndexType>> faces,
                                  std::vector<Matrix4, allocM> matrices)
{
    // save the original vertices and faces
    for (int i = 0; i < vertices.size(); ++i) {
        transformedVertices.push_back(vertices[i]);
    }
    for (const auto &i : faces) {
        transformedFaces.push_back(std::make_tuple(std::get<0>(i), std::get<1>(i), std::get<2>(i)));
    }

    // tranform the vertices with the given matrices
    for (int i = 0; i < vertices.size(); ++i) {
        // convert the 3x1 vertice vector to a homogenous 4x1 vector
        Vector4 vert;
        vert << vertices[i], 1;
        for (int j = 0; j < matrices.size(); ++j) {
            Vector4 temp = matrices[j] * vert;
            Vector3 res = calculateVector(temp);
            transformedVertices.push_back(res);
        }
    }

    // reconstruct the new faces from the transformed vertices
    for (int i = 0; i < matrices.size(); ++i) {
        for (auto &j : faces) {
            IndexType idx0 = std::get<0>(j);
            IndexType idx1 = std::get<1>(j);
            IndexType idx2 = std::get<2>(j);

            IndexType newIdx0 = idx0 * matrices.size() + vertices.size() + i;
            IndexType newIdx1 = idx1 * matrices.size() + vertices.size() + i;
            IndexType newIdx2 = idx2 * matrices.size() + vertices.size() + i;

            transformedFaces.push_back(std::make_tuple(newIdx0, newIdx1, newIdx2));
        }
    }

    std::cout << "#v: " << transformedVertices.size() << std::endl;
    std::cout << "#f: " << transformedFaces.size() << std::endl;

}

std::vector<Vector3, allocV> Transform::getTransformedVertices() {
    return transformedVertices;
}

std::vector<std::tuple<IndexType,IndexType,IndexType>> Transform::getTransformedFaces() {
    return transformedFaces;
}

// transform the homogenous 4x1 vertice vecor back to a 3x1 vector
Vector3 Transform::calculateVector(Vector4 vec) {
    std::vector<Float> values;
    for (int i = 0; i < vec.size(); ++i) {
        values.push_back(vec[i]);
    }
    Float x = values[0] / values[3];
    Float y = values[1] / values[3];
    Float z = values[2] / values[3];

    return Vector3(x, y, z);
}
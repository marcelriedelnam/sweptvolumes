#include <vector>
#include <string>
#include <iostream>

#include "transform.h"

void Transform::transformVertices(const Mesh &mesh,
                                  const std::vector<Matrix4, allocM> &matrices)
{
    // transform the vertices with the given matrices
    for (int i = 0; i < mesh.vertices.size(); ++i) {
        // convert the 3x1 vertice vector to a homogenous 4x1 vector
        Vector4 vert;
        vert << mesh.vertices[i], 1;
        for (int j = 0; j < matrices.size(); ++j) {
            Vector4 temp = matrices[j] * vert;
            Vector3 res(temp[0], temp[1], temp[2]);
            res /= temp[3];
            transformedMesh->vertices.push_back(res);
        }
    }

}

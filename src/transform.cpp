#include <vector>
#include <string>
#include <iostream>

#include "transform.h"

using std::cout;
using std::endl;

std::unique_ptr<Mesh> Transform::transform(const Mesh &inputMesh, const std::vector<Matrix4, allocM> &matrices) {
    auto outputMesh = std::make_unique<Mesh>();
    // transformVerticesNaive(inputMesh, matrices, *outputMesh);
    transformVerticesDM(inputMesh, matrices, *outputMesh);

    return outputMesh;
}


// naively computes the vertices for every time step and returns all matrices
void Transform::transformVerticesNaive(const Mesh &inputMesh,
                                       const std::vector<Matrix4, allocM> &matrices,
                                       Mesh &outputMesh)
{
    // transform the vertices with the given matrices
    for (int i = 0; i < inputMesh.vertices.size(); ++i) {
        Vector4 vert(inputMesh.vertices[i][0], inputMesh.vertices[i][1], inputMesh.vertices[i][2], 1);
        for (int j = 0; j < matrices.size(); ++j) {
            Vector4 temp = matrices[j] * vert;
            Vector3 res(temp[0], temp[1], temp[2]);
            res /= temp[3];
            outputMesh.vertices.push_back(res);
        }
    }

    cout << "Resulting vertices: " << outputMesh.vertices.size() << endl;;

}

/*
 * Thin out the points of the point cloud by calculating the directional movement (DM) of a point
 * and testing it with the normal vector of that point.
 * 
 * A point is a possible candidate of the enveloping surface if the DM and normal vector of a point 
 * are orthogonal.
 * 
 * Also calculates the measure of the orthogonality or the measure of members of the enveloping surface 
 * respectively.
 * 
 * In this case points refer to vertices.
 * 
 * TODO: special cases at time i = first/last.
 */
void Transform::transformVerticesDM(const Mesh &inputMesh,
                                    const std::vector<Matrix4, allocM> &matrices,
                                    Mesh &outputMesh)
{
    
    for (int i = 1; i < matrices.size() - 1; ++i) {
        std::vector<bool> isInserted;
        isInserted.resize(inputMesh.vertices.size(), false);

        Matrix4 diffMat = matrices[i+1] - matrices[i-1];
        Matrix4 currentMat = matrices[i];
        for (int j = 0; j < inputMesh.vertexNormalPairs.size(); ++j) {
            IndexType vertexIdx = std::get<0>(inputMesh.vertexNormalPairs[j]);
            IndexType normalIdx = std::get<1>(inputMesh.vertexNormalPairs[j]);

            Vector4 vertex;
            vertex << inputMesh.vertices[vertexIdx], 1;
            Vector4 normal;
            normal << inputMesh.normals[normalIdx], 0;

            // compute DM of point v and normal at time i
            Vector4 v_diff = (diffMat * vertex).normalized();
            Vector4 n_tnf = currentMat * normal;
            
            // compute the position of the point v at the time i
            Vector4 v_hom = currentMat * vertex;
            Vector3 v(v_hom[0], v_hom[1], v_hom[2]);
            v /= v_hom[3];
            
            // check if DM and normal are orthogonal
            // only add a vertice if it wasn't added before
            const Float eps = 0.1;
            if (v_diff.dot(n_tnf) < eps && v_diff.dot(n_tnf) > -eps) {
                if (!isInserted.at(vertexIdx)) {
                    isInserted.at(vertexIdx) = true;
                    outputMesh.vertices.push_back(v);
                }  
            }

            // compute the measure of orthogonality
            Vector4 prod = (currentMat.inverse() * diffMat * v_diff).normalized();
            Float measure = normal.dot(prod);
           
        }
    }

    cout << "Resulting vertices: " << outputMesh.vertices.size() << endl;;

}

/*
 * Thin out the points of the point cloud with the help of a rotation axis.
 */
/*void Transform::transformVerticesRA(const Mesh &inputMesh,
                                    const std::vector<Matrix4, allocM> &matrices,
                                    Mesh &outputMesh) {

    calculateRA(matrices);

}

Matrix4 Transform::calculateRA(Matrix4 &matrix) {
    Vector3 translation = matrix.block<3,1>(0,3);
    Matrix3 rotation = matrix.block<3,3>(0,0);
    Matrix3 identity = Matrix3::Identity();

}*/
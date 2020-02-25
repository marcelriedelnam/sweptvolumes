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
    // transformVerticesRA(inputMesh, matrices, *outputMesh);

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
 * TODO: special cases at time i = first/last. (For now, simply added ALL the vertices at that time)
 */
void Transform::transformVerticesDM(const Mesh &inputMesh,
                                    const std::vector<Matrix4, allocM> &matrices,
                                    Mesh &outputMesh)
{
    // for special cases at first and last time step, take all vertices.
    for (int i = 0; i < inputMesh.vertices.size(); ++i) {
        Vector4 vertex;
        vertex << inputMesh.vertices[i], 1;

        int firstIdx = 0;
        int lastIdx = matrices.size() - 1;

        Vector4 firstVertex = matrices[firstIdx] * vertex;
        Vector4 lastVertex = matrices[lastIdx] * vertex;

        Vector3 firstVertices(firstVertex[0], firstVertex[1], firstVertex[2]);
        firstVertices /= firstVertex[3];

        Vector3 lastVertices(lastVertex[0], lastVertex[1], lastVertex[2]);
        lastVertices /= lastVertex[3];

        outputMesh.vertices.push_back(firstVertices);
        outputMesh.vertices.push_back(lastVertices);
    }

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
            
            // compute the measure of orthogonality
            Vector4 prod = (currentMat.inverse() * diffMat * v_diff).normalized();
            Float measure = normal.dot(prod);

            // check if DM and normal are orthogonal
            // only add a vertice if it wasn't added before
            const Float eps = 0.1;
            if (v_diff.dot(n_tnf) < eps && v_diff.dot(n_tnf) > -eps) {
                if (!isInserted.at(vertexIdx)) {
                    isInserted.at(vertexIdx) = true;
                    outputMesh.vertices.push_back(v);
                }  
            }
           
        }
    }

    cout << "Resulting vertices: " << outputMesh.vertices.size() << endl;;

}

/*
 * Thin out the points of the point cloud with the help of a rotation axis.
 */
void Transform::transformVerticesRA(const Mesh &inputMesh,
                                    const std::vector<Matrix4, allocM> &matrices,
                                    Mesh &outputMesh) 
{

    // for special cases at first and last time step, take all vertices.
    for (int i = 0; i < inputMesh.vertices.size(); ++i) {
        Vector4 vertex;
        vertex << inputMesh.vertices[i], 1;

        int firstIdx = 0;
        int lastIdx = matrices.size() - 1;

        Vector4 firstVertex = matrices[firstIdx] * vertex;
        Vector4 lastVertex = matrices[lastIdx] * vertex;

        Vector3 firstVertices(firstVertex[0], firstVertex[1], firstVertex[2]);
        firstVertices /= firstVertex[3];

        Vector3 lastVertices(lastVertex[0], lastVertex[1], lastVertex[2]);
        lastVertices /= lastVertex[3];

        outputMesh.vertices.push_back(firstVertices);
        outputMesh.vertices.push_back(lastVertices);
    }

    for (int i = 1; i < matrices.size() - 1; ++i) {
        std::vector<bool> isInserted;
        isInserted.resize(inputMesh.vertices.size(), false);

        Matrix4 axisMat = matrices[i-1] * matrices[i+1].inverse() * matrices[i];
        Matrix4 currentMat = matrices[i];

        std::tuple<Vector3, Vector3> rotationAxis = calculateRA(axisMat);

        Vector3 axisPosition = std::get<0>(rotationAxis);
        Vector3 axisDirection = std::get<1>(rotationAxis);

        auto inverseMat = matrices[i].inverse();

        Vector4 pos_hom;
        pos_hom << axisPosition, 1;
        pos_hom = inverseMat * pos_hom;
        axisPosition = Vector3(pos_hom[0], pos_hom[1], pos_hom[2]) / pos_hom[3];

        Vector4 dir_hom;
        dir_hom << axisDirection, 0;
        dir_hom = inverseMat * dir_hom;
        axisDirection = Vector3(dir_hom[0], dir_hom[1], dir_hom[2]);

        for (int j = 0; j < inputMesh.vertexNormalPairs.size(); ++j) {
            IndexType vertexIdx = std::get<0>(inputMesh.vertexNormalPairs[j]);
            IndexType normalIdx = std::get<1>(inputMesh.vertexNormalPairs[j]);

            Vector3 vertex;
            vertex << inputMesh.vertices[vertexIdx];
            Vector3 normal;
            normal << inputMesh.normals[normalIdx];

            Vector3 pointPositionVec = (vertex - axisPosition).normalized();

            Matrix3 measureMat;
            measureMat << normal[0], pointPositionVec[0], axisDirection[0],
                          normal[1], pointPositionVec[1], axisDirection[1],
                          normal[2], pointPositionVec[2], axisDirection[2];

            float measure = measureMat.determinant();
            if (measure < 0) {
                measure *= -1;
            }

            float threshold = 0.1f;
            if (measure < threshold && !isInserted.at(vertexIdx)) {
                isInserted.at(vertexIdx) = true;

                Vector4 vertex_hom;
                vertex_hom << vertex, 1;
                Vector4 v_hom = currentMat * vertex_hom;
                Vector3 v(v_hom[0], v_hom[1], v_hom[2]);
                v /= v_hom[3];

                outputMesh.vertices.push_back(v);
            }
        }
    }

    cout << "Resulting vertices: " << outputMesh.vertices.size() << endl;

}

std::tuple<Vector3,Vector3> Transform::calculateRA(Matrix4 &matrix) {
    Vector3 translation = matrix.block<3,1>(0,3);
    Matrix3 rotation = matrix.block<3,3>(0,0);

    Matrix3 A = rotation - Matrix3::Identity();

    auto decomp = A.fullPivLu();

    Vector3 position = decomp.solve(-translation);
    Vector3 direction = decomp.kernel().normalized();

    return std::make_tuple(position, direction);
}
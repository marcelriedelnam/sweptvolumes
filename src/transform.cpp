#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

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
 */
void Transform::transformVerticesDM(const Mesh &inputMesh,
                                    const std::vector<Matrix4, allocM> &matrices,
                                    Mesh &outputMesh)
{
    for (int i = 0; i < matrices.size(); ++i) {
        int matSize = matrices.size();

        std::vector<Vector3, allocV> Ns;
        IndexType currVertexIdx = 0;

        Matrix4 diffMat;
        if (i == 0) diffMat = matrices[1] - matrices[0];
        else if (i == matSize - 1) diffMat = matrices[matSize - 1] - matrices[matSize - 2];
        else diffMat = matrices[i+1] - matrices[i-1];

        for (int j = 0; j < inputMesh.vertexNormalPairs.size(); ++j) {
            IndexType vertexIdx = std::get<0>(inputMesh.vertexNormalPairs[j]);
            IndexType normalIdx = std::get<1>(inputMesh.vertexNormalPairs[j]);

            if (vertexIdx != currVertexIdx && j != 0) {
                Vector4 hv;
                hv << inputMesh.vertices[currVertexIdx], 1;
                Cases c = i == 0 ? FIRST : (i == matSize - 1 ? LAST : MID);
                Vector4 hV = matrices[i] * hv;
                Vector3 V(hV[0], hV[1], hV[2]);
                V /= hV[3];

                if(insertIfCriteria(hv, diffMat, Ns, c)) {
                    outputMesh.vertices.push_back(V);
                }

                Ns.clear();
                currVertexIdx = vertexIdx;
            }

            
            Vector4 hn;
            hn << inputMesh.normals[normalIdx], 0;

            Vector4 hN = matrices[i] * hn; // correctly it would be (currentMat^T)^(-1),
                                                 // equal to currentMat due to orthogonality
            Vector3 N(hN[0], hN[1], hN[2]);
            Ns.push_back(N);
        
        }
        Vector4 hv;
        hv << inputMesh.vertices[currVertexIdx], 1;
        Cases c = i == 0 ? FIRST : (i == matSize - 1 ? LAST : MID);
        Vector4 hV = matrices[i] * hv;
        Vector3 V(hV[0], hV[1], hV[2]);
        V /= hV[3];

        if(insertIfCriteria(hv, diffMat, Ns, c)) {
            outputMesh.vertices.push_back(V);
        }

        Ns.clear();
    }

    cout << "Resulting vertices: " << outputMesh.vertices.size() << endl;

}

bool Transform::insertIfCriteria(const Vector4 &hv, const Matrix4 &diffMat, 
                         const std::vector<Vector3, allocV> &Ns, Cases c) {
    Vector4 hv_diff = diffMat * hv;
    Vector3 v_diff = Vector3(hv_diff[0], hv_diff[1], hv_diff[2]).normalized();

    bool isNeg = false;
    bool isPos = false;
    for (auto& N : Ns) {
        bool sign = v_diff.dot(N) > 0;
        isNeg = isNeg || !sign;
        isPos = isPos || sign;
    }

    if (isNeg && isPos) {
        return true;
    }

    const Float eps = 0.1;
    for (auto& N : Ns) {
        if (c == FIRST && v_diff.dot(N) < eps) {
            return true;
        }
        else if (c == LAST && v_diff.dot(N) > - eps) {
            return true;
        }
    }
    return false;
}

/*
 * Thin out the points of the point cloud with the help of a rotation axis.
 */
void Transform::transformVerticesRA(const Mesh &inputMesh,
                                    const std::vector<Matrix4, allocM> &matrices,
                                    Mesh &outputMesh) 
{

    for (int i = 0; i < matrices.size(); ++i) {
        std::vector<bool> isInserted;
        isInserted.resize(inputMesh.vertices.size(), false);

        Matrix4 axisMat;
        Matrix4 currentMat = matrices[i];
        if (i == 0) {
            axisMat = matrices[i] * invert(matrices[i+1]) * matrices[i];
        }
        if (i == matrices.size() - 1) {
            axisMat = matrices[i-1] * invert(matrices[i]) * matrices[i];
        }
        else {
            axisMat = matrices[i-1] * invert(matrices[i+1]) * matrices[i];
        }
        

        std::tuple<Vector3, Vector3> rotationAxis = calculateRA(axisMat);

        Vector3 axisPosition = std::get<0>(rotationAxis);
        Vector3 axisDirection = std::get<1>(rotationAxis);

        auto inverseMat = invert(matrices[i]);

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
/*
 * Homogeneous matrices can be inverted easier than other 4x4 matrices.
 * This function takes advanatge of that and calculates the inverse of a homogeneous 4x4 matrix.
 *  
 *  | R^T | -(R^T) * t  |
 *  |_____|_____________|
 *  |0 0 0|      1      |
 * 
 *  R^T is the transposed matrix of rotation matrix.
 *  t is the tranlation vector of the homogeneous 4x4 matrix.
 */
Matrix4 Transform::invert(Matrix4 matrix) {
    Vector3 translation = matrix.block<3,1>(0,3);
    Matrix3 rotation = matrix.block<3,3>(0,0);

    Matrix3 newRot = rotation.transpose();
    Vector3 newTrans = - (rotation.transpose() * translation);

    Matrix4 res;
    res << 0, 0, 0, 0,
           0, 0, 0, 0,
           0, 0, 0, 0,
           0, 0, 0, 1;
    res.block<3,3>(0,0) = newRot;
    res.block<3,1>(0,3) = newTrans;

    return res;
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

/*
 * calculates the AABB (axis aligned bounding box) of the object at a certain time
 *
std:vector<Vector3, allocV> Transform::getAABB(std::vector<Vector3, allocV> vertices, Matrix4 matrix) {
    Float x_min, x_max, y_min, y_max, z_min, z_max;
    for (int i = 0; i < vertices.size(); ++i) {
        
        Vecor3 vertex = matrix * vertices[i];

        if (i == 0) {
            x_min = vertex[0];
            x_max = vertex[0];
            y_min = vertex[1];
            y_max = vertex[1];
            z_min = vertex[2];
            z_max = vertex[2];
            break;
        }

        x_val = vertex[0];
        y_val = vertex[1];
        z_val = vertex[2];

        if (x_val < x_min) x_min = x_val;
        else if (x_val > x_max) x_max = x_val;
        if (y_val < y_min) y_min = y_val;
        else if (y_val > y_max) y_max = y_val;
        if (z_val < z_min) z_min = z_val;
        else if (z_val > z_max) z_max = z_val;
    }
                7---8
    z          /   /|
    |  y      5---6 4
    | /       |   |/
    |/___ x   1---2

    Vector3 one, two, three, four, five, six, seven, eight;
    one << x_min, y_min, z_min;
    two << x_max, y_min, z_min;
    three << x_min, y_max, z_min;
    four << x_max, y_max, z_min;
    five << x_min, y_min, z_max;
    six << x_max, y_min, z_max;
    seven << x_min, y_max, z_max;
    eight << x_max, y_max, z_max;

    std::vector<Vector3, allocV> aabb = {one, two, three, four, five, six, seven, eight};
    return aabb;
}*/
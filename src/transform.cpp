#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <cmath>

#include "transform.h"

using std::cout;
using std::endl;

std::unique_ptr<Mesh> Transform::transform(const Mesh &inputMesh, const std::vector<Matrix4, allocM> &matrices) {
    auto outputMesh = Mesh::copyEmptyFrom(inputMesh);
    auto subsampledMat = subsampling(matrices);
    // transformVerticesNaive(inputMesh, matrices, *outputMesh);
    transformVerticesDM(inputMesh, subsampledMat, *outputMesh);
    // transformVerticesRA(inputMesh, matrices, *outputMesh);

    return outputMesh;
}


// naively computes the vertices for every time step and returns all matrices
void Transform::transformVerticesNaive(const Mesh &inputMesh,
                                       const std::vector<Matrix4, allocM> &matrices,
                                       Mesh &outputMesh)
{
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

void Transform::transformVerticesDM(const Mesh &inputMesh,
                                    const std::vector<Matrix4, allocM> &matrices,
                                    Mesh &outputMesh)
{
    Float maxVel = 0;
    for (int i = 0; i < matrices.size(); ++i) {

        int matSize = matrices.size();

        //if (i != 76) continue;

		if (i != matSize - 1) {
			auto aabb = getAABB(inputMesh);	
			Matrix4 current = matrices[i];
			Matrix4 next = matrices[i+1];
			auto maxV = getLargestMovement(aabb, current, next);
			if (maxV > maxVel) maxVel = maxV;
		}

        std::vector<Vector3, allocV> Ns;
        IndexType currVertexIdx = 0;

        Matrix4 diffMat;
        if (i == 0) diffMat = matrices[1] - matrices[0];
        else if (i == matSize - 1) diffMat = matrices[matSize - 1] - matrices[matSize - 2];
        else diffMat = matrices[i+1] - matrices[i-1];

        Matrix4 before, after;
        if (i == 0) {
            before = after = matrices[1] - matrices[0];
        }
        else if (i == matSize - 1) {
            before = after = matrices[matSize - 1] - matrices[matSize - 2];
        }
        else {
            before = matrices[i] - matrices[i-1];
            after = matrices[i+1] - matrices[i];
        }

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

            if (insertCriteria(hv, diffMat, before, after, Ns, c)) {
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

        if(insertCriteria(hv, diffMat, before, after, Ns, c)) {
            outputMesh.vertices.push_back(V);
        }

        Ns.clear();
    }

	cout << "Max. movement a single vertex does during one time step: " << maxVel << endl;
	cout << "Largest edge in the input mesh: " << inputMesh.longestMeshEdge << endl;
    cout << endl;
}

bool Transform::insertCriteria(const Vector4 &hv, const Matrix4 &diffMat, const Matrix4 &before,
                                 const Matrix4 &after, const std::vector<Vector3, allocV> &Ns, Cases c) {

    Vector4 hv_diff = diffMat * hv;
    Vector3 v_diff = Vector3(hv_diff[0], hv_diff[1], hv_diff[2]).normalized();
    const Float epsSC = 0.001;

    for (auto& N : Ns) {
        if (c == FIRST && v_diff.dot(N) < epsSC) {
            return true;
        }
        else if (c == LAST && v_diff.dot(N) > - epsSC) {
            return true;
        }
    }

    Vector4 hDeltaT0 = before * hv;
    Vector3 deltaT0 = Vector3(hDeltaT0[0], hDeltaT0[1], hDeltaT0[2]).normalized();
    Vector4 hDeltaT1 = after * hv;
    Vector3 deltaT1 = Vector3(hDeltaT1[0], hDeltaT1[1], hDeltaT1[2]).normalized();
    Float a_min, a_max;
    Float b_min, b_max;

    for (int i = 0; i < Ns.size(); ++i) {
        Float a, b, c;
        a = deltaT0.dot(Ns[i]);
        b = deltaT1.dot(Ns[i]);
        if (i == 0) {
            a_min = a;
            a_max = a;
            b_min = b;
            b_max = b;
        }        
        if (a > a_max) {
            a_max = a;
        }
        else if (a < a_min) {
            a_min = a;
        }
        if (b > b_max) {
            b_max = b;
        }
        else if (b < b_min) {
            b_min = b;
        }
    }

    const Float eps = 0.0001; //PARAMETER
    if (a_max >= -eps && a_min <= eps) {
        return true;
    }
    else if (b_max >= -eps && b_min <= eps) {
        return true;
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
Matrix4 Transform::invert(const Matrix4 &matrix) {
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
 * calculates the AABB (axis aligned bounding box) of an object
 */
std::vector<Vector3, allocV> Transform::getAABB(const Mesh &inputMesh) {
    Float x_min, x_max, y_min, y_max, z_min, z_max;
    x_min = x_max = inputMesh.vertices[0][0];
    y_min = y_max = inputMesh.vertices[0][1];
    z_min = z_max = inputMesh.vertices[0][2];

    for (int i = 1; i < inputMesh.vertices.size(); ++i) {
        
		Vector3 vertex = inputMesh.vertices[i];    

        auto x_val = vertex[0];
        auto y_val = vertex[1];
        auto z_val = vertex[2];

        if (x_val < x_min) x_min = x_val;
        else if (x_val > x_max) x_max = x_val;
        if (y_val < y_min) y_min = y_val;
        else if (y_val > y_max) y_max = y_val;
        if (z_val < z_min) z_min = z_val;
        else if (z_val > z_max) z_max = z_val;
    }
    /*               7---8
     *   z          /   /|
     *   |  y      5---6 4
     *   | /       |   |/
     *   |/___ x   1---2
	 */

    Vector3 one, two, three, four, five, six, seven, eight;
    one <<   x_min, y_min, z_min;
    two <<   x_max, y_min, z_min;
    three << x_min, y_max, z_min;
    four <<  x_max, y_max, z_min;
    five <<  x_min, y_min, z_max;
    six <<   x_max, y_min, z_max;
    seven << x_min, y_max, z_max;
    eight << x_max, y_max, z_max;

    std::vector<Vector3, allocV> aabb = {one, two, three, four, five, six, seven, eight};
    return aabb;
}

Float Transform::getLargestMovement(const std::vector<Vector3, allocV> &aabb, const Matrix4 &current, const Matrix4 &next)
{
	Float max = 0;
	for (int i = 0; i < 8; ++i) {
		Vector4 vertex;
		vertex << aabb[i], 1;
		Vector4 vertex0 = current * vertex;
		Vector4 vertex1 = next * vertex;
		Vector3 v0 = Vector3(vertex0[0], vertex0[1], vertex0[2]);
		v0 /= vertex0[3];
		Vector3 v1 = Vector3(vertex1[0], vertex1[1], vertex1[2]);
		v1 /= vertex1[3];
		Float movement = (v1 - v0).norm();

		if (movement > max) max = movement;
	}
	return max;
}

Matrix4 Transform::buildHomogenousMatrix(const Matrix3 &mat, const Vector3 &vec) {
    Matrix4 res;
    res << 0, 0, 0, 0,
           0, 0, 0, 0,
           0, 0, 0, 0,
           0, 0, 0, 1;
    res.block<3,3>(0,0) = mat;
    res.block<3,1>(0,3) = vec;

    return res;
}

std::vector<Matrix4, allocM> Transform::subsampling(const std::vector<Matrix4, allocM> &matrices) {
    int subsamples = 0; //PARAMETER
    Float nrSubsamples = 1.0 / (subsamples + 1);
    std::vector<Matrix4, allocM> interpolated;

    for (int i = 0; i < matrices.size() - 1; ++i) {

        Vector3 transBeg = matrices[i].block<3,1>(0,3);
        Vector3 transEnd = matrices[i+1].block<3,1>(0,3);
        Matrix3 rotBeg = matrices[i].block<3,3>(0,0);
        Matrix3 rotEnd = matrices[i+1].block<3,3>(0,0);

        Matrix4 hMat;
        hMat = buildHomogenousMatrix(rotBeg, transBeg);
        interpolated.push_back(hMat);
        for (int k = 1; k <= subsamples; k++) {
            Float alpha = k * nrSubsamples;
            // interpolate translation vector
            Vector3 tRes = (1 - alpha) * transBeg + alpha * transEnd;
            // get quaternion from rotation matrix
            Quaternion qBeg(rotBeg);
            Quaternion qEnd(rotEnd);
            // interpolate quaternion
            Quaternion qRes;
            qRes = qBeg.slerp(alpha, qEnd); // TODO: reihenfolge?
            // transform quaternion back to rotation matrix
            Matrix3 rRes = qRes.normalized().toRotationMatrix();
            // push back resulting interpolated matrix
            hMat = buildHomogenousMatrix(rRes, tRes);
            interpolated.push_back(hMat);
        }

    }

    Vector3 translation = matrices[matrices.size() - 1].block<3,1>(0,3);
    Matrix3 rotation = matrices[matrices.size() - 1].block<3,3>(0,0);
    Matrix4 hMat;
    hMat = buildHomogenousMatrix(rotation, translation);
    interpolated.push_back(hMat);
    cout << "Number of interpolated matrices: " << interpolated.size() << endl;
    cout << endl;

    return interpolated;
}
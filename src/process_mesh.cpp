#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <cmath>

#include "process_mesh.h"
#include "config.h"

using std::cout;
using std::endl;

std::vector<Vector3, allocV> ProcessMesh::tempNorms{};
std::vector<Vector3, allocV> ProcessMesh::meanNorms{};
std::vector<IndexTypePair> ProcessMesh::tempPairs{};
GridStructure<IndexType> ProcessMesh::normalIdxGrid;
GridStructure<IndexType> ProcessMesh::vertexIdxGrid;
std::unordered_set<IndexTypePair, IndexTypePairHasher> ProcessMesh::vertexNormalPairSet;

std::unique_ptr<Mesh> ProcessMesh::process(const Mesh &inputMesh) {
    auto outputMesh = std::make_unique<Mesh>();
    processMesh(inputMesh, *outputMesh);

    cout << endl;
    cout << "After processing mesh:" << endl;
    cout << "Resulting vertices: " << outputMesh->vertices.size() << endl;
    cout << "Resulting normals: " << outputMesh->normals.size() << endl;
    cout << endl;

    return outputMesh;
}

void ProcessMesh::processMesh(const Mesh &inputMesh, Mesh &outputMesh) {
    normalIdxGrid.setCellSize(ParameterConfig::gridCellLength);
    vertexIdxGrid.setCellSize(ParameterConfig::gridCellLength);


    for (int i = 0; i < inputMesh.faces.size(); ++i) {
        IndexType vertIdx0 = std::get<0>(inputMesh.faces[i]);
        IndexType vertIdx1 = std::get<1>(inputMesh.faces[i]);
        IndexType vertIdx2 = std::get<2>(inputMesh.faces[i]);

        Vector3 a = inputMesh.vertices[vertIdx0];
        Vector3 b = inputMesh.vertices[vertIdx1];
        Vector3 c = inputMesh.vertices[vertIdx2];

        Vector3 ab = (b - a).normalized();
        Vector3 ac = (c - a).normalized();
        Float angleA = std::acos(ab.dot(ac));
        Vector3 normalA = ab.cross(ac).normalized();
        Vector3 weightedNormalA = angleA * normalA;

        Vector3 ba = (a - b).normalized();
        Vector3 bc = (c - b).normalized();
        Float angleB = std::acos(ba.dot(bc));
        Vector3 normalB = ba.cross(bc).normalized();
        Vector3 weightedNormalB = angleB * normalB;

        Vector3 ca = (a - c).normalized();
        Vector3 cb = (b - c).normalized();
        Float angleC = std::acos(ca.dot(cb));
        Vector3 normalC = ca.cross(cb).normalized();
        Vector3 weightedNormalC = angleC * normalC;

        IndexType normalIdxA, normalIdxB, normalIdxC;

        tempNorms.push_back(weightedNormalA);
        normalIdxA = tempNorms.size() - 1;
        tempNorms.push_back(weightedNormalB);
        normalIdxB = tempNorms.size() - 1;
        tempNorms.push_back(weightedNormalC);
        normalIdxC = tempNorms.size() - 1;

        tempPairs.push_back(std::make_tuple(vertIdx0, normalIdxA));
        tempPairs.push_back(std::make_tuple(vertIdx1, normalIdxB));
        tempPairs.push_back(std::make_tuple(vertIdx2, normalIdxC));
        
    }

    // sort the vertex-normal pairs for later calculations
    std::sort(tempPairs.begin(), tempPairs.end());

    getMeanNormal(outputMesh);
    
    outputMesh.longestMeshEdge = 0;

    for (int i = 0; i < inputMesh.faces.size(); ++i) {
        IndexType vertIdx0 = std::get<0>(inputMesh.faces[i]);
        IndexType vertIdx1 = std::get<1>(inputMesh.faces[i]);
        IndexType vertIdx2 = std::get<2>(inputMesh.faces[i]);

        std::tuple<Vector3, Vector3> tup0 = std::make_tuple(inputMesh.vertices[vertIdx0], meanNorms[vertIdx0]);
        std::tuple<Vector3, Vector3> tup1 = std::make_tuple(inputMesh.vertices[vertIdx1], meanNorms[vertIdx1]);
        std::tuple<Vector3, Vector3> tup2 = std::make_tuple(inputMesh.vertices[vertIdx2], meanNorms[vertIdx2]);

        subdivide(tup0, tup1, tup2, outputMesh);
    }

    // sort pairs for later calulations
    std::sort(outputMesh.vertexNormalPairs.begin(), outputMesh.vertexNormalPairs.end());
}

void ProcessMesh::getMeanNormal(Mesh &outputMesh) {
    IndexType currVertexIdx = 0;
    std::vector<Vector3, allocV> normals;

    for (int i = 0; i < tempPairs.size(); ++i) {
        IndexType vertexIdx = std::get<0>(tempPairs[i]);
        IndexType normalIdx = std::get<1>(tempPairs[i]);

        if (currVertexIdx != vertexIdx) {
            Vector3 meanNormal = Vector3(0,0,0);
            for (int j = 0; j < normals.size(); ++j) {
                meanNormal += normals[j];
            }
            meanNorms.push_back(meanNormal.normalized());
            normals.clear();
            currVertexIdx = vertexIdx;
        }

        Vector3 currNormal = tempNorms[normalIdx];
        normals.push_back(currNormal);
    }

    Vector3 meanNormal = Vector3(0,0,0);
    for (int j = 0; j < normals.size(); ++j) {
        meanNormal += normals[j];
    }
    meanNorms.push_back(meanNormal.normalized());
    normals.clear();
}

Float ProcessMesh::getArea(const Vector3 &v0, const Vector3 &v1, const Vector3 &v2) {

    Float edgeLength0, edgeLength1, edgeLength2;
    edgeLength0 = (v1 - v0).norm();
    edgeLength1 = (v2 - v0).norm();
    edgeLength2 = (v2 - v1).norm();
    Float circumference = (edgeLength0 + edgeLength1 + edgeLength2) / 2.0;

    Float area = std::sqrt(circumference * (circumference - edgeLength0) * (circumference - edgeLength1) * (circumference - edgeLength2));
    return area;
}

void ProcessMesh::subdivide(const std::tuple<Vector3, Vector3> &tup0, const std::tuple<Vector3, Vector3> &tup1,
                            const std::tuple<Vector3, Vector3> &tup2, Mesh &outputMesh) {
    Float maxEdgeLength = ParameterConfig::maxEdgelength; //PARAMETER
    if (maxEdgeLength == 0) maxEdgeLength = std::numeric_limits<Float>::infinity();

    Vector3 v0 = std::get<0>(tup0);
    Vector3 v1 = std::get<0>(tup1);
    Vector3 v2 = std::get<0>(tup2);

    Vector3 n0 = std::get<1>(tup0);
    Vector3 n1 = std::get<1>(tup1);
    Vector3 n2 = std::get<1>(tup2);

    Float edgeLength0 = (v1 - v0).norm();
    Float edgeLength1 = (v2 - v0).norm();
    Float edgeLength2 = (v2 - v1).norm();

    if (edgeLength0 > maxEdgeLength && edgeLength1 > maxEdgeLength && edgeLength2 > maxEdgeLength) {
        // mittelpunkt aller kanten
        Vector3 v01 = (v0 + v1) / 2.0;
        Vector3 v02 = (v0 + v2) / 2.0;
        Vector3 v12 = (v1 + v2) / 2.0;

        // interpolierten normalen der mittelpunkte
        Vector3 n01 = ((n0 + n1) / 2.0).normalized();
        Vector3 n02 = ((n0 + n2) / 2.0).normalized();
        Vector3 n12 = ((n1 + n2) / 2.0).normalized();

        std::tuple<Vector3, Vector3> tup01 = std::make_tuple(v01, n01);
        std::tuple<Vector3, Vector3> tup02 = std::make_tuple(v02, n02);
        std::tuple<Vector3, Vector3> tup12 = std::make_tuple(v12, n12);
       
        subdivide(tup0, tup01, tup02, outputMesh);
        subdivide(tup2, tup02, tup12, outputMesh);
        subdivide(tup1, tup01, tup12, outputMesh);
        subdivide(tup01, tup02, tup12, outputMesh);
    }
    else if (edgeLength0 > maxEdgeLength && edgeLength1 > maxEdgeLength) {
        Vector3 v01 = (v0 + v1) / 2.0;
        Vector3 v02 = (v0 + v2) / 2.0;

        Vector3 n01 = ((n0 + n1) / 2.0).normalized();
        Vector3 n02 = ((n0 + n2) / 2.0).normalized();

        std::tuple<Vector3, Vector3> tup01 = std::make_tuple(v01, n01);
        std::tuple<Vector3, Vector3> tup02 = std::make_tuple(v02, n02);

        subdivide(tup0, tup01, tup02, outputMesh);
        subdivide(tup1, tup01, tup02, outputMesh);
        subdivide(tup1, tup2, tup02, outputMesh);
    }
    else if (edgeLength0 > maxEdgeLength && edgeLength2 > maxEdgeLength) {
        Vector3 v01 = (v0 + v1) / 2.0;
        Vector3 v12 = (v1 + v2) / 2.0;

        Vector3 n01 = ((n0 + n1) / 2.0).normalized();
        Vector3 n12 = ((n1 + n2) / 2.0).normalized();

        std::tuple<Vector3, Vector3> tup01 = std::make_tuple(v01, n01);
        std::tuple<Vector3, Vector3> tup12 = std::make_tuple(v12, n12);

        subdivide(tup1, tup01, tup12, outputMesh);
        subdivide(tup0, tup01, tup12, outputMesh);
        subdivide(tup0, tup2, tup12, outputMesh);
    }
    else if (edgeLength1 > maxEdgeLength && edgeLength2 > maxEdgeLength) {
        Vector3 v02 = (v0 + v2) / 2.0;
        Vector3 v12 = (v1 + v2) / 2.0;

        Vector3 n02 = ((n0 + n2) / 2.0).normalized();
        Vector3 n12 = ((n1 + n2) / 2.0).normalized();

        std::tuple<Vector3, Vector3> tup02 = std::make_tuple(v02, n02);
        std::tuple<Vector3, Vector3> tup12 = std::make_tuple(v12, n12);

        subdivide(tup2, tup02, tup12, outputMesh);
        subdivide(tup0, tup02, tup12, outputMesh);
        subdivide(tup0, tup1, tup12, outputMesh);
    }
    else if (edgeLength0 > maxEdgeLength) {
        Vector3 v01 = (v0 + v1) / 2.0;

        Vector3 n01 = ((n0 + n1) / 2.0).normalized();

        std::tuple<Vector3, Vector3> tup01 = std::make_tuple(v01, n01);

        subdivide(tup0, tup01, tup2, outputMesh);
        subdivide(tup01, tup1, tup2, outputMesh);
    }
    else if (edgeLength1 > maxEdgeLength) {
        Vector3 v02 = (v0 + v2) / 2.0;

        Vector3 n02 = ((n0 + n2) / 2.0).normalized();

        std::tuple<Vector3, Vector3> tup02 = std::make_tuple(v02, n02);

        subdivide(tup0, tup02, tup1, outputMesh);
        subdivide(tup02, tup1, tup2, outputMesh);
    }
    else if (edgeLength2 > maxEdgeLength) {
        Vector3 v12 = (v1 + v2) / 2.0;

        Vector3 n12 = ((n1 + n2) / 2.0).normalized();

        std::tuple<Vector3, Vector3> tup12 = std::make_tuple(v12, n12);

        subdivide(tup0, tup1, tup12, outputMesh);
        subdivide(tup0, tup12, tup2, outputMesh);
    }
    else {
        Vector3 interpNorm = (n0 + n1 + n2) / 3;
        Vector3 nm = interpNorm.normalized();

        insertIfNotExisiting(outputMesh, v0, nm);
        insertIfNotExisiting(outputMesh, v1, nm);
        insertIfNotExisiting(outputMesh, v2, nm);

        if (edgeLength0 > edgeLength1 && edgeLength0 > edgeLength2) {
            if (edgeLength0 > outputMesh.longestMeshEdge) outputMesh.longestMeshEdge = edgeLength0;
        }
        else if (edgeLength1 > edgeLength2) {
            if (edgeLength1 > outputMesh.longestMeshEdge) outputMesh.longestMeshEdge = edgeLength1;
        }
        else if (edgeLength2 > outputMesh.longestMeshEdge) outputMesh.longestMeshEdge = edgeLength2;
    }
}

void ProcessMesh::insertIfNotExisiting(Mesh &outputMesh, const Vector3 &vertex, const Vector3 &normal) {
    IndexType vertexIdx;
    IndexType normalIdx;

    if (!normalIdxGrid.contains(normal, normalIdx)) {
        outputMesh.normals.push_back(normal);
        normalIdx = outputMesh.normals.size() - 1;
        normalIdxGrid.insert(normal, normalIdx);
    }

    if (!vertexIdxGrid.contains(vertex, vertexIdx)) {
        outputMesh.vertices.push_back(vertex);
        vertexIdx = outputMesh.vertices.size() - 1;
        vertexIdxGrid.insert(vertex, vertexIdx);
    }

    IndexTypePair tup = std::make_tuple(vertexIdx, normalIdx);
    if (vertexNormalPairSet.find(tup) == vertexNormalPairSet.end()) {
        outputMesh.vertexNormalPairs.push_back(tup);
        vertexNormalPairSet.insert(tup);
    }
}
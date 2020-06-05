#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <cmath>

#include "process_mesh.h"

using std::cout;
using std::endl;

std::vector<Vector3, allocV> ProcessMesh::tempNorms{};
std::vector<Vector3, allocV> ProcessMesh::meanNorms{};
std::vector<IndexTypePair> ProcessMesh::tempPairs{};

std::unique_ptr<Mesh> ProcessMesh::process(const Mesh &inputMesh) {
    auto outputMesh = std::make_unique<Mesh>();
    processMesh(inputMesh, *outputMesh);

    cout << inputMesh.vertices.size() << " vertices read." << endl;
    cout << endl;
    cout << "After processing mesh:" << endl;
    cout << "Resulting vertices: " << outputMesh->vertices.size() << endl;
    cout << "Resulting normals: " << outputMesh->normals.size() << endl;
    cout << "Resulting vertex-normal pairs: " << outputMesh->vertexNormalPairs.size() << endl;
    cout << endl;

    return outputMesh;
}

void ProcessMesh::processMesh(const Mesh &inputMesh, Mesh &outputMesh) {
    
    for (int i = 0; i < inputMesh.faces.size(); ++i) {
        IndexType vertIdx0 = std::get<0>(inputMesh.faces[i]);
        IndexType vertIdx1 = std::get<1>(inputMesh.faces[i]);
        IndexType vertIdx2 = std::get<2>(inputMesh.faces[i]);

        Vector3 a = inputMesh.vertices[vertIdx0];
        Vector3 b = inputMesh.vertices[vertIdx1];
        Vector3 c = inputMesh.vertices[vertIdx2];

        Float areaSize = getArea(a, b, c);

        Vector3 ab = (b - a).normalized();
        Vector3 ac = (c - a).normalized();

        Vector3 normal = ab.cross(ac).normalized();
        Vector3 weightedNormal = areaSize * normal;
        IndexType normalIdx;

        tempNorms.push_back(weightedNormal);
        normalIdx = tempNorms.size() - 1;
        
        
        std::array<std::tuple<IndexType, IndexType>, 3> indexTups {
            std::make_tuple(vertIdx0, normalIdx),
            std::make_tuple(vertIdx1, normalIdx),
            std::make_tuple(vertIdx2, normalIdx)
        };
        for (auto& tup : indexTups) {
            tempPairs.push_back(tup);
        }
        
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
    Float maxEdgeLength = 0.04; //PARAMETER

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
        Vector3 v01 = (v0 + v1) / 2.0;
        Vector3 v02 = (v0 + v2) / 2.0;
        Vector3 v12 = (v1 + v2) / 2.0;

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
        // radius incircle triangle: r = 2A / (a + b + c)
        Float area = getArea(v0, v1, v2);
        Float circumference = edgeLength0 + edgeLength1 + edgeLength2;
        Float radius = 2 * area / circumference;

        Float areaTri0 = (edgeLength0 * radius) / 2.0;
        Float areaTri1 = (edgeLength1 * radius) / 2.0;
        Float areaTri2 = (edgeLength2 * radius) / 2.0;

        Vector3 interpNorm = areaTri0 * n2 + areaTri1 * n1 + areaTri2 * n0;
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

void ProcessMesh::insertIfNotExisiting(Mesh &outputMesh, const Vector3 &vertice, const Vector3 &normal) {
    IndexType vertexIdx;
    IndexType normalIdx;

    auto it = std::find_if(outputMesh.normals.begin(), outputMesh.normals.end(), 
            [&normal](const Vector3 &vec){return (normal - vec).squaredNorm() < 1e-8;});
    if (it == outputMesh.normals.end()) {
        outputMesh.normals.push_back(normal);
        normalIdx = outputMesh.normals.size() - 1;
    }
    else {
        normalIdx = std::distance(outputMesh.normals.begin(), it);
    }

    auto iter = std::find_if(outputMesh.vertices.begin(), outputMesh.vertices.end(),
            [&vertice](const Vector3 &vec) {return (vertice - vec).squaredNorm() < 1e-8; });
    if (iter == outputMesh.vertices.end()) {
        outputMesh.vertices.push_back(vertice);
        vertexIdx = outputMesh.vertices.size() - 1;
    }
    else {
        vertexIdx = std::distance(outputMesh.vertices.begin(), iter);
    }
    IndexTypePair tup = std::make_tuple(vertexIdx, normalIdx);
    if (std::find(outputMesh.vertexNormalPairs.begin(), outputMesh.vertexNormalPairs.end(), tup)
        == outputMesh.vertexNormalPairs.end()) {
        outputMesh.vertexNormalPairs.push_back(tup);
    }
}
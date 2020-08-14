#ifndef _PROCESS_MESH
#define _PROCESS_MESH

#include "common.h"
#include "mesh.h"
#include "obj_reader.h"
#include "grid_structure.h"

#include <vector>

class ProcessMesh {
public:
    static std::unique_ptr<Mesh> process(const Mesh &inputMesh);

private:
    static std::vector<Vector3, allocV> tempNorms;
    static std::vector<Vector3, allocV> meanNorms;
    static std::vector<IndexTypePair> tempPairs;

    ProcessMesh() {}

    static void processMesh(const Mesh &inputMesh, Mesh &outputMesh);
    static void getMeanNormal(Mesh &outputMesh);
    static Float getArea(const Vector3 &v0, const Vector3 &v1, const Vector3 &v2);
    static void subdivide(const std::tuple<Vector3, Vector3> &tup0, const std::tuple<Vector3, Vector3> &tup1,
                          const std::tuple<Vector3, Vector3> &tup2, Mesh &outputMesh);
    static void insertIfNotExisiting(Mesh &outputMesh, const Vector3 &vertice, const Vector3 &normal);

    static GridStructure<IndexType> normalIdxGrid;
    static GridStructure<IndexType> vertexIdxGrid;
    static std::unordered_set<IndexTypePair, IndexTypePairHasher> vertexNormalPairSet;
};

#endif
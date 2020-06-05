#ifndef _OBJECT_READER_H
#define _OBJECT_READER_H

#include "common.h"
#include "mesh.h"

class ObjReader {
public:
    static std::unique_ptr<Mesh> read(const std::string &path);
    
private:
    ObjReader() {}

    static void insertVertex(const std::string &line, Mesh &mesh);
    static void handleNormalsAndPairs(const std::string &line, Mesh &mesh);
    static void simplfyEdge(Mesh &mesh, const Vector3 &v1, const Vector3 &v2, IndexType normalIdx);
};

#endif
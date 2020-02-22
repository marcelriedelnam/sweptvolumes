#ifndef _OBJECT_WRITER_H
#define _OBJECT_WRITER_H

#include "common.h"
#include "mesh.h"

class ObjWriter {
public:
    ObjWriter(const std::string &path, 
              const Mesh &mesh) : path(path){
        openFile(path);
        writeVertices(mesh.vertices);
        closeFile();
    }

private:
    void writeVertices(const std::vector<Vector3, allocV> &vertices);
    void writeFaces(const std::vector<std::tuple<IndexType,IndexType,IndexType>> &faces);
    void writeNormals(const std::vector<Vector3, allocV> &normals);
    void openFile(const std::string &path);
    void closeFile();

private:
    std::string path;
};


#endif
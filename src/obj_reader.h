#ifndef _OBJECT_READER_H
#define _OBJECT_READER_H

#include "common.h"

class ObjReader {
public:
    ObjReader(const std::string &path) : path(path) {
        readVerticesFromFile();
        readFacesFromFile();
    }
    std::vector<Vector3, allocV> getVertices();
    std::vector<std::tuple<IndexType,IndexType,IndexType>> getFaces();
    
private:
    void readVerticesFromFile();
    void readFacesFromFile();
    void insertVertex(std::string line);
    void insertFace(std::string line);
    
private:
    std::string path;
    std::vector<Vector3, allocV> vertices;
    std::vector<std::tuple<IndexType,IndexType,IndexType>> faces;
};

#endif
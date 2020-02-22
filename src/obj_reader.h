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
    static void insertPairs( const std::string &line, Mesh &mesh);
    static void insertNormal(const std::string &line, Mesh &mesh);
};

#endif
#ifndef _OBJECT_READER_H
#define _OBJECT_READER_H

#include "common.h"
#include "mesh.h"

class ObjReader {
public:
    ObjReader(const std::string &path) : path(path) {
		mesh = std::make_unique<Mesh>();
        readVerticesFromFile();
        readFacesFromFile();
    }
    
private:
    void readVerticesFromFile();
    void readFacesFromFile();
    void insertVertex(const std::string &line);
    void insertFace(const std::string &line);

public:
	std::unique_ptr<Mesh> mesh;

private:
    std::string path;
};

#endif
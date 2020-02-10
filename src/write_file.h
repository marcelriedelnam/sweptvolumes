#ifndef _WRITE_FILE_H
#define _WRITE_FILE_H

#include "common.h"

class WriteFile {
public:
    WriteFile(const std::string &path, 
              std::vector<Vector3, allocV> vertices, 
              std::vector<std::tuple<IndexType,IndexType,IndexType>> faces) : path(path){
        openFile(path);
        writeVertices(vertices);
        writeFaces(faces);
        closeFile();
    }

private:
    void writeVertices(std::vector<Vector3, allocV> vertices);
    void writeFaces(std::vector<std::tuple<IndexType,IndexType,IndexType>> faces);
    void openFile(std::string path);
    void closeFile();
    std::string toString(Eigen::Matrix<Float, 3, 1> mat);

private:
    std::string path;
};


#endif
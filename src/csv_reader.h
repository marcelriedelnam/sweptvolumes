#ifndef _CSV_READER_H
#define _CSV_READER_H

#include "common.h"

class CSVReader {
public:
    CSVReader(const std::string &path) : path(path) {
        readMatricesFromFile();
    }

    std::vector<Matrix4, allocM> getMatrices();

private:
    void readMatricesFromFile();
    void insertMatrix(std::string line);

private:
    std::string path;
    std::vector<Matrix4, allocM> matrices;

};

#endif

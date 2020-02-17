#ifndef _CSV_READER_H
#define _CSV_READER_H

#include "common.h"

class CSVReader {
public:
    CSVReader(const std::string &path) : path(path) {
        matrices = std::make_unique<std::vector<Matrix4, allocM>>();
        readMatricesFromFile();
    }

    std::vector<Matrix4, allocM> getMatrices();

private:
    void readMatricesFromFile();
    void insertMatrix(std::string &line);

public:
    std::unique_ptr<std::vector<Matrix4, allocM>> matrices;

private:
    std::string path;

};

#endif

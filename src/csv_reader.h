#ifndef _CSV_READER_H
#define _CSV_READER_H

#include "common.h"

class CSVReader {
public:
    static std::unique_ptr<std::vector<Matrix4, allocM>> read(const std::string &path);

private:
    CSVReader() {}

    static void insertMatrix(std::string line, std::vector<Matrix4, allocM> &matrices);

};

#endif

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "csv_reader.h"


using std::endl;
using std::cout;
using std::vector;
using std::string;

void CSVReader::readMatricesFromFile() {
    std::ifstream file(path);

    if (!file.is_open()) {
        cout << "Did not find file " << path << endl;
        throw std::runtime_error("Did not find file " + path);
    }

    IndexType matrixCount = 0;
    string input_line;
    while (std::getline(file, input_line)) { 
		if (input_line[0] == 'M') {
			continue;
		}
        matrixCount++;
    }

    file.clear();
    file.seekg(0, std::ios::beg);

    while (std::getline(file, input_line)) {
        if (input_line[0] == 'M') {
            continue;
        }
        insertMatrix(input_line);
    }

    file.close();
    cout << matrixCount << " matrices read." << endl;

}

void CSVReader::insertMatrix(string &line) {
    std::replace(line.begin(), line.end(), ';', ' ');
	std::stringstream ss(line);

    Float a11, a12, a13, a14,
	      a21, a22, a23, a24,
	      a31, a32, a33, a34,
	      a41, a42, a43, a44;

    Eigen::Matrix<Float, 4, 4> mat;
	
    ss >> a11 >> a12 >> a13 >> a14 
       >> a21 >> a22 >> a23 >> a24
	   >> a31 >> a32 >> a33 >> a34 
       >> a41 >> a42 >> a43 >> a44;

    mat << a11, a12, a13, a14, 
           a21, a22, a23, a24, 
           a31, a32, a33, a34, 
           a41, a42, a43, a44;

    matrices->push_back(mat);

}

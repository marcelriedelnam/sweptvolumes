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

std::unique_ptr<vector<Matrix4, allocM>> CSVReader::read(const std::string &path) {
    auto matrices = std::make_unique<vector<Matrix4, allocM>>();
    std::ifstream file(path);

        if (!file.is_open()) {
        throw std::runtime_error("Did not find file \"" + path + "\".");
    }
    size_t dot = path.find_last_of(".");
    string ext = path.substr(dot, path.size() - dot);
    if (dot != string::npos) {
        if (ext != ".csv") {
            throw std::out_of_range("File is not the right format.");
        }
    }
    
    string input_line;

    while (std::getline(file, input_line)) {
        if (input_line[0] == 'M') {
            continue;
        }
        if(input_line.size() == 0) {
            continue;
        }
        insertMatrix(input_line, *matrices);
    }

    file.close();
    cout << matrices->size() << " matrices read." << endl;

    return matrices;
}

void CSVReader::insertMatrix(string line, vector<Matrix4, allocM> &matrices) {
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

    matrices.push_back(mat);

}

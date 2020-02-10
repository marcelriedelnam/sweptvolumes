#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "write_file.h"
#include "obj_reader.h"

using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::to_string;

std::ofstream file;

void WriteFile::openFile(string path) {
	file.open(path);
}

void WriteFile::closeFile() {
	file.close();
}

void WriteFile::writeVertices(vector<Vector3, allocV> vertices) {
	cout << "Writing vertices." << endl;
	
	string buffer;
	Vector3 temp;
	for (int i = 0; i < vertices.size(); ++i) { 
		temp = vertices[i];
		string arrayValues = toString(temp);
		arrayValues.erase(std::remove(arrayValues.begin(), arrayValues.end(), ' '), arrayValues.end());
		std::replace(arrayValues.begin(), arrayValues.end(), '\n', ' ');
		buffer.append("v ");
		buffer.append(arrayValues);
		buffer.append(1, '\n');
	}
	
	file << buffer;
}

std::string WriteFile::toString(Eigen::Matrix<Float, 3, 1> mat) {
	std::stringstream ss;
	ss << mat;
	return ss.str();
}

void WriteFile::writeFaces(vector<std::tuple<IndexType,IndexType,IndexType>> faces) {
	cout << "Writing faces." <<endl;
	
	string buffer;
	for (const auto &i : faces) {
		buffer.append("f ");
		buffer.append(to_string(std::get<0>(i)));
		buffer.append(1, ' ');
		buffer.append(to_string(std::get<1>(i)));
		buffer.append(1, ' ');
		buffer.append(to_string(std::get<2>(i)));
		buffer.append(1, ' ');
		buffer.append(1, '\n');
	}
	
	file << buffer;
}
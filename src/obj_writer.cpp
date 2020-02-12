#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "obj_writer.h"
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
	
	Vector3 temp;
	for (int i = 0; i < vertices.size(); ++i) { 
		temp = vertices[i];
		file << "v " << temp.segment<1>(0) << ' ' << temp.segment<1>(1) << ' ' << temp.segment<1>(2) << '\n';
	}
}

void WriteFile::writeFaces(vector<std::tuple<IndexType,IndexType,IndexType>> faces) {
	cout << "Writing faces." <<endl;
	
	for (const auto &i : faces) {
		file << "f " << std::get<0>(i) << ' ' << std::get<1>(i) << ' ' << std::get<2>(i) << '\n';
	}
}
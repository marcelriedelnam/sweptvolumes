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

void ObjWriter::openFile(const string &path) {
	file.open(path);
}

void ObjWriter::closeFile() {
	file.close();
}

void ObjWriter::writeVertices(const vector<Vector3, allocV> &vertices) {
	cout << "Writing vertices." << endl;
	

	for (int i = 0; i < vertices.size(); ++i) { 
		file << "v " << vertices[i][0] << ' ' << vertices[i][1] << ' ' << vertices[i][2] << '\n';
	}
}

void ObjWriter::writeFaces(const vector<std::tuple<IndexType,IndexType,IndexType>> &faces) {
	cout << "Writing faces." <<endl;
	
	for (const auto &i : faces) {
		file << "f " << std::get<0>(i) << ' ' << std::get<1>(i) << ' ' << std::get<2>(i) << '\n';
	}
}
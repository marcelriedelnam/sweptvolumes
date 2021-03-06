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
		Vector3 vec = vertices[i];
		Matrix3 rotateX;
		rotateX << 1,  0, 0,
		           0,  0, 1,
				   0, -1, 0;

		Vector3 vertex = rotateX * vec;

		file << "v " << vertex[0] << ' ' << vertex[1] << ' ' << vertex[2] << '\n';
	}
}

void ObjWriter::writeFaces(const vector<std::tuple<IndexType,IndexType,IndexType>> &faces) {
	cout << "Writing faces." <<endl;
	
	for (const auto &i : faces) {
		// +1: indices start at 0 but should start at 1
		file << "f " << std::get<0>(i)+1 << ' ' << std::get<1>(i)+1 << ' ' << std::get<2>(i)+1 << '\n';
	}
}

void ObjWriter::writeNormals(const vector<Vector3, allocV> &normals) {
	cout << "Writing normals." << endl;
	

	for (int i = 0; i < normals.size(); ++i) { 
		Vector3 vec = normals[i];
		Matrix3 rotateX;
		rotateX << 1,  0, 0,
		           0,  0, 1,
				   0, -1, 0;

		Vector3 normal = rotateX * vec;

		file << "vn " << normal[0] << ' ' << normal[1] << ' ' << normal[2] << '\n';
	}
}
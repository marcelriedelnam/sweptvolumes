#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <memory>
#include <stdexcept>
#include <cmath>
#include <algorithm>
#include <iterator>

#include "obj_reader.h"


using std::string;
using std::vector;
using std::cout;
using std::endl;


std::unique_ptr<Mesh> ObjReader::read(const std::string &path) {
    auto mesh = std::make_unique<Mesh>();
    std::ifstream file(path);

    if (!file.is_open()) {
        //cout << "Did not find file " << path << endl;
        throw std::runtime_error("Did not find file.");
    }
    size_t dot = path.find_last_of(".");
    string ext = path.substr(dot, path.size() - dot);
    if (dot != string::npos) {
        if (ext != ".obj") {
            throw std::out_of_range("File is not the right format.");
        }
    }

    string input_line;

    while (std::getline(file, input_line)) {
        if (input_line[0] == 'v' && input_line[1] == ' ') {
            insertVertex(input_line, *mesh);
        } else if (input_line[0] == 'f' && input_line[1] == ' ') {
            insertFace(input_line, *mesh);
        }
    }

    file.close();
    cout << mesh->vertices.size() << " vertices read." << endl;
    return mesh;
}

void ObjReader::insertVertex(const string &line, Mesh &mesh) {
    
    string temp;
    for (int i = 2; i < line.length(); ++i) {
        temp.push_back(line[i]);
    }

    std::stringstream ss(temp);

    Float x, y, z;
    ss >> x >> y >> z;

    Vector3 vec(x, y, z);

    Matrix3 rotateX;
    rotateX << 1, 0,  0,
               0, 0, -1,
               0, 1,  0;
    
    Vector3 vertex = rotateX * vec;

	mesh.vertices.push_back(vertex);
}

void ObjReader::insertFace(const string &line, Mesh &mesh) {

    bool vertexIndex = true;
	std::string temp;
	for (int i = 2; i < line.length(); ++i) {
		switch(line[i]) {
			case '/':
				vertexIndex = false;
				break;
			case ' ':
				vertexIndex = true;
				temp.append(1, ' ');
				break;
			default:
				if (!vertexIndex) {
					break;
				}
				temp.push_back(line[i]);
        }
	}

    std::stringstream ss(temp);

    IndexType v0, v1, v2;
    ss >> v0 >> v1 >> v2;     

    // -1: indices start at 1 but for later purposes it's easier if they start at 0
    std::tuple<IndexType, IndexType, IndexType> face = std::make_tuple(v0 - 1, v1 - 1, v2 - 1);
    mesh.faces.push_back(face);

}
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <memory>
#include <stdexcept>

#include "obj_reader.h"


using std::string;
using std::vector;
using std::cout;
using std::endl;


std::unique_ptr<Mesh> ObjReader::read(const std::string &path) {
    auto mesh = std::make_unique<Mesh>();
    std::ifstream file(path);

    if (!file.is_open()) {
        cout << "Did not find file " << path << endl;
        throw std::runtime_error("Did not find file " + path);
    }

    string input_line;
    mesh->longestEdge = 0;

    while (std::getline(file, input_line)) {
        if (input_line[0] == 'v' && input_line[1] == ' ') {
            insertVertex(input_line, *mesh);
        } else if (input_line[0] == 'f' && input_line[1] == ' ') {
            handleNormalsAndPairs(input_line, *mesh);
        }
    }

    file.close();

    cout << mesh->vertices.size() << " vertices read." << endl;
    cout << mesh->normals.size() << " normals read." << endl;
    cout << mesh->vertexNormalPairs.size() << " vertex-normal pairs read."  << endl;

    // sort the vertex-normal pairs for later calculations
    std::sort(mesh->vertexNormalPairs.begin(), mesh->vertexNormalPairs.end());
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

    //const Float scale = 0.001; // m to mm
    //vec *= scale;

	mesh.vertices.push_back(vertex);
}

void ObjReader::handleNormalsAndPairs(const string &line, Mesh &mesh) {

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

    Vector3 a = mesh.vertices[v0 - 1];
    Vector3 b = mesh.vertices[v1 - 1];
    Vector3 c = mesh.vertices[v2 - 1];

    Vector3 ab = (b - a).normalized();
    Vector3 ac = (c - a).normalized();

    Vector3 normal = ab.cross(ac);
    IndexType normalIdx;

    mesh.normals.push_back(normal.normalized());
    normalIdx = mesh.normals.size();
    

    // -1: indices start at 1 but for later purposes it's easier if they start at 0
    std::array<std::tuple<IndexType, IndexType>, 3> indexTups {
        std::make_tuple(v0 - 1, normalIdx - 1),
        std::make_tuple(v1 - 1, normalIdx - 1),
        std::make_tuple(v2 - 1, normalIdx - 1)
    };
    for (auto& tup : indexTups) {
        if (std::find(mesh.vertexNormalPairs.begin(), mesh.vertexNormalPairs.end(), tup)
                == mesh.vertexNormalPairs.end()) {
            mesh.vertexNormalPairs.push_back(tup);
        }
    }

 	// calculate the longest edge in the input mesh
	Float edge0 = (a - b).norm();
	Float edge1 = (a - c).norm();
	Float edge2 = (b - c).norm();

	Float longestEdge = edge0;
    if (edge1 > edge0) longestEdge = edge1;
    if (edge2 > edge1) longestEdge = edge2;

	if (longestEdge > mesh.longestEdge) {
		mesh.longestEdge = longestEdge;
	}
}
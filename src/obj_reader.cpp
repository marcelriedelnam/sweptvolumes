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

    while (std::getline(file, input_line)) {
        if (input_line[0] == 'v' && input_line[1] == ' ') {
            insertVertex(input_line, *mesh);
        } else if (input_line[0] == 'v' && input_line[1] == 'n') {
            insertNormal(input_line, *mesh);
        } else if (input_line[0] == 'f' && input_line[1] == ' ') {
            insertPairs(input_line, *mesh);
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

    //const Float scale = 0.001; // m to mm
    //vec *= scale;

	mesh.vertices.push_back(vec);
}

void ObjReader::insertPairs(const string &line, Mesh &mesh) { 
    
    bool vertexIndex = true;
    int normalIndex = 0;
	std::string temp;
	for (int i = 2; i < line.length(); ++i) {
		switch(line[i]) {
			case '/':
				vertexIndex = false;
                normalIndex++;
                if (normalIndex == 2) {
                    temp.append(1, ' ');
                }
				break;
			case ' ':
				vertexIndex = true;
                normalIndex = 0;
				temp.append(1, ' ');
				break;
			default:
				if (!vertexIndex && normalIndex != 2) {
					break;
				}
				temp.push_back(line[i]);
        }
	}
	
	std::stringstream ss(temp);

    IndexType v0, vn0, v1, vn1, v2, vn2;
    ss >> v0 >> vn0 >> v1 >> vn1 >> v2 >> vn2; 

    if(vn0 == 0 || vn1 == 0 || vn2 == 0) {
        throw std::runtime_error("normal missing in face definition (ObjReader)");
    }
    // -1: indices start at 1 but for later purposes it's easier if they start at 0
    std::array<std::tuple<IndexType, IndexType>, 3> indexTups {
        std::make_tuple(v0 - 1, vn0 - 1),
        std::make_tuple(v1 - 1, vn1 - 1),
        std::make_tuple(v2 - 1, vn2 - 1)
    };
    for(auto& tup : indexTups) {
        if(std::find(mesh.vertexNormalPairs.begin(), mesh.vertexNormalPairs.end(), tup)
                == mesh.vertexNormalPairs.end()) {
            mesh.vertexNormalPairs.push_back(tup);
        }
    }
}

void ObjReader::insertNormal(const string &line, Mesh &mesh) {
    
    string temp;
    for (int i = 2; i < line.length(); ++i) {
        temp.push_back(line[i]);
    }

    std::stringstream ss(temp);

    Float x, y, z;
    ss >> x >> y >> z;

    Vector3 normal(x, y, z);
	mesh.normals.push_back(normal.normalized());
}
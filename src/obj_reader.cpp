#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <memory>

#include "obj_reader.h"


using std::string;
using std::vector;
using std::cout;
using std::endl;


void ObjReader::readVerticesFromFile() {
    std::ifstream file(path);

    if (!file.is_open()) {
        cout << "Did not find file " << path << endl;
        throw std::runtime_error("Did not find file " + path);
    }

    IndexType vertexCount = 0;
    string input_line;
    while (std::getline(file, input_line)) {
        if (input_line[0] == 'v' && input_line[1] == ' ') {
            vertexCount++;
        } else if (input_line[0] == 'f' && input_line[1] == ' ') {
            break;
        }
    }

    file.clear();
    file.seekg(0, std::ios::beg);

    while (std::getline(file, input_line)) {
        if (input_line[0] == 'v' && input_line[1] == ' ') {
            insertVertex(input_line);
        } else if (input_line[0] == 'f' && input_line[1] == ' ')  {
            break;
        }
    }

    file.close();
    cout << vertexCount << " vertices read." << endl;
}

void ObjReader::insertVertex(const string &line) {
    
    string temp;
    for (int i = 2; i < line.length(); ++i) {
        temp.push_back(line[i]);
    }

    std::stringstream ss(temp);

    Float x, y, z;
    ss >> x >> y >> z;

	mesh->vertices.push_back(Vector3(x, y, z));
}

void ObjReader::readFacesFromFile() {
    std::ifstream file(path);

    if(!file.is_open()) {
        cout << "Did not find file " << path << endl;
        throw std::runtime_error("Did not find file " + path);
    }

    IndexType faceCount = 0;
    string input_line;
    while (std::getline(file, input_line)) {
        if (input_line[0] == 'f' && input_line[1] == ' ') {
            faceCount++;
        } 
    }

    file.clear();
    file.seekg(0, std::ios::beg);

    while (std::getline(file, input_line)) {
        if (input_line[0] == 'f' && input_line[1] == ' ') {
            insertFace(input_line);
        }
    }

    file.close();
    cout << faceCount << " faces read." << endl;
}
/**
 * For now the program only reads the indices of the vertices of the face, but not the texture coordinates and normals
 */
void ObjReader::insertFace(const string &line) { 
    
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

    IndexType x, y, z;
    ss >> x >> y >> z; 
	mesh->faces.push_back(std::make_tuple(x, y, z));
}
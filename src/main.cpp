#include "obj_reader.h"
#include "csv_reader.h"
#include "obj_writer.h"
#include "transform.h"

#include <iostream>


int main(int argc, char **argv) {

	// read the *.obj file from the first argument
	// unique_ptr<Mesh> mesh = ObjReader::read(argv[1]);
	ObjReader objreader(argv[1]); 

	// read the *.csv file from the second argument
	CSVReader csvreader(argv[2]); 

	std::unique_ptr<Mesh> inputMesh = std::move(objreader.mesh);
	std::unique_ptr<std::vector<Matrix4, allocM>> transformationMatrices = std::move(csvreader.matrices);

	// transform the vertices
	Transform transform(*inputMesh, *transformationMatrices);
	
	std::unique_ptr<Mesh> outputMesh = std::move(transform.transformedMesh);
	// write a *.obj file at the given destination
	//WriteFile writefile("../output/output.obj", *outputMesh);
	// writes back the input file
	ObjWriter objwriter("../output/output.obj", *inputMesh);
}

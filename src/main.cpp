#include "obj_reader.h"
#include "csv_reader.h"
#include "obj_writer.h"
#include "transform.h"

#include <iostream>


int main(int argc, char **argv) {

	// read the *.obj file from the first argument
	ObjReader objreader(argv[1]); 

	// read the *.csv file from the second argument
	CSVReader csvreader(argv[2]); 

	std::unique_ptr<Mesh> inputMesh = std::move(objreader.mesh);
	// transform the vertices
	Transform transform(inputMesh->vertices, inputMesh->faces, csvreader.getMatrices());
	
	std::unique_ptr<Mesh> outputMesh = std::move(transform.transformedMesh);
	// write a *.obj file at the given destination
	//WriteFile writefile("../output/output.obj", outputMesh->vertices, outputMesh->faces);
	// writes back the input file
	WriteFile writefile("../output/output.obj", inputMesh->vertices, inputMesh->faces);
}

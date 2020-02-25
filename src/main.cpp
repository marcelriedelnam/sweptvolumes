#include "obj_reader.h"
#include "csv_reader.h"
#include "obj_writer.h"
#include "transform.h"

#include <iostream>
#include <exception>


int main(int argc, char **argv) {

	std::unique_ptr<Mesh> inputMesh;

	// read the *.obj file from the first argument
	try {
		inputMesh = ObjReader::read(argv[1]);
	}
	catch(std::exception& e) {
		std::cerr << "File " << argv[1] << " malformed: " << e.what() << std::endl;
		return 1;
	}
	
	// read the *.csv file from the second argument
	std::unique_ptr<std::vector<Matrix4, allocM>> transformedMatrices;
	transformedMatrices = CSVReader::read(argv[2]); 

	// transform the vertices
	std::unique_ptr<Mesh> outputMesh;
	outputMesh = Transform::transform(*inputMesh, *transformedMatrices);

	// write a *.obj file at the given destination
	ObjWriter objWriter("../output/output.obj", *outputMesh);
}

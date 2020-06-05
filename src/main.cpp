#include "obj_reader.h"
#include "csv_reader.h"
#include "obj_writer.h"
#include "transform.h"
#include "simplify.h"
#include "process_mesh.h"

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

	// pre-process mesh before sweeping
	std::unique_ptr<Mesh> processedMesh;
	processedMesh = ProcessMesh::process(*inputMesh);

	// transform the vertices
	std::unique_ptr<Mesh> transformedMesh;
	transformedMesh = Transform::transform(*processedMesh, *transformedMatrices);

	// remove inner vertices and make point cloud uniform
	std::unique_ptr<Mesh> outputMesh;
	outputMesh = Simplify::simplify(*transformedMesh);

	// write a *.obj file at the given destination
	ObjWriter objWriter("../output/output.obj", *outputMesh);
	//ObjWriter objWriter("../output/output.obj", *transformedMesh);
}

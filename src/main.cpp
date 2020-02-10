#include "obj_reader.h"
#include "csv_reader.h"
#include "write_file.h"
#include "tranform.h"

#include <iostream>


int main(int argc, char **argv) {

	// read the *.obj file from the first argument
	ObjReader objreader(argv[1]); 
	// read the *.csv file from the second argument
	CSVReader csvreader(argv[2]); 
	// transform the vertices
	Transform transform(objreader.getVertices(), objreader.getFaces(), csvreader.getMatrices());
	// write a *.obj file at the given destination
	//WriteFile writefile("../output/output.obj", transform.getTransformedVertices(), 
	//transform.getTransformedFaces());
	WriteFile writefile("../output/output.obj", objreader.getVertices(), objreader.getFaces());
}

#include "obj_reader.h"
#include "csv_reader.h"
#include "obj_writer.h"
#include "transform.h"
#include "simplify.h"
#include "process_mesh.h"
#include "config.h"

#include <iostream>
#include <exception>
#include <iomanip>
#include <string>

using std::cout;
using std::endl;
using std::string;


void callHelpMessage(char *exec_name) {
	cout << exec_name << " <object> <motion> {OPTIONS}" << endl << endl;
	cout << std::left << std::setw(3) << "   " << std::setw(40) << "object" 
	     << "An OBJ file that contains the object that is to be swept" << endl;
	cout << std::left << std::setw(3) << "   " << std::setw(40) << "motion" 
	     << "A CSV file that contains 4x4 matrices that describe discrete time steps of the motion" << endl << endl;
	cout << "OPTIONS:" << endl << endl;
	cout << std::left << std::setw(3) << "   " << std::setw(40) << "-h, --help" 
	     << "Display this help menu" << endl;
	cout << std::left << std::setw(3) << "   " << std::setw(40) << "-e, --epsilon" 
	     << "Sets the fault tolerance a vertex normal may have when compared to the velocity vector" << endl;
	cout << std::left << std::setw(3) << "   " << std::setw(40) << "-m, --max-edge-length" 
	     << "Sets the maximum length two neighboring vertices may have" << endl;
	cout << std::left << std::setw(3) << "   " << std::setw(40) << "-g, --grid-cell-length" 
	     << "Sets the grid cell size that is used to downsample the point cloud" << endl;
	cout << std::left << std::setw(3) << "   " << std::setw(40) << "-s, --samples" 
	     << "Sets how many interpolated matrices between two time steps should be calculated" << endl;
	cout << std::left << std::setw(3) << "   " << std::setw(40) << "-r, --radius"
	     << "Sets the radius used for the Poisson-disk sampling" << endl;
}

int handleArguments(int argc, char **argv) {

	if (argc < 2 || static_cast<string>(argv[1]) == "-h" || static_cast<string>(argv[1]) == "--help") {
		callHelpMessage(argv[0]);
		return -1;
	}

	int argCount = 0;
	for (int i = 1; i < argc; ++i) {
		string arg = static_cast<string>(argv[i]);
		if (arg == "-e" || arg == "--epsilon") {
			ParameterConfig::epsilon = std::stof(argv[++i]);
		}
		else if (arg == "-m" || arg == "--max-edge-length") {
			ParameterConfig::maxEdgelength = std::stof(argv[++i]);
		}
		else if (arg == "-g" || arg == "--grid-cell-length") {
			ParameterConfig::gridCellLength = std::stof(argv[++i]);
		}
		else if (arg == "-s" || arg == "--samples") {
			ParameterConfig::samples = std::stoi(argv[++i]);
		}
		else if (arg == "-r" || arg == "--radius") {
			ParameterConfig::simplifyRadius = std::stof(argv[++i]);
		}
	}
	return 1;
}

int main(int argc, char **argv) {

	int i = handleArguments(argc, argv);
	if (i == -1) return 0;

	std::unique_ptr<Mesh> inputMesh;

	// read the *.obj file from the first argument
	try {
		inputMesh = ObjReader::read(argv[1]);
	}
	catch(std::exception& e) {
		std::cerr << "Problem with file " << "\"" << argv[1] << "\": " << e.what() << std::endl;
		cout << endl;
		callHelpMessage(argv[0]);
		return 1;
	}
	
	// read the *.csv file from the second argument
	std::unique_ptr<std::vector<Matrix4, allocM>> transformedMatrices;
	try {
		transformedMatrices = CSVReader::read(argv[2]); 
	}
	catch(std::exception& e) {
		std::cerr << "File " << "\"" << argv[2] << "\"" << " not specified: " << e.what() << std::endl;
		cout << endl;
		callHelpMessage(argv[0]);
		return 1;
	}
	

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
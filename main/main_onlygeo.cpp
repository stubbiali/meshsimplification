/*!	\file	main_onlygeo.cpp
	\brief	Executable running the iterative mesh simplification process 
			on a mesh with associated data. The edge cost function does not take
			into account statistical considerations. 
			Settings are given by command line. */
	
#include <chrono>
	
#include "simplification.hpp"

int main(int argc, char * argv[])
{
	using namespace geometry;
	using namespace std::chrono;
	
	//
	// Read from command line
	//

	// If no arguments provided or -h flag: print help
	if ((argc == 1) || (argc == 2 && !strcmp(argv[1],"-h")))
	{
		cout << "Driver for the iterative, edge collapse-based simplification" << endl
		<< "process applied to a mesh with distributed data." << endl
		<< "The edge cost function does not take statistical considerations into account." << endl
		<< "To run it, from the current directory type: " << endl
		<< "    " << argv[0] << " [options] [arguments]" << endl
		<< "List of available options:" << endl
		<< "-h, --help                 " << "print help" << endl
		<< "-i, --input [file]         " << "specify path to input file (mandatory)" << endl
		<< "-n, --nodes [n]            " << "set target number of nodes (mandatory)" << endl
		<< "-o, --output [file]        " << "specify path to output file (default: none)" << endl
		<< "--disable-fixed-element    " << "disable fixed element" << endl;
		return 0;
	}
	
	// Initialize settings
	string iFile, oFile;
	UInt n(0);
	bool fixedElem(true);
	
	// Read arguments from command line
	for (UInt i = 1; i < argc; i+=2)
	{
		if (!strcmp(argv[i],"-i") || !strcmp(argv[i],"--input"))
			iFile = argv[i+1];
		else if (!strcmp(argv[i],"-n") || !strcmp(argv[i],"--nodes"))
			n = atoi(argv[i+1]);
		else if (!strcmp(argv[i],"-o") || !strcmp(argv[i],"--output"))
			oFile = argv[i+1];
		else if (!strcmp(argv[i],"--disable-fixed-element"))
			fixedElem = false;
	}
	
	// Check on input file
	if (!iFile.compare(""))
	{
		cout << "Input file not provided. Aborted." << endl;
		return 0;
	}
	
	// Check on final number of nodes
	if (n == 0)
	{
		cout << "Target number of nodes not provided or zero. Aborted." << endl;
		return 0;
	}	
	
	//
	// Run
	//
	
	#ifdef NDEBUG
	high_resolution_clock::time_point start = high_resolution_clock::now();
	#endif
	
	simplification<Triangle, MeshType::DATA, OnlyGeo<MeshType::DATA>> simplifier(iFile);
	simplifier.simplify(n, fixedElem, oFile);
		
	#ifdef NDEBUG
	high_resolution_clock::time_point stop = high_resolution_clock::now();
	auto dif = duration_cast<milliseconds>(stop-start).count();
	cout << "Total elapsed time: " << dif << " ms" << endl;
	#endif
}

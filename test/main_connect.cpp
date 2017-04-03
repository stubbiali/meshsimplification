/*!	\file	main_connect.cpp
	\brief	A small executable to test class connect. */
	
#include <iostream>
#include <chrono>

#include "mesh.hpp"
#include "connect.hpp"

int main()
{
	using namespace geometry;
	using namespace std::chrono;
	
	high_resolution_clock::time_point start = high_resolution_clock::now();
	
	// Import mesh
	string inputfile("mesh/bunny.inp");
	
	// Create connections
	connect<Triangle, MeshType::GEO> conn(inputfile);
	
	// Print mesh to file
	string outputfile("mesh/out_bunny.inp");
	conn.getPointerToMesh()->print(outputfile);
	
	// Elapsed time
	high_resolution_clock::time_point end = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(end-start).count();
    cout << "Elapsed time: " << duration << " ms" << endl;
}

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
	string inputfile("mesh/brain.inp");
	
	// Create connections
	connect<Triangle, MeshType::DATA> conn(inputfile);
	
	// Print mesh to file
	//string outputfile("mesh/out_bunny.inp");
	//conn.getPointerToMesh()->print(outputfile);
	
	// Elapsed time
	high_resolution_clock::time_point end = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(end-start).count();
    cout << "Elapsed time: " << duration << " ms" << endl;
        
    // Test constructor from Eigen arrays
    /*
	MatrixXd nds(2,3);
	nds(0,0) = 0;
	nds(0,1) = 1;
	nds(0,2) = 2;
	nds(1,0) = 4.5;
	nds(1,1) = 2.1;
	nds(1,2) = -10;
	
	MatrixXi els(1,3);
	els(0,0) = 1;
	els(0,1) = 0;
	els(0,2) = 2;
	
	VectorXd val(3);
	val(0) = -500.3;
	val(1) = 234;
	val(2) = 1.5463;
	
	MatrixXd loc(3,3);
	loc(0,0) = 0.5;
	loc(0,1) = 1.5;
	loc(0,2) = 4.5;
	
	connect<Triangle, MeshType::DATA> conn(nds, els, loc, val);
	*/
}

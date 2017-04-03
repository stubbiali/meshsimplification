/*!	\file	main_bconnect.cpp
	\brief	A small executable to test class bconnect. */
	
#include <iostream>

#include "mesh.hpp"
#include "bconnect.hpp"

int main()
{
	using namespace geometry;
				
	// Import mesh
	string inputfile("mesh/bunny.inp");
		
	// Create connections
	bconnect<Triangle, MeshType::GEO> bc(inputfile);
	
	// Print mesh to file
	string outputfile("mesh/out_bunny.inp");
	bc.getMesh().print(outputfile);
}

/*! \file	main_mesh.cpp
	\brief	A small executable to test class mesh. */
	
#include <iostream>

#include "mesh.hpp"

using namespace geometry;

int main()
{	
	// Read from file
	string inputfile("mesh/bunny.inp");
	mesh<Triangle, MeshType::DATA> m(inputfile);
	cout << "Number of nodes       : " << m.getNumNodes() << endl
		 << "Number of elements    : " << m.getNumElems() << endl
		 << "Number of data points : " << m.getNumData() << endl;
		 
	cout << m.getNode(45) << endl;
	cout << m.getData(45) << endl;
		 
	// Print to file
	string outputfile("mesh/out_bunny.inp");
	m.print(outputfile);
}

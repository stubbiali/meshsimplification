/*! \file	main_bmesh.cpp
	\brief	A small executable to test class bmesh. */
	
#include <iostream>

#include "bmesh.hpp"

using namespace geometry;

int main()
{	
	// Read from file
	string inputfile("mesh/brain.inp");
	bmesh<Triangle> bm(inputfile);
	cout << "Number of nodes    : " << bm.getNumNodes() << endl
		 << "Number of elements : " << bm.getNumElems() << endl;
		 
	cout << bm.getNode(45) << endl;
	cout << bm.getElem(0) << endl;
	cout << bm.getElem(1) << endl;
	cout << bm.getElem(81919) << endl;
		 
	// Print to file
	string outputfile("mesh/brain.inp");
	bm.print(outputfile);
}

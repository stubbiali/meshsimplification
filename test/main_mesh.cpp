/*! \file	main_mesh.cpp
	\brief	A small executable to test class mesh. */
	
#include <iostream>

#include "mesh.hpp"

using namespace geometry;

int main()
{	
	// Read from file
	string inputfile("mesh/pawn.inp");
	mesh<Triangle, MeshType::DATA> m(inputfile);
	cout << "Number of nodes       : " << m.getNumNodes() << endl
		 << "Number of elements    : " << m.getNumElems() << endl
		 << "Number of data points : " << m.getNumData() << endl;
		 
	cout << m.getNode(45) << endl;
	cout << m.getData(45) << endl;
		 
	// Print to file
	//string outputfile("mesh/out_bunny.inp");
	//m.print(outputfile);
	
	/*
	// Test constructor from Eigen arrays
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
	loc(0,1) = 0.5;
	loc(0,2) = 1.5;
	loc(0,3) = 4.5;
	
	mesh<Triangle, MeshType::DATA> m(nds, els, loc, val);
	cout << m.getNumData() << endl;
	cout << m.getNode(1) << endl;
	*/
}

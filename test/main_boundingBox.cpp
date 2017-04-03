/*!	\file	main_boundingBox
	\brief	A small executable testing the class boundingBox. */

#include <chrono>

#include "boundingBox.hpp"
#include "meshInfo.hpp"

int main()
{
	using namespace geometry;
	using namespace std::chrono;
	
	//
	// Import mesh and set static attributes
	//
		
	#ifdef NDEBUG
	high_resolution_clock::time_point start, stop; 
	start = high_resolution_clock::now();
	#endif
	
	string inputfile("mesh/bunny.inp");
	bbox3d::setup(bmeshInfo<Triangle, MeshType::DATA>(inputfile));
	
	#ifdef NDEBUG
	stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop-start).count();
	cout << "Elapsed time: " << duration << " ms" << endl << endl;
	#endif
	
	#ifndef NDEBUG
	cout << "North-East: " << bbox3d::getGlobalNE() << endl
		<< "South-West: " << bbox3d::getGlobalSW() << endl
		<< "dx = " << bbox3d::getCellSize(0) 
		<< ", dy = " << bbox3d::getCellSize(1) 
		<< ", dz = " << bbox3d::getCellSize(2) << endl
		<< "Nx = " << bbox3d::getNumCells(0) 
		<< ", Ny = " << bbox3d::getNumCells(1) 
		<< ", Nz = " << bbox3d::getNumCells(2) << endl << endl;
	#endif
	
	//
	// Test constructor with arbitrary number of points
	//
	
	{
		point3d A(0,0,0);
		point3d B(1,2,3);
		point3d C(-1,6,6);
		point3d D(4,6,0);
		
		bbox3d bb(A,B,C,D);
		cout << bb << endl;
	}
}

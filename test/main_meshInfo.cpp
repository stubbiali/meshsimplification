/*!	\file	main_meshInfo.hpp
	\brief	A small executable testing some functionality of class meshInfo. */
	
#include <iostream>
#include <chrono>

#include "meshInfo.hpp"

int main()
{
	using namespace geometry;
	using namespace std::chrono;
	
	high_resolution_clock::time_point start, stop; 
	
	start = high_resolution_clock::now();
	
	// Import mesh
	string inputfile("mesh/bunny.inp");
	
	// Build connections
	meshInfo<Triangle, MeshType::DATA> news(inputfile);
		
	stop = high_resolution_clock::now();
	auto duration_read = duration_cast<milliseconds>(stop-start).count();
	
	//
	// Edge (6721,16057)
	//
	// Elements connected to 6721: 886,888,1119,8034,8194,8356
	// Elements connected to 16057: 1552,1833,1983,5775,8034,8194
	
	//
	// Test results
	//
	
	#ifndef NDEBUG
	{
		UInt id1(6721), id2(16057);
		
		cout << "Boundary flag for " << id1 << ": " <<
			news.getPointerToMesh()->getNode(id1).getBoundary() << endl;
		cout << "Boundary flag for " << id2 << ": " <<
			news.getPointerToMesh()->getNode(id2).getBoundary() << endl;
			
		auto invElems = news.getElemsInvolvedInEdgeCollapsing(id1,id2);
		auto toRemove = news.getElemsOnEdge(id1,id2);
		auto toKeep = news.getElemsModifiedInEdgeCollapsing(id1,id2);
		auto invData = news.getDataInvolvedInEdgeCollapsing(id1,id2);
		auto toMove = news.getDataModifiedInEdgeCollapsing(id1,id2);
		
		auto node2elem1 = news.getPointerToConnectivity()->getNode2Elem(id1).getConnected();
		cout << "node2elem for " << id1 << " = { ";
		for (auto el : node2elem1)
			cout << el << " ";
		cout << "}" << endl;
		
		auto node2elem2 = news.getPointerToConnectivity()->getNode2Elem(id2).getConnected();
		cout << "node2elem for " << id2 << " = { ";
		for (auto el : node2elem2)
			cout << el << " ";
		cout << "}" << endl;
		
		cout << "invElems = { ";
		for (auto el : invElems)
			cout << el << " ";
		cout << "}" << endl;
	
		cout << "toRemove = { ";
		for (auto el : toRemove)
			cout << el << " ";
		cout << "}" << endl;
		
		for (auto id : toRemove)
		{
			auto elem = news.getPointerToMesh()->getElem(id);
			cout << "Elem " << id << ":" << endl;
			cout << news.getPointerToMesh()->getNode(elem[0]) << endl;
			cout << news.getPointerToMesh()->getNode(elem[1]) << endl;
			cout << news.getPointerToMesh()->getNode(elem[2]) << endl;
		}
	
		cout << "toKeep   = { ";
		for (auto el : toKeep)
			cout << el << " ";
		cout << "}" << endl;
		
		for (auto id : toKeep)
		{
			auto elem = news.getPointerToMesh()->getElem(id);
			cout << "Elem " << id << ":" << endl;
			cout << news.getPointerToMesh()->getNode(elem[0]) << endl;
			cout << news.getPointerToMesh()->getNode(elem[1]) << endl;
			cout << news.getPointerToMesh()->getNode(elem[2]) << endl;
		}
	
		cout << "invData  = { ";
		for (auto el : invData)
			cout << el << " ";
		cout << "}" << endl;
	
		cout << "toMove   = { ";
		for (auto el : toMove)
			cout << el << " ";
		cout << "}" << endl << endl;
	}
	#endif
	
	//
	// Test performance
	//
	
	#ifdef NDEBUG
	UInt times(2e5);
	start = high_resolution_clock::now();
	for (UInt i = 0; i < times; i++)
	{
		UInt id1(6721), id2(16057);

		auto invElems = news.getElemsInvolvedInEdgeCollapsing(id1,id2);
		auto toRemove = news.getElemsOnEdge(id1,id2);
		auto toKeep = news.getElemsModifiedInEdgeCollapsing(id1,id2);		
		auto toMove = news.getDataModifiedInEdgeCollapsing(invElems);
	}
	stop = high_resolution_clock::now();
	auto duration_news = duration_cast<milliseconds>(stop-start).count();
	
	cout << "Time for reading the mesh: " << duration_read << " ms" << endl
		 << "Time for extracting info : " << duration_news << " ms" << endl << endl;
	#endif
		 
	//
	// Test getQuantityOfInformation
	//
	
	#ifndef NDEBUG
	UInt id = 20000;
	auto qoi = news.getQuantityOfInformation(id);
	cout << "Quantity of information associated with triangle " << id << ": " << qoi << endl;
	#endif
}



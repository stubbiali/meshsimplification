/*!	\file	main_collapse.cpp
	\brief	A small executable testing the collapse procedure.
	
	For a temporary collapse, the procedure for collapsing the
	edge (id1,id2) in a point which is assigned the Id id1
	comprises the following steps:
	<ol>
	<li>	make id2 inactive in the list of nodes
	<li>	modify element-node, node-node, node-element connections
	<li>	get potentially valid collapsing points just looking
			at the boundary flags of the edge end-points
	<li>	for each potentially valid point:
			<ol>
			<li>	substitute its coordinates and boundary flag to id1
			<li>	project data points and modify data-element
					and element-data connections
			<li>	check for triangle invertions
			<li>	check for empty triangles (i.e. triangles not associated
					with any data point)
			<li>	check for mesh self-intersections
			<li>	if the edge has passed the previous three tests,
					compute the cost and check if it smaller than
					the best so far
			<li>	restore data-element and element-data connections
			<\ol>	
	<li>	come back to the status prior the collapse
	<\ol>
	Note that the element-element connections are never used,
	then they can be neglected for the sake of performance. */
	
#include <iostream>
#include <limits>
#include <chrono>
	
#include "connect.hpp"
#include "meshInfo.hpp"
#include "projection.hpp"
#include "OnlyGeo.hpp"
#include "DataGeo.hpp"
#include "structuredData.hpp"
#include "intersection.hpp"

int main()
{
	using namespace geometry;
	#ifdef NDEBUG
	using namespace std::chrono;
	high_resolution_clock::time_point start, stop;
	#endif
	
	// File to mesh
	string inputfile("mesh/brain.inp");
	
	//
	// Test connections restoration
	//
	// For bunny.inp: edge (6721,16057)
	// Nodes connected to 6721: 2161 6392 15741 15890 16052 16057
	// Nodes connected to 16057: 2161 6721 14414 14443 16052 16058
	// Elements connected to 6721: 886 888 1119 8034 8194 8356
	// Elements connected to 16057: 1552 1833 1983 5775 8034 8194
	//
	// For left_hemisphere.vtk: edge (38521,39551)
	
	{
		//
		// Initialization
		//
		
		#ifdef NDEBUG
		start = high_resolution_clock::now();
		#endif
		
		projection<Triangle> prj(inputfile);
		auto conn = prj.getPointerToConnectivity();
		//OnlyGeo<MeshType::DATA> costFunction(&prj);
		DataGeo costFunction(&prj, 1./3., 1./3., 1./3.);
		structuredData<Triangle> sdata(prj);
		intersection<Triangle> intrs(prj.getPointerToMesh());
				
		#ifdef NDEBUG
		stop = high_resolution_clock::now();
		auto init_duration = duration_cast<milliseconds>(stop-start).count();
		cout << "Elapsed time: " << init_duration << " ms" << endl;
		#endif
		
		#ifndef NDEBUG
		auto nc = bbox3d::getNumCells();
		cout << "Number of cells: " << nc[0] << " " << nc[1] << " " << nc[2] << endl;
		#endif
				
		// Edge end-points
		UInt id1(6721), id2(16057);
		//UInt id1(38521), id2(39551);
		
		// Test points
		UInt id3(888), id4(8034), id5(1983), id6(2161);
		//UInt id3(38496), id4(39531), id5(40499);
		
		#ifndef NDEBUG
		// Extract node-node and node-element connections
		// for test points before the collapse
		{
			auto id3_node2node = conn->getNode2Node(id3).getConnected();
			auto id4_node2node = conn->getNode2Node(id4).getConnected();
			auto id5_node2node = conn->getNode2Node(id5).getConnected();
			auto id3_node2elem = conn->getNode2Elem(id3).getConnected();
			auto id4_node2elem = conn->getNode2Elem(id4).getConnected();
			auto id5_node2elem = conn->getNode2Elem(id5).getConnected();
		
			cout << "id3_node2node: ";
			for (auto v : id3_node2node)
				cout << v << " ";
			cout << endl;
		
			cout << "id4_node2node: ";
			for (auto v : id4_node2node)
				cout << v << " ";
			cout << endl;
			
			cout << "id5_node2node: ";
			for (auto v : id5_node2node)
				cout << v << " ";
			cout << endl;
			
			cout << "id3_node2elem: ";
			for (auto v : id3_node2elem)
				cout << v << " ";
			cout << endl;
			
			cout << "id4_node2elem: ";
			for (auto v : id4_node2elem)
				cout << v << " ";
			cout << endl;
			
			cout << "id5_node2elem: ";
			for (auto v : id5_node2elem)
				cout << v << " ";
			cout << endl;
		}
		#endif
		
		#ifdef NDEBUG
		high_resolution_clock::time_point start, stop; 
		start = high_resolution_clock::now();
		for (UInt t = 0; t < 1e4; ++t)
		{
		#endif
		
		//
		// Extract elements and data points involved in the collapse
		//
		
		auto invElems = prj.getElemsInvolvedInEdgeCollapsing(id1,id2);
		auto toRemove = prj.getElemsOnEdge(id1,id2);
		auto toKeep = prj.getElemsModifiedInEdgeCollapsing(id1,id2);
		auto toMove = prj.getDataModifiedInEdgeCollapsing(invElems);
		
		// Get normals to the elements involved in the collapse
		vector<point3d> oldNormals;
		oldNormals.reserve(toKeep.size());
		for (auto elem : toKeep)
			oldNormals.emplace_back(prj.getNormal(elem));
		
		#ifndef NDEBUG
		{
			auto id6_data2elem = conn->getData2Elem(id6).getConnected();
			cout << "id6_data2elem: ";
			for (auto v : id6_data2elem)
				cout << v << " ";
			cout << endl;
			
			auto el1_elem2data = conn->getElem2Data(toKeep[2]).getConnected();
			cout << "el1_elem2data: ";
			for (auto v : el1_elem2data)
				cout << v << " ";
			cout << endl;
		}
		#endif
					
		//
		// Update connections
		//
		
		// Store old id1
		auto P(prj.getPointerToMesh()->getNode(id1));
		
		// Update node-node, node-element and element-node connections
		auto oldConnections = conn->applyEdgeCollapse(id2, id1, toRemove, toKeep);
		
		vector<vector<UInt>> patches;
		patches.reserve(toKeep.size());
		for (auto elem : toKeep)
			patches.emplace_back(prj.getTriPatch(elem));
				
		//
		// Get potentially valid points
		//
		
		auto pointsList = costFunction.getPointsList(id1, id2);
		#ifndef NDEBUG
		cout << endl << "Points list:" << endl;
		for (auto v : pointsList)
			cout << v << endl;
		#endif
		
		//
		// Get the cheapest edge
		//
				
		// Auxiliary variables
		bool valid;
		vector<UInt>::const_iterator it1;
		vector<point3d>::const_iterator oldNormal; 
		Real opt_cost(numeric_limits<Real>::max());
		UInt opt_cPoint;
		
		for (UInt i = 0; i < pointsList.size(); ++i)
		{
			//
			// Set collapsing point
			//
		
			// Change coordinates and boundary flag of id1
			prj.getPointerToMesh()->setNode(id1, pointsList[i]);
			
			// Update structured data
			sdata.update(toKeep); 
			
			// Project data points and update data-element and element-data connections
			auto oldData = prj.project(toMove, toKeep);
			conn->eraseElemInData2Elem(toRemove);
			
			#ifndef NDEBUG
			if (i == 0)
			{
				auto id6_data2elem = conn->getData2Elem(id6).getConnected();
				cout << "id6_data2elem: ";
				for (auto v : id6_data2elem)
					cout << v << " ";
				cout << endl;
			}
			#endif
			
			//
			// Check collapse validity
			//
			
			valid = true;
			
			for (it1 = toKeep.cbegin(), oldNormal = oldNormals.cbegin(); 
				it1 != toKeep.cend() && oldNormal != oldNormals.cend() && valid; 
				++it1, ++oldNormal)
			{
				// No triangle inversions
				valid = valid && ((*oldNormal) * prj.getNormal(*it1) > TOLL);
				
				// No empty triangles
				valid = valid && !(prj.isEmpty(*it1));
				
				// No mesh self-intersections
				for (auto elem : patches[it1-toKeep.cbegin()])
					prj.getPointerToMesh()->setElemInactive(elem);
					
				auto elems = sdata.getNeighbouringElements(*it1);
				
				#ifndef NDEBUG
				if (i == 0)
					cout << "Number of intersection tests: " << elems.size() << endl; 
				#endif
				
				for (auto it2 = elems.cbegin(); it2 != elems.cend() && valid; ++it2)
				{
					valid = valid && !(intrs.intersect(*it1, *it2));
					#ifndef NDEBUG
					if (!(valid))
					{
						cout << "Intersecting triangles: " << *it1 << " and " << *it2 << endl;
						
						auto it1_conn = prj.getPointerToMesh()->getElem(*it1).getVertices();
						cout << "Vertices of " << *it1 << ":" << endl;
						cout << prj.getPointerToMesh()->getNode(it1_conn[0]) << endl;
						cout << prj.getPointerToMesh()->getNode(it1_conn[1]) << endl;
						cout << prj.getPointerToMesh()->getNode(it1_conn[2]) << endl;
						
						auto it2_conn = prj.getPointerToMesh()->getElem(*it2).getVertices();
						cout << "Vertices of " << *it2 << ":" << endl;
						cout << prj.getPointerToMesh()->getNode(it2_conn[0]) << endl;
						cout << prj.getPointerToMesh()->getNode(it2_conn[1]) << endl;
						cout << prj.getPointerToMesh()->getNode(it2_conn[2]) << endl;
					}
					#endif
				}
				
				for (auto elem : patches[it1-toKeep.cbegin()])
					prj.getPointerToMesh()->setElemActive(elem);
			}
					
			//
			// Get cost associated with edge collapse
			//
			
			if (valid)
			{
				//auto cost = costFunction.getCost(id1, id2, pointsList[i]);
				auto cost = costFunction.getCost(id1, id2, pointsList[i], toKeep, toMove);
				
				#ifndef NDEBUG
				cout << "Edge collapse cost: " << cost << endl;
				#endif
				
				// Check if it is the smallest so far
				if (cost < opt_cost - TOLL)
				{
					opt_cost = cost;
					opt_cPoint = i;
				}
			}
			
			//
			// Restore data-element and element-data connections
			//
			
			prj.undo(toMove, oldData);
			conn->insertElemInData2Elem(toRemove);
		}
		
		//
		// Restoration
		//
		
		// Restore connections
		conn->undoEdgeCollapse(id2, id1, oldConnections.first, oldConnections.second,
			toRemove); 
			
		// Restore list of nodes
		prj.getPointerToMesh()->setNode(id1, P);
		
		// Restore structured data
		sdata.update(toKeep);
		
		#ifdef NDEBUG
		}
		stop = high_resolution_clock::now();
		auto duration = duration_cast<milliseconds>(stop-start).count();
		cout << "Elapsed time: " << duration << " ms" << endl;
		#endif
		
		#ifndef NDEBUG 
		// Extract node-node and node-element connections
		// for test points after the collapse
		{
			auto id3_node2node = conn->getNode2Node(id3).getConnected();
			auto id4_node2node = conn->getNode2Node(id4).getConnected();
			auto id5_node2node = conn->getNode2Node(id5).getConnected();
			auto id3_node2elem = conn->getNode2Elem(id3).getConnected();
			auto id4_node2elem = conn->getNode2Elem(id4).getConnected();
			auto id5_node2elem = conn->getNode2Elem(id5).getConnected();
			auto id6_data2elem = conn->getData2Elem(id6).getConnected();
			auto el1_elem2data = conn->getElem2Data(toKeep[2]).getConnected();
		
			cout << endl << "id3_node2node: ";
			for (auto v : id3_node2node)
				cout << v << " ";
			cout << endl;
		
			cout << "id4_node2node: ";
			for (auto v : id4_node2node)
				cout << v << " ";
			cout << endl;
			
			cout << "id5_node2node: ";
			for (auto v : id5_node2node)
				cout << v << " ";
			cout << endl;
			
			cout << "id3_node2elem: ";
			for (auto v : id3_node2elem)
				cout << v << " ";
			cout << endl;
			
			cout << "id4_node2elem: ";
			for (auto v : id4_node2elem)
				cout << v << " ";
			cout << endl;
			
			cout << "id5_node2elem: ";
			for (auto v : id5_node2elem)
				cout << v << " ";
			cout << endl;
			
			cout << "id6_data2elem: ";
			for (auto v : id6_data2elem)
				cout << v << " ";
			cout << endl;
			
			cout << "el1_elem2data: ";
			for (auto v : el1_elem2data)
				cout << v << " ";
			cout << endl;
		}
		#endif
	}
}



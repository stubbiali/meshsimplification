/*!	\file	imp_simplification.hpp
	\brief	Implementations of members of class simplification. */
	
#ifndef HH_IMPSIMPLIFICATION_HH
#define HH_IMPSIMPLIFICATION_HH

#include <unordered_set>
#ifdef NDEBUG
#include <chrono>
#endif

namespace geometry
{
	//
	// Constructors
	//
	
	template<MeshType MT, typename CostClass>
	simplification<Triangle, MT, CostClass>::simplification
		(const string & file) :
		gridOperation(file), costObj(&gridOperation), 
		structData(gridOperation), intrs(gridOperation.getPointerToMesh()), 
		dontTouch(true), dontTouchId(0)
	{
		initialize();
	}
	
	
	template<MeshType MT, typename CostClass>
	simplification<Triangle, MT, CostClass>::simplification
		(const string & file, const Real & wgeo, const Real & wdis, const Real & wequ) :
		gridOperation(file), costObj(&gridOperation, wgeo, wdis, wequ), 
		structData(gridOperation), intrs(gridOperation.getPointerToMesh()), 
		dontTouch(true), dontTouchId(0)
	{
		initialize();
	}
	
	
	template<MeshType MT, typename CostClass>
	simplification<Triangle, MT, CostClass>::simplification
		(const string & file, const vector<Real> & val, 
		const Real & wgeo, const Real & wdis, const Real & wequ) :
		gridOperation(file, val), costObj(&gridOperation, wgeo, wdis, wequ), 
		structData(gridOperation), intrs(gridOperation.getPointerToMesh()), 
		dontTouch(true), dontTouchId(0)
	{
		initialize();
	}
	
	
	template<MeshType MT, typename CostClass>
	simplification<Triangle, MT, CostClass>::simplification
		(const MatrixXd & nds, const MatrixXi & els) :
		gridOperation(nds, els), costObj(&gridOperation), 
		structData(gridOperation), intrs(gridOperation.getPointerToMesh()), 
		dontTouch(true), dontTouchId(0)
	{
		initialize();
	}
	
	
	template<MeshType MT, typename CostClass>
	simplification<Triangle, MT, CostClass>::simplification
		(const MatrixXd & nds, const MatrixXi & els,
		const Real & wgeo, const Real & wdis, const Real & wequ) :
		gridOperation(nds, els), costObj(&gridOperation, wgeo, wdis, wequ), 
		structData(gridOperation), intrs(gridOperation.getPointerToMesh()), 
		dontTouch(true), dontTouchId(0)
	{
		initialize();
	}
		
	
	template<MeshType MT, typename CostClass>
	simplification<Triangle, MT, CostClass>::simplification
		(const MatrixXd & nds, const MatrixXi & els, const MatrixXd & loc,
		const Real & wgeo, const Real & wdis, const Real & wequ) :
		gridOperation(nds, els, loc), costObj(&gridOperation, wgeo, wdis, wequ), 
		structData(gridOperation), intrs(gridOperation.getPointerToMesh()), 
		dontTouch(true), dontTouchId(0)
	{
		initialize();
	}
	
	
	template<MeshType MT, typename CostClass>
	simplification<Triangle, MT, CostClass>::simplification
		(const MatrixXd & nds, const MatrixXi & els, const MatrixXd & loc, const VectorXd & val,
		const Real & wgeo, const Real & wdis, const Real & wequ) :
		gridOperation(nds, els, loc, val), costObj(&gridOperation, wgeo, wdis, wequ), 
		structData(gridOperation), intrs(gridOperation.getPointerToMesh()), 
		dontTouch(true), dontTouchId(0)
	{
		initialize();
	}
		
	
	//
	// Initialization and refreshing methods
	//

	template<MeshType MT, typename CostClass>
	void simplification<Triangle, MT, CostClass>::setupCollapsingSet()
	{
		// Extract edges
		auto edges = gridOperation.getCPointerToConnectivity()->getEdges();
	
		// Loop on the edges list and for each edge take the cost
		// information and add it to the lists
		#ifdef NDEBUG
			UInt barWidth(35), numEdges(edges.size()), counter(0);
		#endif
		for (auto edge : edges) 
		{
			getCost_f(edge[0], edge[1]);
			
			#ifdef NDEBUG
				// Update progress bar
				++counter;
				Real progress(counter / (static_cast<Real>(numEdges)));
				cout << "Setup                         [";
				UInt pos(barWidth * progress);
				for (UInt i = 0; i < barWidth; ++i) 
				{
					if (i < pos) 
						cout << "=";
					else if (i == pos) 
						cout << ">";
					else 
						cout << " ";
				}
				cout << "] " << UInt(progress * 100.0) << " %\r";
				cout.flush();
			#endif
		}
		#ifdef NDEBUG
			cout << endl;
		#endif
	}
	
	
	template<MeshType MT, typename CostClass>
	void simplification<Triangle, MT, CostClass>::rebuildCollapsingSet()
	{
		// Clear the list of collapseInfo's
		costObj.clear();
		 
		// Copy current collapsingEdge's list to a temporary one
		// and clear collapsingEdge's list
		set<collapsingEdge> tmp_collapsingSet(collapsingSet.cbegin(), 
			collapsingSet.cend());
		collapsingSet.clear();
	
		// Loop on the edges list and for each edge take the cost
		// information and add it to a temporary list
		for (auto edge : tmp_collapsingSet) 
			getCost_f(edge.getId1(), edge.getId2()); 
	}
	
	
	template<MeshType MT, typename CostClass>
	void simplification<Triangle, MT, CostClass>::initialize()
	{
		#ifdef NDEBUG
			using namespace std::chrono;
			high_resolution_clock::time_point start, stop;
		#endif
		
		// Important control on coherence between the inputs:
		// error if the CostClass supports distributed data but the
		// mesh is purely geometrical
		static_assert(std::is_base_of<bcost<Triangle, MT, CostClass>, CostClass>::value,
			"CostClass must be coherent with the mesh type.");

		// Create the set of collapsingEdge's ordered by cost
		#ifndef NDEBUG
			cout << "Initialize list of edges ordered by the associated collapsing cost ... ";
			setupCollapsingSet();
			cout << "done" << endl;
		#else
			start = high_resolution_clock::now();
			setupCollapsingSet();
			stop = high_resolution_clock::now();
			auto dif_collapsingSet = duration_cast<milliseconds>(stop-start).count();
			cout << "Setup for the simplification process completed in " << dif_collapsingSet/1000 << " seconds." << endl;
		#endif

		// Define the fixed element
		findDontTouchId();
	}
	
	
	template<MeshType MT, typename CostClass>
	void simplification<Triangle, MT, CostClass>::refreshCollapsingSet
		(map<UInt,UInt> old2new)
	{
		// Copy collapsingSet into an auxiliary container
		set<collapsingEdge> collapsingSet_t(collapsingSet.cbegin(), collapsingSet.cend());
		
		// Clear collapsingSet
		collapsingSet.clear();
		
		// Re-insert all collapsingEdge's to collapsingSet, applying the old-to-new
		// map to the nodes but leaving the costs and the collapsing points unchanged
		for (auto cEdge : collapsingSet_t)
			collapsingSet.emplace(old2new[cEdge.getId1()], old2new[cEdge.getId2()],
				cEdge.getCost(), cEdge.getCollapsingPoint()); 
	}
	
	
	template<MeshType MT, typename CostClass>
	void simplification<Triangle, MT, CostClass>::refresh()
	{
		// Remove inactive nodes and elements from grid
		gridOperation.getPointerToMesh()->refresh();
		
		// Re-build connectivity
		gridOperation.getPointerToConnectivity()->refresh();
		
		// Possibly update CostClass object and re-build
		// set of collapsingEdge's
		costObj.setMeshOperation(&gridOperation);
		setupCollapsingSet();
		
		// Update structured data
		structData.setMesh(gridOperation);
		
		// Possibly update fixed element
		findDontTouchId();
	}
	
	
	//
	// Set methods
	//
	
	template<MeshType MT, typename CostClass>
	void simplification<Triangle, MT, CostClass>::setGrid(const mesh<Triangle,MT> & grid)
	{
		// Set mesh and build connections
		gridOperation.setMesh(grid);
		
		// Possibly update CostClass object and re-build
		// set of collapsingEdge's
		costObj.setMeshOperation(&gridOperation);
		setupCollapsingSet();
		
		// Update structured data
		structData.setMesh(gridOperation);
		
		// Set mesh for class intersection
		intrs.setMesh(gridOperation.getCPointerToMesh());
		
		// Possibly find fixed element
		findDontTouchId();
	}


	//
	// Compute cost and apply collapse
	//
	
	template<MeshType MT, typename CostClass>
	void simplification<Triangle, MT, CostClass>::
		getCost(const UInt & id1, const UInt & id2)
	{
		// First make sure the fixed element is not involved
		if (dontTouch)
			if ((id1 == gridOperation.getCPointerToMesh()->getElem(dontTouchId)[0]) ||
				(id1 == gridOperation.getCPointerToMesh()->getElem(dontTouchId)[1]) ||
				(id1 == gridOperation.getCPointerToMesh()->getElem(dontTouchId)[2]) ||
				(id2 == gridOperation.getCPointerToMesh()->getElem(dontTouchId)[0]) ||
				(id2 == gridOperation.getCPointerToMesh()->getElem(dontTouchId)[1]) ||
				(id2 == gridOperation.getCPointerToMesh()->getElem(dontTouchId)[2]))
				return;
				
		//
		// Get potentially valid points
		//
		
		auto pointsList = costObj.getPointsList(id1, id2);
		if (pointsList.empty())
			return;
			
		// Declare "local" multi-set of collapsingEdge
		// It stores the collapse information for each 
		// potentially valid collapsing points
		multiset<collapsingEdge> collapsingSet_l;
				
		//
		// Extract elements and data involved in the collapse
		//
		
		auto invElems = gridOperation.getElemsInvolvedInEdgeCollapsing(id1,id2);
		auto toRemove = gridOperation.getElemsOnEdge(id1,id2);
		auto toKeep = gridOperation.getElemsModifiedInEdgeCollapsing(id1,id2);
		auto toMove = gridOperation.getDataModifiedInEdgeCollapsing(invElems);
			
		// Furthermore, check that the number of elements insisting 
		// on the edge is exactly two
		if (toRemove.size() != 2)
			return;
		
		// Get normals to the elements involved in the collapse
		vector<point3d> oldNormals;
		oldNormals.reserve(toKeep.size());
		for (auto elem : toKeep)
			oldNormals.emplace_back(gridOperation.getNormal(elem));
			
		//
		// Update connections
		//
		
		// Store old id1 
		auto P(gridOperation.getCPointerToMesh()->getNode(id1));
		
		// Update node-node, node-element and element-node connections
		auto oldConnections = gridOperation.getPointerToConnectivity()
			->applyEdgeCollapse(id2, id1, toRemove, toKeep);	
		
		//	
		// No edges sharing more than two nodes
		//
		
		auto nodes = gridOperation.getCPointerToConnectivity()->getNode2Node(id1).getConnected();
		for (auto node : nodes)
		{
			auto shared = gridOperation.getNodesOnEdge(id1, node);
			if (shared.size() != 2)
			{
				// Restore connections
				gridOperation.getPointerToConnectivity()
					->undoEdgeCollapse(id2, id1, oldConnections.first, oldConnections.second, toRemove); 
			
				// Restore list of nodes
				gridOperation.getPointerToMesh()->setNode(id1, P);
				
				return;
			}
		}	
															
		//
		// Get the cheapest collapsing point
		//
		// Since checking for grid self-intersections is expensive,
		// these checks will be performed only when the cheapest  
		// collapsing point has been identified
												
		for (UInt i = 0; i < pointsList.size(); ++i)
		{
			//
			// Set collapsing point
			//
		
			// Change coordinates and boundary flag of id1
			gridOperation.getPointerToMesh()->setNode(id1, pointsList[i]);
										
			// Project data points and update data-element 
			// and element-data connections
			auto oldData = gridOperation.project(toMove, toKeep);
			gridOperation.getPointerToConnectivity()->eraseElemInData2Elem(toRemove);
			
			//
			// Check collapse validity (except for grid self-intersections)
			//
			
			bool valid(true);
						
			for (UInt j = 0; j < toKeep.size() && valid; ++j)
			{
				// No degenerate triangles
				valid = (gridOperation.getTriArea(toKeep[j]) > TOLL);
				
				// No triangle inversions
				valid = valid && (oldNormals[j] * gridOperation.getNormal(toKeep[j]) > TOLL);
				
				// No empty triangles
				valid = valid && !(gridOperation.isEmpty(toKeep[j]));
			}
			
			//
			// Get cost associated with edge collapse and update local collapsingEdge
			//
			
			if (valid)
			{
				auto cost = costObj.getCost(id1, id2, pointsList[i], toKeep, toMove);
				collapsingSet_l.emplace(id1, id2, cost, pointsList[i]);
			}
			
			//
			// Restore data-element and element-data connections
			//
			
			gridOperation.undo(toMove, oldData);
			gridOperation.getPointerToConnectivity()->insertElemInData2Elem(toRemove);
		}
			
		//
		// If no valid points have been found, restore and return
		//
		
		if (collapsingSet_l.empty())
		{
			// Restore connections
			gridOperation.getPointerToConnectivity()
				->undoEdgeCollapse(id2, id1, oldConnections.first, oldConnections.second, toRemove); 
			
			// Restore list of nodes
			gridOperation.getPointerToMesh()->setNode(id1, P);
			
			return;
		}
							
		//
		// Check for grids self-intersections
		//
		// Start from the cheapest collapsing point: if this does not lead to
		// self-intersections, add it to collapsingEdge; otherwise,
		// consider the second less expensive point and so on and so forth
		
		#ifdef ENABLE_SELF_INTERSECTIONS
			// Restore connections
			gridOperation.getPointerToConnectivity()
				->undoEdgeCollapse(id2, id1, oldConnections.first, oldConnections.second, toRemove); 
			
			// Restore list of nodes
			gridOperation.getPointerToMesh()->setNode(id1, P);
			
			// Update collapseInfo's and collapsingEdge's lists
			costObj.addCollapseInfo(id1, id2, collapsingSet_l.cbegin()->getCost(),
				collapsingSet_l.cbegin()->getCollapsingPoint());
			collapsingSet.emplace(id1, id2, collapsingSet_l.cbegin()->getCost(),
				collapsingSet_l.cbegin()->getCollapsingPoint());
		#else
			for (auto it = collapsingSet_l.cbegin(); it != collapsingSet_l.cend(); ++it)
			{
				// Set collapsing point
				gridOperation.getPointerToMesh()->setNode(id1, it->getCollapsingPoint());
				
				// Update structured data
				structData.update_f(toKeep);
				
				// Test self-intersections
				bool valid(true);
				for (auto it1 = toKeep.cbegin(); it1 != toKeep.cend() && valid; ++it1)
				{
					auto elems = structData.getNeighbouringElements(*it1);
					for (auto it2 = elems.cbegin(); it2 != elems.cend() && valid; ++it2)
						valid = valid && !(intrs.intersect(*it1, *it2));
				}
				
				// Possibly update collapseInfo's and collapsingEdge's lists
				if (valid)
				{
					// Restore connections
					gridOperation.getPointerToConnectivity()
						->undoEdgeCollapse(id2, id1, oldConnections.first, oldConnections.second, toRemove); 
			
					// Restore list of nodes
					gridOperation.getPointerToMesh()->setNode(id1, P);
					
					// Restore structured data
					structData.update_f(toKeep);
					
					// Update collapseInfo's and collapsingEdge's lists
					costObj.addCollapseInfo(id1, id2, it->getCost(), it->getCollapsingPoint());
					collapsingSet.emplace(id1, id2, it->getCost(), it->getCollapsingPoint());
						
					return;
				}
			}
			
			//
			// No collapsing point turned out to be valid, so just restore everything
			//
			
			// Restore connections
			gridOperation.getPointerToConnectivity()
				->undoEdgeCollapse(id2, id1, oldConnections.first, oldConnections.second, toRemove); 
	
			// Restore list of nodes
			gridOperation.getPointerToMesh()->setNode(id1, P);
			
			// Restore structured data
			structData.update_f(toKeep);
		#endif
	}
	

	// Specialization for grids without distributed data
	// This method requires a specialization for each purely geometric cost class 
	template<>
	void simplification<Triangle, MeshType::GEO, OnlyGeo<MeshType::GEO>>::
		getCost(const UInt & id1, const UInt & id2);
		
		
	// Specialization for grids with distributed data and a purely geometric
	// cost function
	template<>
	void simplification<Triangle, MeshType::DATA, OnlyGeo<MeshType::DATA>>::
		getCost(const UInt & id1, const UInt & id2);
				
		
	template<MeshType MT, typename CostClass>
	void simplification<Triangle, MT, CostClass>::
		getCost_f(const UInt & id1, const UInt & id2)
	{
		// First make sure the fixed element is not involved
		if (dontTouch)
			if ((id1 == gridOperation.getCPointerToMesh()->getElem(dontTouchId)[0]) ||
				(id1 == gridOperation.getCPointerToMesh()->getElem(dontTouchId)[1]) ||
				(id1 == gridOperation.getCPointerToMesh()->getElem(dontTouchId)[2]) ||
				(id2 == gridOperation.getCPointerToMesh()->getElem(dontTouchId)[0]) ||
				(id2 == gridOperation.getCPointerToMesh()->getElem(dontTouchId)[1]) ||
				(id2 == gridOperation.getCPointerToMesh()->getElem(dontTouchId)[2]))
				return;
				
		//
		// Get potentially valid points
		//
		
		auto pointsList = costObj.getPointsList(id1, id2);
		if (pointsList.empty())
			return;
			
		// Declare "local" multi-set of collapsingEdge
		// It stores the collapse information for each 
		// potentially valid collapsing points
		multiset<collapsingEdge> collapsingSet_l;
				
		//
		// Extract elements and data involved in the collapse
		//
		
		auto invElems = gridOperation.getElemsInvolvedInEdgeCollapsing(id1,id2);
		auto toRemove = gridOperation.getElemsOnEdge(id1,id2);
		auto toKeep = gridOperation.getElemsModifiedInEdgeCollapsing(id1,id2);
		auto toMove = gridOperation.getDataModifiedInEdgeCollapsing(invElems);
			
		// Furthermore, check that the number of elements insisting 
		// on the edge is exactly two
		if (toRemove.size() != 2)
			return;
		
		// Get normals to the elements involved in the collapse
		vector<point3d> oldNormals;
		oldNormals.reserve(toKeep.size());
		for (auto elem : toKeep)
			oldNormals.emplace_back(gridOperation.getNormal(elem));
			
		//
		// Update connections
		//
		
		// Store old id1 
		auto P(gridOperation.getCPointerToMesh()->getNode(id1));
		
		// Update node-node, node-element and element-node connections
		auto oldConnections = gridOperation.getPointerToConnectivity()
			->applyEdgeCollapse(id2, id1, toRemove, toKeep);	
		
		//	
		// No edges sharing more than two nodes
		//
		
		auto nodes = gridOperation.getCPointerToConnectivity()->getNode2Node(id1).getConnected();
		for (auto node : nodes)
		{
			auto shared = gridOperation.getNodesOnEdge(id1, node);
			if (shared.size() != 2)
			{
				// Restore connections
				gridOperation.getPointerToConnectivity()
					->undoEdgeCollapse(id2, id1, oldConnections.first, oldConnections.second, toRemove); 
			
				// Restore list of nodes
				gridOperation.getPointerToMesh()->setNode(id1, P);
				
				return;
			}
		}	
															
		//
		// Get the cheapest collapsing point
		//
		// Since checking for grid self-intersections is expensive,
		// these checks will be performed only when the cheapest  
		// collapsing point has been identified
												
		for (UInt i = 0; i < pointsList.size(); ++i)
		{
			//
			// Set collapsing point
			//
		
			// Change coordinates and boundary flag of id1
			gridOperation.getPointerToMesh()->setNode(id1, pointsList[i]);
										
			// Project data points and update data-element 
			// and element-data connections
			auto oldData = gridOperation.project(toMove, toKeep);
			gridOperation.getPointerToConnectivity()->eraseElemInData2Elem(toRemove);
			
			//
			// Check collapse validity (except for grid self-intersections)
			//
			
			bool valid(true);
						
			for (UInt j = 0; j < toKeep.size() && valid; ++j)
			{
				// No degenerate triangles
				valid = (gridOperation.getTriArea(toKeep[j]) > TOLL);
				
				// No triangle inversions
				valid = valid && (oldNormals[j] * gridOperation.getNormal(toKeep[j]) > TOLL);
				
				// No empty triangles
				valid = valid && !(gridOperation.isEmpty(toKeep[j]));
			}
			
			//
			// Get cost associated with edge collapse and update local collapsingEdge
			//
			
			if (valid)
			{
				auto cost = costObj.getCost(id1, id2, pointsList[i], toKeep, toMove);
				collapsingSet_l.emplace(id1, id2, cost, pointsList[i]);
			}
			
			//
			// Restore data-element and element-data connections
			//
			
			gridOperation.undo(toMove, oldData);
			gridOperation.getPointerToConnectivity()->insertElemInData2Elem(toRemove);
		}
			
		//
		// If no valid points have been found, restore and return
		//
		
		if (collapsingSet_l.empty())
		{
			// Restore connections
			gridOperation.getPointerToConnectivity()
				->undoEdgeCollapse(id2, id1, oldConnections.first, oldConnections.second, toRemove); 
			
			// Restore list of nodes
			gridOperation.getPointerToMesh()->setNode(id1, P);
			
			return;
		}
							
		//
		// Check for grids self-intersections
		//
		// Start from the cheapest collapsing point: if this does not lead to
		// self-intersections, add it to collapsingEdge; otherwise,
		// consider the second less expensive point and so on and so forth
		
		#ifdef ENABLE_SELF_INTERSECTIONS
			// Restore connections
			gridOperation.getPointerToConnectivity()
				->undoEdgeCollapse(id2, id1, oldConnections.first, oldConnections.second, toRemove); 
			
			// Restore list of nodes
			gridOperation.getPointerToMesh()->setNode(id1, P);
			
			// Update collapseInfo's and collapsingEdge's lists
			costObj.addCollapseInfo_f(id1, id2, collapsingSet_l.cbegin()->getCost(),
				collapsingSet_l.cbegin()->getCollapsingPoint());
			collapsingSet.emplace(id1, id2, collapsingSet_l.cbegin()->getCost(),
				collapsingSet_l.cbegin()->getCollapsingPoint());
		#else
			for (auto it = collapsingSet_l.cbegin(); it != collapsingSet_l.cend(); ++it)
			{
				// Set collapsing point
				gridOperation.getPointerToMesh()->setNode(id1, it->getCollapsingPoint());
				
				// Update structured data
				structData.update_f(toKeep);
				
				// Test self-intersections
				bool valid(true);
				for (auto it1 = toKeep.cbegin(); it1 != toKeep.cend() && valid; ++it1)
				{
					auto elems = structData.getNeighbouringElements(*it1);
					for (auto it2 = elems.cbegin(); it2 != elems.cend() && valid; ++it2)
						valid = valid && !(intrs.intersect(*it1, *it2));
				}
				
				// Possibly update collapseInfo's and collapsingEdge's lists
				if (valid)
				{
					// Restore connections
					gridOperation.getPointerToConnectivity()
						->undoEdgeCollapse(id2, id1, oldConnections.first, oldConnections.second, toRemove); 
			
					// Restore list of nodes
					gridOperation.getPointerToMesh()->setNode(id1, P);
					
					// Restore structured data
					structData.update_f(toKeep);
					
					// Update collapseInfo's and collapsingEdge's lists
					costObj.addCollapseInfo_f(id1, id2, it->getCost(), it->getCollapsingPoint());
					collapsingSet.emplace(id1, id2, it->getCost(), it->getCollapsingPoint());
						
					return;
				}
			}
					
			//
			// No collapsing point turned out to be valid, so just restore everything
			//
			
			// Restore connections
			gridOperation.getPointerToConnectivity()
				->undoEdgeCollapse(id2, id1, oldConnections.first, oldConnections.second, toRemove); 
	
			// Restore list of nodes
			gridOperation.getPointerToMesh()->setNode(id1, P);
			
			// Restore structured data
			structData.update_f(toKeep);
		#endif
	}
	
	
	// Specialization for grids without distributed data
	// This method requires a specialization for each purely geometric cost class 
	template<>
	void simplification<Triangle, MeshType::GEO, OnlyGeo<MeshType::GEO>>::
		getCost_f(const UInt & id1, const UInt & id2);
		
		
	// Specialization for grids with distributed data and a purely geometric
	// cost function
	template<>
	void simplification<Triangle, MeshType::DATA, OnlyGeo<MeshType::DATA>>::
		getCost_f(const UInt & id1, const UInt & id2);
	
	
	template<MeshType MT, typename CostClass>
	void simplification<Triangle, MT, CostClass>::
		update(const UInt & id1, const UInt & id2, const point3d & cPoint)
	{
		//
		// Extract elements and data involved in the collapse
		//
		
		auto invElems = gridOperation.getElemsInvolvedInEdgeCollapsing(id1,id2);
		auto toRemove = gridOperation.getElemsOnEdge(id1,id2);
		auto toKeep = gridOperation.getElemsModifiedInEdgeCollapsing(id1,id2);
		auto toMove = gridOperation.getDataModifiedInEdgeCollapsing(invElems);
				
		//
		// Update mesh and connections
		//
		
		// Set node id1 as collapsing point
		gridOperation.getPointerToMesh()->setNode(id1, cPoint);
		
		// Set node id2 inactive
		gridOperation.getPointerToMesh()->setNodeInactive(id2);
					
		// Update element-node, node-node and node-element connections
		gridOperation.getPointerToConnectivity()
			->applyEdgeCollapse(id2, id1, toRemove, toKeep);
			
		// Project involved data points and update data-element
		// and element-data connections
		gridOperation.project(toMove, toKeep);
		gridOperation.getPointerToConnectivity()->eraseElemInData2Elem(toRemove);
		
		// 
		// Update CostClass object and structured data
		//
		
		costObj.update(id1, id2, toRemove);
		#ifndef ENABLE_SELF_INTERSECTIONS
			structData.update(toKeep);
			
			// Possibly, refresh structured data
			if (structData.toRefresh())
				structData.refresh(gridOperation);
		#endif
		
		// 
		// Remove from collapseInfo's and collapsingEdge's lists
		// the edges previously connected to id2
		//
		
		auto id2Conn = gridOperation.getCPointerToConnectivity()->getNode2Node(id2).getConnected();
		for (auto conn : id2Conn)
		{
			auto costPair = costObj.eraseCollapseInfo(id2, conn);
			if (costPair.first)
				id2 < conn ? collapsingSet.erase({id2, conn, costPair.second}) :
					collapsingSet.erase({conn, id2, costPair.second});
		}
			
		//
		// Extract edges whose cost must be re-computed
		//
		// These edges are the ones which shared at least a vertex 
		// with the triangles in the extended patch of the collapsing node
		
		unordered_set<pair<UInt,UInt>> invEdges;
		
		// Get node-node connections for collapsing node, ...
		auto id1Conn = gridOperation.getCPointerToConnectivity()
			->getNode2Node(id1).getConnected();
		// ... for each node get the node-node connections...
		for (auto node_i : id1Conn)
		{
			auto iConn = gridOperation.getCPointerToConnectivity()
				->getNode2Node(node_i).getConnected();
			// ... and also for these nodes get node-node connections
			for (auto node_j : iConn)
			{
				auto jConn = gridOperation.getCPointerToConnectivity()
					->getNode2Node(node_j).getConnected();
					
				// Finally, save the edge
				for (auto node_k : jConn)
				{
					(node_k > node_j) ? invEdges.emplace(node_j, node_k) :
						invEdges.emplace(node_k, node_j);
				}
			}
		}
					
		//
		// Re-compute cost for involved edges
		//
		// First erase old costs from lists of collapseInfo's and
		// collapsingEdge's, then insert updated values
		
		for (auto edge : invEdges)
		{
			// Remove edge from collapseInfo's list and get cost
			auto costPair = costObj.eraseCollapseInfo(edge.first, edge.second);
			
			// Remove edge from collapsingEdge's list
			if (costPair.first)
				collapsingSet.erase({edge.first, edge.second, costPair.second});
			
			// Compute new cost and possibly add it to collapseInfo's
			// and collapsingEdge's lists
			getCost(edge.first, edge.second);
		}
		
		//
		// Check if the costs must be re-computed
		//
		
		if (costObj.toUpdate())
			rebuildCollapsingSet();
	}
	
	
	// Specialization for grids without distributed data
	// This method requires a specialization for each purely geometric cost class
	template<>
	void simplification<Triangle, MeshType::GEO, OnlyGeo<MeshType::GEO>>::
		update(const UInt & id1, const UInt & id2, const point3d & cPoint);
	
	
	template<MeshType MT, typename CostClass>
	void simplification<Triangle, MT, CostClass>::
		update(const vector<UInt> & id1, const vector<UInt> & id2, const vector<point3d> & cPoint)
	{
		for (UInt i = 0; i < id1.size(); ++i)
		{
			//
			// Extract elements and data involved in the collapse
			//
		
			auto invElems = gridOperation.getElemsInvolvedInEdgeCollapsing(id1[i],id2[i]);
			auto toRemove = gridOperation.getElemsOnEdge(id1[i],id2[i]);
			auto toKeep = gridOperation.getElemsModifiedInEdgeCollapsing(id1[i],id2[i]);
			auto toMove = gridOperation.getDataModifiedInEdgeCollapsing(invElems);
		
			//
			// Update mesh and connections
			//
		
			// Set node id1 as collapsing point
			gridOperation.getPointerToMesh()->setNode(id1[i], cPoint[i]);
		
			// Set node id2 inactive
			gridOperation.getPointerToMesh()->setNodeInactive(id2[i]);
					
			// Update element-node, node-node and node-element connections
			gridOperation.getPointerToConnectivity()
				->applyEdgeCollapse(id2[i], id1[i], toRemove, toKeep);
			
			// Project involved data points and update data-element
			// and element-data connections
			gridOperation.project(toMove, toKeep);
			gridOperation.getPointerToConnectivity()->eraseElemInData2Elem(toRemove);
		
			// 
			// Update CostClass object and structured data
			//
		
			costObj.update(id1[i], id2[i], toRemove);
			#ifndef ENABLE_SELF_INTERSECTIONS
				structData.update(toKeep);
			#endif
		
			// 
			// Remove from collapseInfo's and collapsingEdge's lists
			// the edges previously connected to id2
			//
		
			auto id2Conn = gridOperation.getCPointerToConnectivity()->getNode2Node(id2[i]).getConnected();
			for (auto conn : id2Conn)
			{
				auto costPair = costObj.eraseCollapseInfo(id2[i], conn);
				if (costPair.first)
					id2[i] < conn ? collapsingSet.erase({id2[i], conn, costPair.second}) :
						collapsingSet.erase({conn, id2[i], costPair.second});
			}
			
			//
			// Extract edges whose cost must be re-computed
			//
			// These edges are the ones which shared at least a vertex 
			// with the triangles in the extended patch of the collapsing node
		
			unordered_set<pair<UInt,UInt>> invEdges;
		
			// Get node-node connections for collapsing node, ...
			auto id1Conn = gridOperation.getCPointerToConnectivity()
				->getNode2Node(id1[i]).getConnected();
			// ... for each node get the node-node connections...
			for (auto node_i : id1Conn)
			{
				auto iConn = gridOperation.getCPointerToConnectivity()
					->getNode2Node(node_i).getConnected();
				// ... and also for these nodes get node-node connections
				for (auto node_j : iConn)
				{
					auto jConn = gridOperation.getCPointerToConnectivity()
						->getNode2Node(node_j).getConnected();
					
					// Finally, save the edge
					for (auto node_k : jConn)
					{
						(node_k > node_j) ? invEdges.emplace(node_j, node_k) :
							invEdges.emplace(node_k, node_j);
					}
				}
			}
					
			//
			// Re-compute cost for involved edges
			//
			// First erase old costs from lists of collapseInfo's and
			// collapsingEdge's, then insert updated values
		
			for (auto edge : invEdges)
			{
				// Remove edge from collapseInfo's list and get cost
				auto costPair = costObj.eraseCollapseInfo(edge.first, edge.second);
			
				// Remove edge from collapsingEdge's list
				if (costPair.first)
					collapsingSet.erase({edge.first, edge.second, costPair.second});
			
				// Compute new cost and possibly add it to collapseInfo's
				// and collapsingEdge's lists
				getCost(edge.first, edge.second);
			}
		}
		
		//
		// Check if the costs must be re-computed
		//
		
		#ifndef ENABLE_SELF_INTERSECTIONS
			// Possibly, refresh structured data
			if (structData.toRefresh())
				structData.refresh(gridOperation);
		#endif
		
		if (costObj.toUpdate())
			rebuildCollapsingSet();
	}
	
	
	// Specialization for grids without distributed data
	// This method requires a specialization for each purely geometric cost class
	template<>
	void simplification<Triangle, MeshType::GEO, OnlyGeo<MeshType::GEO>>::
		update(const vector<UInt> & id1, const vector<UInt> & id2, const vector<point3d> & cPoint);
		

	//
	// Methods for handling the fixed element
	//
	
	template<MeshType MT, typename CostClass>
	INLINE void simplification<Triangle, MT, CostClass>::enableDontTouch()
	{
		dontTouch = true;
	}
	
	
	template<MeshType MT, typename CostClass>
	INLINE void simplification<Triangle, MT, CostClass>::disableDontTouch()
	{
		dontTouch = false;
	}
	
	
	template<MeshType MT, typename CostClass>
	INLINE bool simplification<Triangle, MT, CostClass>::getDontTouch() const
	{
		return dontTouch;
	}
	
	
	template<MeshType MT, typename CostClass>
	INLINE void simplification<Triangle, MT, CostClass>::setDontTouchId(const UInt & Id)
	{
		dontTouchId = Id;
	}
	
	
	template<MeshType MT, typename CostClass>
	INLINE UInt simplification<Triangle, MT, CostClass>::getDontTouchId() const
	{
		return dontTouchId;
	}
	
	
	template<MeshType MT, typename CostClass>
	void simplification<Triangle, MT, CostClass>::findDontTouchId()
	{		
		dontTouch = true;
		
		// Compute global barycenter
		auto global_bar = gridOperation.getMeshBarycenter();
		
		// Get the element whose barycenter is closest to the global one
		Real opt_dist(numeric_limits<Real>::max());
		for (UInt i = 0; i < gridOperation.getCPointerToMesh()->getElemsListSize(); ++i)
		{
			if (gridOperation.getCPointerToMesh()->getElem(i).isActive())
			{
				// Compute local barycenter
				auto local_bar = gridOperation.getElemBarycenter(i);
				
				// Check if it is the closest element
				auto dist = (local_bar - global_bar).norm2();
				if (dist < opt_dist - TOLL)
				{
					opt_dist = dist;
					dontTouchId = i;
				}
			}
		}
	}


	//
	// Methods which make the simplification
	//
	
	template<MeshType MT, typename CostClass>
	void simplification<Triangle, MT, CostClass>::simplify(const UInt & numNodesMax,
		const bool & enableDontTouch, const string & file)
	{				
		// Check if the current number of nodes is below the threshold
		auto numNodesStart(gridOperation.getCPointerToMesh()->getNumNodes());
		if (numNodesMax >= numNodesStart)
		{
			cout << "The number of mesh points is " << gridOperation.getCPointerToMesh()->getNumNodes()
				<< ", already below the given threshold " << numNodesMax << "." << endl;
			return;
		}
						
		//
		// Run simplification
		//
		
		dontTouch = enableDontTouch;
				
		#ifdef NDEBUG
		using namespace std::chrono;
		high_resolution_clock::time_point start = high_resolution_clock::now();
		#endif

		// Iterative collapse until numNodeMax is reached
		#ifdef NDEBUG
			UInt barWidth(35);
		#endif
		while (gridOperation.getCPointerToMesh()->getNumNodes() > numNodesMax)
		{
			if (collapsingSet.size() > 0)
			{
				// Take the first valid collapsing edge with the minimum cost
				auto minCostEdge = collapsingSet.cbegin();
				auto id1 = minCostEdge->getId1();
				auto id2 = minCostEdge->getId2();
				auto cPoint = minCostEdge->getCollapsingPoint();
						
				// Update the mesh, the connectivities, the structured data, CostClass object
				// Re-compute cost for involved edges
				update(id1, id2, cPoint);
			
				#ifdef NDEBUG
					// Update progress bar
					Real progress((numNodesStart - gridOperation.getCPointerToMesh()->getNumNodes())
						/ (static_cast<Real>(numNodesStart - numNodesMax)));
					cout << "Simplification process        [";
					UInt pos(barWidth * progress);
					for (UInt i = 0; i < barWidth; ++i) 
					{
						if (i < pos) 
							cout << "=";
						else if (i == pos) 
							cout << ">";
						else 
							cout << " ";
					}
					cout << "] " << UInt(progress * 100.0) << " %\r";
					cout.flush();
				#endif
			}
			else
			{
				// No valid edges left, then stop
				cout << endl << "The process stopped prematurely since there are no valid edges left.";
				break;
			}
		}
		#ifdef NDEBUG
		cout << endl;
		high_resolution_clock::time_point stop = high_resolution_clock::now();
		auto dif = duration_cast<milliseconds>(stop-start).count();
		#endif
		
		//
		// Refresh the mesh, the connections, cInfoList, collapsingSet and fixed element
		//
		
		auto old2new = gridOperation.refresh();
		costObj.refreshCInfoList(old2new.first);
		refreshCollapsingSet(old2new.first);
		dontTouchId = old2new.second[dontTouchId];
		
		//
		// Print ...
		//
		
		// ... to screen
		#ifdef NDEBUG
		cout << "Simplification process completed in " <<  dif/1000 << " seconds." << endl;
		#endif
		cout << "The mesh size passed from " << numNodesStart << " to " << 
			gridOperation.getCPointerToMesh()->getNumNodes() << " nodes." << endl;
		
		// ... to file
		if (!(file.empty()))
			gridOperation.getPointerToMesh()->print(file);
			//gridOperation.printMesh(file);
	}
}

#endif

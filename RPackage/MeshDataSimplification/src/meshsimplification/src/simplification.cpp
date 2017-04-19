/*!	\file	simplification.cpp
	\brief	Specialization of some members of class simplification. */
			
#include "simplification.hpp"

// Include inlined class members
#ifndef INLINED
#include "inline/inline_simplification.hpp"
#endif

namespace geometry
{
	//
	// Compute cost and apply collapse
	//
		
	template<>
	void simplification<Triangle, MeshType::GEO, OnlyGeo<MeshType::GEO>>::
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
		
		auto toRemove = gridOperation.getElemsOnEdge(id1,id2);
		auto toKeep = gridOperation.getElemsModifiedInEdgeCollapsing(id1,id2);
			
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
			}
			
			//
			// Get cost associated with edge collapse and update local collapsingEdge
			//
			
			if (valid)
			{
				auto cost = costObj.getCost(id1, id2, pointsList[i]);
				collapsingSet_l.emplace(id1, id2, cost, pointsList[i]);
			}
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
	
	
	template<>
	void simplification<Triangle, MeshType::DATA, OnlyGeo<MeshType::DATA>>::
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
			}
			
			//
			// Get cost associated with edge collapse and update local collapsingEdge
			//
			
			if (valid)
			{
				auto cost = costObj.getCost(id1, id2, pointsList[i]);
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
		
	
	template<>
	void simplification<Triangle, MeshType::GEO, OnlyGeo<MeshType::GEO>>::
		update(const UInt & id1, const UInt & id2, const point3d & cPoint)
	{
		//
		// Extract elements involved in the collapse
		//
		
		auto toRemove = gridOperation.getElemsOnEdge(id1,id2);
		auto toKeep = gridOperation.getElemsModifiedInEdgeCollapsing(id1,id2);
		
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
		// with the edges sharing the collapsing point
		
		unordered_set<pair<UInt,UInt>> invEdges;
		
		// Get node-node connections for collapsing node...
		auto id1Conn = gridOperation.getCPointerToConnectivity()
			->getNode2Node(id1).getConnected();
		// ... and for each node get node-node connections
		for (auto node_i : id1Conn)
		{
			auto iConn = gridOperation.getCPointerToConnectivity()
				->getNode2Node(node_i).getConnected();
				
			// Save the egde
			for (auto node_j : iConn)
				(node_j > node_i) ? invEdges.emplace(node_i, node_j) :
					invEdges.emplace(node_j, node_i);
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
			getCost_f(edge.first, edge.second);
		}
	}
	
	
	template<>
	void simplification<Triangle, MeshType::GEO, OnlyGeo<MeshType::GEO>>::
		update(const vector<UInt> & id1, const vector<UInt> & id2, const vector<point3d> & cPoint)
	{
		for (UInt i = 0; i < id1.size(); ++i)
		{
			//
			// Extract elements involved in the collapse
			//
		
			auto toRemove = gridOperation.getElemsOnEdge(id1[i],id2[i]);
			auto toKeep = gridOperation.getElemsModifiedInEdgeCollapsing(id1[i],id2[i]);
		
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
			// with the edges sharing the collapsing point
		
			unordered_set<pair<UInt,UInt>> invEdges;
		
			// Get node-node connections for collapsing node...
			auto id1Conn = gridOperation.getCPointerToConnectivity()
				->getNode2Node(id1[i]).getConnected();
			// ... and for each node get node-node connections
			for (auto node_i : id1Conn)
			{
				auto iConn = gridOperation.getCPointerToConnectivity()
					->getNode2Node(node_i).getConnected();
				
				// Save the egde
				for (auto node_j : iConn)
					(node_j > node_i) ? invEdges.emplace(node_i, node_j) :
						invEdges.emplace(node_j, node_i);
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
				getCost_f(edge.first, edge.second);
			}
		}
		
		#ifndef ENABLE_SELF_INTERSECTIONS
			// Possibly, refresh structured data
			if (structData.toRefresh())
				structData.refresh(gridOperation);
		#endif
	}
}

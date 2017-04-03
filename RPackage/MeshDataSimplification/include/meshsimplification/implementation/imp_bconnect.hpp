/*! \file	imp_bconnect.hpp
	\brief	Definitions of members of class bconnect. */
	
#ifndef HH_IMPBCONNECT_HH
#define HH_IMPBCONNECT_HH

#include <array>
#include <stdexcept>

namespace geometry
{
	// 
	// Constructor
	//
	
	template<typename SHAPE, MeshType MT>
	bconnect<SHAPE,MT>::bconnect(const bmesh<SHAPE> & bg) : 
		grid(bg)
	{
		// Build all connections and fill set of edges
		buildNode2Node();
		buildNode2Elem();
	}
			
	
	template<typename SHAPE, MeshType MT>
	template<typename... Args>
	bconnect<SHAPE,MT>::bconnect(Args... args) :
		grid(args...)
	{
		// Build all connections and fill set of edges
		buildNode2Node();
		buildNode2Elem();
	}
	
	
	//
	// Initialize and clear connections
	//
	
	template<typename SHAPE, MeshType MT>
	void bconnect<SHAPE,MT>::buildNode2Node()
	{
		// Clear and reserve memory
		edges.clear();
		node2node.clear();
		node2node.reserve(grid.getNumNodes());
					
		// Set nodes Id's
		for (UInt id = 0; id < grid.getNumNodes(); ++id)
			node2node.emplace_back(id);
						
		// Loop over all elements
		geoElement<SHAPE> elem;
		UInt id1, id2;
		for (UInt id = 0; id < grid.getNumElems(); ++id)
		{
			// Extract element
			elem = grid.getElem(id);
							
			// Loop over edges
			for (UInt j = 0; j < N; j+=2)
			{
				// Extract extrema of the edge
				id1 = elem[SHAPE::edgeConn[j]];
				id2 = elem[SHAPE::edgeConn[j+1]];
									
				// Add node-node connections
				node2node[id1].insert(id2);
				node2node[id2].insert(id1);
									
				// Update set of edges
				(id1 < id2) ? edges.emplace(array<UInt,2>({{id1,id2}})) :
					edges.emplace(array<UInt,2>({{id2,id1}}));
			}
		}
	}
	
	
	template<typename SHAPE, MeshType MT>
	void bconnect<SHAPE,MT>::buildNode2Elem()
	{
		// Reserve memory
		node2elem.clear();
		node2elem.reserve(grid.getNumNodes());
		
		// Set nodes Id's
		for (UInt id = 0; id < grid.getNumNodes(); ++id)
			node2elem.emplace_back(id);
			
		// Loop over elements
		geoElement<SHAPE> elem;
		for (UInt id = 0; id < grid.getNumElems(); ++id)
		{
			// Extract element
			elem = grid.getElem(id);
			
			// Loop over its vertices and add node-element connections
			for (UInt j = 0; j < NV; ++j)
				node2elem[elem[j]].insert(id);
		}
	}
		
	
	template<typename SHAPE, MeshType MT>
	void bconnect<SHAPE,MT>::refresh()
	{
		// Refresh the mesh
		grid.refresh();
		
		// Re-build all connections and the set of edges
		buildNode2Node();
		buildNode2Elem();
	}
	
	
	template<typename SHAPE, MeshType MT>
	void bconnect<SHAPE,MT>::clear()
	{
		// Clear connections
		node2node.clear();
		node2elem.clear();
				
		// Clear set of edges
		edges.clear();
	}
	
	
	//
	// Modify connections
	//
	
	template<typename SHAPE, MeshType MT>
	void bconnect<SHAPE,MT>::replaceNodeInElem2Node(const UInt & oldId, const UInt & newId)
	{
		// Get elements connected to oldId but not to newId
		auto toKeep = set_difference(node2elem[oldId], node2elem[newId]);
		
		// For all extracted elements, replace oldId with newId
		for (auto elem : toKeep)
			grid.replaceVertex(elem, oldId, newId);
	}
	
	
	template<typename SHAPE, MeshType MT>
	INLINE void bconnect<SHAPE,MT>::replaceNodeInElem2Node(const UInt & oldId, const UInt & newId,
		const vector<UInt> & toKeep)
	{
		for (auto elem : toKeep)
			grid.replaceVertex(elem, oldId, newId);
	}
	
	
	template<typename SHAPE, MeshType MT>
	vector<UInt> bconnect<SHAPE,MT>::replaceNodeInNode2Node(const UInt & oldId, const UInt & newId)
	{
		// Extract node-node connections for newId
		auto newId_oldNode2Node = node2node[newId].getConnected();
		
		// Make oldId inactive and extract its connected
		node2node[oldId].setInactive();
		auto oldIdConn = node2node[oldId].getConnected();
				
		// For all connected, replace oldId with newId
		for (auto id : oldIdConn)
			node2node[id].replace(oldId,newId);
			
		// The nodes previously connected to oldId are now connected to newId...
		node2node[newId].insert(oldIdConn);
		// ... but make sure to avoid self-connections!
		node2node[newId].erase(newId);
		
		return newId_oldNode2Node;
	}
	
	
	template<typename SHAPE, MeshType MT>
	vector<UInt> bconnect<SHAPE,MT>::replaceNodeInNode2Node(const UInt & oldId, const UInt & newId, 
		const vector<UInt> & involved)
	{
		// Extract node-node connections for newId
		auto newId_oldNode2Node = node2node[newId].getConnected();
		
		// Make oldId inactive
		node2node[oldId].setInactive();
		
		// The nodes previously connected to oldId are now connected to newId
		node2node[newId].setConnected(involved);
		
		// For all connected, replace oldId with newId
		for (auto id : involved)
			node2node[id].replace(oldId,newId);
			
		return newId_oldNode2Node;
	}
	
	
	template<typename SHAPE, MeshType MT>
	vector<UInt> bconnect<SHAPE,MT>::replaceNodeInNode2Elem(const UInt & oldId, const UInt & newId)
	{
		// Extract node-element connections for newId
		auto newId_oldNode2Elem = node2elem[newId].getConnected();
		
		// Move the connected from oldId to newId
		node2elem[newId].insert(node2elem[oldId].getConnected());
		
		// Make oldId inactive
		node2elem[oldId].setInactive();
		
		return newId_oldNode2Elem;
	}
	
	
	template<typename SHAPE, MeshType MT>
	void bconnect<SHAPE,MT>::eraseElemsInNode2Elem(const vector<UInt> & toRemove)
	{
		for (auto id : toRemove)
		{
			// Set element inactive in element-node connections
			grid.setElemInactive(id);
			
			// Erase element from node-element connections
			auto elem = grid.getElem(id);
			for (UInt j = 0; j < NV; ++j)
				node2elem[elem[j]].erase(id);
		}
	}
	
	
	template<typename SHAPE, MeshType MT>
	void bconnect<SHAPE,MT>::eraseElemsInNode2Elem(const vector<UInt> & toRemove, 
		const UInt & newId, const vector<UInt> & involved)
	{
		// Set element inactive in element-node connections
		grid.setElemInactive(toRemove[0]);
		grid.setElemInactive(toRemove[1]);
		
		// Remove elements from newId
		node2elem[newId].erase(toRemove[0]);
		node2elem[newId].erase(toRemove[1]);
		
		// Remove elements from all other involved nodes
		for (auto node : involved)
		{
			node2elem[node].erase(toRemove[0]);
			node2elem[node].erase(toRemove[1]);
		}
	}
	
	
	template<typename SHAPE, MeshType MT>
	pair<vector<UInt>, vector<UInt>> bconnect<SHAPE,MT>::applyEdgeCollapse(const UInt & oldId, const UInt & newId, 
		const vector<UInt> & toRemove)
	{
		// Update element-node connections
		replaceNodeInElem2Node(oldId, newId);
		
		// Update node-node connections
		auto newId_oldNode2Node = replaceNodeInNode2Node(oldId, newId);
		
		// Update node-element connections
		auto newId_oldNode2Elem = replaceNodeInNode2Elem(oldId, newId);
		eraseElemsInNode2Elem(toRemove);
				
		return make_pair(newId_oldNode2Node, newId_oldNode2Elem);
	}
	
	
	template<typename SHAPE, MeshType MT>
	pair<vector<UInt>, vector<UInt>> bconnect<SHAPE,MT>::applyEdgeCollapse(const UInt & oldId, const UInt & newId, 
		const vector<UInt> & toRemove, const vector<UInt> & toKeep)
	{
		// Update element-node connections
		replaceNodeInElem2Node(oldId, newId, toKeep);
		
		// Update node-node connections
		auto newId_oldNode2Node = replaceNodeInNode2Node(oldId, newId);
		
		// Update node-element connections
		auto newId_oldNode2Elem = replaceNodeInNode2Elem(oldId, newId);
		eraseElemsInNode2Elem(toRemove);
				
		return make_pair(newId_oldNode2Node, newId_oldNode2Elem);
	}
	
	
	template<typename SHAPE, MeshType MT>
	pair<vector<UInt>, vector<UInt>> bconnect<SHAPE,MT>::applyEdgeCollapse(const UInt & oldId, const UInt & newId, 
		const vector<UInt> & toRemove, const vector<UInt> & toKeep, const vector<UInt> & involved)
	{
		// Update element-node connections
		replaceNodeInElem2Node(oldId, newId, toKeep);
		
		// Update node-node connections
		auto newId_oldNode2Node = replaceNodeInNode2Node(oldId, newId, involved);
		
		// Update node-element connections
		auto newId_oldNode2Elem = replaceNodeInNode2Elem(oldId, newId);
		eraseElemsInNode2Elem(toRemove, newId, involved);
		
		return make_pair(newId_oldNode2Node, newId_oldNode2Elem);
	}
	
	
	//
	// Methods to restore connections to the status 
	// prior to a temporary edge collapse
	//
	
	template<typename SHAPE, MeshType MT>
	void bconnect<SHAPE,MT>::restoreNode2Node(const UInt & oldId, const UInt & newId,
		const vector<UInt> & newId_oldNode2Node)
	{
		// Restore node-node connections for oldId and newId
		node2node[oldId].setActive();
		node2node[newId].setConnected(newId_oldNode2Node);
		
		// Get nodes insisting on the edge (oldId,newId)
		// and re-stablish connections with oldId
		auto onEdge = set_intersection(node2node[oldId], node2node[newId]);
		for (auto id : onEdge)
			node2node[id].insert(oldId);
		
		// For all other nodes connected to oldId, re-stablish 
		// connections with oldId by replacing  newId with oldId 
		auto nodes = set_difference(node2node[oldId], node2node[newId]);
		for (auto node : nodes)
			node2node[node].replace(newId, oldId);
	}
	
	
	template<typename SHAPE, MeshType MT>
	void bconnect<SHAPE,MT>::restoreNode2Elem(const UInt & oldId, const UInt & newId,
		const vector<UInt> & newId_oldNode2Elem)
	{
		// Restore node-element connections for oldId and newId
		node2elem[oldId].setActive();
		node2elem[newId].setConnected(newId_oldNode2Elem);
		
		// Extract the elements sharing the edge
		auto onEdge = set_intersection(node2elem[oldId], node2elem[newId]);
		
		// Restore node-element connections for the nodes
		// insisting on the edge 
		for (auto elemId : onEdge)
		{
			auto elem = grid.getElem(elemId);
			node2elem[elem[0]].insert(elemId);
			node2elem[elem[1]].insert(elemId);
			node2elem[elem[2]].insert(elemId);
		}
	}
	
	
	template<typename SHAPE, MeshType MT>
	void bconnect<SHAPE,MT>::restoreNode2Elem(const UInt & oldId, const UInt & newId,
		const vector<UInt> & newId_oldNode2Elem, const vector<UInt> & onEdge)
	{
		// Restore node-element connections for oldId and newId
		node2elem[oldId].setActive();
		node2elem[newId].setConnected(newId_oldNode2Elem);
		
		// Restore node-element connections for the nodes
		// insisting on the edge 
		for (auto elemId : onEdge)
		{
			auto elem = grid.getElem(elemId);
			node2elem[elem[0]].insert(elemId);
			node2elem[elem[1]].insert(elemId);
			node2elem[elem[2]].insert(elemId);
		}
	}
	
	
	template<typename SHAPE, MeshType MT>
	void bconnect<SHAPE,MT>::restoreElem2Node(const UInt & oldId, const UInt & newId)
	{
		// Get elements sharing the edge
		auto onEdge = set_intersection(node2elem[oldId], node2elem[newId]);
		
		// Make the previously removed elements active again
		for (auto elem : onEdge)
			grid.setElemActive(elem);
		
		// Get elements connected to oldId but not to newId
		auto oldIdOnly = set_difference(node2elem[oldId], node2elem[newId]);
		
		// For all extracted elements, replace newId with oldId
		for (auto elem : oldIdOnly)
			grid.replaceVertex(elem, newId, oldId);
	}
	
	
	template<typename SHAPE, MeshType MT>
	void bconnect<SHAPE,MT>::restoreElem2Node(const UInt & oldId, const UInt & newId,
		const vector<UInt> & onEdge)
	{
		// Make the previously removed elements active again
		for (auto elem : onEdge)
			grid.setElemActive(elem);
		
		// Get elements connected to oldId but not to newId
		auto oldIdOnly = set_difference(node2elem[oldId], node2elem[newId]);
		
		// For all extracted elements, replace newId with oldId
		for (auto elem : oldIdOnly)
			grid.replaceVertex(elem, newId, oldId);
	}
	
	
	template<typename SHAPE, MeshType MT>
	void bconnect<SHAPE,MT>::undoEdgeCollapse(const UInt & oldId, const UInt & newId, 
		const vector<UInt> & newId_oldNode2Node, const vector<UInt> & newId_oldNode2Elem)
	{ 
		// Restore node-node connections
		restoreNode2Node(oldId, newId, newId_oldNode2Node);
		
		// Restore node-element connections
		restoreNode2Elem(oldId, newId, newId_oldNode2Elem);
		
		// Restore element-node connections
		restoreElem2Node(oldId, newId);
	}
	
	
	template<typename SHAPE, MeshType MT>
	void bconnect<SHAPE,MT>::undoEdgeCollapse(const UInt & oldId, const UInt & newId, 
		const vector<UInt> & newId_oldNode2Node, const vector<UInt> & newId_oldNode2Elem,
		const vector<UInt> & onEdge)
	{ 
		// Restore node-node connections
		restoreNode2Node(oldId, newId, newId_oldNode2Node);
		
		// Restore node-element connections
		restoreNode2Elem(oldId, newId, newId_oldNode2Elem, onEdge);
		
		// Restore element-node connections
		restoreElem2Node(oldId, newId, onEdge);
	}
	
	
	//
	// Get methods
	//
	
	template<typename SHAPE, MeshType MT>
	INLINE mesh<SHAPE,MT> bconnect<SHAPE,MT>::getMesh() const
	{
		return grid;
	}
	
	
	template<typename SHAPE, MeshType MT>
	INLINE mesh<SHAPE,MT> * bconnect<SHAPE,MT>::getPointerToMesh()
	{
		return &grid;
	}
	
	
	template<typename SHAPE, MeshType MT>
	INLINE UInt bconnect<SHAPE,MT>::getNumEdges() const
	{
		return edges.size();
	}
	
	
	template<typename SHAPE, MeshType MT>
	INLINE vector<geoElement<Line>> bconnect<SHAPE,MT>::getEdges() const
	{
		return vector<geoElement<Line>>(edges.cbegin(), edges.cend());
	}
	
	
	template<typename SHAPE, MeshType MT>
	INLINE graphItem bconnect<SHAPE,MT>::getNode2Node(const UInt & Id) const
	{
		return node2node[Id];
	}
	
	
	template<typename SHAPE, MeshType MT>
	INLINE vector<graphItem> bconnect<SHAPE,MT>::getNode2Node() const
	{
		return node2node;
	}
	
	
	template<typename SHAPE, MeshType MT>
	INLINE graphItem bconnect<SHAPE,MT>::getNode2Elem(const UInt & Id) const
	{
		return node2elem[Id];
	}
	
	
	template<typename SHAPE, MeshType MT>
	INLINE vector<graphItem> bconnect<SHAPE,MT>::getNode2Elem() const
	{
		return node2elem;
	}
	
	
	//
	// Set methods
	//
	
	template<typename SHAPE, MeshType MT>
	void bconnect<SHAPE,MT>::setMesh(const bmesh<SHAPE> & g)
	{
		// Set mesh 
		grid = g;
		
		// (Re-)build connections
		refresh();
	}
}

#endif

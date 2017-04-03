/*!	\file	imp_bmeshInfo.hpp
	\brief	Implementations of members of class bmeshInfo. */
	
/*
	Note: some methods are provided only for some classes of grids,
	e.g. only for triangular grids. To accomplish this, we add some 
	static_assert's evaluating the number of vertices for each element. 
	This is possible since the number of vertices is stored as a static 
	const class attribute. However, in debugging phase we force the
	compiler to generate the code for all methods of template classes.
	Since for each shape there is at least one method not defined, not 
	to get errors at compile time, all static_assert's are enabled 
	only in release mode.
	
	The involved methods are:
		* 	getTriPatch()		provided only for triangular grids;
								for over shapes, use getElemPatch()
		* 	getIntervalLength() provided only for 1D grids
		* 	getTriArea() 		provided only for triangular grids
		* 	getNormal() 		provided only for triangular grids
		
	Methods which may be implemented in future:
		*	getElemArea()		compute the area of a generically-shaped
								element of a 2D grid
		*	getElemVolume()		compute the volume of a generically-shaped
								element of a 3D grid
		*	getElemNormals()	get the normal for each face of an
								element of a 3D grid
*/

#ifndef HH_IMPBMESHINFO_HH
#define	HH_IMPBMESHINFO_HH

#include <limits>
#include <cmath>

namespace geometry
{
	//
	// Constructors
	//
	
	template<typename SHAPE, MeshType MT>
	bmeshInfo<SHAPE,MT>::bmeshInfo(const bmesh<SHAPE> & bg) :
		connectivity(bg)
	{
		// Set boundary flag for each node
		setBoundary();
	}
	
	
	template<typename SHAPE, MeshType MT>
	template<typename... Args>
	bmeshInfo<SHAPE,MT>::bmeshInfo(Args... args) :
		connectivity(args...)
	{
		// Set boundary flag for each node
		setBoundary();
	}
	
	
	//
	// Access attributes
	//
	
	template<typename SHAPE, MeshType MT>
	INLINE mesh<SHAPE,MT> bmeshInfo<SHAPE,MT>::getMesh() const
	{
		return connectivity.grid;
	}
	
	
	template<typename SHAPE, MeshType MT>
	INLINE mesh<SHAPE,MT> * bmeshInfo<SHAPE,MT>::getPointerToMesh()
	{
		return &connectivity.grid;
	}
	
	
	template<typename SHAPE, MeshType MT>
	INLINE const mesh<SHAPE,MT> * bmeshInfo<SHAPE,MT>::getCPointerToMesh() const
	{
		return &connectivity.grid;
	}
	
	
	template<typename SHAPE, MeshType MT>
	INLINE connect<SHAPE,MT> bmeshInfo<SHAPE,MT>::getConnectivity() const
	{
		return connectivity;
	}
	
	template<typename SHAPE, MeshType MT>
	INLINE connect<SHAPE,MT> * bmeshInfo<SHAPE,MT>::getPointerToConnectivity()
	{
		return &connectivity;
	}
	
	
	template<typename SHAPE, MeshType MT>
	INLINE const connect<SHAPE,MT> * bmeshInfo<SHAPE,MT>::getCPointerToConnectivity() const
	{
		return &connectivity;
	}
	
	
	template<typename SHAPE, MeshType MT>
	INLINE void bmeshInfo<SHAPE,MT>::setMesh(const bmesh<SHAPE> & g)
	{
		connectivity.setMesh(g);
	}
	
	
	//
	// Refresh method
	//
	
	template<typename SHAPE, MeshType MT>
	INLINE void bmeshInfo<SHAPE,MT>::refresh()
	{
		connectivity.refresh();
	}
	
	
	//
	// Print methods
	//
	
	template<typename SHAPE, MeshType MT>
	INLINE void bmeshInfo<SHAPE,MT>::printMesh(const string & filename)
	{
		// First refresh, then print
		connectivity.refresh();
		connectivity.grid.print(filename);
	}
		
	
	//
	// Get topological info
	//
	
	template<typename SHAPE, MeshType MT>
	INLINE vector<UInt> bmeshInfo<SHAPE,MT>::getNodesOnEdge
		(const UInt & id1, const UInt & id2) const
	{
		// Get nodes connected both to id1 and id2
		auto s = set_intersection(connectivity.node2node[id1], connectivity.node2node[id2]);		
		return {s.cbegin(), s.cend()};
	}
	
	
	template<typename SHAPE, MeshType MT>
	vector<UInt> bmeshInfo<SHAPE,MT>::getNodesInvolvedInEdgeCollapsing
		(const UInt & id1, const UInt & id2) const
	{
		// Get nodes connected either to id1 or id2
		auto s = set_union(connectivity.node2node[id1], connectivity.node2node[id2]);
		
		// Remove id1 and id2
		s.erase(id1);
		s.erase(id2);
		
		return {s.cbegin(), s.cend()};
	}
	
	
	template<typename SHAPE, MeshType MT>
	INLINE vector<UInt> bmeshInfo<SHAPE,MT>::getElemsOnEdge
		(const UInt & id1, const UInt & id2) const
	{
		// Get elements connected both to id1 and id2
		auto s = set_intersection(connectivity.node2elem[id1], 
			connectivity.node2elem[id2]);
		return vector<UInt>(s.begin(), s.end()); 
	}
	
	
	template<typename SHAPE, MeshType MT>
	INLINE vector<UInt> bmeshInfo<SHAPE,MT>::getElemsInvolvedInEdgeCollapsing
		(const UInt & id1, const UInt & id2) const
	{
		// Get elements connected either to id1 or id2
		auto s = set_union(connectivity.node2elem[id1], connectivity.node2elem[id2]);
		return vector<UInt>(s.begin(), s.end()); 
	}
	
	
	template<typename SHAPE, MeshType MT>
	vector<UInt> bmeshInfo<SHAPE,MT>::getElemsModifiedInEdgeCollapsing
		(const UInt & id1, const UInt & id2) const
	{
		// Get elements connected either to id1 or id2, but not both
		auto s = set_symmetric_difference(connectivity.node2elem[id1], 
			connectivity.node2elem[id2]);
		return vector<UInt>(s.begin(), s.end());
	}
	
	
	template<typename SHAPE, MeshType MT>
	vector<UInt> bmeshInfo<SHAPE,MT>::getExtendedNodePatch(const UInt & Id) const
	{
		set<UInt> s;
		
		// Extract nodes connected to Id
		auto nodes = connectivity.node2node[Id].getConnected();
		
		// Extract elements connected to each node,
		// then insert them in the set
		for (auto node : nodes)
			set_union(connectivity.node2elem[node], s);
			
		return {s.cbegin(), s.cend()};
	}
	
	
	template<typename SHAPE, MeshType MT>
	vector<UInt> bmeshInfo<SHAPE,MT>::getTriPatch(const UInt & Id) const
	{
		// This method is provided only for triangular grids
		#ifdef NDEBUG
		static_assert(NV <= 3, 
			"getTriPatch() is provided only for triangular grids.");
		#endif
			
		// Get element 
		auto elem = connectivity.grid.getElem(Id);
		
		// Get elements connected to at least one of the vertices of the element
		auto s = set_union(connectivity.node2elem[elem[0]],
			connectivity.node2elem[elem[1]], connectivity.node2elem[elem[2]]);
			
		// Remove Id
		s.erase(Id);
		
		return {s.cbegin(), s.cend()};
	}
	
	
	template<typename SHAPE, MeshType MT>
	vector<UInt> bmeshInfo<SHAPE,MT>::getElemPatch(const UInt & Id) const
	{
		// Get element 
		auto elem = connectivity.grid.getElem(Id);
		
		// Get elements connected to at least one of the vertices of the element
		auto s = set_union(connectivity.node2elem[elem[0]], connectivity.node2elem[elem[1]]);
		for (UInt j = 2; j < NV; j++)
			s = set_union(graphItem(s), connectivity.node2elem[elem[j]]);
					
		// Remove Id
		s.erase(Id);
		
		return vector<UInt>(s.begin(), s.end());
	}
	
	
	//
	// Get geometric info
	//
	
	template<typename SHAPE, MeshType MT>
	Real bmeshInfo<SHAPE,MT>::getIntervalLength(const UInt & Id) const
	{
		// This method is provided only for 1D grids
		#ifdef NDEBUG
		static_assert(NV == 2,
			"getIntervalLength() is provided only for 1D grids.");
		#endif
			
		assert(Id < connectivity.grid.getElemsListSize());
		
		// Get interval length
		auto elem = connectivity.grid.getElem(Id);
		return (connectivity.grid.getNode(elem[0]) - connectivity.grid.getNode(elem[1])).norm2();
	}
	
	
	template<typename SHAPE, MeshType MT>
	Real bmeshInfo<SHAPE,MT>::getTriArea(const UInt & Id) const
	{
		// This method is provided only for triangular grids
		#ifdef NDEBUG
		static_assert(NV == 3, 
			"getTriaArea() is provided only for triangular grids.");
		#endif
			
		assert(Id < connectivity.grid.getElemsListSize());
				
		// Get element vertices
		auto elem = connectivity.grid.getElem(Id);
		auto pA = connectivity.grid.getNode(elem[0]);
		auto pB = connectivity.grid.getNode(elem[1]);
		auto pC = connectivity.grid.getNode(elem[2]);
		
		// Get element area
		return 0.5 * ((pB - pA)^(pC - pA)).norm2();
	}
		
	
	template<typename SHAPE, MeshType MT>
	point3d bmeshInfo<SHAPE,MT>::getNormal(const UInt & Id) const
	{
		// This method is provided only for triangular grids
		#ifdef NDEBUG
		static_assert(NV == 3, 
			"getNormal() is provided only for triangular grids.");
		#endif
			
		assert(Id < connectivity.grid.getElemsListSize());
				
		// Get the element vertices
		auto elem = connectivity.grid.getElem(Id);
		auto pA = connectivity.grid.getNode(elem[0]);
		auto pB = connectivity.grid.getNode(elem[1]);
		auto pC = connectivity.grid.getNode(elem[2]);
				
		// Get element normal
		return ((pB - pA)^(pC - pB)).normalize();
	}
	
	
	template<typename SHAPE, MeshType MT>
	point3d bmeshInfo<SHAPE,MT>::getNorthEastPoint() const
	{
		// Initialize output point
		auto aux = numeric_limits<Real>::lowest();
		point3d NE(aux,aux,aux);
			
		// Loop over all nodes and extract the maximum
		// for each coordinate
		auto nodes = connectivity.grid.getNodes();
		for (auto p : nodes)
		{
			if (p.isActive())
			{
				if (p[0] > NE[0])
					NE[0] = p[0];
				if (p[1] > NE[1])
					NE[1] = p[1];
				if (p[2] > NE[2])
					NE[2] = p[2];
			}
		}
		
		return NE;
	}
	
	
	template<typename SHAPE, MeshType MT>
	point3d bmeshInfo<SHAPE,MT>::getSouthWestPoint() const
	{
		// Initialize output point
		auto aux = numeric_limits<Real>::max();
		point3d SW(aux,aux,aux);
			
		// Loop over all nodes and extract the minimum
		// for each coordinate
		auto nodes = connectivity.grid.getNodes();
		for (auto p : nodes)
		{
			if (p.isActive())
			{
				if (p[0] < SW[0])
					SW[0] = p[0];
				if (p[1] < SW[1])
					SW[1] = p[1];
				if (p[2] < SW[2])
					SW[2] = p[2];
			}
		}
		
		return SW;
	}
	
	
	template<typename SHAPE, MeshType MT>
	pair<point3d,point3d> bmeshInfo<SHAPE,MT>::getBoundingBoxVertices() const
	{
		// Initialize output points
		auto aux = numeric_limits<Real>::max();
		point3d NE(-aux,-aux,-aux);
		point3d SW(aux,aux,aux);
			
		// Loop over all nodes and extract the 
		// maximum and minimum for each coordinate
		auto nodes = connectivity.grid.getNodes();
		for (auto p : nodes)
		{
			if (p.isActive())
			{
				for (UInt i = 0; i < 3; i++)
				{ 
					// Update North-East point
					if (p[i] > NE[i])
						NE[i] = p[i];
										
					// Update South-West point
					if (p[i] < SW[i])
						SW[i] = p[i];
				}
			}
		}
		
		return make_pair(NE,SW);
	}
	
	
	template<typename SHAPE, MeshType MT>
	array<Real,3> bmeshInfo<SHAPE,MT>::getCellSize() const
	{
		Real dx(0.), dy(0.), dz(0.);
			
		// Loop over all edges to extract the maximum
		// length along each coordinate
		auto edges = connectivity.getEdges();
		for (auto edge : edges)
		{
			// Make sure the edge exists
			if (connectivity.grid.isNodeActive(edge[0]) && connectivity.grid.isNodeActive(edge[1]))
			{	
				// Extract end-points of the edge
				auto p = connectivity.grid.getNode(edge[0]); 
				auto q = connectivity.grid.getNode(edge[1]);

				// Update dx
				Real pq_x = abs(p[0] - q[0]);
				if (pq_x > dx)
					dx = pq_x; 

				// Update dy
				Real pq_y = abs(p[1] - q[1]);
				if (pq_y > dy)
					dy = pq_y; 

				// Update dz
				Real pq_z = abs(p[2] - q[2]);
				if (pq_z > dz)
					dz = pq_z; 
			}
		}
		
		return array<Real,3>({{dx,dy,dz}});
	}
	
	
	template<typename SHAPE, MeshType MT>
	point3d bmeshInfo<SHAPE,MT>::getElemBarycenter(const UInt & Id) const
	{
		auto elem = connectivity.grid.getElem(Id);
		point3d p(0.,0.,0.);
		
		// Loop over all vertices
		for (UInt i = 0; i < bmeshInfo<SHAPE,MT>::NV; ++i)
			p = p + connectivity.grid.getNode(elem[i]);
			
		return (p / static_cast<Real>(bmeshInfo<SHAPE,MT>::NV));
	}
	
	
	template<typename SHAPE, MeshType MT>
	point3d bmeshInfo<SHAPE,MT>::getMeshBarycenter() const
	{
		point3d p(0.,0.,0.);
		
		// Loop over all nodes
		for (UInt i = 0; i < connectivity.grid.getNodesListSize(); ++i)
		{
			if (connectivity.grid.getNode(i).isActive())
				p = p + connectivity.grid.getNode(i);
		}
			
		return (p / static_cast<Real>(connectivity.grid.getNumNodes()));
	}
	
	
	//
	// Set geometric features
	//
	
	template<typename SHAPE, MeshType MT>
	void bmeshInfo<SHAPE,MT>::setBoundary(const UInt & Id)
	{
		assert(Id < connectivity.grid.getElemsListSize());
		
		// Extract node-element connections
		auto conn = connectivity.node2elem[Id].getConnected();
		
		// Get the number of different geometric Id's surrounding the node
		set<UInt> geoIds;
		for (auto el : conn)
			geoIds.insert(connectivity.grid.getElem(el).getGeoId());
			
		// Switch the number of different geometric Id's
		auto numGeoIds = geoIds.size();
		if (numGeoIds == 1)	
			connectivity.grid.setBoundary(Id,0);
		else if (numGeoIds == 2)
			connectivity.grid.setBoundary(Id,1);
		else
			connectivity.grid.setBoundary(Id,2);
			
		// To avoid holes, check that the number of elements insisting
		// on each edge sharing the node is equal to two
		// If not, that edge belongs to the boundary, then the node
		// cannot be moved
		auto nodes = connectivity.node2node[Id].getConnected();
		for (auto node : nodes)
		{
			auto onEdge = getElemsOnEdge(Id,node);
			if (onEdge.size() < 2)
			{
				connectivity.grid.setBoundary(Id,2);
				break;
			}
		}
	}
	
	
	template<typename SHAPE, MeshType MT>
	INLINE void bmeshInfo<SHAPE,MT>::setBoundary()
	{
		for (UInt id = 0; id < connectivity.grid.getNodesListSize(); id++)
			this->setBoundary(id);
	}
}

#endif

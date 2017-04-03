/*!	\file	bmeshInfo.hpp
	\brief	Base class performing operations on and getting info from a mesh 
			without modifying its connections. */
			
#ifndef HH_BMESHINFO_HH
#define HH_BMESHINFO_HH

#include <utility>

#include "connect.hpp"

namespace geometry
{
	/*!	This class performs some operations on a mesh (e.g. set the
		geometric Id's) and retrieve some info on it just exploiting
		the geometry and the topology (i.e. the connectivity).
	
		This class is not allowed to modify the connections of a mesh.
		This privilege is exclusively given to doctor and, in some sense,
		the classes computing the edge cost function. 
		Hence the name of the class.
		
		This is a base class working both with and without data distributed 
		over the grid. Specific methods for handling distributed data will 
		be provided by meshInfo - a class inherits bmeshInfo.
		
		\sa meshInfo.hpp, doctor.hpp */
	template<typename SHAPE, MeshType MT = MeshType::GEO>
	class bmeshInfo
	{
		public:
			/*! Number of vertices for each element. */
			static constexpr UInt NV = SHAPE::numVertices;
			
		protected:
			/*!	Class storing the topological informations. 
				This is a friend class. */
			connect<SHAPE,MT> connectivity;
			
		public:
			//
			// Constructors
			//
			
			/*!	Synthetic default constructor. */
			bmeshInfo() = default;
			
			/*!	Constructor.
				\param bg	a (base) grid */
			bmeshInfo(const bmesh<SHAPE> & bg);
						
			/*!	Constructor.
				\param args	arguments to forward to mesh 
				
				\sa mesh.hpp */
			template<typename... Args>
			bmeshInfo(Args... args);
			
			//
			// Access attributes
			//
			
			/*!	Get the mesh.
				\return		the mesh */
			mesh<SHAPE,MT> getMesh() const;
			
			/*!	Get a pointer to the mesh.
				\return		pointer to the mesh */
			mesh<SHAPE,MT> * getPointerToMesh();
			
			/*!	Get a const pointer to the mesh.
				\return		const pointer to the mesh */
			const mesh<SHAPE,MT> * getCPointerToMesh() const;
			
			/*!	Get mesh connectivity.
				\return		the connectivity */
			connect<SHAPE,MT> getConnectivity() const;
			
			/*!	Get a pointer to the mesh connectivity.
				\return 	pointer to the connectivity */
			connect<SHAPE,MT> * getPointerToConnectivity();
			
			/*!	Get a const pointer to the mesh connectivity.
				\return 	const pointer to the connectivity */
			const connect<SHAPE,MT> * getCPointerToConnectivity() const;
			
			/*!	Set the mesh.
				\param g	the new mesh */
			void setMesh(const bmesh<SHAPE> & g);
			
			//
			// Refresh method
			//
			
			/*!	Refresh the mesh and all the connections. */
			void refresh();
						
			//
			// Print methods
			//
			
			/*!	Print the mesh. 
				\param filename	name of the output file */
			virtual void printMesh(const string & filename);
						
			//
			// Get topological info
			//
			
			/*!	Get the nodes shared by the nodes.
				\param id1	first node
				\param id2	second node
				\return		shared nodes */
			vector<UInt> getNodesOnEdge(const UInt & id1, const UInt & id2) const;
			
			/*!	Get nodes involved in an edge collapsing except for the end-points of the edge.
				\param id1	first end-point of the edge
				\param id2	second end-point of the edge
				\return		Id's of involved nodes */ 
			vector<UInt> getNodesInvolvedInEdgeCollapsing(const UInt & id1, const UInt & id2) const;
			
			/*!	Get elements insisting on an edge.
				\param id1	first end-point of the edge
				\param id2	second end-point of the edge
				\return		Id's of the elements sharing both end-points of the edge */
			vector<UInt> getElemsOnEdge(const UInt & id1, const UInt & id2) const;
			
			/*!	Get elements involved in an edge collapsing, included the elements to remove.
				\param id1	first end-point of the edge
				\param id2	second end-point of the edge
				\return		Id's of the involved elements */
			vector<UInt> getElemsInvolvedInEdgeCollapsing(const UInt & id1, const UInt & id2) const;
			
			/*!	Get elements involved in an edge collapsing, except for the elements to remove.
				\param id1	first end-point of the edge
				\param id2	second end-point of the edge
				\return		Id's of the involved elements */
			vector<UInt> getElemsModifiedInEdgeCollapsing(const UInt & id1, const UInt & id2) const;
			
			/*!	Get all triangles connected with at least one node connected with a given node.
				This method may be useful in updating the average quantity of information in DataGeo.
				
				\param Id	Id of the "central" node
				\return		the elements in the extended patch
				
				\sa DataGeo.hpp */
			vector<UInt> getExtendedNodePatch(const UInt & Id) const;
			
			/*!	Get the patch of a triangle, i.e. all triangles sharing at least a node with the triangle.
				Of course, this method is provided only for triangular grids.
				
				\param Id	triangle Id
				\return		Id's of the triangles belonging to the patch */
			vector<UInt> getTriPatch(const UInt & Id) const;
			
			/*!	Get the patch of an element, i.e. all elements sharing at least a node with the element.
				\param Id	element Id
				\return		Id's of the elements belonging to the patch */
			vector<UInt> getElemPatch(const UInt & Id) const;
						
			//
			// Get geometric info
			//
			
			/*!	Get the length of an interval.
				This method is provided only for 1D grids.
				
				\param Id	element Id
				\return		element length */
			Real getIntervalLength(const UInt & Id) const;
			
			/*!	Get the area of a triangle.
				This method is provided only for triangular grids.
				
				\param Id	element Id
				\return		element area */
			Real getTriArea(const UInt & Id) const;
						
			/*!	Get the normal of an element.
				This method is provided only for triangular grids.
				
				\param Id	element Id
				\return		its normal */
			point3d getNormal(const UInt & Id) const;
			
			/*!	Get the North-East vertex of the bounding box 
				surrounding the grid.
				
				\return		the North-East point */
			point3d getNorthEastPoint() const;
			
			/*!	Get the South-West vertex of the bounding box 
				surrounding the grid.
				
				\return		the South-West point */
			point3d getSouthWestPoint() const;
			
			/*!	Get the the bounding box surrounding the grid.
				\return		a pair with the North-East and the
							South-West point of the bounding box */
			pair<point3d,point3d> getBoundingBoxVertices() const;
			
			/*!	Get the size of the cells for structured data search.
				\return		array with cells size */
			array<Real,3> getCellSize() const;
			
			/*!	Get the barycenter of an element.
				\param Id	element Id
				\return		the barycenter */
			point3d getElemBarycenter(const UInt & Id) const;
			
			/*!	Get the barycenter of the mesh.
				\return		the global barycenter */
			point3d getMeshBarycenter() const;
			
			//
			// Set geometric features
			//
			
			/*!	Set the boundary flag of a node.
				\param Id	node Id */
			void setBoundary(const UInt & Id);
			
			/*!	Set the boundary flag for all nodes. */
			void setBoundary();
	};
}

/*!	Include implementations of class members. */
#include "implementation/imp_bmeshInfo.hpp"

#endif

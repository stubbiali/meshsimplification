/*!	\file	meshInfo.hpp
	\brief	Class performing operations on and retreving info from a mesh 
			without modifying its connections. */
			
#ifndef HH_MESHINFO_HH
#define HH_MESHINFO_HH

#include "bmeshInfo.hpp"

namespace geometry
{
	/*!	This class performs some operations on a mesh (e.g. set the
		geometric Id's) and retrieve some info on it just exploiting
		the geometry and the topology (i.e. the connectivity). 	
	
		This class is not allowed to modify the connections of a mesh.
		This privilege is exclusively given to doctor and, in some sense,
		the classes computing the edge cost function. 
		Hence the name of the class.
		
		This class inherits bmeshInfo, hence allowing to handle data
		distributed over the mesh.
		
		\sa bmeshInfo.hpp, doctor.hpp */
	template<typename SHAPE, MeshType MT = MeshType::GEO>
	class meshInfo : public bmeshInfo<SHAPE,MT>
	{
	};
	
	/*!	Partial specialization for grids without distributed data. */
	template<typename SHAPE>
	class meshInfo<SHAPE, MeshType::GEO> : public bmeshInfo<SHAPE, MeshType::GEO>
	{
		public:
			//
			// Constructors
			//
			
			/*!	Synthetic default constructor. */
			meshInfo() = default;
			
			/*!	Constructor.
				\param bg	a (base) grid */
			meshInfo(const bmesh<SHAPE> & bg);
									
			/*!	Constructor.
				\param args	arguments to forward to mesh 
				
				\sa mesh.hpp */
			template<typename... Args>
			meshInfo(Args... args);
	};
	
	/*!	Partial specialization for grids with distributed data. */
	template<typename SHAPE>
	class meshInfo<SHAPE, MeshType::DATA> : public bmeshInfo<SHAPE, MeshType::DATA>
	{
		public:
			//
			// Constructors
			//
			
			/*!	Synthetic default constructor. */
			meshInfo() = default;
			
			/*!	Constructor.
				\param bg	a (base) grid */
			meshInfo(const bmesh<SHAPE> & bg);
			
			/*!	Constructor.
				\param args	arguments to forward to mesh 
				
				\sa mesh.hpp */
			template<typename... Args>
			meshInfo(Args... args);
						
			//
			// Print methods
			//
			
			/*!	Print the mesh with quantity of information for each element. 
				\param filename	name of the output file 
				
				\sa getQuantityOfInformation() */
			virtual void printMesh(const string & filename);
	
			//
			// Get topological info
			//
			
			/*!	Get data involved in an edge collapsing, i.e. belonging to the elements
				involved in the edge collapsing, included the elements to remove.
				Debug mode.
				
				\param id1	first end-point of the edge
				\param id2	second end-point of the edge
				\return		Id's of the involved data */
			vector<UInt> getDataInvolvedInEdgeCollapsing(const UInt & id1, const UInt & id2) const;
			
			/*!	Get data involved in an edge collapsing, i.e. belonging to the elements
				involved in the edge collapsing, included the elements to remove.
				Release mode.
				
				\param invElems	elements involved in the edge collapsing
				\return			Id's of the involved data */	
			vector<UInt> getDataInvolvedInEdgeCollapsing(const vector<UInt> & invElems) const;
			
			/*!	Get data involved in an edge collapsing and 
				whose location may change after the collapsing itself.
				Debug mode.
				
				\param id1	first end-point of the edge
				\param id2	second end-point of the edge
				\return		Id's of the involved data */
			vector<UInt> getDataModifiedInEdgeCollapsing(const UInt & id1, const UInt & id2) const;
			
			/*!	Get data involved in an edge collapsing and 
				whose location may change after the collapsing itself.
				Release mode.
								
				\param invElems	elements involved in the collapsing
				\return			Id's of the involved data */
			vector<UInt> getDataModifiedInEdgeCollapsing(const vector<UInt> & invElems) const;
			
			/*!	Get data involved in an edge collapsing and 
				whose location may change after the collapsing itself.
				Release mode.
								
				\param invElems	elements involved in the collapsing
				\param invData	data involved in the collapsing
				\return			Id's of the involved data */
			vector<UInt> getDataModifiedInEdgeCollapsing(const vector<UInt> & invElems, 
				const vector<UInt> & invData) const;
				
			/*!	Get data associated with the nodes insisting on an edge.
				In an edge collapse, these data points do not need to be moved
				but their data-element connections must be updated.
				
				\param toRemove	elements insisting on the edge, then to remove
				\param toMove	data involved in the collapse which may 
								require a projection
				\return			data involved in the collapse which for sure
								do not require a projection */
			vector<UInt> getDataOnEdge(const vector<UInt> & toRemove,
				const vector<UInt> & toMove) const;
												
			/*!	Get the quantity of information associated with an element.
				See F. Dassi et al., Equation (5).
				
				\param Id	element Id
				\return		quantity of information */
			Real getQuantityOfInformation(const UInt & Id) const;
			
			/*!	Check if an element is not associated any data point.
				\param Id	element Id
				\return		TRUE if no data points are associated to the element,
							FALSE otherwise */
			bool isEmpty(const UInt & Id) const;
			
		protected:
			//
			// Print in different formats
			//
			
			/*! Print in .inp format. 
				\param filename	name of the output file */
			void print_inp(const string & filename) const;				
	};
}

/*!	Include definitions of class members. */
#include "implementation/imp_meshInfo.hpp"

#endif


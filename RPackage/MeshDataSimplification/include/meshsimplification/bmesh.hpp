/*! \file	bmesh.hpp
	\brief	A base class for storing a mesh. */
	
#ifndef HH_BMESH_HH
#define HH_BMESH_HH

#include <vector>
#include <string>
#include <map>

#include "point.hpp"
#include "geoElement.hpp"

namespace geometry
{
	/*! A mesh is stored as a vector of nodes and a vector of elements.
		In both vectors, the Id of a node/element coincides with its position 
		in the vector even after an erase. However, as the class has been designed, 
		one should avoid to erase nodes or elements but make them unactive. 
		Note however that once a node is set as unactive, the vector of elements is 
		not properly updated. This task will be accomplished by the class connect.
		
		Although this class can be instantiated, the user should preferer the class mesh, 
		which inherits from bmesh and also allows for data distributed over the mesh. 
		
		\sa mesh.hpp, connect.hpp */
	template<typename SHAPE>
	class bmesh
	{
		public:
			/*! Number of vertices for each element. */
			static constexpr UInt NV = SHAPE::numVertices;
			
		protected:	
			/*!	Number of nodes and elements. */
			UInt numNodes;
			UInt numElems;
					
			/*! Vector of nodes. */
			vector<point> 				nodes;
			
			/*! Vector of elements. */
			vector<geoElement<SHAPE>> 	elems;
					
		public:
			//
			// Constructors and destructor
			//
			
			/*! (Default) constructor.
				\param nNodes	number of nodes
				\param nElems	number of elements */
			bmesh(const UInt & nNodes = 0, const UInt & nElems = 0);
			
			/*! Constructor.
				\param nds	vector of nodes
				\param els	vector of elements */
			bmesh(const vector<point> & nds, const vector<geoElement<SHAPE>> & els);
			
			/*! Constructor.
				\param filename	name of the file storing the mesh */
			bmesh(const string & filename);
			
			/*!	Constructor specifically designed for the R interface.
				\param nds	#nodes-by-dim Eigen matrix storing the coordinates
							of the nodes
				\param els	#elements-by-NV Eigen matrix storing for each element
							the Id's of its vertices */
			bmesh(const MatrixXd & nds, const MatrixXi & els);
			
			/*! Synthetic copy constructor. 
				\param bm	another mesh */
			bmesh(const bmesh & bm) = default;
			
			/*! Synthetic destructor. */
			virtual ~bmesh() = default;
			
			//
			// Operators
			//
			
			/*!	Copy-assignment operator.
				\param bm	another bmesh
				\return		the current, updated object */
			virtual bmesh<SHAPE> & operator=(const bmesh<SHAPE> & bm);
			
			/*! Output stream operator.
				\param out	output stream
				\param bm	a mesh
				\return		updated output stream */
			template<typename S>
			friend ostream & operator<<(ostream & out, const bmesh<S> & bm);
						
			//
			// Get methods
			//
			
			/*! Get a node.
				\param Id	node Id
				\return		the point */
			point getNode(const UInt & Id) const;
			
			/*!	Get full list of nodes.
				\return		list of nodes */
			vector<point> getNodes() const;
						
			/*! Get an element.
				\param Id	element Id
				\return		the element */
			geoElement<SHAPE> getElem(const UInt & Id) const;
			
			/*!	Get full list of elements.
				\return		list of elements */
			vector<geoElement<SHAPE>> getElems() const;
			
			/*! Get number of nodes.
				\return		number of nodes */
			UInt getNumNodes() const;
			
			/*!	Get size of nodes list, i.e. number of nodes both
				active and inactive.
				\return		size of nodes list */
			UInt getNodesListSize() const;
			
			/*! Get number of elements.
				\return		number of elements */
			UInt getNumElems() const;
			
			/*!	Get size of elements list, i.e. number of elements both
				active and inactive.
				\return		size of elements list */
			UInt getElemsListSize() const;
			
			/*!	Know whether a node is active or not.
				\param Id	node Id
				\return		TRUE if the node is active,
							FALSE otherwise */
			bool isNodeActive(const UInt & Id) const; 
			
			/*!	Know whether an element is active or not.
				\param Id	element Id
				\return		TRUE if the element is active,
							FALSE otherwise */
			bool isElemActive(const UInt & Id) const; 
			
			//
			// Set methods
			//
						
			/*! Set the coordinates and the boundary flag of a node. 
				Note that node Id will not change.
				\param Id	node Id
				\param p	the new point */
			void setNode(const UInt & Id, const point & p);
			
			/*! Set geometric Id and vertices Id's of an element. 
				Note that the element Id will not change.
				\param Id	element Id
				\param g	the new element */
			void setElem(const UInt & Id, const geoElement<SHAPE> & g);
			
			/*! Resize the vector of nodes.
				\param numNodes	new number of nodes */
			void resizeNodes(const UInt & nNodes);
			
			/*! Make a reserve for the vector of nodes.
				\param numNodes	new number of nodes */
			void reserveNodes(const UInt & nNodes);
			
			/*! Resize the vector of elements.
				\param numElems	new number of elements */
			void resizeElems(const UInt & nElems);
			
			/*! Make a reserve for the vector of elements.
				\param numElems new number of elements */
			void reserveElems(const UInt & nElems);
						
			/*! Set boundary flag for a node.
				\param Id		node Id
				\param bound	boundary flag */
			void setBoundary(const UInt & Id, const UInt & bound);
			
			/*!	Set a node active.
				\param Id	node Id */
			void setNodeActive(const UInt & Id);
			
			/*! Set a node inactive.
				\param Id	node Id */
			void setNodeInactive(const UInt & Id);
			
			/*!	Set index for structured data search.
				\param Id	element Id
				\param idx	element index */
			void setIdx(const UInt & Id, const UInt & idx);
			
			/*! Set an element active.
				\param Id	element Id */
			void setElemActive(const UInt & Id); 
			
			/*! Set an element inactive.
				\param Id	element Id */
			void setElemInactive(const UInt & Id); 
						
			//
			// Insert and replace methods
			//
			
			/*! Insert a new node at the back of the list.
				\param coor		array with the coordinates
				\param bound	boundary flag */	
			void insertNode(const array<Real,3> & coor, const UInt & bound);
						
			/*! Insert a new element at the back of the list.
				\param geoId	geometric Id
				\param vert		array with vertices Id's */
			void insertElem(const array<UInt,NV> & vert, const UInt & geoId = 0);
						
			/*! Replace the vertex of an element.
				\param elemId	element Id
				\param oldId	old vertex Id
				\param newId	new vertex Id */
			void replaceVertex(const UInt & elemId, const UInt & oldId, const UInt & newId); 
			
			//
			// Erase and clear methods
			//
			
			/*! Remove a node.
				\param Id	Id of node to remove */
			void eraseNode(const UInt & Id);
			
			/*! Remove an element. 
				\param Id	Id of element to remove */
			void eraseElem(const UInt & Id);
			
			/*! Clear the lists. */			
			void clear();
			
			//
			// Refresh methods
			//
			
			/*!	Remove inactive nodes and elements from the lists and update Id's.
				\return 	old-to-new map for nodes Id's
				\return		old-to-new map for elements Id's */
			pair<map<UInt,UInt>, map<UInt,UInt>> refresh();
			
			//
			// Print
			//
						
			/*! Print to file. The allowed extensions are:
				<ol>
				<li> .inp
				<li> .txt
				<\ol>
								
				\param filename	name of the output file */
			void print(const string & filename); 
			
		protected:
			//
			// Update Id's
			//
			
			/*! Update nodes Id's so to make them coincide with the position in the vector. */
			void setUpNodesIds();
			
			/*! Update elements Id's so to make them coincide with the position in the vector. */
			void setUpElemsIds();
						
			//
			// Read mesh from file
			//
			
			/*! Read mesh from a .inp file.
				\param filename	name of the input file */
			void read_inp(const string & filename);
			
			/*! Read mesh from a .vtk file.
				\param filename	name of the input file */
			void read_vtk(const string & filename);
			
			/*! Read mesh from a .obj file.
				\param filename	name of the input file */
			void read_obj(const string & filename);
			
			//
			// Print in different format
			//
			
			/*! Print in .inp or .txt format. 
				\param filename	name of the output file */
			virtual void print_inp(const string & filename) const;
	};
}

/*! Include implementations. */
#include "implementation/imp_bmesh.hpp"

#endif

/*!	\file	bconnect.hpp
	\brief	A class creating the connections for a mesh. */
	
#ifndef HH_BCONNECT_HH
#define HH_BCONNECT_HH

#include <memory>
#include <unordered_set>

#include "hash.hpp"
#include "graphItem.hpp"
#include "mesh.hpp"

namespace geometry
{
	/*!	This is a base class creating the following connections for
		a standard mesh (i.e. without distributed data):
		<ol>
		<li> node-node;
		<li> node-element;
		<\ol>
		Moreover, the set of edges is created.
		For grids with distributed data, we delegate to the derived class
		connect the creation of the following two connections:
		<ol>
		<li> data-element;
		<li> element-data.
		<\ol> 
		When the class is required to modify some connections,
		it supposes the mesh has been already accordingly modified.
		This class does not directly modify the mesh in any way,
		other classes (e.g. simplification) will be in charge of this.
		To make the connections updated with the mesh, call refresh().
		
		\sa connect.hpp, simplification.hpp */
		
	template<typename SHAPE, MeshType MT>
	class bconnect
	{
		public:
			/*! Number of vertices for each element. */
			static constexpr UInt NV = SHAPE::numVertices;
			
			/*! Number of edges for each element. */
			static constexpr UInt NE = SHAPE::numEdges;
			
			/*! Number of vertices per edge times number of edges. */
			static constexpr UInt N = 
				SHAPE::numVerticesPerEdge * SHAPE::numEdges;
			
		protected:
			/*! The mesh. */
			mesh<SHAPE,MT> grid;
			
			/*! Set of edges. */
			unordered_set<geoElement<Line>> edges;
			
			/*! Node-node connections. */
			vector<graphItem> node2node;
			
			/*! Node-element connections. */
			vector<graphItem> node2elem;
						
		public:
			//
			// Constructor and destructor
			//
			
			/*!	Synthetic default constructor. */
			bconnect() = default;
			
			/*!	Constructor.
				\param bg	a (base) grid */
			bconnect(const bmesh<SHAPE> & bg);
						
			/*! Constructor.
				\param args	arguments to forward to mesh 
				
				\sa mesh.hpp */
			template<typename... Args>
			bconnect(Args... args);
			
			/*! Synthetic destructor. */
			virtual ~bconnect() = default;
			
			//
			// Initialize and clear connections
			//
			
			/*! Initialize node-node connections. It also fill the set of edges. */
			void buildNode2Node();
			
			/*! Initialize node-element connections. */
			void buildNode2Elem();
						
			/*! Re-build all connections. */
			virtual void refresh();
			
			/*! Clear all connections and the set of edges. */
			virtual void clear();
			
			//
			// Modify connections 
			//
			// These methods are particularly designed for applying
			// a possibly temporary edge collapse
			
			/*!	Replace a node in element-node connections.
				Debug mode: the method finds itself the elements connected
				to the node to substitute but not the new one.
				Ideally, these elements are the ones involved in an edge collapsing.
				
				\param oldId	the Id to substitute
				\param newId	the new Id */
			void replaceNodeInElem2Node(const UInt & oldId, const UInt & newId);
			
			/*!	Replace a node in element-node connections.
				Release mode: the elements connected to the node to substitute
				but not to the new one.
				Ideally, these elements are the ones involved in an edge collapsing.
				
				\param oldId	the Id to substitute
				\param newId	the new Id
				\param elems	elements connected to oldId (but not to newId) */
			void replaceNodeInElem2Node(const UInt & oldId, const UInt & newId,
				const vector<UInt> & toKeep);
			
			/*! Replace a node in node-node connections.
				Debug mode: the method finds itself the nodes connected to the old Id.
				
				\param oldId	the Id to substitute
				\param newId	the new Id 
				\return			old node-node connections for newId;
								these may be useful for the method undoEdgeCollapse() 
								
				\sa undoEdgeCollapse() */
			vector<UInt> replaceNodeInNode2Node(const UInt & oldId, const UInt & newId);
			
			/*! Replace a node in node-node connections.
				Release mode: the nodes connected to the old one are already provided.
				
				\param oldId	the Id to substitute
				\param newId	the new Id 
				\param involved	nodes involved in the changes 
				\return			old node-node connections for newId;
								these may be useful for the method undoEdgeCollapse() 
								
				\sa undoEdgeCollapse() */
			vector<UInt> replaceNodeInNode2Node(const UInt & oldId, const UInt & newId, const vector<UInt> & involved);
			
			/*! Replace a node in node-element connections.
				This method simply moves all connected elements 
				from the old Id to the new one.
				
				\param oldId	the Id to substitute
				\param newId	the new Id 
				\return			old node-element connections for newId;
								these may be useful for the methods undoEdgeCollapse() 
								
				\sa undoEdgeCollapse() */
			vector<UInt> replaceNodeInNode2Elem(const UInt & oldId, const UInt & newId);
			
			/*! Remove elements from node-element connections.
				Debug mode: the method find itself the vertices of the elements to remove. 
				
				\param toRemove	vector with the Id's to remove */
			void eraseElemsInNode2Elem(const vector<UInt> & toRemove);
			
			/*! Remove two elements from node-element connections after an edge contraction.
				Release mode: the nodes to update are already provided.
				
				\param toRemove	Id's of the elements to remove 
				\param newId	Id of the node resulting from the collapse
				\param involved	Id's of nodes involved in the contraction */
			void eraseElemsInNode2Elem(const vector<UInt> & toRemove, const UInt & newId, 
				const vector<UInt> & involved);
			
			/*! Update connections after an edge contraction (debug mode).
				Note that the set of edges is not updated. 
				This method shows the order the various methods of the class should be used.
				
				\param oldId	Id of the node to remove
				\param newId	Id of the node to keep
				\param toRemove	Id's of the elements to remove 
				\return			old node-node and node-element connections for newId */
			pair<vector<UInt>, vector<UInt>> applyEdgeCollapse(const UInt & oldId, const UInt & newId, 
				const vector<UInt> & toRemove);
			
			/*! Update connections after an edge contraction (release mode).
				Note that the set of edges is not updated. 
				This method shows the order the various methods of the class should be used.
				
				\param oldId	Id of the node to remove
				\param newId	Id of the node to keep
				\param toRemove	Id's of the elements to remove 
				\param toKeep	Id's of the elements involved in the collapsing but to keep 
				\return			old node-node and node-element connections for newId */
			pair<vector<UInt>, vector<UInt>> applyEdgeCollapse(const UInt & oldId, const UInt & newId, 
				const vector<UInt> & toRemove, const vector<UInt> & toKeep);
			
			/*! Update connections after an edge contraction (release mode).
				Note that the set of edges is not updated. 
				This method shows the order the various methods of the class should be used.
				
				\param oldId	Id of the node to remove
				\param newId	Id of the node to keep
				\param toRemove	Id's of the elements to remove 
				\param toKeep	Id's of the elements involved in the collapsing but to keep
				\param involved	Id's of the nodes involved in the contraction 
				\return			old node-node and node-element connections for newId */
			pair<vector<UInt>, vector<UInt>> applyEdgeCollapse(const UInt & oldId, const UInt & newId, 
				const vector<UInt> & toRemove, const vector<UInt> & toKeep, 
				const vector<UInt> & involved);
				
			//
			// Methods to restore connections to the status 
			// prior to a temporary edge collapse
			//
			
			/*!	Restore node-node connections to the status 
				prior to an edge collapse.
			
				\param oldId				Id of the node previously removed
				\param newId				Id of the node temporarily replacing oldId
				\param newId_oldNode2Node	old node-node connections for newId;
											these may be given by replaceNodeInNode2Node()
											
				\sa replaceNodeInNode2Node() */
			void restoreNode2Node(const UInt & oldId, const UInt & newId,
				const vector<UInt> & newId_oldNode2Node);
			
			/*!	Restore node-element connections to the status 
				prior to an edge collapse (debug mode).
				
				\param oldId				Id of the node previously removed
				\param newId				Id of the node temporarily replacing oldId
				\param newId_oldNode2Elem	old node-element connections for newId;
											these may be given by replaceNodeInNode2Elem()
				
				\sa replaceNodeInNode2Elem() */
			void restoreNode2Elem(const UInt & oldId, const UInt & newId,
				const vector<UInt> & newId_oldNode2Elem);
				
			/*!	Restore node-element connections to the status 
				prior to an edge collapse (release mode).
				
				\param oldId				Id of the node previously removed
				\param newId				Id of the node temporarily replacing oldId
				\param newId_oldNode2Elem	old node-element connections for newId;
											these may be given by replaceNodeInNode2Elem()
				\param onEdge				Id's of elements insisting on the edge
				
				\sa replaceNodeInNode2Elem() */
			void restoreNode2Elem(const UInt & oldId, const UInt & newId,
				const vector<UInt> & newId_oldNode2Elem, const vector<UInt> & onEdge);
			
			/*!	Restore element-node connections to the status 
				prior to an edge collapse (debug mode).
				\param oldId	Id of the node previously removed
				\param newId	Id of the node temporarily replacing oldId */
			void restoreElem2Node(const UInt & oldId, const UInt & newId);
			
			/*!	Restore element-node connections to the status 
				prior to an edge collapse (release mode).
				\param oldId		Id of the node previously removed
				\param newId		Id of the node temporarily replacing oldId 
				\param onEdge		Id's of the elements insisting on the edge */
			void restoreElem2Node(const UInt & oldId, const UInt & newId,
				const vector<UInt> & onEdge);
				
			/*!	Restore connections to the status prior to an edge collapse
				(debug mode).
				This method shows the order the previous methods should be called.
				
				\param oldId				Id of the node previously removed
				\param newId				Id of the node temporarily replacing oldId
				\param newId_oldNode2Node	old node-node connections for newId;
											these may be given by replaceNodeInNode2Node()
				\param newId_oldNode2Elem	old node-element connections for newId;
											these may be given by replaceNodeInNode2Elem()	
											
				\sa replaceNodeInElem2Node(), replaceNodeInNode2Node(), replaceNodeInNode2Elem() */
			void undoEdgeCollapse(const UInt & oldId, const UInt & newId,
				const vector<UInt> & newId_oldNode2Node, const vector<UInt> & newId_oldNode2Elem);
				
			/*!	Restore connections to the status prior to an edge collapse
				(release mode).
				This method shows the order the previous methods should be called.
				
				\param oldId				Id of the node previously removed
				\param newId				Id of the node temporarily replacing oldId
				\param newId_oldNode2Node	old node-node connections for newId;
											these may be given by replaceNodeInNode2Node()
				\param newId_oldNode2Elem	old node-element connections for newId;
											these may be given by replaceNodeInNode2Elem()
				\param onEdge				Id's of the elements insisting on the edge 
											
				\sa replaceNodeInElem2Node(), replaceNodeInNode2Node(), replaceNodeInNode2Elem() */
			void undoEdgeCollapse(const UInt & oldId, const UInt & newId,
				const vector<UInt> & newId_oldNode2Node, const vector<UInt> & newId_oldNode2Elem,
				const vector<UInt> & onEdge);
									
			//
			// Get methods
			//
			
			/*!	Get the mesh. 
				\return 	the mesh */
			mesh<SHAPE,MT> getMesh() const;
			
			/*! Get pointer to the mesh.
				\return		pointer the mesh */
			mesh<SHAPE,MT> * getPointerToMesh();
			
			/*!	Get number of edges.
				\return		number of edges */
			UInt getNumEdges() const;
			
			/*! Get edges of the mesh.
				\return		vector of edges */
			// Note: we return a (possible huge) vector by value for Return Value Optimization (RVO).
			// Reference: https://web.archive.org/web/20130930101140/http://cpp-next.com/archive/2009/08/want-speed-pass-by-value
			vector<geoElement<Line>> getEdges() const;
						
			/*! Get the node-node connections for a node.
				\param Id	node Id
				\return		the connections */
			graphItem getNode2Node(const UInt & Id) const;
			
			/*! Get node-node connections for all nodes.
				\return		vector of connections */			
			vector<graphItem> getNode2Node() const;
			
			/*! Get the node-element connections for a node.
				\param Id	node Id
				\return		the connections */
			graphItem getNode2Elem(const UInt & Id) const;
			
			/*! Get node-element connections for all nodes.
				\return		vector of connections */
			vector<graphItem> getNode2Elem() const;
						
			//
			// Set methods
			//
			
			/*! Set mesh. After that, the connections are (re-)built.
				\param g	the new mesh */
			void setMesh(const bmesh<SHAPE> & g);
	};
}

/*! Include implementations of class members. */
#include "implementation/imp_bconnect.hpp"

#endif

/*!	\file	simplification.hpp
	\brief	Class performing a mesh simplification by iterative edge collapsing. */

#ifndef HH_SIMPLIFICATION_HH
#define HH_SIMPLIFICATION_HH

#include "utility.hpp"
#include "bmeshOperation.hpp"
#include "bcost.hpp"
#include "OnlyGeo.hpp"
#include "DataGeo.hpp"
#include "collapsingEdge.hpp"
#include "structuredData.hpp"
#include "intersection.hpp"

namespace geometry
{
    /*!	Class for the simplification process on the mesh elements by iterative edge collapse.
		The selection of the edge to contract is done with respect to a cost functional
		which measures the loss of information for the analysis. In case of a meshType MT::GEO,
		the loss of information is just geometrical, otherwise for meshType MT::DATA the cost
		functional takes into account the loss of statistical information as well.
		At each iteration the edge with minimum cost is collapsed into a predefined point.
		The edges are organized in a set in ascending order with respect to their costs called
		collapsingSet which is used to extract the edge to contract. In case the edge has to be
		searched for its ending points, refer to the unordered set cInfoList contained in the
		cost class object.
		The point for the collapse is chosen among the two ending points of the selected edge,
		their middle point and the so-called optimal point. The latter derives from an explicit
		analytical formula based on proposition 5.2.2 from the thesis essay "Advanced Techniques for the
		Generation and the Adaptation of Complex Surface Meshes" by F. Dassi.
		The selection of the collapsing point depends on several controls. First of all, the control
		in the cost class takes into account whether the edge or an endpoint belongs to the border.
		In addition, the selected point has to pass the following geometric controls:
			- the collapse does not invert triangles
			- the contraction has not to bring to mesh self-intersections
			- no degenerated triangles are generated
		In case the MeshType is of type DATA, there are the further data controls.
			- the fixed element is not affected by the edge collapse
			- no empty triangle are created after the data projection
		Each edge collapse is implemented by setting the id of the collapse point to the id of an
		end point (the one with minor id) and setting to inactive the flag of the second end point.
		The elements which insist on the edge are removed from the mesh.
		Subsequently, the costs of the involved edges are recomputed and the connections are
		locally updated. In case of mesh DATA the involved data points are projected.
		The class presents three templates:
			1. SHAPE, geometrical shape of the elements of the mesh
			2. MeshType, GEO or DATA, whether the mesh contains only mesh points or mesh and data points
			3. CostClass, OnlyGeo or GeoData, wheter the cost used is based only on the geometrical information
			   or also on the statical
		The critical situation corresponds to a CostType::GeoData in combination with a MeshType::GEO. This
		conflicting situation is handled at compile time in the constructor checking the templates coherence
		with type_traits.
		The class contains the following attributes:
			\param gridOperation, object of conditional class depending on the MeshType:
				- if MT::GEO, it corresponds to a meshInfo object
				- if MT::DATA, it is a projection object to handle distrubuted data
			\param costObj, cost class of CostClass
			\param collapsingSet, set of collapsingEdge's ordered by cost in ascending order
			\param structData, structured data necessary to support the intersection control
			\param intersec, interesection object for the related control
			\param dontTouch, boolean to indicate if the fixed element is used
			\param dontTouchId, id of the fixed element 
			
		\sa OnlyGeo.hpp, DataGeo.hpp */
    template<typename SHAPE, MeshType MT, typename CostClass> 
    class simplification
    {
    };

    /*! Specialization for triangular meshes. */
    template<MeshType MT, typename CostClass>
    class simplification<Triangle, MT, CostClass>
	{
		private:
			/*! MeshOperation object.  */
			bmeshOperation<Triangle,MT>	gridOperation;

			/*! CostClass object for the cost computing of the edges. */
			CostClass					costObj;

			/*! Set of edges ordered by cost (ascending order). */
			set<collapsingEdge>			collapsingSet;

			/*! Object for the bounding boxes structure.  */
			structuredData<Triangle>	structData;
			
			/*! Object for the control of triangle intersections. */
			intersection<Triangle>		intrs;

			/*! Fixed element to not touch.
				Boolean to indicate if it used or not. */
			bool						dontTouch;
			
			/*! Id of the fixed element. */
			UInt	  					dontTouchId;
			
		public:
			//
			// Constructors
			//
						  
			/*!	Constructor.
				\param file	path to the file storing the mesh
								
				\sa bcost, OnlyGeo, DataGeo */
			simplification(const string & file);
			
			/*!	Constructor, provided only for grids with associated data.
				\param file	path to the file storing the mesh
				\param wgeo	weight for geometric cost function
				\param wdis	weight for data displacement cost function
				\param wequ	weight for data equidistribution cost function	
							
				\sa bcost, DataGeo */
			simplification(const string & file, const Real & wgeo,
				const Real & wdis, const Real & wequ);
			
			/*!	Constructor, provided only for grids with dassociated data.
				\param file	path to the file storing the mesh
				\param val	data observations
				\param wgeo	weight for geometric cost function
				\param wdis	weight for data displacement cost function
				\param wequ	weight for data equidistribution cost function
								
				\sa bcost, DataGeo */
			simplification(const string & file, const vector<Real> & val, 
				const Real & wgeo = 1./3, const Real & wdis = 1./3, const Real & wequ = 1./3);
									
			/*!	Constructor specifically designed for the R interface. 
				In case of grids with distributed data, the data locations are supposed 
				to coincide with the grid nodes and the observations are set to zero.
				
				\param nds	#nodes-by-3 Eigen matrix storing the coordinates
							of the nodes
				\param els	#elements-by-3 Eigen matrix storing for each element
							the Id's of its vertices 
												
				\sa bcost, OnlyGeo, DataGeo */
			simplification(const MatrixXd & nds, const MatrixXi & els);
			
			/*!	Constructor specifically designed for the R interface and 
				provided only for grids with associated data. 
				The data locations are supposed to coincide with the grid 
				nodes and the observations are set to zero.
				
				\param nds	#nodes-by-3 Eigen matrix storing the coordinates
							of the nodes
				\param els	#elements-by-3 Eigen matrix storing for each element
							the Id's of its vertices 
				\param wgeo	weight for geometric cost function
				\param wdis	weight for data displacement cost function
				\param wequ	weight for data equidistribution cost function
												
				\sa bcost, DataGeo */
			simplification(const MatrixXd & nds, const MatrixXi & els,
				const Real & wgeo, const Real & wdis, const Real & wequ);
						
			/*!	Constructor specifically designed for the R interface and
				provided only for grids with associated data.
				The data locations are specified by the user, while the 
				observations are all set to zero.
								
				\param nds	#nodes-by-3 Eigen matrix storing the coordinates
							of the nodes
				\param els	#elements-by-3 Eigen matrix storing for each element
							the Id's of its vertices 
				\param loc	#data-by-3 Eigen matrix storing the coordinates of
							data locations
				\param wgeo	weight for geometric cost function
				\param wdis	weight for data displacement cost function
				\param wequ	weight for data equidistribution cost function
								
				\sa bcost, DataGeo */
			simplification(const MatrixXd & nds, const MatrixXi & els, const MatrixXd & loc,
				const Real & wgeo = 1./3, const Real & wdis = 1./3, const Real & wequ = 1./3);
				
			/*!	Constructor specifically designed for the R interface and
				provided only for grids with associated data.
				Both data locations and values are specified by the user.
								
				\param nds	#nodes-by-3 Eigen matrix storing the coordinates
							of the nodes
				\param els	#elements-by-3 Eigen matrix storing for each element
							the Id's of its vertices 
				\param loc	#data-by-3 Eigen matrix storing the coordinates of
							data locations
				\param val	#data-by-1 Eigen array with data observations 
				\param wgeo	weight for geometric cost function
				\param wdis	weight for data displacement cost function
				\param wequ	weight for data equidistribution cost function 
								
				\sa bcost, DataGeo */
			simplification(const MatrixXd & nds, const MatrixXi & els, 
				const MatrixXd & loc, const VectorXd & val, 
				const Real & wgeo = 1./3, const Real & wdis = 1./3, const Real & wequ = 1./3);			
						
			//
			// Initialization and refreshing methods
			//
			
			/*! Method that builds the set of CollapsingEdge ordered by cost and
				the unordered set of cInfoList in the cost class.
				The method uses the edge list from the connections and adds the 
				cost information. */
			void setupCollapsingSet();
			
			/*!	Method re-building collapsingSet, i.e. re-computing all the costs. */
			void rebuildCollapsingSet();
			
			/*!	Method refreshing
				<ol>
				<li> the mesh
				<li> the connectivities
				<li> the structured data
				<li> the set of collapsingEdge's
				<li> the set of collapseInfo's
				<\ol> */
			void refresh();
			
			//
			// Get methods
			//
			
			/*!	Get const pointer to mesh.
				\out	const pointer to the mesh */
			const mesh<Triangle,MT> * getCPointerToMesh() const;
			
			/*!	Get const pointer to connectivity.
				\out	const pointer to the connectivity */
			const connect<Triangle,MT> * getCPointerToConnectivity() const;
			
			/*! Get const pointer to mesh operator.
				\out 	const pointer to mesh operator */
			const bmeshOperation<Triangle,MT> * getCPointerToMeshOperator() const;
									
			//
			// Set methods
			//

			/*! Method which changes the pointer to the mesh.
				\param grid pointer to the new mesh */
			void setGrid(const mesh<Triangle,MT> & grid);

		  	//
		  	// Compute cost and apply collapse
		  	//

			/*!	Method which takes the cost data for the contraction of the edge.
				The function operates by step:
				- extracts from the CostClass object the list of possible collapse points
				- controls the validity of the points
				- takes from the CostClass object the minimum cost value
				- possibly insert the collapse information to the lists of collapseInfo
				  and collapsingEdge objects
								
				\param id1	Id of the first end-point of the edge
				\param id2	Id of the second end-point of the edge */
			void getCost(const UInt & id1, const UInt & id2);
						
			/*!	Method which takes the cost data for the contraction of the edge.
				The "fast" version of the methods of CostClass are employed.
				The function operates by step:
				- extracts from the CostClass object the list of possible collapse points
				- controls the validity of the points
				- takes from the CostClass object the minimum cost value
				- possibly insert the collapse information to the lists of collapseInfo
				  and collapsingEdge objects
								
				\param id1	Id of the first end-point of the edge
				\param id2	Id of the second end-point of the edge */
			void getCost_f(const UInt & id1, const UInt & id2);
						
			/*! Method which updates:
				<ol>
				<li> the mesh
				<li> the connectivities
				<li> the structured data
				<li> the set of collapsingEdge's
				<li> the set of collapseInfo's
				<\ol>
				after each contraction.
				In case of grids with distributed data, there is the further update 
				of the distribution of the data points.
			
				\param id1		Id of the first end-point of the edge
				\param id2		Id of the second end-point of the edge
				\param cPoint	collapsing point */
			void update(const UInt & id1, const UInt & id2, const point3d & cPoint);
			
			/*! Method which updates:
				<ol>
				<li> the mesh
				<li> the connectivities
				<li> the structured data
				<li> the set of collapsingEdge's
				<li> the set of collapseInfo's
				<\ol>
				for a set of contractions.
				In case of grids with distributed data, there is the further update 
				of the distribution of the data points.
			
				\param id1		Id's of the first end-points of the edges
				\param id2		Id's of the second end-points of the edges
				\param cPoint	collapsing points */
			void update(const vector<UInt> & id1, const vector<UInt> & id2, const vector<point3d> & cPoint);	  

			//
			// Methods for handling the fixed element
			//

			/*! Enable fixed element. */
			void enableDontTouch();

			/*!	Disable fixed element. */
			void disableDontTouch();

			/*!	Check if fixed element is enable or disable.
				\return		TRUE if fixed element enable,
							FALSE otherwise */
			bool getDontTouch() const;

			/*! Set the fixed element.
				\param Id	Id of the element not to touch */
			void setDontTouchId(const UInt & Id);
			
			/*!	Get Id of fixed element.
				\return		Id of the element not to touch */
			UInt getDontTouchId() const;
			
			/*! Method which automatically finds the element to fix. 
				This is the element whose barycenter is closest to
				the global barycenter of the mesh. */
			void findDontTouchId();
			
			//
			// Methods which make the simplification
			//

			/*! Method which iteratively contracts the edge with minimum cost until 
				reaching a maximum amount of nodes.
				Once the procedure is done, the mesh is possibly print to file.
				
				\param numNodesMax		maximum number of nodes
				\param enableDontTouch	TRUE if one element must be fixed,
										FALSE otherwise
				\param file				path to output file; if empty, nothing is printed */
			void simplificate(const UInt & numNodesMax, const bool & enableDontTouch,
				const string & file = "");
												
		private:
			/*!	Initialize the class, i.e. build collapsingSet and find the element
				possibly to preserve throughout the simplification process. 
				This method is just call in the constructor. */
			void initialize();
	};
}

/*!	Include implementations of class members. */
#include "implementation/imp_simplification.hpp"

/*!	Include implementations of inlined members. */
#ifdef INLINED
#include "inline/inline_simplification.hpp"
#endif

#endif 

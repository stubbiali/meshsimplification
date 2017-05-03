/*!	\file	RcppSimplification.hpp
	\brief	An R wrapper for the class simplification of the meshsimplification library. */
	
#ifndef HH_RCPPSIMPLIFICATION_HH
#define HH_RCPPSIMPLIFICATION_HH
	
#include <Rcpp.h>
#include <RcppEigen.h>
#include "simplification.hpp"

using namespace Rcpp;
using namespace Eigen;
using namespace geometry;

/*!	Class RcppSimplification. This is an R wrapper for the template class 
	simplification<Triangle, MeshType::DATAGEO, DataGeo> of the
	meshsimplification library. Exploiting the Rcpp API, through
	this class the main functionalities of the class simplification are made 
	available at the R level. In particular, RcppSimplification is exposed to R
	by enclosing it in a module, then loadable directly from R. 
	RcppSimplification actually acts as a proxy. It stores an instance of the 
	aforementioned class simplification. Then each method simply forwards its 
	own arguments to the correspondent method provided by simplification; 
	if needed, the arguments are first converted from Rcpp to STL classes,
	to comply with the user interface provided by simplification. 
	Note that all computing-intensive calculations are performed by the
	meshsimplification library, i.e. at the C++ level, thus ensuring
	high performance. */
class RcppSimplification
{
	private:
		/*! An object of class simplification. */
		simplification<Triangle, MeshType::DATA, DataGeo> simplifier;
		
	public:		
		//
		// Constructors
		//
		
		/*!	Constructor.
			\param file		string storing path to input mesh
			\param wgeom	weight for geometric cost
			\param wdisp	weight for displacement cost
			\param wequi 	weight for equidistribution cost */
		RcppSimplification(const CharacterVector & file, 
			const double & wgeom, const double & wdisp, const double & wequi);
						
		/*!	Constructor.
			\param nds 		#nodes-by-3 NumericMatrix storing the nodes
			\param els 		#elements-by-3 IntegerMatrix storing the elements
			\param wgeom	weight for geometric cost
			\param wdisp	weight for displacement cost
			\param wequi 	weight for equidistribution cost*/
		RcppSimplification(const NumericMatrix & nds, const IntegerMatrix & els,
			const double & wgeom, const double & wdisp, const double & wequi);
			
		/*!	Constructor.
			\param nds 		#nodes-by-3 NumericMatrix storing the nodes
			\param els 		#elements-by-3 IntegerMatrix storing the elements
			\param loc		#data-by-3 NumericMatrix storing data locations 
			\param wgeom	weight for geometric cost
			\param wdisp	weight for displacement cost
			\param wequi 	weight for equidistribution cost*/
		RcppSimplification(const NumericMatrix & nds, const IntegerMatrix & els,
			const NumericMatrix & loc,
			const double & wgeom, const double & wdisp, const double & wequi);
								
		/*!	Constructor.
			\param nds 		#nodes-by-3 NumericMatrix storing the nodes
			\param els 		#elements-by-3 IntegerMatrix storing the elements
			\param loc		#data-by-3 NumericMatrix storing data locations
			\param val		#data-by-1 NumericVector storing data observations 
			\param wgeom	weight for geometric cost
			\param wdisp	weight for displacement cost
			\param wequi 	weight for equidistribution cost*/
		RcppSimplification(const NumericMatrix & nds, const IntegerMatrix & els,
			const NumericMatrix & loc, const NumericVector & val,
			const double & wgeom, const double & wdisp, const double & wequi);
			
		//
		// Get methods
		//
		
		/*!	Get number of nodes.
			\out 	Number of nodes */
		int getNumNodes() const;
			
		/*!	Get the list of nodes.
			\out 	#nodes-by-3 NumericMatrix storing the nodes */
		NumericMatrix getNodes() const;
		
		/*!	Get the list of edges.
			\out	#edges-by-2 IntegerMatrix where the i-th row stores
					the Id's of the end-points of the i-th edge */
		IntegerMatrix getEdges() const; 
		
		/*!	Get coordinates of end-points for each edge. */
		NumericMatrix getEndPoints() const;
		
		/*!	Get number of elements.
			\out 	Number of elements */
		int getNumElems() const;
		
		/*!	Get the list of elements, i.e. for each triangle the Id's
			of its vertices.
			
			\out	#elems-by-3 IntegerMatrix storing the elements */
		IntegerMatrix getElems() const;
		
		NumericMatrix getElemsVertices() const;
		
		/*!	Get number of data points.
			\out 	Number of data */
		int getNumData() const;
		
		/*!	Get the list of data locations.
			\out 	#data-by-3 NumericMatrix storing data locations */
		NumericMatrix getDataLocations() const;
		
		/*!	Get the list of observations.
			\out	#data-by-1 NumericVector storing data observations */
		NumericVector getObservations() const;
				
		/*!	Get the quantity of information for each element.
			\out 	vector with the quantity of information for each triangle */
		NumericVector getQuantityOfInformation() const;
		
		/*!	Given an edge, returns the triangles sharing it.
			\param id1	Id of the first end-point
			\param id2	Id of the second end-point
			\out  		IntegerVector with the Id's of the required triangles */
		IntegerVector getElemsOnEdge(const int & id1, const int & id2) const;
		
		/*!	Build the quadratic Finite Elements mesh correspondent to the
			linear Finite Elements grid stored by the C++ simplification class.
			In the new list of elements, vertices and nodes are ordered as described at
			https://www.cs.cmu.edu/~quake/triangle.highorder.html .
			
			\out	A List with:
					- nodes: #nodes-by-3 NumericMatrix storing the nodes (both vertices and not)
					- triangles: #elements-by-6 IntegerMatrix storing the triangles */ 
		List getQuadraticFEMesh() const;
		
		//
		// Run the simplification
		//
			
		/*!	Run the simplification process.
			\param numNodesMax	final number of nodes
			\param file	 		path to file which will store the final mesh */
		void simplify(const int & numNodesMax, const CharacterVector & file);
};

/*!	Auxiliary function, building a mesh with linear elements from a mesh
	with quadratic elements.
	
	\param nodes	#nodes-by-3 NumericMatrix storing the nodes
	\param elems	#elements-by-6 IntegerMatrix storing the elements
	\out			list holding for the linear elements mesh:
					- nodes: #vertices-by-3 NumericMatrix storing the vertices (nodes) 
					- triangles: #elements-by-3 IntegerMatrix storing the triangles */
List getLinearFEMesh(const NumericMatrix & nodes, const IntegerMatrix & elems);

// Include implementations of inlined methods
#include "inline_RcppSimplification.hpp"

// Exposed the class to R by enclosing it in a Rcpp module.
RCPP_MODULE(mod_RcppSimplification)
{
	using namespace Rcpp;
		
	class_<RcppSimplification>("RcppSimplification")
		
		// Expose the constructors
		.constructor<CharacterVector, double, double, double>
			("Constructor taking as arguments a string to the "
			 "file storing the mesh (.inp and .vtk file format supported) "
			 "and the weights for the geometric, "
			 "displacement and distribution cost functions.")
		.constructor<NumericMatrix, IntegerMatrix, double, double, double>
			("Constructor taking as arguments a #nodes-by-3 matrix storing the nodes "
			 "and a #elements-by-3 matrix storing the elements (i.e. the i-th row "
			 "contains the Id's of the vertices of the i-th element) and the weights "
			 "for the geometric, displacement and distribution cost functions.")
		.constructor<NumericMatrix, IntegerMatrix, NumericMatrix, double, double, double>
			("Constructor taking as arguments a #nodes-by-3 matrix storing the nodes "
			 "and a #elements-by-3 matrix storing the elements (i.e. the i-th row "
			 "contains the Id's of the vertices of the i-th element), a #data-by-3 matrix "
			 "storing the data locations and the weights for the geometric, displacement "
			 "and distribution cost functions.")
		.constructor<NumericMatrix, IntegerMatrix, NumericMatrix, NumericVector,
			double, double, double>
			("Constructor taking as arguments a #nodes-by-3 matrix "
			 "storing the nodes, a #elements-by-3 storing the elements "
			 "(i.e. the i-th row contains the Id's of the vertices of the "
			 "i-th triangle), a #data-by-3 matrix storing data locations "
			 "and a #data-by-1 vector storing the observations.")
			 			 
		// Expose get methods
		.const_method("getNumNodes", &RcppSimplification::getNumNodes,
			"Get number of nodes.")
		.const_method("getNodes", &RcppSimplification::getNodes,
			"Get the list of nodes as a #nodes-by-3 matrix.")
		.const_method("getEdges", &RcppSimplification::getEdges,
			"Get the list of edges as a #edges-by-2 matrix, "
			"where the i-th row stores the Id's of end-points "
			"of the i-th edge.")
		.const_method("getEndPoints", &RcppSimplification::getEndPoints,
			"Get a #edges-by-6 matrix where the first (last) three "
			"columns store the coordinates of the first (second) "
			"end-point of the i-th edge.")
		.const_method("getNumElems", &RcppSimplification::getNumElems,
			"Get number of triangles.")
		.const_method("getElems", &RcppSimplification::getElems,
			"Get the list of elements as a #elements-by-3 matrix, "
			"where the i-th row stores the Id's of the vertices "
			"of the i-th triangle.")
			
		.const_method("getElemsVertices", &RcppSimplification::getElemsVertices)
			
		.const_method("getNumData", &RcppSimplification::getNumData,
			"Get number of data points.")
		.const_method("getDataLocations", &RcppSimplification::getDataLocations,
			"Get the list of data locations as a #data-by-3 matrix.")
		.const_method("getObservations", &RcppSimplification::getObservations,
			"Get the list of observations as a #data-by-1 vector.")
		.const_method("getQuantityOfInformation", 
			&RcppSimplification::getQuantityOfInformation,
			"Get the quantity of information for each triangle.")
		.const_method("getElemsOnEdge", 
			&RcppSimplification::getElemsOnEdge,
			"Get the Id's of the triangles insisting on an edge.")
		.const_method("getQuadraticFEMesh", 
			&RcppSimplification::getQuadraticFEMesh,
			"Return a quadratic Finite Elements mesh, built from the "
			"linear Finite Elements mesh stored by the C++ simplification class.")
		 
		// Expose method simplify
		.method("simplify", &RcppSimplification::simplify, 
			"Run the simplification procedure. This method takes in input "
			"the final number of nodes and a string with the path to the "
			"output mesh (.inp file format supported).")
		;
		
	// Expose auxiliary function
	Rcpp::function("getLinearFEMesh", &getLinearFEMesh,
		"Build a mesh with linear elements from a mesh with quadratic elements.")
	;
}

#endif

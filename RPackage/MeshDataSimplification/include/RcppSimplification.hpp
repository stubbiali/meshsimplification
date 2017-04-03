/*!	\file	RcppSimplification.hpp
	\brief	An R wrapper for the class simplification of the meshsimplification library. */
	
#ifndef HH_RCPPSIMPLIFICATION_HH
#define HH_RCPPSIMPLIFICATION_HH
	
#include <Rcpp.h>
#include "simplification.hpp"

using namespace Rcpp;
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
		/*!	Constructor.
			\param file		string storing path to input mesh
			\param wgeom	weight for geometric cost
			\param wdisp	weight for displacement cost
			\param wequi 	weight for equidistribution cost */
		RcppSimplification(const CharacterVector & file, const double & wgeom,
			const double & wdisp, const double & wequi);
			
		/*!	Get the list of nodes.
			\out 	#nodes-by-3 NumericMatrix storing the nodes */
		NumericMatrix getNodes() const;
		
		/*!	Get the list of elements, i.e. for each triangle the Id's
			of its vertices.
			
			\out	#elems-by-3 IntegerMatrix storing the elements */
		IntegerMatrix getElems() const;
		
		/*!	Get the list of data points.
			\out 	#data-by-4 NumericMatrix storing the data;
					the first three columns store the coordinates,
					while the last one reports the data values */
		NumericMatrix getData() const;
				
		/*!	Get the quantity of information for each element.
			\out 	vector with the quantity of information for each triangle */
		NumericVector getQuantityOfInformation() const;
			
		/*!	Run the simplification process.
			\param numNodesMax	final number of nodes
			\param file	 		path to file which will store the final mesh */
		void simplificate(const int & numNodesMax, const CharacterVector & file);
};

// Include implementations of inlined methods
#include "inline_RcppSimplification.hpp"

// Exposed the class to R by enclosing it in a Rcpp module.
RCPP_MODULE(mod_RcppSimplification)
{
	using namespace Rcpp;
	
	class_<RcppSimplification>("RcppSimplification")
		
		// Expose the constructor
		.constructor<CharacterVector,double,double,double>
			("Constructor taking as arguments a string to the "
			 "file storing the mesh (.inp and .vtk file format supported) "
			 "and the weights for the geometric, "
			 "displacement and distribution cost functions.")
			 			 
		// Expose get methods
		.const_method("getNodes", &RcppSimplification::getNodes,
			"Get the list of nodes as a #nodes-by-3 matrix.")
		.const_method("getElems", &RcppSimplification::getElems,
			"Get the list of elements as a #elements-by-3 matrix, "
			"where the i-th row stores the Id's of the vertices "
			"of the i-th triangle.")
		.const_method("getData", &RcppSimplification::getData,
			"Get the list of data points as a #data-by-4 matrix, "
			"where the first three columns store the coordinates "
			"of data points, while the last one reports the data values.")
		.const_method("getQuantityOfInformation", 
			&RcppSimplification::getQuantityOfInformation,
			"Get the quantity of information for each triangle.")
		 
		// Expose method simplificate
		.method("simplificate", &RcppSimplification::simplificate, 
			"Run the simplification procedure. This method takes in input "
			"the final number of nodes and a string with the path to the "
			"output mesh (.inp file format supported).")
		;
}

#endif

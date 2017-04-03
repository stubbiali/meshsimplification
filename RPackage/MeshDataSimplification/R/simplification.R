#'	Create a simplification structure
#' 
#'	@param	file	A string reporting the absolute or relative path to the input mesh;
#' 					.inp and .vtk file formats are supported.
#'	@param	wgeom	Weight for the geometric component of the edge cost function;
#'					default is 1/3. Note that the all weights should be positive and
#' 					sum up to one.
#'	@param	wdisp 	Weight for the data displacement component of the edge cost function;
#'					default is 1/3. Note that the all weights should be positive and
#' 					sum up to one.
#'	@param	wequi	Weight for the data equidistribution component of the edge cost function;
#'					default is 1/3. Note that the all weights should be positive and
#' 					sum up to one.
#'	@description	This function instantiates a class storing all informations regarding
#'					a surface two dimensional grid and useful to run the simplification process.
#'					This function heavily relies on \code{RcppSimplification} - a wrapper for the
#'					template class \code{simplification<Triangle, MeshType::DATA, DataGeo>} 
#'					provided by the C++ library \code{meshsimplification}.
#'	@usage			initialize.simplification(file, wgeom = 1/3, wdisp = 1/3, wequi = 1/3)
#'	@return 		An object of class simplification, provided with the following fields:
#'					\item{\code{simplifier}} An object of class \code{RcppSimplification}
#'	@export

initialize.simplification <- function(file, wgeom = 1/3, wdisp = 1/3, wequi = 1/3)
{
	# Check whether the weights sum to one
	s <- wgeom + wdisp + wequi
	tol <- 1e-15
	if (wgeom < 0 | wdisp < 0 | wequi < 0 | abs(s - 1) > tol)
		stop("The weights must be positive and sum up to one.") 
	
	# Create an RcppSimplification object
	mod_RcppSimplification <- Module("mod_RcppSimplification", PACKAGE = "MeshDataSimplification")
	simplifier <- new(mod_RcppSimplification$RcppSimplification, file, wgeom, wdisp, wequi)
	
	# Create a simplification object
	out <- list(simplifier)
	names(out)[1] <- "simplifier"
	class(out) <- "simplification"
	
	return(out)
}


#'	Run the simplification process.
#'
#'	@param	x			An object of class \code{simplification}, created through 
#'						\code{initialize.simplification}
#'	@param	numNodesMax	Final number of nodes.
#'	@param	file		String specifying the path to the location where the decimated
#'						mesh will be stored; .inp file format supported. If the path
#'						is not provided, the mesh will no printed.
#'	@description		Function running the simplification process by invoking the 
#'						method \code{simplificate} of the \code{RcppSimplification}
#'						through the attribute \code{simplifier} of the \code{simplification}
#'						object given as argument. Note that the procedure is completely 
#'						carried out at the C++ level by the \code{meshsimplification} 
#'						library.
#'	@usage				run.simplification(x, numNodesMax, file = '')
#'	@export
#'	@examples
#'	## Instantiate a simplification object for the simplification of pawn geometry
#'	## Suppose that the mesh file is placed in the R working folder
#'	## Components of the edge cost function are equally weighted
#'	obj <- initialize.simplification('pawn.inp')
#'	## Run the simplification strategy, reducing the mesh down to n = 1000 nodes
#'	## Once the procedure is over, print the mesh to file
#'	run.simplification(1000, 'pawn_simplified.inp')

run.simplification <- function(x, numNodesMax, file = '')
{
	x$simplifier$simplificate(numNodesMax, file)
}


#'	Get the list of nodes.
#'
#'	@param	x	An object of class simplification, created through
#'				\code{initialize.simplification}
#'	@usage		get.nodes(x)
#'	@return		A #nodes-by-3 matrix, where the i-th row stores
#'				the coordinates of the i-th node.	
#'	@export

get.nodes <- function(x)
{
	out <- x$simplifier$getNodes()
	return(out)
}


#'	Get the list of elements.
#'
#'	@param	x	An object of class simplification, created through
#'				\code{initialize.simplification}
#'	@usage		get.elems(x)
#'	@return		A #elements-by-3 matrix, where the i-th row	stores
#'				the Id's of the vertices of the i-th triangle.
#'	@export

get.elems <- function(x)
{
	out <- x$simplifier$getElems()
	return(out)
}


#'	Get the list of data points.
#'
#'	@param	x	An object of class simplification, created through
#'				\code{initialize.simplification}
#'	@usage		get.data(x)
#'	@return		A #data-by-4 matrix, where the first three columns
#'				stores the coordinates of the data points, while
#'				the data values are reported in the fourth column.
#'	@export	

get.data <- function(x)
{
	out <- x$simplifier$getData()
	return(out)
}


#'	Get the quantity of information for each element.
#'
#'	@param	x	An object of class simplification, created through
#'				\code{initialize.simplification}
#'	@usage		get.quantity.of.information(x)
#'	@return		A #elements-by-1 vector, storing the quantity of
#'				information for each element.
#'	@export

get.quantity.of.information <- function(x)
{
	out <- x$simplifier$getQuantityOfInformation()
	return(out)
}



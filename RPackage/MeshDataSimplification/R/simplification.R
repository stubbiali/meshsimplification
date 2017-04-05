#'	Create a simplification structure from file
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
#'	@usage			setup.simplification.from.file(file, wgeom = 1/3, wdisp = 1/3, wequi = 1/3)
#'	@return 		An object of class simplification, provided with the following fields:
#'					\item{\code{simplifier}} An object of class \code{RcppSimplification}
#'	@export

setup.simplification.from.file <- function(file, wgeom = 1/3, wdisp = 1/3, wequi = 1/3)
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
	out <- list(simplifier = simplifier)
	class(out) <- "simplification"
	
	return(out)
}


#'	Create a simplification structure from a SURFACE_MESH object
#' 
#'	@param	mesh	A SURFACE_MESH object; currently, only first order grids are supported.
#'	@param	loc 	#data-by-3 vector with data locations; default is NULL, i.e. locations
#'					are supposed to coincide with grid nodes.
#'	@param	val		#data-by-1 vector with the observations; default is NULL, i.e. observations
#'					set to zero.
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
#'	@usage			setup.simplification(mesh, loc = NULL, val = NULL, wgeom = 1/3, wdisp = 1/3, wequi = 1/3)
#'	@return 		An object of class simplification, provided with the following fields:
#'					\item{\code{simplifier}} An object of class \code{RcppSimplification}
#'	@export

setup.simplification <- function(mesh, loc = NULL, val = NULL, wgeom = 1/3, wdisp = 1/3, wequi = 1/3)
{
	# Check whether the weights sum to one
	s <- wgeom + wdisp + wequi
	tol <- 1e-15
	if (wgeom < 0 | wdisp < 0 | wequi < 0 | abs(s - 1) > tol)
		stop("The weights must be positive and sum up to one.") 
		
	# Check if mesh is a SURFACE_MESH class object
	if (class(mesh) != "SURFACE_MESH")
		stop("mesh must be an object of class SURFACE_MESH.")
		
	# Check if mesh is first order; second order grids are not supported yet
	if (mesh$order > 1)
		stop("Only first order grids currently supported.")
	
	# Create an RcppSimplification object
	mod_RcppSimplification <- Module("mod_RcppSimplification", PACKAGE = "MeshDataSimplification")
	if (is.null(loc))
		simplifier <- new(mod_RcppSimplification$RcppSimplification, mesh$nodes, mesh$triangles, wgeom, wdisp, wequi)
	else if (is.null(val))
		simplifier <- new(mod_RcppSimplification$RcppSimplification, mesh$nodes, mesh$triangles, loc, wgeom, wdisp, wequi)
	else
		simplifier <- new(mod_RcppSimplification$RcppSimplification, mesh$nodes, mesh$triangles, loc, val, wgeom, wdisp, wequi)
	
	# Create a simplification object
	out <- list(simplifier = simplifier)
	class(out) <- "simplification"
	
	return(out)
}


#'	Run the simplification process
#'
#'	@param	x			An object of class \code{simplification}, created through 
#'						\code{setup.simplification} or \code{setup.simplification.from.file}.
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
#'	@return				A SURFACE_MESH object, storing the simplified grid.
#'	@return				List with data points locations onto the simplified grid.
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
	# Run the simplification procedure
	x$simplifier$simplificate(numNodesMax, file)
	
	# Create SURFACE_MESH object
	mesh <- get.surface.mesh(x)
	
	# Get list of data points
	locations <- get.data.locations(x)
	
	return(list(mesh, locations))
}


#'	Get the list of nodes
#'
#'	@param	x	An object of class simplification, created through
#'				\code{setup.simplification} or \code{setup.simplification.from.file}.
#'	@usage		get.nodes(x)
#'	@return		A #nodes-by-3 matrix, where the i-th row stores
#'				the coordinates of the i-th node.	
#'	@export

get.nodes <- function(x)
{
	out <- x$simplifier$getNodes()
	return(out)
}


#' Get the list of edges
#'
#'	@param	x	An object of class simplification, created through
#'				\code{setup.simplification} or \code{setup.simplification.from.file}.
#'	@usage		get.edges(x)
#'	@return		A #edges-by-2 matrix, where the i-th row stores
#'				the Id's of the end-points of the i-th edge.	
#'	@export

get.edges <- function(x)
{
	out <- x$simplifier$getEdges()
	return(out)
}


#'	Get the list of elements
#'
#'	@param	x	An object of class simplification, created through
#'				\code{setup.simplification} or \code{setup.simplification.from.file}.
#'	@usage		get.triangles(x)
#'	@return		A #elements-by-3 matrix, where the i-th row	stores
#'				the Id's of the vertices of the i-th triangle.
#'	@export

get.triangles <- function(x)
{
	out <- x$simplifier$getElems()
	return(out)
}


#'	Get the list of data points locations
#'
#'	@param	x	An object of class simplification, created through
#'				\code{setup.simplification} or \code{setup.simplification.from.file}.
#'	@usage		get.data.locations(x)
#'	@return		A #data-by-3 matrix storing the coordinates of data points locations.
#'	@export	

get.data.locations <- function(x)
{
	out <- x$simplifier$getDataLocations()
	return(out)
}


#'	Get the list of observations
#'
#'	@param	x	An object of class simplification, created through
#'				\code{setup.simplification} or \code{setup.simplification.from.file}.
#'	@usage		get.observations(x)
#'	@return		A #data-by-1 vector storing the observations.
#'	@export	

get.observations <- function(x)
{
	out <- x$simplifier$getObservations()
	return(out)
}


#'	Get the quantity of information for each element
#'
#'	@param	x	An object of class simplification, created through
#'				\code{setup.simplification} or \code{setup.simplification.from.file}.
#'	@usage		get.quantity.of.information(x)
#'	@return		A #elements-by-1 vector, storing the quantity of
#'				information for each element.
#'	@export

get.quantity.of.information <- function(x)
{
	out <- x$simplifier$getQuantityOfInformation()
	return(out)
}


#'	Get triangles sharing a given edge
#'
#'	@param	x	An object of class simplification, created through
#'				\code{setup.simplification} or \code{setup.simplification.from.file}.
#'	@param	id1	Id of the first end-point of the edge.
#'	@param	id2	Id of the second end-point of the edge.
#'	@usage		get.triangles.on.edge(x, id1, id2)
#'	@return		A vector with the Id's of the desired triangles (if any).
#'	@export

get.triangles.on.edge <- function(x, id1, id2)
{
	out <- x$simplifier$getElemsOnEdge(id1,id2)
	return(out)
}


#'	Get surface mesh
#'
#'	@param	x	An object of class simplification, created through
#'				\code{setup.simplification} or \code{setup.simplification.from.file}.
#'	@usage		get.surface.mesh(x)
#'	@return		A SURFACE_MESH object.
#'	@export

get.surface.mesh <- function(x)
{
	# Extract required information
	nnodes <- x$simplifier$getNumNodes()
	nodes <- x$simplifier$getNodes()
	ntriangles <- x$simplifier$getNumElems()
	triangles <- x$simplifier$getElems()
	
	# Create a mesh
	out <- list(nnodes = nnodes, nodes = nodes, ntriangle = ntriangles, triangles = triangles, order = 1)
	class(out) <- "SURFACE_MESH"
	return(out)
}



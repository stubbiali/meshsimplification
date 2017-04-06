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
	out <- list(simplifier = simplifier, order = 1)
	class(out) <- "simplification"
	
	return(out)
}


#'	Create a simplification structure from a SURFACE_MESH object
#' 
#'	@param	mesh	A SURFACE_MESH object.
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
		
	# Load module
	mod_RcppSimplification <- Module("mod_RcppSimplification", PACKAGE = "MeshDataSimplification")
		
	# Extract vertices and triangles of the grid
	nodes <- mesh$nodes
	triangles <- mesh$triangles
	
	# Convert from R 1-based indexing to C++ 0-based indexing
	triangles <- triangles-1
		
	# If the mesh consists of quadratic elements, extract linear elements
	if (mesh$order == 2)
	{
		res <- mod_RcppSimplification$getLinearFEMesh(nodes, triangles)
		nodes <- res$nodes
		triangles <- res$triangles
	}
		
	# Create an RcppSimplification object
	if (is.null(loc))
		simplifier <- new(mod_RcppSimplification$RcppSimplification, nodes, triangles, wgeom, wdisp, wequi)
	else if (is.null(val))
		simplifier <- new(mod_RcppSimplification$RcppSimplification, nodes, triangles, loc, wgeom, wdisp, wequi)
	else
		simplifier <- new(mod_RcppSimplification$RcppSimplification, nodes, triangles, loc, val, wgeom, wdisp, wequi)
	
	# Create a simplification object
	out <- list(simplifier = simplifier, order = mesh$order)
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
#'	@return				A list with the following fields:
#'						- mesh: a SURFACE_MESH object, storing the simplified grid;
#'						- locations: a #data-by-3 matrix with the locations of data points
#'							onto the simplified grid
#'						- qoi: a #elements-by-1 vector with the quantity of information
#'							(QOI) for each triangle.
#'	@export
#'	@examples
#'	## Instantiate a simplification object for the simplification of pawn geometry
#'	## Suppose that the mesh file is placed in the R working directory
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
	
	# Get quantity of information for each triangle
	qoi <- get.quantity.of.information(x)
	
	return(list(mesh = mesh, locations = locations, qoi = qoi))
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
	# Preliminary check
	if (class(x) != "simplification")
		stop("The function takes an object of class simplification as input.")
		
	# Run		
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
	# Preliminary check
	if (class(x) != "simplification")
		stop("The function takes an object of class simplification as input.")
		
	# Run, converting from C++ 0-based indexing to R 1-based indexing
	out <- x$simplifier$getEdges()
	out < out + 1
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
	# Preliminary check
	if (class(x) != "simplification")
		stop("The function takes an object of class simplification as input.")
		
	# Run, converting from C++ 0-based indexing to R 1-based indexing
	out <- x$simplifier$getElems()
	out <- out + 1
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
	# Preliminary check
	if (class(x) != "simplification")
		stop("The function takes an object of class simplification as input.")
		
	# Run
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
	# Preliminary check
	if (class(x) != "simplification")
		stop("The function takes an object of class simplification as input.")
		
	# Run
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
	# Preliminary check
	if (class(x) != "simplification")
		stop("The function takes an object of class simplification as input.")
		
	# Run
	out <- x$simplifier$getQuantityOfInformation()
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
	# Preliminary check
	if (class(x) != "simplification")
		stop("The function takes an object of class simplification as input.")
		
	# Extract nodes and triangles, differentiating between
	# linear and quadratic Finite Elements
	if (x$order == 1)
	{
		nodes <- x$simplifier$getNodes()
		triangles <- x$simplifier$getElems()
	}
	else if (x$order == 2)
	{
		res <- x$simplifier$getQuadraticFEMesh()
		nodes <- res$nodes
		triangles <- res$triangles
	}
	
	# Convert from C++ 0-based indexing to R 1-based indexing
	triangles <- triangles + 1
	
	# Create the mesh
	out <- list(nnodes = dim(nodes)[1], nodes = nodes, 
		ntriangles = dim(triangles)[1], triangles = triangles, order = x$order)
	class(out) <- "SURFACE_MESH"
	return(out)
}



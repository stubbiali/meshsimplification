#'	Initialize the simplification framework from file
#' 
#'	@param	file	Absolute or relative path to the input mesh; .inp and .vtk file formats are supported.
#'	@param	index	Either '0' or '1', saying whether vertices Id's are enumerated according to a 0-based 
#'					or 1-based indexing, respectively. Default is '1'.
#'	@param	wgeom	Weight for the geometric component of the edge cost function;
#'					default is 1/3. Note that the all weights should be positive and
#' 					sum up to one.
#'	@param	wdisp 	Weight for the data displacement component of the edge cost function;
#'					default is 1/3. Note that the all weights should be positive and
#' 					sum up to one.
#'	@param	wequi	Weight for the data equidistribution component of the edge cost function;
#'					default is 1/3. Note that the all weights should be positive and
#' 					sum up to one.
#'	@description	This function instantiates an object of class \code{simplification}, holding all informations regarding
#'					a surface two dimensional grid, and useful to run the simplification process.
#'					This function heavily relies on \code{RcppSimplification} - a wrapper for the
#'					template class \code{simplification<Triangle, MeshType::DATA, DataGeo>} 
#'					provided by the C++ library \code{meshsimplification}.
#'	@seealso		\code{\link{plot.surface.mesh}}, \code{\link{run.simplification}}
#'	@usage			setup.simplification.from.file(file, index = 1, wgeom = 1/3, wdisp = 1/3, wequi = 1/3)
#'	@return 		An object of class simplification, provided with the following fields:
#'					\item{\code{simplifier}}{An object of class \code{RcppSimplification}.}
#'					\item{\code{order}}{Either '1' or '2', saying whether each triangle should be represented 
#'					through three points (the vertices) or six points (the vertices plus 
#'					the midpoints of the edges). These representations respectively allow 
#'					to build a linear or quadratic Finite Element basis over the mesh.
#'					In case of the simplification framework initialized from file, the
#'					order is supposed to be '1'.}
#'	@export

setup.simplification.from.file <- function(file, index = 1, wgeom = 1/3, wdisp = 1/3, wequi = 1/3)
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


#'	Initialize the simplification framework from an object of class SURFACE_MESH
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
#'	@description	This function instantiates an object of class \code{simplification}, holding all informations regarding
#'					a surface two dimensional grid, and useful to run the simplification process.
#'					This function heavily relies on \code{RcppSimplification} - a wrapper for the
#'					template class \code{simplification<Triangle, MeshType::DATA, DataGeo>} 
#'					provided by the C++ library \code{meshsimplification}.
#'	@usage			setup.simplification(mesh, loc = NULL, val = NULL, wgeom = 1/3, wdisp = 1/3, wequi = 1/3)
#'	@seealso		\code{\link{plot.surface.mesh}}, \code{\link{run.simplification}}
#'	@return 		An object of class simplification, provided with the following fields:
#'					\item{\code{simplifier}}{An object of class \code{RcppSimplification}.}
#'					\item{\code{order}}{Either '1' or '2', saying whether each triangle should be represented 
#'					through three points (the vertices) or six points (the vertices plus 
#'					the midpoints of the edges). These representations respectively allow 
#'					to build a linear or quadratic Finite Element basis over the mesh.
#'					The Finite Element order is determined from the input SURFACE_MESH object.}
#'	@export
#'	@examples
#'	## Instantiate an object of class simplification for the simplification of a pawn geometry; 
#'	## suppose that the components of the edge cost function are equally weighted
#'	data(pawn)
#'	obj <- setup.simplification(mesh)
#'	## Plot the original mesh
#'	plot.surface.mesh(obj, main = "Original mesh, 2522 nodes")
#'	## Run the simplification strategy, reducing the mesh down to n = 1500 nodes
#'	run.simplification(obj, 1500)
#'	## Plot the simplified mesh
#'	plot.surface.mesh(obj, main = "Simplified mesh, 1500 nodes")
#'	## Resume the simplification procedure, reducing the mesh down to n = 1000 nodes
#'	run.simplification(obj, 1000)
#'	## Plot the simplified mesh
#'	plot.surface.mesh(obj, main = "Simplified mesh, 1000 nodes")

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
#'						\code{\link{setup.simplification}} or \code{\link{setup.simplification.from.file}}.
#'	@param	numNodesMax	Final number of nodes.
#'	@param	file		String specifying the path to the location where the decimated
#'						mesh will be stored; .inp file format supported. If the path
#'						is not provided, the mesh will no printed.
#'	@description		Function running the simplification process by invoking the 
#'						method \code{simplify} of the \code{RcppSimplification} class
#'						through the attribute \code{simplifier} of the \code{simplification}
#'						object given as argument. Note that the procedure is completely 
#'						carried out at the C++ level by the \code{meshsimplification} 
#'						library.
#'	@usage				run.simplification(x, numNodesMax, file = '')
#'	@return				A list with the following fields:
#'						\item{mesh}{a SURFACE_MESH object, storing the simplified grid;}
#'						\item{locations}{a #data-by-3 matrix with the locations of data points
#'						onto the simplified grid;}
#'						\item{qoi}{a #elements-by-1 vector with the quantity of information
#'						(QOI) for each triangle.}
#'	@export
#'	@examples
#'	## Instantiate an object of class simplification for the simplification of a pawn geometry; 
#'	## suppose that the components of the edge cost function are equally weighted
#'	data(pawn)
#'	obj <- setup.simplification(mesh)
#'	## Plot the original mesh
#'	plot.surface.mesh(obj, main = "Original mesh, 2522 nodes")
#'	## Run the simplification strategy, reducing the mesh down to n = 1500 nodes
#'	run.simplification(obj, 1500)
#'	## Plot the simplified mesh
#'	plot.surface.mesh(obj, main = "Simplified mesh, 1500 nodes")
#'	## Resume the simplification procedure, reducing the mesh down to n = 1000 nodes
#'	run.simplification(obj, 1000)
#'	## Plot the simplified mesh
#'	plot.surface.mesh(obj, main = "Simplified mesh, 1000 nodes")

run.simplification <- function(x, numNodesMax, file = '')
{
	# Preliminary check
	if (class(x) != "simplification")
		stop("The function takes an object of class simplification as input.")
		
	# Run the simplification procedure
	x$simplifier$simplify(numNodesMax, file)
	
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
#'				\code{\link{setup.simplification}} or \code{\link{setup.simplification.from.file}}.
#'	@usage		get.nodes(x)
#'	@return		A #nodes-by-3 matrix, where the i-th row stores the coordinates of the i-th node.	
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
#'				\code{\link{setup.simplification}} or \code{\link{setup.simplification.from.file}}.
#'	@usage		get.edges(x)
#'	@return		A #edges-by-2 matrix, where the i-th row stores the Id's of the end-points of the i-th edge.	
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
#'				\code{\link{setup.simplification}} or \code{\link{setup.simplification.from.file}}.
#'	@usage		get.triangles(x)
#'	@return		A #elements-by-3 matrix, where the i-th row	stores the Id's of the vertices of the i-th triangle.
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
#'				\code{\link{setup.simplification}} or \code{\link{setup.simplification.from.file}}.
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
#'				\code{\link{setup.simplification}} or \code{\link{setup.simplification.from.file}}.
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
#'				\code{\link{setup.simplification}} or \code{\link{setup.simplification.from.file}}.
#'	@usage		get.quantity.of.information(x)
#'	@return		A #elements-by-1 vector, storing the quantity of information for each element.
#'	@export

get.quantity.of.information <- function(x)
{
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
#'				\code{\link{setup.simplification}} or \code{\link{setup.simplification.from.file}}.
#'	@usage		get.surface.mesh(x)
#'	@return		A SURFACE_MESH object, whose order is compliant with the mesh used to
#'				initialize the simplification framework.
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


#'	Plot a mesh in a 3D perspective
#'
#'	@param	x		An object of class simplification, created through
#'					\code{\link{setup.simplification}} or \code{\link{setup.simplification.from.file}}.
#'	@param	phi		Colatitude (in degrees) characterizing the viewing direction; default is 40.
#'	@param	theta	Longitude (in degrees) characterizing the viewing direction; default is 40.
#'	@param	...		Additional arguments passed to the plotting methods; these include:
#'					xlim, ylim, zlim, xlab, ylab, zlab, main, sub, r, d, scale, expand, box, axes, nticks, ticktype.
#'	@description	Plot the three-dimensional surface mesh held by an object of class simplification.
#'					The package plot3D is used.
#'	@usage			plot.surface.mesh(x, phi = 40, theta = 40, ...)
#'	@export
#'	@examples
#'	## Instantiate an object of class simplification for the simplification of a pawn geometry; 
#'	## suppose that the components of the edge cost function are equally weighted
#'	data(pawn)
#'	obj <- setup.simplification(mesh)
#'	## Plot the original mesh
#'	plot.surface.mesh(obj, main = "Original mesh, 2522 nodes")
#'	## Run the simplification strategy, reducing the mesh down to n = 1500 nodes
#'	run.simplification(obj, 1500)
#'	## Plot the simplified mesh
#'	plot.surface.mesh(obj, main = "Simplified mesh, 1500 nodes")
#'	## Resume the simplification procedure, reducing the mesh down to n = 1000 nodes
#'	run.simplification(obj, 1000)
#'	## Plot the simplified mesh
#'	plot.surface.mesh(obj, main = "Simplified mesh, 1000 nodes")

plot.surface.mesh <- function(x, phi = 40, theta = 40, ...)
{
	# Preliminary check
	if (class(x) != "simplification")
		stop("The function takes an object of class simplification as input.")
				
	# Get triangles
	trs <- x$simplifier$getElemsVertices()
	
	# Array of colors
	n <- (nrow(trs)+1)/4
	clr <- matrix("blue", nrow = n, ncol = 1)
	
	# Plot
	polygon3D (x = trs[,1], y = trs[,2], z = trs[,3], ...,
      	colvar = NULL, phi = phi, theta = theta,
      	col = clr, NAcol = "white", breaks = NULL,
      	facets = FALSE, bty = "b", pty = "s",
      	add = FALSE, plot = TRUE)
}



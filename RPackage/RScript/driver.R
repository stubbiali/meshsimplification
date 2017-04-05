#' Driver running the simplification process within R.
#' User-defined settings:
#' @param iFile	path to input mesh
#' @param oFile	path to output mesh
#' @param n		final number of nodes
#' @param wgeom	weight for geometric cost function
#' @param wdisp	weight for displacement cost function
#' @param wequi	weight for equidistribution cost function

iFile <- '../../mesh/pawn.inp'
oFile <- '../../results/pawn_R.inp'
n1 <- 2000
n2 <- 1000
wgeom <- 1/3
wdisp <- 1/3
wequi <- 1/3

#
# Run
#

require(Rcpp)
require(RcppEigen)
require(MeshDataSimplification)

#
# Initialize simplification from file
#

obj <- setup.simplification.from.file(iFile)
out <- run.simplification(obj, n1)

#
# Initialize simplification through a SURFACE_MESH object
#

nodes <- get.nodes(obj)
triangles <- get.triangles(obj)
#loc <- get.data.locations(obj)
#val <- get.observations(obj)

mesh <- list(nnodes = dim(nodes)[1], nodes = nodes, 
		ntriangles = dim(triangles)[1], triangles = triangles, order = 1)
class(mesh) <- "SURFACE_MESH"

obj2 <- setup.simplification(mesh)
out <- run.simplification(obj2, n2, oFile)




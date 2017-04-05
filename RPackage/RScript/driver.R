#' Driver running the simplification process within R.
#' User-defined settings:
#' @param iFile	path to input mesh
#' @param oFile	path to output mesh
#' @param n		final number of nodes
#' @param wgeom	weight for geometric cost function
#' @param wdisp	weight for displacement cost function
#' @param wequi	weight for equidistribution cost function

iFile <- '../../mesh/pawn.inp'
oFile <- '../../mesh/pawn.inp'
n <- 2000
wgeom <- 1/3
wdisp <- 1/3
wequi <- 1/3

# Run

require(Rcpp)
require(MeshDataSimplification)
module <- Module("mod_RcppSimplification", PACKAGE = "MeshDataSimplification")
#obj <- new(module$RcppSimplification, iFile, wgeom, wdisp, wequi)
#obj$simplificate(n, oFile)
obj <- setup.simplification.from.file(iFile)
#out <- run.simplification(obj, n)
#out

nodes <- get.nodes(obj)
triangles <- get.triangles(obj)
triangles <- cbind(triangles, triangles)
#loc <- get.data.locations(obj)
#val <- get.observations(obj)

mesh <- list(nodes=nodes, triangles=triangles, order=1)
class(mesh) <- "SURFACE_MESH"

#obj2 <- setup.simplification(mesh)
#out <- run.simplification(obj, n)
#out

out <- module$getMeshLinearFEM(nodes, triangles)
out

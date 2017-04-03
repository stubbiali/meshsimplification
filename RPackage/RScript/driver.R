#' Driver running the simplification process within R.
#' User-defined settings:
#' @param iFile	path to input mesh
#' @param oFile	path to output mesh
#' @param n		final number of nodes
#' @param wgeom	weight for geometric cost function
#' @param wdisp	weight for displacement cost function
#' @param wequi	weight for equidistribution cost function

iFile <- '../mesh/pawn.inp'
oFile <- '../mesh/pawn_R.inp'
n <- 2000
wgeom <- 1/3
wdisp <- 1/3
wequi <- 1/3

# Run

require(Rcpp)
require(MeshDataSimplification)
#module <- Module("mod_MeshDataSimplification", PACKAGE = "MeshDataSimplification")
#obj <- new(module$RcppSimplification, iFile, wgeom, wdisp, wequi)
#obj$simplificate(n, oFile)
obj <- initialize.simplification(iFile, wgeom, wdisp, wequi)
run.simplification(obj, n, oFile)
nodes <- get.nodes(obj)
nodes
elems <- get.elems(obj)
elems
data <- get.data(obj)
data
qoi <- get.quantity.of.information(obj)
qoi

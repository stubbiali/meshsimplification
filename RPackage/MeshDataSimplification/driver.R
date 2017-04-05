#' Driver running the simplification process within R.
#' User-defined settings:
#' @param iFile	path to input mesh
#' @param oFile	path to output mesh
#' @param n		final number of nodes
#' @param wgeom	weight for geometric cost function
#' @param wdisp	weight for displacement cost function
#' @param wequi	weight for equidistribution cost function

iFile <- '../../mesh/bunny.inp'
oFile1 <- '../../mesh/bunny_R_29000.inp'
oFile2 <- '../../mesh/bunny_R_20000_bis.inp'
n1 <- 29000
n2 <- 20000
wgeom <- 1/3
wdisp <- 1/3
wequi <- 1/3

# Run

require(Rcpp)
require(RcppEigen)
require(MeshDataSimplification)
module <- Module("mod_RcppSimplification", PACKAGE = "MeshDataSimplification")
obj <- new(module$RcppSimplification, iFile, wgeom, wdisp, wequi)
obj$simplificate(n1, oFile1)

nodes <- obj$getNodes()
elems <- obj$getElems()
loc <- obj$getDataLocations()
val <- obj$getObservations()

obj2 <- new(module$RcppSimplification, nodes, elems, loc, val, wgeom, wdisp, wequi)
obj2$simplificate(n2, oFile2)

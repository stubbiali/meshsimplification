#' Driver running the simplification process within R.
#' 
#' @param dataset	name of dataset storing the mesh as an object of class SURFACE_MESH:
#'					- pawn: pawn geometry, 2522 nodes;
#'					- bunny: Stanford Bunny geometry, 34834 nodes;
#'					- brain: cortical surface, 40962 nodes.
#' @param n1,n2		two different levels of simplification
#' @param wgeom		weight for the geometric cost function
#' @param wdisp		weight for the displacement cost function
#' @param wequi		weight for the equidistribution cost function

dataset <- "pawn"
n1 <- 2000
n2 <- 1000
wgeom <- 1/3
wdisp <- 1/3
wequi <- 1/3

#
# Initialize simplification framework and plot initial mesh
#

library(methods)
library(utils)
require(MeshDataSimplification)

if (dataset == "pawn")
	data(pawn)
if (dataset == "bunny")
	data(bunny)
if (dataset == "brain")
	data(brain)

x <- setup.simplification(mesh)
plot.surface.mesh(x, main = sprintf("Original mesh, %i nodes", mesh$nnodes))

#
# Simplified the mesh, then plot it again
#

y1 <- run.simplification(x, n1)
plot.surface.mesh(x, main = sprintf("Simplified mesh, %i nodes", n1))

#
# Resume the simplification, then plot the final mesh
#

y2 <- run.simplification(x, n2)
plot.surface.mesh(x, main = sprintf("Simplified mesh, %i nodes", n2))




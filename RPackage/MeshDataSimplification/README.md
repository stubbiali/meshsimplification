The R package MeshDataSimplification
====================================

The R package `MeshDataSimplification` wraps the C++ library `meshsimplification`, thus providing an high-level access to its functionalities. To expose the C++ code to R, the API provided by `Rcpp` and `RcppEigen` is used.

The tree structure of the package root folder is compliant with the standard. In detail:
- `src` contains C++ header and source files;
- `R` contains R source files;
- `demo` contains scripts examplifying the use of the package;
- `data` contains R datasets storing three-dimensional surface grids.

Lastly, the package comes with a Makefile. Then:
- to install the package, type `make install`; administrator privileges may be required;
- to test whether the installation has been successfully completed, type `make test`;
- to generate the documentation, placing it in the folder `man`, type `make documentation`;
- to clean the directory removing compiled code and the documentation, type `make clean`;
- to clean the directory and uninstall the package, type `make distclean`; administrator privileges may be required.


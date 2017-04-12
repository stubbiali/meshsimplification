An iterative simplification procedure for surface grids augmented with distributed data
=====================================================================

For a detailed description of the method, please refer to:

[1] Dassi F., Ettinger B., Perotto S., Sangalli L. M.
"A mesh simplification strategy for a spatial regression analysis over the cortical surface of the brain". 
Applied Numerical Mathematics 90:111-131, 2015.
    
C++ library
---------------------------------------------------------------------

The main output is the `meshsimplification` library.
The C++ code for the library, as well as that one for the applications and the tests, comes with the following folders:
- `include/` : header files for the `meshsimplification` library; even for template classes, declarations have been separated from implementations, with the former stored in `include/implementation/` (for non inlined methods and functions) and `include/inline/` (for inlined methods and functions);
- `src/` : source files for the `meshsimplification` library;
- `main/` : source files for the applications;
- `test/` : source files for the tests;
- `spike/` : extra code which may be useful in the future.
	 
To compile the library `libmeshsimplification.so` and compile and link the
applications using the GNU C++ compiler, from the project root folder type:

	make [DEBUG=yes] [ENABLE_SELF_INTERSECTIONS=yes] [STATIC=yes] [EIGEN_DIR=<path>]
	
Square brackets delimit options and arguments to, respectively:
- disable compile-time optimizations and enable debug symbols; 
- disable controls on grid self intersections throughout the simplification routine;
- build and link against the static version of the `meshsimplification` library;
- specify the path to the include directory of `Eigen` library.
By default:
- compile-time optimizations are enabled;
- grid self-intersections are forbidden; 
- the executable are linked against the dynamic version of `meshsimplification`, which is <eventually> built;
- path to `Eigen` include directory is supposed to be specified by the environmental variable `mkEigenInc`,
respectively. Please observe that `Eigen` is the only third-party library the code relies on. 
In case `Eigen` is not available on the current workstation, or the environmental variable `mkEigenInc` is not set, or the location specified by command line is incorrect, the header files provided in `lib/Eigen` are used for the compilation.

The aforementioned command generates the following folders:
- `build/` : object files for the static (`build/static`) and shared (`build/dynamic`) version of `meshsimplification` library, the executables (`build/main`) and the tests (`build/test`);
- `bin/` : binaries for the executables (`bin/main`) and the tests (`bin/test`).
Furthermore, the `meshsimplification` library is placed in the `lib/` folder.

To ease the usability of the library, three executables accepting options and arguments from command line are provided in the directory `bin/main`:
- `main_geo` : perform the simplification procedure for a purely geometric mesh, i.e. a mesh without distributed data;
- `main_onlygeo` : perform the simplification procedure for a mesh with distributed data; at each iteration, the selection of the edge to collapse derives from a purely geometric cost function (see Equation (3) in [1]);
- `main_datageo` : perform the simplification procedure for a mesh augmented with distributed data; at each iteration, the selection of the edge to contract derives from an edge cost function accounting for both geometric and statistical properties of the mesh (see Equation (3),(4),(6),(7) in [1]).
To get the list of available options and respective arguments for each executable, simply call the executable itself without any argument to print the help. For instance, to run the simplification routine on a pawn geometry, reducing the number of grid points from 2522 to 1000, and finally print the output grid to file, from the project root folder type:

	./bin/main/main\_datageo -i mesh/pawn.inp -n 1000 -o results/out\_pawn.inp

where `mesh/` contains some grids which may be input to the simplification routine, while `results` stores some simplified grids obtained through the `meshsimplification` library.

Other Makefile targets are:
- `folders` : create output folders;
- `static` : build the static `meshsimplification` library;
- `dynamic` : build the shared `meshsimplification` library;
- `install` : install the shared `meshsimplification` library; you may need `root` privileges;
- `test` : compile and link tests executable;
- `main` : compile and link applications;
- `doc` : create code documentation through Doxygen and place it in `doc/`; the online documentation can then be accessed through `doc/html/index.html`.
	
Typing

	make clean
	
from the current folder, the folders `build/`, `bin/` and `/doc` and the compiled library `meshsimplification` get removed.
To clean and disinstall the `meshsimplification` library, type

	make distclean
	
Administrator access rights might be required.

R package
------------------------------------------------------------------------------

This project comes also with the R package `MeshDataSimplification`, wrapping the C++ library `meshsimplification`, thus providing a high-level access to its functionalities. To expose the C++ code to R, the `Rcpp` and `RcppEigen` API is used.

The source code for the package is placed in the folder `RPackage/MeshDataSimplification`and its subfolders, namely:
- `include` : C++ header files for the package;
- `src` : C++ source files;
- `R` : R source files.
	
To build, compile and install the package, first install the `meshsimplification` library through

	make install
	
from the project root folder. Then, move to the `RPackage/MeshDataSimplification` folder and type

	R -e "library(devtools); install()" --silent
	
For both commands above, you may need administrator privileges. Furthermore, observe that the R package `devtools` is required. To check whether the installation has been successfull, run one of the scripts contained in the folder `RPackage/RScript`, aiming to exemplify the use of the functions provided by the package. 

Lastly, to build the Roxygen documentation and place it in `RPackage/MeshDataSimplification/man/`, run

	R -e "library(devtools); document()" --silent








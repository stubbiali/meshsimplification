An iterative simplification procedure for 
surface grids with data distributed over the surface
=====================================================================

For a detailed description of the method, please refer to:

[1] Dassi F., Ettinger B., Perotto S., Sangalli L. M.
"A mesh simplification strategy for a spatial regression analysis
over the cortical surface of the brain". 
Applied Numerical Mathematics 90:111-131, 2015.
    
C++ library
---------------------------------------------------------------------

The main output is the `meshsimplification` library.
The C++ code for the library, as well as that one for the applications
and the tests, comes with the following folders:
- `include/` : header files for the `meshsimplification` library;
- `src/` : source files for the `meshsimplification` library;
- `main/` : source files for the applications;
- `test/` : source files for the tests;
- `spike/` : extra code which may turn out to be useful in the future;
- `lib/` : third-party libraries required by the code.
	
To compile the library `libmeshsimplification.so` and compile and link the
applications using the GNU C++ compiler, from the project root folder type:

	make [DEBUG=yes] [ENABLE_SELF_INTERSECTIONS=yes] [STATIC=yes] [STATIC_STL=yes]
	
Square brackets delimit optional arguments to disable compile-time 
optimizations and enable debug symbols, disable controls on grid self intersections 
throughout the procedure, and link against the static version of `meshsimplification` 
library and the C++ Stardard Template Library (STL), respectively.
By default, compile-time optimizations are enabled, self-intersections are forbidden
and dynamic version of both `meshsimplification` and STL are considered.
Note that when compiling in debug mode, all warnings are converted into errors
(flag `-Werror`).
In case there were issues with the run-time loading of the STL, e.g. if the 
available version is too old, we suggest to link against the static version of STL.

The aforementioned command generates the following folders:
- `build/` : object files for the static (`build/static`) and shared (`build/dynamic`) version of `meshsimplification` library, the executables (`build/main`) and the tests (`build/test`);
- `bin/` : binaries for the executables (`bin/main`) and the tests (`bin/test`);
Furthermore, the `meshsimplification` library is placed in the `lib/` folder.

To ease the usability of the library, three executables accepting options and arguments
from command line are provided in the directory `bin/main`:
- `main_geo` : perform the simplification procedure for a purely geometric mesh, i.e. a mesh without distributed data;
- `main_onlygeo` : perform the simplification procedure for a mesh with distributed data; the selection of which edge to contract at each iteration is driven by a purely geometric cost function (see Equation (3) in [1]);
- `main_datageo` : perform the simplification procedure for a mesh with distributed data; the selection of which edge to contract at each iteration is driven by an edge cost function which takes both geometric and statistical informations into account (see Equation (3),(4),(6),(7) in [1]).
To get the list of available options for each executable, simply call the executable itself
without any argument to print the help. 

Other Makefile targets are:
- `folders` : create output folders;
- `static` : build the static `meshsimplification` library;
- `dynamic` : build the shared `meshsimplification` library;
- `install` : install the shared `meshsimplification` library; run this command as `root`;
- `test` : compile and link tests executable;
- `main` : compile and link applications;
- `doc` : create code documentation through Doxygen and place it in `doc/`; the online documentation can then be accessed through `doc/html/index.html`.
	
Typing

	make clean
	
from the current folder, the folders `build/`, `bin/` and `/doc` 
and the compiled library `meshsimplification` get removed.
To clean and disinstall the `meshsimplification` library, type

	make distclean
	
`root` access rights are required.

Example grids can be found in the folder `mesh/` while `post_processing/`
stores MATLAB datasets and scripts for postprocessing. 
Finally, `results/` contains some results obtained using the aforementioned library.

R package
------------------------------------------------------------------------------

This project comes also with the R package `MeshDataSimplification`, wrapping the C++ 
library `meshsimplification`, thus providing a high-level access to its functionalities.
To expose the C++ code to R, the `Rcpp` API is used, so make sure you have the `Rcpp`
package correctly installed.

The source code for the package is placed in the folder `RPackage/MeshDataSimplification`
and its subfolders, namely:
- `include` : C++ header files for the package and for the `meshsimplification` library (`include/meshsimplification/`);
- `src` : C++ source files;
- `R` : R source files;
- `lib` : the Eigen (`lib/Eigen/`) and `meshsimplification` library.
	
Note that the headers for the `meshsimplification` library, as well as the library itself,
are automatically placed in the respective locations within `RPackage/MeshDataSimplification`
every time the commands

	make, make dynamic, make tests
	 
are invoked.

In order to build, compile and install the package, first install the `meshsimplification` library
through

	make install
	
from the project root folder. Then, move to the `RPackage/MeshDataSimplification` folder and type:

	sudo R -e "library(devtools); install()" --silent
	
Observe that the R package `devtools` is required.

Lastly, to build the Roxygen documentation and place it in `RPackage/MeshDataSimplification/man/`,
run

	R -e "library(devtools); document()" --silent





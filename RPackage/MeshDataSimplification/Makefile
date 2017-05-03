# Makefile for the R Package MeshDataSimplification, automating the following tasks:
# - install the package (target "install", administrator privileges may be required);
# - run a simple example to check whether installation has been completed successfully (target "test")
# - generate the documentation (target "document");
# - clean the folder (target "clean");
# - clean the folder and uninstall the package (target "distclean", administrator privileges may be required).

FILES=$(wildcard src/meshsimplification/include/*.hpp src/meshsimplification/include/implementation/*.hpp src/meshsimplification/include/inline/*.hpp src/meshsimplification/src/*.cpp src/*.hpp src/*.cpp R/*.R)

all: install document

install: .installed

.installed: $(FILES)
	@R -e "library(devtools); install()" --silent
	@touch .installed
	
test: install
	@Rscript demo/example.R
	@$(RM) Rplots.pdf 
	
document: $(FILES)
	@R -e "library(devtools); document()" --silent
	
clean:
	@$(RM) -r src/MeshDataSimplification.so src/RcppSimplification.o man lib .installed src/symbols.rds
	
distclean: clean
	@cd src/meshsimplification && $(MAKE) distclean

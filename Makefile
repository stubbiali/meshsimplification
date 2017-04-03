# Makefile.

#
# Directories
#

# Directories storing headers, sources, tests and executables
LIB_INC_DIR=include
LIB_SRC_DIR=src
TEST_SRC_DIR=test
MAIN_SRC_DIR=main

# Directories which will store object files for the library,
# the tests and the executables
OBJ_DIR=build
S_LIB_OBJ_DIR=$(OBJ_DIR)/static
D_LIB_OBJ_DIR=$(OBJ_DIR)/dynamic
TEST_OBJ_DIR=$(OBJ_DIR)/test
MAIN_OBJ_DIR=$(OBJ_DIR)/main

# Directory storing third-part libraries (if any) and
# which will store the project library
LIB_DIR=lib
# Path to Eigen directory
EIGEN_DIR=$(LIB_DIR)/Eigen

# Directories which will store binary files for tests and executables
BIN_DIR=bin
TEST_BIN_DIR=$(BIN_DIR)/test
MAIN_BIN_DIR=$(BIN_DIR)/main

# Directory storing the documentation; Doxygen files will be
# placed here
DOC_DIR=doc

# Path to R stuff
R_DIR=RPackage
R_PACKAGE_DIR=$(R_DIR)/MeshDataSimplification
R_PACKAGE_INC_DIR=$(R_PACKAGE_DIR)/include
R_PACKAGE_SRC_DIR=$(R_PACKAGE_DIR)/src
R_PACKAGE_RSRC_DIR=$(R_PACKAGE_DIR)/R
R_PACKAGE_LIB_DIR=$(R_PACKAGE_DIR)/lib

#
# Files
#

# Name of the library
LIB=meshsimplification
VRS=

# Include, source and object files for meshsimplification library
LIB_INC=$(wildcard $(LIB_INC_DIR)/*.hpp $(LIB_INC_DIR)/implementation/*.hpp $(LIB_INC_DIR)/inline/*.hpp)
LIB_SRC=$(wildcard $(LIB_SRC_DIR)/*.cpp)
S_LIB_OBJ=$(patsubst $(LIB_SRC_DIR)/%.cpp, $(S_LIB_OBJ_DIR)/%.o, $(LIB_SRC))
D_LIB_OBJ=$(patsubst $(LIB_SRC_DIR)/%.cpp, $(D_LIB_OBJ_DIR)/%.o, $(LIB_SRC)) 

# Source, object and binary files for tests
TEST_SRC=$(wildcard $(TEST_SRC_DIR)/*.cpp)
TEST_OBJ=$(patsubst $(TEST_SRC_DIR)/%.cpp, $(TEST_OBJ_DIR)/%.o, $(TEST_SRC))
TEST_BIN=$(patsubst $(TEST_SRC_DIR)/%.cpp, $(TEST_BIN_DIR)/%, $(TEST_SRC))

# Source, object and binary files for executables
MAIN_SRC=$(wildcard $(MAIN_SRC_DIR)/*.cpp)
MAIN_OBJ=$(patsubst $(MAIN_SRC_DIR)/%.cpp, $(MAIN_OBJ_DIR)/%.o, $(MAIN_SRC))
MAIN_BIN=$(patsubst $(MAIN_SRC_DIR)/%.cpp, $(MAIN_BIN_DIR)/%, $(MAIN_SRC))

# Header and source files for R package
R_INC=$(wildcard $(R_PACKAGE_INC_DIR)/*.hpp)
R_SRC=$(wildcard $(R_PACKAGE_SRC_DIR)/*.cpp)
R_RSRC=$(wildcard $(R_PACKAGE_RSRC_DIR)/*.R)

# Name of R package
R_PACKAGE=MeshDataSimplification

#
# Flags
#

# Compilation mode; default is DEBUG
DEBUG=no
RELEASE=yes

# Disable checks on mesh self intersections; default is NO
ENABLE_SELF_INTERSECTIONS=no

# Link against static version of meshsimplification library; default is NO
STATIC=no

# Link against static version of Standard Library; default is NO
# Note that the static version will increase the size of the executables,
# but may be necessary whenever the runtime loading of the dynamic version fails
STATIC_STL=no

# Set the compiler
CXX=g++

# Flags for the compiler
ifeq ($(RELEASE),yes)
	CXXFLAGS=-std=c++11 -DNDEBUG -O3 -ftree-vectorize -I $(LIB_INC_DIR) -I $(EIGEN_DIR)
else
	CXXFLAGS=-std=c++11 -g -Werror -I $(LIB_INC_DIR) -I $(EIGEN_DIR)
endif
	
ifeq ($(ENABLE_SELF_INTERSECTIONS),yes)
	CXXFLAGS+= -DENABLE_SELF_INTERSECTIONS 
endif
	
# Flags for the linker for the library
LDFLAGS_LIB=-L $(LIB_DIR)
ifeq ($(STATIC_STL),yes)
	LDFLAGS_LIB+= -static-libstdc++
endif

# Flags for the linker for the executables and the tests
LDFLAGS=-L $(LIB_DIR)
ifeq ($(STATIC),no)
	LDFLAGS+= -Wl,-rpath,$(LIB_DIR)
	DEPENDENCY=dynamic
else
	DEPENDENCY=static
endif
LDFLAGS+= -l$(LIB)
ifeq ($(STATIC_STL),yes)
	LDFLAGS+= -Wl,-Bstatic -static-libstdc++
endif

#
# Targets
#

all: folders main 

#
# Build static library
#

$(S_LIB_OBJ_DIR)/%.o: $(LIB_SRC_DIR)/%.cpp $(LIB_INC_DIR)/%.hpp $(LIB_INC)
	@echo -n "Compiling $@ ... " 
	@$(CXX) $(CXXFLAGS)	-c -o	$@	$<	$(LDFLAGS_LIB)
	@echo "done"

static: folders $(S_LIB_OBJ)
	@echo -n "Creating lib$(LIB).a ... "
	@ar -r -s	$(LIB_DIR)/lib$(LIB).a	$(S_LIB_OBJ) > /dev/null 2>&1
	@echo "done"
	@echo "\033[92mStatic version of meshsimplification library successfully built\n\033[0m"
	
#
# Build dynamic library
#

$(D_LIB_OBJ_DIR)/%.o: $(LIB_SRC_DIR)/%.cpp $(LIB_INC_DIR)/%.hpp $(LIB_INC)
	@echo -n "Compiling $@ ... " 
	@$(CXX) $(CXXFLAGS)	-fPIC -c -o	$@	$<	$(LDFLAGS_LIB)
	@echo "done"

dynamic: folders $(D_LIB_OBJ)
	@echo -n "Creating lib$(LIB).so$(VRS) ... "
	@$(CXX) $(CXXFLAGS) -shared 	-o	lib$(LIB).so$(VRS)	$(D_LIB_OBJ)	$(LDFLAGS_LIB) 
	@mv -f lib$(LIB).so$(VRS) $(LIB_DIR)
	@cp -r -f $(LIB_INC_DIR)/*  $(R_PACKAGE_INC_DIR)/$(LIB)
	@cp -f $(LIB_DIR)/lib$(LIB).so$(VRS) $(R_PACKAGE_LIB_DIR)
	@echo "done"
	@echo "\033[92mDynamic version of meshsimplification library successfully built\n\033[0m"
	
install: dynamic
	@cp $(LIB_DIR)/lib$(LIB).so$(VRS) /usr/lib

#
# Build the tests
#

$(TEST_OBJ_DIR)/%.o: $(TEST_SRC_DIR)/%.cpp $(LIB_INC)
	@echo -n "Compiling $@ ... " 
	@$(CXX) $(CXXFLAGS)	-c -o	$@	$<	$(LDFLAGS)
	@echo "done"
	
$(TEST_BIN_DIR)/%: $(TEST_OBJ_DIR)/%.o $(DEPENDENCY)
	@echo -n "Linking $@ ... " 
	@$(CXX) $(CXXFLAGS)	-o	$@	$<	$(LDFLAGS)	
	@echo "done"
	
test: folders $(TEST_BIN)
	@echo "\033[92mTests successfully compiled and linked\n\033[0m"

#
# Build the executables
#

$(MAIN_OBJ_DIR)/%.o: $(MAIN_SRC_DIR)/%.cpp $(LIB_INC)
	@echo -n "Compiling $@ ... " 
	@$(CXX) $(CXXFLAGS)	-c -o	$@	$<	$(LDFLAGS)
	@echo "done"
	
$(MAIN_BIN_DIR)/%: $(MAIN_OBJ_DIR)/%.o $(DEPENDENCY)
	@echo -n "Linking $@ ... " 
	@$(CXX) $(CXXFLAGS)	-o	$@	$<	$(LDFLAGS)
	@echo "done"
	
main: $(MAIN_BIN)
	@echo "\033[92mExecutables successfully compiled and linked\n\033[0m"
	

#
# Phony targets
#

.PHONY: clean distclean folders doc

#
# Clean everything
#

clean:
	@$(RM) -r $(OBJ_DIR) $(BIN_DIR) $(LIB_DIR)/lib$(LIB).* $(DOC_DIR)/Doxyfile $(DOC_DIR)/html $(DOC_DIR)/latex $(R_PACKAGE_LIB_DIR)/lib$(LIB).so.$(VRS) $(R_PACKAGE_DIR)/man $(R_DIR)/MeshDataSimplification_* $(R_PACKAGE_SRC_DIR)/*.o
	
distclean: clean
	@$(RM) /usr/lib/lib$(LIB)*
	
	
#
# Create folders
#
		
folders: 
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(S_LIB_OBJ_DIR)
	@mkdir -p $(D_LIB_OBJ_DIR)
	@mkdir -p $(TEST_OBJ_DIR)
	@mkdir -p $(MAIN_OBJ_DIR)
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(MAIN_BIN_DIR)
	@mkdir -p $(TEST_BIN_DIR)
	@mkdir -p $(R_PACKAGE_INC_DIR)/$(LIB)
	
#
# Build Doxygen documentation
#

doc:
	@echo -n "Building Doxygen documentation ... "
	@$(RM) -r $(DOC_DIR)/Doxyfile $(DOC_DIR)/html $(DOC_DIR)/latex
	@mkdir -p $(DOC_DIR)
	@doxygen -g $(DOC_DIR)/Doxyfile > /dev/null 2>&1
	@sed -i 's:PROJECT_NAME           = "My Project":PROJECT_NAME           = "A mesh simplification procedure by iterative edge collapsing":' $(DOC_DIR)/Doxyfile
	@sed -i 's:OUTPUT_DIRECTORY       =:OUTPUT_DIRECTORY       = $(DOC_DIR):' $(DOC_DIR)/Doxyfile
	@sed -i 's:INPUT                  =:INPUT                  = $(LIB_INC_DIR) $(LIB_INC_DIR)/implementation $(LIB_INC_DIR)/inline $(LIB_SRC_DIR):' $(DOC_DIR)/Doxyfile
	@sed -i 's:RECURSIVE              =:RECURSIVE              = YES:' $(DOC_DIR)/Doxyfile
	@doxygen $(DOC_DIR)/Doxyfile > /dev/null 2>&1
	@echo "done"
	
	
#
# Prevent object files to be deleted
#

.SECONDARY: $(TEST_OBJ) $(MAIN_OBJ)
	
	

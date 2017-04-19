# Makefile to automate compilation of the meshsimplification library.

#
# Directories
#

# Directories storing header and source files
INC_DIR=include
SRC_DIR=src

# Directories which will store object files for the library
OBJ_DIR=build
S_OBJ_DIR=$(OBJ_DIR)/static
D_OBJ_DIR=$(OBJ_DIR)/dynamic

# Directory storing third-part libraries (if any) and
# which will store the meshsimplification library
LIB_DIR=lib
# Path to Eigen directory
EIGEN_DIR=$(mkEigenInc)

# Directory storing the documentation; Doxygen files will be
# placed here
DOC_DIR=doc


#
# Files
#

# Name and version of the library
LIB=meshsimplification
VRS=

# Header, source and object files for the meshsimplification library
INC=$(wildcard $(INC_DIR)/*.hpp $(INC_DIR)/implementation/*.hpp $(INC_DIR)/inline/*.hpp)
SRC=$(wildcard $(SRC_DIR)/*.cpp)
S_OBJ=$(patsubst $(SRC_DIR)/%.cpp, $(S_OBJ_DIR)/%.o, $(SRC))
D_OBJ=$(patsubst $(SRC_DIR)/%.cpp, $(D_OBJ_DIR)/%.o, $(SRC)) 


#
# Flags
#

# Compilation mode; default is RELEASE
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
PKG_CXX=g++

# Check whether the Eigen directory actually exists
ifneq ("$(wildcard $(EIGEN_DIR)/Eigen/Eigen)","")
	EIGEN_INC=$(EIGEN_DIR)
else
	EIGEN_INC=$(LIB_DIR)/Eigen
endif

# Flags for the compiler
ifeq ($(RELEASE),yes)
	PKG_CXXFLAGS=-std=c++11 -DNDEBUG -O3 -ftree-vectorize -I $(INC_DIR) -I $(EIGEN_INC)
else
	PKG_CXXFLAGS=-std=c++11 -g -Werror -I $(INC_DIR) -I $(EIGEN_INC)
endif
	
ifeq ($(ENABLE_SELF_INTERSECTIONS),yes)
	PKG_CXXFLAGS+= -DENABLE_SELF_INTERSECTIONS 
endif
	
# Flags for the linker
LDFLAGS=-L $(LIB_DIR)
ifeq ($(STATIC_STL),yes)
	LDFLAGS+= -static-libstdc++
endif


#
# Targets
#

all: install clean

#
# Build static library
#

$(S_OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(INC_DIR)/%.hpp $(INC)
	$(PKG_CXX) $(CXXFLAGS) -c -o $@ $< $(LDFLAGS)

static: folders $(S_OBJ)
	ar -r -s $(LIB_DIR)/lib$(LIB).a $(S_OBJ) > /dev/null 2>&1
	@echo "\033[92mStatic version of the meshsimplification library successfully built\n\033[0m"
	
#
# Build dynamic library
#

$(D_OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(INC_DIR)/%.hpp $(INC)
	$(PKG_CXX) $(PKG_CXXFLAGS) -fpic -c -o $@ $< $(LDFLAGS)
	
dynamic: folders $(D_OBJ)
	$(PKG_CXX) $(PKG_CXXFLAGS) -shared -o lib$(LIB).so$(VRS) $(D_OBJ) $(LDFLAGS) 
	@mv -f lib$(LIB).so$(VRS) $(LIB_DIR)
	@echo "\033[92mDynamic version of the meshsimplification library successfully built\n\033[0m"
	
install: dynamic
	@cp $(LIB_DIR)/lib$(LIB).so$(VRS) /usr/lib

	
#
# Phony targets
#

.PHONY: clean distclean folders doc

#
# Clean everything
#

clean:
	@$(RM) -r $(OBJ_DIR) $(LIB_DIR)/lib$(LIB).* $(DOC_DIR)/Doxyfile $(DOC_DIR)/html $(DOC_DIR)/latex 
	
distclean: clean
	@$(RM) /usr/lib/lib$(LIB)*
	
#
# Create folders
#
		
folders: 
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(S_OBJ_DIR)
	@mkdir -p $(D_OBJ_DIR)
	
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

	
	

/*! \file 	inc.hpp
	\brief 	Definitions of macros, typedef's and namespaces used throughout the library. */
	
#ifndef HH_INC_HH
#define HH_INC_HH

namespace geometry
{
	//
	// Macros
	//
	
	/*! Disable inlining in debug phase. 
		Remember that constexpr implies inline. */
	#ifndef NDEBUG
		#define INLINE
		#define CONSTEXPR
	#else
		#define INLINED
		#define INLINE inline
		#define CONSTEXPR constexpr
	#endif
	
	/*! Tolerance. */
	#define TOLL 1e-15
	
	/*! Maximum number of nodes a mesh class can store. */
	#define MAX_NUM_NODES 1e8
	
	/*! Maximum number of elements a mesh class can store. */
	#define MAX_NUM_ELEMS 1e8
		
	//
	// Aliases
	//
	
	using UInt = unsigned int;
	using Real = double;

	//
	// Namespaces
	//
	
	using namespace std;
}

#endif

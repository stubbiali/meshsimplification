/*!	\file	inline_intersection.hpp
	\brief	Definitions of inlined members of class intersection<Triangle>. */
	
#ifndef HH_INLINEINTERSECTION_HH
#define HH_INLINEINTERSECTION_HH

namespace geometry
{
	//
	// Access mesh
	//
	
	INLINE void intersection<Triangle>::setMesh(const bmesh<Triangle> * pg)
	{
		grid = pg;
	}
}

#endif

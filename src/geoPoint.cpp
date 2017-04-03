/*!	\file	geoPoint.cpp
	\brief	Specializations of some friend functions of class geoPoint. */
	
#include "geoPoint.hpp"

// Include definitions of inlined friend functions
#ifndef INLINED
#include "inline/inline_geoPoint.hpp"
#endif

namespace geometry
{
	//
	// Constructors
	//
	
	// Specialization for two-dimensional points
	template<>
	geoPoint<2>::geoPoint(const Real & val) :
		coor{{val,val}}
	{
	}
	
	
	// Specialization for three-dimensional points
	template<>
	geoPoint<3>::geoPoint(const Real & val) :
		coor{{val,val,val}}
	{
	}
}

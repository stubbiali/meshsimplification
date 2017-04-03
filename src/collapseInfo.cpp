/*!	\file	collapseInfo.cpp
	\brief	Implementations of members and friend functions of class collapseInfo. */
	
#include "collapseInfo.hpp"

// Include implementations of inlined class members and friend functions
#ifndef INLINED
#include "inline/inline_collapseInfo.hpp"
#endif

namespace geometry
{
	// 
	// Constructor
	//
	
	collapseInfo::collapseInfo(const UInt & id1, const UInt & id2, const Real & val, const point3d & cp) :
		Id1(id1), Id2(id2), cPoint(cp), cost(val)
	{
	}
	
	
	collapseInfo::collapseInfo(const vector<UInt> & ids, const Real & val, const point3d & cp) :
		Id1(ids[0]), Id2(ids[1]), cPoint(cp), cost(val)
	{
	}
	
	
	//
	// Operators
	//
	
	collapseInfo & collapseInfo::operator=(const collapseInfo & cInfo)
	{
		// Copy end-points Id's
		Id1 = cInfo.Id1;
		Id2 = cInfo.Id2;
		
		// Copy collapsing point
		cPoint = cInfo.cPoint;
		
		// Copy collapsing cost
		cost = cInfo.cost;
		
		return *this;
	}
}

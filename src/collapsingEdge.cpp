/*!	\file	collapsingEdge.cpp
	\brief	Implementations of members and friend functions of class collapsingEdge. */
	
#include <set>

#include "collapsingEdge.hpp"

// Include implementations of inlined class members and friend functions
#ifndef INLINED
#include "inline/inline_collapsingEdge.hpp"
#endif

namespace geometry
{
	// 
	// Constructor
	//
	
	collapsingEdge::collapsingEdge(const UInt & id1, const UInt & id2, const Real & val, const point3d & cp) :
		Id1(id1), Id2(id2), cPoint(cp), cost(val)
	{
	}
	
	
	collapsingEdge::collapsingEdge(const vector<UInt> & ids, const Real & val, const point3d & cp) :
		Id1(ids[0]), Id2(ids[1]), cPoint(cp), cost(val)
	{
	}
	
	
	//
	// Operators
	//
	
	collapsingEdge & collapsingEdge::operator=(const collapsingEdge & cEdge)
	{
		// Copy end-points Id's
		Id1 = cEdge.Id1;
		Id2 = cEdge.Id2;
		
		// Copy collapsing point
		cPoint = cEdge.cPoint;
		
		// Copy collapsing cost
		cost = cEdge.cost;
		
		return *this;
	}
}

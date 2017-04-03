/*!	\file	inline_collapsingEdge.hpp
	\brief	Implementations of inlined members and friend functions 
			of class collapsingEdge. */
	
#ifndef HH_INLINECOLLAPSINGEDGE_HH
#define HH_INLINECOLLAPSINGEDGE_HH

#include <tuple>

namespace geometry
{
	//
	// Operators
	//
	
	INLINE bool operator<(const collapsingEdge & cEdge1, const collapsingEdge & cEdge2)
	{
		// If the costs are equal, compare the Id's
		// Otherwise, compare the costs
		return (cEdge1.cost != cEdge2.cost) ? (cEdge1.cost < cEdge2.cost) :
			(tie(cEdge1.Id1, cEdge1.Id2) < tie(cEdge2.Id1, cEdge2.Id2));
	}
	
	
	//
	// Get methods
	//
	
	INLINE UInt collapsingEdge::getId1() const
	{
		return Id1;
	}
	
	
	INLINE UInt collapsingEdge::getId2() const
	{
		return Id2;
	}
	
	
	INLINE point collapsingEdge::getCollapsingPoint() const
	{
		return cPoint;
	}
	
	
	INLINE Real collapsingEdge::getCost() const
	{
		return cost;
	}
	
	
	//
	// Set methods
	//
	
	INLINE void collapsingEdge::setId1(const UInt & id1)
	{
		Id1 = id1;
	}
	
	
	INLINE void collapsingEdge::setId2(const UInt & id2)
	{
		Id2 = id2;
	}
	
	
	INLINE void collapsingEdge::setCollapsingPoint(const point3d & cp)
	{
		cPoint = cp;
	}
	
	
	INLINE void collapsingEdge::setCost(const Real & val)
	{
		cost = val;
	}
}

#endif

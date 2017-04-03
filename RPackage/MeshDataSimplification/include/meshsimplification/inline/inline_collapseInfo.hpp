/*!	\file	inline_collapseInfo.hpp
	\brief	Implementations of inlined members and friend functions 
			of class collapseInfo. */
	
#ifndef HH_INLINECOLLAPSEINFO_HH
#define HH_INLINECOLLAPSEINFO_HH

namespace geometry
{
	//
	// Operators
	//
	
	INLINE bool operator==(const collapseInfo & cInfo1, const collapseInfo & cInfo2)
	{
		return (((cInfo1.Id1 == cInfo2.Id1) && (cInfo1.Id2 == cInfo2.Id2))
			|| ((cInfo1.Id1 == cInfo2.Id2) && (cInfo1.Id2 == cInfo2.Id1)));
	}
	
	
	//
	// Get methods
	//
	
	INLINE UInt collapseInfo::getId1() const
	{
		return Id1;
	}
	
	
	INLINE UInt collapseInfo::getId2() const
	{
		return Id2;
	}
	
	
	INLINE point collapseInfo::getCollapsingPoint() const
	{
		return cPoint;
	}
	
	
	INLINE Real collapseInfo::getCost() const
	{
		return cost;
	}
	
	
	//
	// Set methods
	//
	
	INLINE void collapseInfo::setId1(const UInt & id1)
	{
		Id1 = id1;
	}
	
	
	INLINE void collapseInfo::setId2(const UInt & id2)
	{
		Id2 = id2;
	}
	
	
	INLINE void collapseInfo::setCollapsingPoint(const point3d & cp)
	{
		cPoint = cp;
	}
	
	
	INLINE void collapseInfo::setCost(const Real & val)
	{
		cost = val;
	}
}

#endif

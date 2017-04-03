/*! \file 	inline_searchPoint.hpp
	\brief 	Definitions of members and friend functions of class searchPoint. */
	
#ifndef HH_INLINESEARCHPOINT_HH
#define HH_INLINESEARCHPOINT_HH

namespace geometry
{
	//
	// Definitions of inlined members
	//
	
	INLINE UInt & searchPoint::operator[](const UInt & i) 
	{
		return idx[i];
	}
	
	
	INLINE UInt searchPoint::operator[](const UInt & i) const 
	{
		return idx[i];
	}
	
	
	INLINE UInt searchPoint::getId() const 
	{
		return Id;
	}
			
	
	INLINE point3d searchPoint::getGlobalNE() 
	{
		return searchPoint::NE_global;
	}
				
	
	INLINE point3d searchPoint::getGlobalSW() 
	{
		return searchPoint::SW_global;
	}
	
	
	INLINE Real searchPoint::getCellSize(const UInt & i) 
	{
		return searchPoint::cellSize[i];
	}
	
	
	INLINE array<Real,3> searchPoint::getCellSize() 
	{
		return searchPoint::cellSize;
	}
	
	
	INLINE UInt searchPoint::getNumCells(const UInt & i) 
	{
		return searchPoint::numCells[i];
	}
	
	
	INLINE array<UInt,3> searchPoint::getNumCells() 
	{
		return searchPoint::numCells;
	}
	
	
	INLINE void searchPoint::setId(const UInt & idNew) 
	{
		Id = idNew;
	}
	
	
	//
	// Definitions of inlined friend functions
	//
	
	INLINE searchPoint & searchPoint::operator=(const searchPoint & V)
	{
		copy(V.idx.cbegin(), V.idx.cend(), idx.begin());
		return *this;
	}
			
	INLINE bool operator!=(const searchPoint & pA, const searchPoint & pB) 
	{
		return (pA.idx != pB.idx);
	}
	
	
	INLINE bool operator==(const searchPoint & pA, const searchPoint & pB) 
	{
		return (pA.idx == pB.idx);
	}
}

#endif

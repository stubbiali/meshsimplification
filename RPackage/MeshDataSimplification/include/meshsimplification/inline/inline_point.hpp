/*! \file 	inline_point.hpp
	\brief 	Definitions of inlined members of class point. */
	
#ifndef HH_INLINEPOINT_HH
#define HH_INLINEPOINT_HH

#include <algorithm>
	
namespace geometry
{
	//
	// Get methods
	//
	
	INLINE UInt point::getId() const 
	{
		return Id;
	}


	INLINE UInt point::getBoundary() const 
	{
		return boundary;
	}
	
	
	INLINE bool point::isActive() const
	{
		return active;
	}


	//
	// Set methods
	//
	
	INLINE void point::setId(const UInt & newId) 
	{
		Id = newId;
	}


	INLINE void point::setBoundary(const UInt & newBond) 
	{
		boundary = newBond;
	}
	
	
	INLINE void point::setActive()
	{
		active = true;
	}
	
	
	INLINE void point::setInactive()
	{
		active = false;
	}
	
	
	INLINE void point::setCoor(const array<Real,3> & newCoor)
	{
		copy(newCoor.begin(), newCoor.end(), this->coor.begin());
	}
}

#endif

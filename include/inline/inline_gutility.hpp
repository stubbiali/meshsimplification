/*!	\file	inline_gutility.hpp
	\brief	Implementations of inlined members of class gutility. */
	
#ifndef HH_INLINEGUTILITY_HH
#define HH_INLINEGUTILITY_HH

namespace geometry
{
	INLINE Real gutility::getTriArea2d(const point2d & a, 
		const point2d & b, const point2d & c)
	{
		return 0.5 * ((b - a)^(c - b))[2];
	}
}

#endif

/*! \file 	inline_dataPoint.hpp
	\brief 	Definitions of inlined members for class dataPoint. */
	
#ifndef HH_INLINEDATAPOINT_HH
#define HH_INLINEDATAPOINT_HH

namespace geometry
{
	INLINE Real dataPoint::getDatum() const
	{
		return datum;
	}
	
	
	INLINE void dataPoint::setDatum(const Real & dat)
	{
		datum = dat;
	}
}

#endif

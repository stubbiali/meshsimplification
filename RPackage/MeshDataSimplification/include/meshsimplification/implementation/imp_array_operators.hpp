/*!	\file	imp_array_operators.hpp
	\brief	Implementations of some mathematical operators for STL array's. */
	
#ifndef HH_IMPARRAYOPERATORS_HH
#define HH_IMPARRAYOPERATORS_HH

#include <algorithm>
#include <functional>

namespace geometry
{
	template<ULInt N>
	array<Real,N> operator+(const array<Real,N> & a, const array<Real,N> & b)
	{
		array<Real,N> c;
		transform(a.cbegin(), a.cend(), b.cbegin(), c.begin(), plus<Real>());
		return c;
	}
	
	
	template<ULInt N>
	array<Real,N> & operator+=(array<Real,N> & a, const array<Real,N> & b)
	{
		transform(a.begin(), a.end(), b.cbegin(), a.begin(), plus<Real>());
		return a;
	}
	
	
	template<ULInt N>
	array<Real,N> operator*(const Real & s, const array<Real,N> & a)
	{
		array<Real,N> res;
		transform(a.cbegin(), a.cend(), res.begin(), [s](const Real & el){ return s*el; });
		return res;
	}
	
	
	template<ULInt N>
	INLINE array<Real,N> operator*(const array<Real,N> & a, const Real & s)
	{
		return s*a;
	}
}

#endif

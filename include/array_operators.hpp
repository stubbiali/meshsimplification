/*!	\file	array_operators.hpp
	\brief	Some mathematical operators for STL array's. 
			These may be particularly useful for cost classes.
			
			\sa bcost.cpp, OnlyGeo.hpp, DataGeo.hpp */
	
#ifndef HH_ARRAYOPERATORS_HH
#define HH_ARRAYOPERATORS_HH

#include <array>

#include "inc.hpp"

namespace geometry
{
	/*!	A typedef to simplify notation. */
	using ULInt = unsigned long int;
	
	/*!	Addition operator.
		\param a	first array
		\param b	second array
		\return		the sum */
	template<ULInt N>
	array<Real,N> operator+(const array<Real,N> & a, const array<Real,N> & b);
	
	/*!	Addition assignment operator.
		\param a	first array
		\param b	second array
		\return		the sum */
	template<ULInt N>
	array<Real,N> & operator+=(array<Real,N> & a, const array<Real,N> & b);
	
	/*!	Multiplication by a scalar.
		\param s	the scalar
		\param a	the array
		\return		the multiplication */
	template<ULInt N>
	array<Real,N> operator*(const Real & s, const array<Real,N> & a);
	
	/*!	Multiplication by a scalar.
		\param a	the array
		\param s	the scalar
		\return		the multiplication */
	template<ULInt N>
	array<Real,N> operator*(const array<Real,N> & a, const Real & s);
}

/*!	Include implementations of the operators. */
#include "implementation/imp_array_operators.hpp"

#endif

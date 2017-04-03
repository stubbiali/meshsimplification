/*!	\file	imp_geoPoint.hpp
	\brief	Implementations of members and friend functions of class geoPoint. 
			For some specializations for two- and three-dimensional point,
			see geoPoint.cpp */
	
#ifndef HH_IMPGEOPOINT_HH
#define HH_IMPGEOPOINT_HH

#include <algorithm>
#include <numeric>
#include <iterator>
#include <cmath>

namespace geometry
{
	//
	// Constructors
	//
	
	template<UInt N>
	geoPoint<N>::geoPoint(const Real & val)
	{
		coor.fill(val);
	}
	
	
	// Declare specialization for two-dimensional points
	template<>
	geoPoint<2>::geoPoint(const Real & val);
	
	
	// Declare specialization for three-dimensional points
	template<>
	geoPoint<3>::geoPoint(const Real & val);
	
	
	template<UInt N>
	geoPoint<N>::geoPoint(const Real & x, const Real & y) :
		coor{{x,y}}
	{
		static_assert(N == 2,
			"This constructor is provided only for two-dimensional points.");
	}
		
	
	template<UInt N>
	geoPoint<N>::geoPoint(const Real & x, const Real & y, const Real & z) :
		coor{{x,y,z}}
	{
		static_assert(N == 3,
			"This constructor is provided only for three-dimensional points.");
	}
	
	
	template<UInt N>
	geoPoint<N>::geoPoint(initializer_list<Real> lst)
	{
		typename array<Real,N>::size_type i = 0;
		auto it = lst.begin();
		
		// Fill the coordinates
		for ( ; i < N && it != lst.end(); ++i, ++it)
			coor[i] = *it;
			
		// If the size of the initializer list is smaller than
		// the spatial dimension, set the remaining coordinates 
		// to zero
		for ( ; i < N; ++i)
			coor[i] = 0.;
	}
	
	
	template<UInt N>
	geoPoint<N>::geoPoint(const array<Real,N> & c) :
		coor(c)
	{
	}
	
	
	template<UInt N>
	template<UInt DIM>
	geoPoint<N>::geoPoint(const geoPoint<DIM> & gp) 
	{
		typename array<Real,N>::size_type i = 0;
				
		// Fill the coordinates
		for ( ; i < N && i < DIM; ++i)
			coor[i] = gp[i];
			
		// If N > DIM, set the remaining coordinates to zero
		for ( ; i < N; ++i)
			coor[i] = 0.;
	}
	
	
	//
	// Operators
	//
		
	template<UInt N>
	INLINE geoPoint<N> & geoPoint<N>::operator=(const geoPoint<N> & gp)
	{
		copy(gp.coor.cbegin(), gp.coor.cend(), coor.begin());
		return *this;
	}
	
	
	template<UInt N>
	geoPoint<N> operator+(const geoPoint<N> & gpA, const geoPoint<N> & gpB)
	{
		// Store new coordinates in an array
		array<Real,N> newCoor;
		transform(gpA.coor.cbegin(), gpA.coor.cend(), gpB.coor.cbegin(),
			newCoor.begin(), plus<Real>());
			
		return geoPoint<N>(newCoor);
	}
	
	
	// Declare specialization for two-dimensional points 
	template<>
	geoPoint<2> operator+(const geoPoint<2> & gpA, const geoPoint<2> & gpB);
	
	
	// Declare specialization for three-dimensional points 
	template<>
	geoPoint<3> operator+(const geoPoint<3> & gpA, const geoPoint<3> & gpB);
				
		
	template<UInt N>
	geoPoint<N> operator-(const geoPoint<N> & gpA, const geoPoint<N> & gpB)
	{
		// Store new coordinates in an array
		array<Real,N> newCoor;
		transform(gpA.coor.cbegin(), gpA.coor.cend(), gpB.coor.cbegin(),
			newCoor.begin(), minus<Real>());
			
		return geoPoint<N>(newCoor);
	}
	
	
	// Declare specialization for two-dimensional points 
	template<>
	geoPoint<2> operator-(const geoPoint<2> & gpA, const geoPoint<2> & gpB);
	
	
	// Declare specialization for three-dimensional points 
	template<>
	geoPoint<3> operator-(const geoPoint<3> & gpA, const geoPoint<3> & gpB);
			
		
	template<UInt N>
	geoPoint<3> operator^(const geoPoint<N> & gpA, const geoPoint<N> & gpB)
	{
		static_assert(N == 2 || N == 3,
			"Cross product defined only for two- and three-dimensional points.");
		return {};
	}
	
	
	// Declare specialization for two-dimensional points
	template<>
	geoPoint<3> operator^(const geoPoint<2> & gpA, const geoPoint<2> & gpB);
	
	
	// Declare specialization for three-dimensional points
	template<>
	geoPoint<3> operator^(const geoPoint<3> & gpA, const geoPoint<3> & gpB);
		
		
	template<UInt N>
	geoPoint<N> operator/(const geoPoint<N> & gp, const Real & a)
	{
		// Store new coordinates in an array
		array<Real,N> newCoor;
		transform(gp.coor.cbegin(), gp.coor.cend(), newCoor.begin(),
			[=](const Real & el){ return el/a; });
			
		return geoPoint<N>(newCoor);
	}
	
	
	// Declare specialization for two-dimensional points (to avoid any for loop)
	template<>
	geoPoint<2> operator/(const geoPoint<2> & gp, const Real & a);
			
		
	// Declare specialization for three-dimensional points (to avoid any for loop)
	template<>
	geoPoint<3> operator/(const geoPoint<3> & gp, const Real & a);
	
	
	template<UInt N>
	geoPoint<N> operator*(const geoPoint<N> & gp, const Real & a)
	{
		// Store new coordinates in an array
		array<Real,N> newCoor;
		transform(gp.coor.cbegin(), gp.coor.cend(), newCoor.begin(),
			[=](const Real & el){ return el*a; });
			
		return geoPoint<N>(newCoor);
	}
	
	
	// Declare specialization for two-dimensional points (to avoid any for loop)
	template<>
	geoPoint<2> operator*(const geoPoint<2> & gp, const Real & a);
			
		
	// Declare specialization for three-dimensional points (to avoid any for loop)
	template<>
	geoPoint<3> operator*(const geoPoint<3> & gp, const Real & a);
			
	
	template<UInt N>
	INLINE geoPoint<N> operator*(const Real & a, const geoPoint<N> & gp)
	{
		return (gp * a);
	}
	
	
	template<UInt N>
	INLINE Real operator*(const geoPoint<N> & gpA, const geoPoint<N> & gpB)
	{
		return inner_product(gpA.coor.cbegin(), gpA.coor.cend(), gpB.coor.cbegin(), 0.);
	}
	
	
	template<UInt N>	
	bool operator<(const geoPoint<N> & gpA, const geoPoint<N> & gpB) 
	{
		for (typename array<Real,N>::size_type i = 0; i < N; ++i) 
		{
			// If the i-th coordinate of the first point is greater
			// or equal than the i-th coordinate of the second point,
			// return false
			if (gpA.coor[i] >= (gpB.coor[i] - TOLL))
				return false;
		}
		
		// gpA is smaller than gpB
		return true;
	}
	
	
	// Declare specialization for two-dimensional points (to avoid any for loop)
	template<>
	bool operator<(const geoPoint<2> & gpA, const geoPoint<2> & gpB);
	
	
	// Declare specialization for three-dimensional points (to avoid any for loop)
	template<>
	bool operator<(const geoPoint<3> & gpA, const geoPoint<3> & gpB);
	
	
	template<UInt N>	
	bool operator<=(const geoPoint<N> & gpA, const geoPoint<N> & gpB) 
	{
		for (typename array<Real,N>::size_type i = 0; i < N; ++i) 
		{
			// If the i-th coordinate of the first point is greater
			// than the i-th coordinate of the second point,
			// return false
			if (gpA.coor[i] > (gpB.coor[i] + TOLL))
				return false;
		}
		
		// gpA is smaller or equal than gpB
		return true;
	}
	
	
	// Declare specialization for two-dimensional points (to avoid any for loop)
	template<>
	bool operator<=(const geoPoint<2> & gpA, const geoPoint<2> & gpB);
	
	
	// Declare specialization for three-dimensional points (to avoid any for loop)
	template<>
	bool operator<=(const geoPoint<3> & gpA, const geoPoint<3> & gpB);
		
	
	template<UInt N>	
	bool operator>(const geoPoint<N> & gpA, const geoPoint<N> & gpB) 
	{
		for (typename array<Real,N>::size_type i = 0; i < N; ++i) 
		{
			// If the i-th coordinate of the first point is smaller
			// or equal than the i-th coordinate of the second point,
			// return false
			if (gpA.coor[i] <= (gpB.coor[i] + TOLL))
				return false;
		}
		
		// gpA is greater than gpB
		return true;
	}
	
	
	// Declare specialization for two-dimensional points (to avoid any for loop)
	template<>
	bool operator>(const geoPoint<2> & gpA, const geoPoint<2> & gpB);
	
	
	// Declare specialization for three-dimensional points (to avoid any for loop)
	template<>
	bool operator>(const geoPoint<3> & gpA, const geoPoint<3> & gpB);
		
	
	template<UInt N>	
	bool operator>=(const geoPoint<N> & gpA, const geoPoint<N> & gpB) 
	{
		for (typename array<Real,N>::size_type i = 0; i < N; ++i) 
		{
			// If the i-th coordinate of the first point is smaller
			// than the i-th coordinate of the second point,
			// return false
			if (gpA.coor[i] < (gpB.coor[i] - TOLL))
				return false;
		}
		
		// gpA is greater or equal than gpB
		return true;
	}
	
	
	// Declare specialization for two-dimensional points (to avoid any for loop)
	template<>
	bool operator>=(const geoPoint<2> & gpA, const geoPoint<2> & gpB);
	
	
	// Declare specialization for three-dimensional points (to avoid any for loop)
	template<>
	bool operator>=(const geoPoint<3> & gpA, const geoPoint<3> & gpB);
		
	
	template<UInt N>	
	bool operator!=(const geoPoint<N> & gpA, const geoPoint<N> & gpB) 
	{
		for (typename array<Real,N>::size_type i = 0; i < N; ++i) 
		{
			if (abs(gpA.coor[i] - gpB.coor[i]) > TOLL)
				return true;
		}
		
		// The two points are equal
		return false;
	}
	
	
	template<UInt N>	
	bool operator==(const geoPoint<N> & gpA, const geoPoint<N> & gpB) 
	{
		for (typename array<Real,N>::size_type i = 0; i < N; ++i) 
		{
			if (abs(gpA.coor[i] - gpB.coor[i]) > TOLL)
				return false;
		}
		
		// The two points are equal
		return true;
	}
	
	
	template<UInt N>
	INLINE Real & geoPoint<N>::operator[](const UInt & i)
	{
		return coor[i];
	}
	
	
	template<UInt N>
	INLINE Real geoPoint<N>::operator[](const UInt & i) const
	{
		return coor[i];
	}
	
	
	template<UInt N>
	template<UInt DIM>
	geoPoint<N>::operator geoPoint<DIM>() const
	{
		array<Real,DIM> newCoor;
		
		// Start copying the coordinates
		typename array<Real,DIM>::size_type i = 0;
		for ( ; i < DIM && i < N; ++i)
			newCoor[i] = coor[i];
			
		// If DIM > N, set the remaining coordinates to zero
		for ( ; i < DIM; ++i)
			newCoor[i] = 0.;
			
		return geoPoint<DIM>(newCoor);
	}
	
	
	template<UInt N>
	ostream & operator<<(ostream & out, const geoPoint<N> & gp)
	{
		out << "[ ";
		for (auto el : gp.coor)
			out << el << " ";
		out << "]";
		return out;
	}
	
	
	//
	// Norm methods
	//
	
	template<UInt N>
	INLINE Real geoPoint<N>::norm2() const
	{
		return sqrt(inner_product(coor.cbegin(), coor.cend(), coor.cbegin(), 0.));
	}
	
	
	template<UInt N>
	geoPoint<N> & geoPoint<N>::normalize()
	{
		auto len = norm2();
		
		// Make sure the point is not the origin
		if (len != 0.)
			transform(coor.begin(), coor.end(), coor.begin(),
				[=](const Real & el){ return el/len; });
					
		return *this;
	}
	
	
	//
	// Get methods
	//
	
	template<UInt N>
	INLINE array<Real,N> geoPoint<N>::getCoor() const
	{
		return coor;
	}
	
	
	template<UInt N>
	UInt geoPoint<N>::getMaxCoor() const
	{
		array<Real,N> absCoor;
		transform(coor.cbegin(), coor.cend(), absCoor.begin(),
			[](const Real & el){ return abs(el); });
		return distance(absCoor.begin(), max_element(absCoor.begin(), absCoor.end()));
	}
	
	
	//
	// Set methods
	//
	
	template<UInt N>
	INLINE void geoPoint<N>::reset(const Real & val)
	{
		coor.fill(val);
	}
}

#endif

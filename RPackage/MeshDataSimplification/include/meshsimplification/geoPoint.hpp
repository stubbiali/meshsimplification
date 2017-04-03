/*!	\file	geoPoint.hpp
	\brief	A class storing an N-dimensional point. */
	
#ifndef HH_GEOPOINT_HH
#define HH_GEOPOINT_HH

#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>
#include <initializer_list>

#include "shapes.hpp"

namespace geometry
{
	/*!	This class just stores the coordinates of the point.
		It is templated in the spatial dimension.
		This class is not suitable for representing a node of
		a mesh. For that task, the user may find the class
		point - inheriting geoPoint<3> - useful.
		
		For the moment, except for a couple of methods this 
		class is designed to deal with points all having the 
		same spatial dimension as its one.
		Dealing with points belonging to different Euclidean
		spaces may be tackled in future. 
		
		\sa point.hpp */
	template<UInt N>
	class geoPoint : public simplePoint
	{
		protected:
			/*!	The coordinates. */
			array<Real,N> coor;
			
		public:
			//
			// Constructors
			//
			
			/*! Synthetic default constructor. */
			geoPoint() = default;
				
			/*!	Constructor.
				\param val	initialization value for all coordinates */
			geoPoint(const Real & val);
					
			/*!	Constructor.
				Provided only for two-dimensional points.
				
				\param x	x-coordinate
				\param y	y-coordinate */
			geoPoint(const Real & x, const Real & y);
			
			/*!	Constructor.
				Provided only for three-dimensional points.
				
				\param x	x-coordinate
				\param y	y-coordinate 
				\param z	z-coordinate */
			geoPoint(const Real & x, const Real & y, const Real & z);
			
			/*!	Constructor.
				\param lst	initializer list */
			geoPoint(initializer_list<Real> lst);
			
			/*! Constructor.
				\param c	array of coordinates */
			geoPoint(const array<Real,N> & c);
			
			/*!	Constructor.
				\param c	array of coordinates;
							the number of coordinates may not coincide
							with the dimension N of the point; only the
							first N inputs are taken into account */
			template<UInt DIM>
			geoPoint(const array<Real,DIM> & c);
			
			/*!	Synthetic copy constructor.
				\param gp	another point */
			geoPoint(const geoPoint & gp) = default;
			
			/*!	Copy constructor.
				The input point may have a different dimension than
				the current one. Only the first N dimensions are taken
				into account. 
				
				\param gp	another point */
			template<UInt DIM>
			geoPoint<N>(const geoPoint<DIM> & gp);
			
			/*!	Synthetic destructor. */
			virtual ~geoPoint() = default;
			
			//
			// Operators
			//
			
			/*!	Copy-assignment operator. 
				\param gp	another point
				\return		the current point updated */
			virtual geoPoint & operator=(const geoPoint & gp);
			
			/*! Sum operator. 
				\param gpA	first point
				\param gpB	second point 
				\return 	result */
			template<UInt DIM>
			friend geoPoint<DIM> operator+(const geoPoint<DIM> & gpA, const geoPoint<DIM> & gpB);

			/*! Difference operator. 
				\param gpA	first point
				\param gpB	second point 
				\return 	result */
			template<UInt DIM>
			friend geoPoint<DIM> operator-(const geoPoint<DIM> & gpA, const geoPoint<DIM> & gpB);

			/*! Cross product. 
				This operator is provided only for two- and three-dimensional points.
				
				\param gpA	first point
				\param gpB	second point 
				\return 	result */
			template<UInt DIM>
			friend geoPoint<3> operator^(const geoPoint<DIM> & gpA, const geoPoint<DIM> & gpB);

			/*! Division by a scalar. 
				\param gp	point
				\param a	scalar 
				\return 	result */
			template<UInt DIM>
			friend geoPoint<DIM> operator/(const geoPoint<DIM> & gp, const Real & a);
			
			/*! Product by a scalar. 
				\param gp	point
				\param a	scalar 
				\return 	result */
			template<UInt DIM>
			friend geoPoint<DIM> operator*(const geoPoint<DIM> & gp, const Real & a);
			
			/*! Product by a scalar. 
				\param a	scalar 
				\param gp	point
				\return 	result */
			template<UInt DIM>
			friend geoPoint<DIM> operator*(const Real & a, const geoPoint<DIM> & gp);
			
			/*! Scalar product. 
				\param gpA	first point
				\param gpB	second point 
				\return 	result */
			template<UInt DIM>
			friend Real operator*(const geoPoint<DIM> & gpA, const geoPoint<DIM> & gpB);

			/*! Less than operator: a point is "less" than another 
				if all its coordinates are smaller than the corresponding
				ones of the other point. 
				
				\param gpA	LHS
				\param gpB	RHS 
				\return 	bool reporting the result */
			template<UInt DIM>
			friend bool operator<(const geoPoint<DIM> & gpA, const geoPoint<DIM> & gpB);
			
			/*! Less or equal than operator: a point is "less" than another 
				if all its coordinates are smaller than the corresponding
				ones of the other point. 
				
				\param gpA	LHS
				\param gpB	RHS 
				\return 	bool reporting the result */
			template<UInt DIM>
			friend bool operator<=(const geoPoint<DIM> & gpA, const geoPoint<DIM> & gpB);
			
			/*! Greater than operator: a point is "greater" than another 
				if all its coordinates are greater than the corresponding
				ones of the other point. 
				
				\param gpA	LHS
				\param gpB	RHS 
				\return 	bool reporting the result */
			template<UInt DIM>
			friend bool operator>(const geoPoint<DIM> & gpA, const geoPoint<DIM> & gpB);
			
			/*! Greater or equal than operator: a point is "greater" than another 
				if all its coordinates are greater than the corresponding
				ones of the other point. 
				
				\param gpA	LHS
				\param gpB	RHS 
				\return 	bool reporting the result */
			template<UInt DIM>
			friend bool operator>=(const geoPoint<DIM> & gpA, const geoPoint<DIM> & gpB);

			/*! Inequality operator: two points are equal if their components 
				are equal up to the geometric tolerance. 
				
				\param gpA	LHS
				\param gpB	RHS 
				\return 	bool reporting the result */ 
			template<UInt DIM>
			friend bool operator!=(const geoPoint<DIM> & gpA, const geoPoint<DIM> & gpB);

			/*! Inequality operator: two points are equal if their components 
				are equal up to the geometric tolerance. 
				
				\param gpA	LHS
				\param gpB	RHS 
				\return 	bool reporting the result */
			template<UInt DIM>
			friend bool operator==(const geoPoint<DIM> & gpA, const geoPoint<DIM> & gpB);
			
			/*! Access operator (non const version).
				\param i	index of the coordinate
				\return		coordinate */
			Real & operator[](const UInt & i);
			
			/*! Access operator (const version).
				\param i	index of the coordinate
				\return		coordinate */
			Real operator[](const UInt & i) const;
			
			/*!	Conversion operator: convert to a point with a
				(possible) different spatial dimension. */
			template<UInt DIM>
			operator geoPoint<DIM>() const;
						
			/*! Output stream operator.
				\param out	output stream
				\param gp	point to print
				\return 	update output stream */
			template<UInt DIM>
			friend ostream & operator<<(ostream & out, const geoPoint<DIM> & gp);
									
			//
			// Norm methods
			//
				
			/*! The Euclidean norm of the vector.
				\return		the norm */
			Real norm2() const;

			/*! Normalize the coordinates vector. 
				\return		the normalized vector */
			geoPoint & normalize();
			
			//
			// Get methods
			//
			
			/*!	Get coordinates.
				\return		array with coordinates */
			array<Real,N> getCoor() const;
			
			/*!	Get the index of the greatest coordinate (in absolute value).
				\return		the greatest coordinate */
			UInt getMaxCoor() const;
			
			//
			// Set methods
			//
			
			/*!	Reset coordinates.
				\param val	new values to set all coordinates to */
			void reset(const Real & val = 0.);
	};
	
	/*!	A couple of typedef's for ease of notation. */
	using point2d = geoPoint<2>;
	using point3d = geoPoint<3>;
}

/*!	Include implementations of class members and friend functions. */
#include "implementation/imp_geoPoint.hpp"

/*!	Include specializations for some inlined friend functions. */
#ifdef INLINED
#include "inline/inline_geoPoint.hpp"
#endif

#endif

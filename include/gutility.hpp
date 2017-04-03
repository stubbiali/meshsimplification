/*!	\file	gutility.hpp
	\brief	A class providing different geometrical utilities
			as static members ("g" stands for geometrical).
			Moreover, some type-safe enumerations are defined.
			
			\sa intersect.hpp, project.hpp */
			
#ifndef HH_GUTILITY_HH
#define HH_GUTILITY_HH

#include <tuple>

#include "point.hpp"

namespace geometry
{
	/*! Relative position of a point with respect to a segment. */
	enum class Point2Seg {EXTERN, INTERN, ONVERTEX};
	
	/*! Relative position of a point with respect to a triangle. */
	enum class Point2Tri {EXTERN, INTERN, ONEDGE, ONVERTEX};
	
	/*! Relative position of a straight line with respect to a plane. */
	enum class Line2Plane {PARALLEL, COMPLANAR, INCIDENT};
	
	/*!	Type of intersection:
		<ol>
		<li> NONE:		the elements do not intersect;
		<li> VALID:		the elements intersect in a conformal way,
						i.e. they share a vertex or an entire edge;
		<li> UNVALID:	the elements intersect in a non-conformal way.
		<\ol> */
	enum class IntersectionType {NONE, VALID, INVALID};
	
	/*!	All class members are static, so that they can be used
		without the need of a class instantiation. 
		
		Reference: 
		O’Rourke J. "Computational geometry in C". 
		Cambridge (UK), Cambridge University Press, 1998. */
	class gutility
	{
		public:
			//
			// Two-dimensional methods
			//
			
			/*!	Get the signed area of a two-dimensional triangle.
				\param a	first vertex of the triangle
				\param b	second vertex of the triangle
				\param c	third vertex of the triangle
				\return		the signed area */
			static Real getTriArea2d(const point2d & a, const point2d & b, const point2d & c);
			
			/*!	Test if a two-dimensional point lays within a triangle. 
				\param p	the point
				\param a	first vertex of the triangle
				\param b	second vertex of the triangle
				\param c	third vertex of the triangle
				\return		relative position of the point w.r.t. the triangle */
			static Point2Tri inTri2d(const point2d & p, const point2d & a,
				const point2d & b, const point2d & c);
			
			/*!	Test if two segments in the plane intersect one each other. 
				\param q1	querying end-point of first segment
				\param r1	ray end-point of first segment
				\param q2	querying end-point of second segment
				\param r2	ray end-point of second segment 
				\return		intersection type */
			static IntersectionType intSegSeg2d(const point2d & q1, const point2d & r1,
				const point2d & q2, const point2d & r2);
			
			//
			// Three-dimensional methods
			//
			
			/*!	Test of a point lays within a three-dimensional triangle.
				\param P	the point
				\param A	first vertex of the triangle
				\param B	second vertex of the triangle
				\param C	thrid vertex of the triangle
				\return		relative position of the point w.r.t. the triangle */
			static Point2Tri inTri3d(const point3d & P, const point3d & A,
				const point3d & B, const point3d & C);
				
			/*!	Test if a segment intersect a plane.
				\param Q	querying end-point of the segment
				\param R	ray end-point of the segment 
				\param N	unit normal to the triangle 
				\param D	RHS term in the equation of the plane
							determined by the triangle
				\return		relative position line-plane
				\return		position of the intersection point \$ p \$
							(if any) in the segment
				\return		\$ t \$ s.t. \$ p = q + t \cdot (r - q) \$ */
			static tuple<Line2Plane, Point2Seg, Real> intSegPlane(const point3d & Q, 
				const point3d & R, const point3d & N, const Real & D); 
						
			/*!	Test if a segment intersect a triangle (debug mode).
				\param Q	querying end-point of the segment
				\param R	ray end-point of the segment
				\param A	first vertex of the triangle
				\param B	second vertex of the triangle
				\param C	third vertex of the triangle 
				\return		intersection type */ 
			static IntersectionType intSegTri(const point3d & Q, const point3d & R,
				const point3d & A, const point3d & B, const point3d & C);
				
			/*!	Test if a segment intersect a triangle (release mode).
				\param Q	querying end-point of the segment
				\param R	ray end-point of the segment
				\param a	first vertex of the triangle 
							projected onto the "xy"-plane
				\param b	second vertex of the triangle
							projected onto the "xy"-plane
				\param c	third vertex of the triangle 
							projected onto the "xy"-plane
				\param N	unit normal to the triangle 
				\param D	RHS term in the equation of the plane
							determined by the triangle 
				\param x	"x"-coordinate
				\param y	"y"-coordinate
				\return		intersection type */
			static IntersectionType intSegTri(const point3d & Q, const point3d & R,
				const point2d & a, const point2d & b, const point2d & c, 
				const point3d & N, const Real & D, const UInt & x, const UInt & y);
	};
}

/*!	Include implementations of inlined members. */
#ifdef INLINED
#include "inline/inline_gutility.hpp"
#endif

#endif

/*!	\file	gutility.cpp
	\brief	Implementation of members of class gutility. */
	
#include <cmath>
#include <cstdlib>
	
#include "utility.hpp"
#include "gutility.hpp"

// Include implementation of inlined members
#ifndef INLINED
#include "inline/inline_gutility.hpp"
#endif

namespace geometry
{
	//
	// Two-dimensional methods
	//
	
	Point2Tri gutility::inTri2d(const point2d & p, 
		const point2d & a, const point2d & b, const point2d & c)
	{
		// Compute signed area of the triangle pab, pbc and pac
		auto pab = gutility::getTriArea2d(p,a,b);
		auto pbc = gutility::getTriArea2d(p,b,c);
		auto pca = gutility::getTriArea2d(p,c,a);
				
		// If the areas are all positive or all negative:
		// the point is internal to the triangle
		if (((pab > TOLL) && (pbc > TOLL) && (pca > TOLL)) ||
			((pab < -TOLL) && (pbc < -TOLL) && (pca < -TOLL)))
			return Point2Tri::INTERN;
			
		// If two areas are zero: the point coincides with
		// the vertex shared by the associated edges
		bool pab_iszero = (-TOLL <= pab) && (pab <= TOLL);
		bool pbc_iszero = (-TOLL <= pbc) && (pbc <= TOLL);
		bool pca_iszero = (-TOLL <= pca) && (pca <= TOLL);
		if ((pab_iszero && pbc_iszero) ||
			(pbc_iszero && pca_iszero) ||
			(pca_iszero && pab_iszero))
			return Point2Tri::ONVERTEX;
			
		// If one area is zero and the others are concorde:
		// the point lays on an edge
		if ((pab_iszero && (((pbc > 0) && (pca > 0)) || ((pbc < 0) && (pca < 0)))) ||
			(pbc_iszero && (((pab > 0) && (pca > 0)) || ((pab < 0) && (pca < 0)))) ||
			(pca_iszero && (((pab > 0) && (pbc > 0)) || ((pab < 0) && (pbc < 0)))))
			return Point2Tri::ONEDGE;
						
		// Otherwise, the point does not belong to the triangle
		return Point2Tri::EXTERN;	
	}
	
	
	IntersectionType gutility::intSegSeg2d(const point2d & q1, 
		const point2d & r1, const point2d & q2, const point2d & r2)
	{
		// Compute signed area of the triangles q1r1q2, q1r1r2, q2r2q1, q2r2r1
		auto q1r1q2 = gutility::getTriArea2d(q1,r1,q2);
		auto q1r1r2 = gutility::getTriArea2d(q1,r1,r2);
		auto q2r2q1 = gutility::getTriArea2d(q2,r2,q1);
		auto q2r2r1 = gutility::getTriArea2d(q2,r2,r1);
		
		//
		// The segments are collinear
		//
		// We have to check if either q2 or r2 belongs to q1-r1
		// A point p belongs to a segment qr iff
		//	\$ p = q + t \cdot (r - q) \$
		// with \$ 0 \leq t \leq 1 \$
		//
		// To test if a point p is collinear with other two points q and r:
		// check if the (signed) area of the triangle qrp is zero
		if (((-TOLL <= q1r1q2) && (q1r1q2 <= TOLL))
			&& ((-TOLL <= q1r1r2) && (q1r1r2 <= TOLL)))
		{
			Real t_q2, t_r2;
			
			// Properly check for horizontal or vertical segments
			auto den = r1[0] - q1[0];
			if ((den < -TOLL) || (den > TOLL))
			{
				t_q2 = (q2[0] - q1[0])/den;
				t_r2 = (r2[0] - q1[0])/den;
			}
			else
			{
				den = r1[1] - q1[1];
				t_q2 = (q2[1] - q1[1])/den;
				t_r2 = (r2[1] - q1[1])/den;
			}
			
			// Check if q2 and/or r2 are internal to q1-r1
			if (((TOLL < t_q2) && (t_q2 < 1.-TOLL)) ||
				((TOLL < t_r2) && (t_r2 < 1.-TOLL)))
				return IntersectionType::INVALID;
				
			// If two vertices coincide: we need to check if
			// either q1 or r1 are internal to q2-r2
			if (((-TOLL <= t_q2) && (t_q2 <= TOLL)) ||
				((1.-TOLL <= t_q2) && (t_q2 <= 1.+TOLL)) ||
				((-TOLL <= t_r2) && (t_r2 <= TOLL)) ||
				((1.-TOLL <= t_r2) && (t_r2 <= 1.+TOLL)))
			{
				Real t_q1, t_r1;
			
				// Properly check for horizontal or vertical segments
				auto den = r2[0] - q2[0];
				if ((den < -TOLL) || (den > TOLL))
				{
					t_q1 = (q1[0] - q2[0])/den;
					t_r1 = (r1[0] - q2[0])/den;
				}
				else
				{
					den = r2[1] - q2[1];
					t_q1 = (q1[1] - q2[1])/den;
					t_r1 = (r1[1] - q2[1])/den;
				}
			
				// Check if either q1 or r1 are internal to q2-r2...
				if (((TOLL < t_q1) && (t_q1 < 1.-TOLL)) ||
					((TOLL < t_r1) && (t_r1 < 1.-TOLL)))
					return IntersectionType::INVALID;
				
				// ...If not, the segments share a vertex
				return IntersectionType::VALID;
			}
			
			// Last remaining scenarios: the segments do not intersect
			return IntersectionType::NONE;
		}
		
		//
		// The segments do not intersect
		//
		// One of the following two statements must hold:
		//	- q2 and r2 are both on the left or on the right
		//	  of the segment q1-r1 
		//	- q1 and r1 are both on the left or on the right
		//	  of the segment q2-r2
		//
		// To test if a point p is on the left (right) of a line qr:
		// check if the signed area of the triangle qrp is positive (negative)
		if (((q1r1q2 > TOLL && q1r1r2 > TOLL) || (q1r1q2 < -TOLL && q1r1r2 < -TOLL))
			|| ((q2r2q1 > TOLL && q2r2r1 > TOLL) || (q2r2q1 < -TOLL && q2r2r1 < -TOLL)))
			return IntersectionType::NONE; 
		
		//
		// The intersection point is internal to both segments
		//
		// Sufficient condition is that the following two statements
		// simultaneously hold:
		//	- q2 is on the left (right) of q1-r1 and r2 is on the right 
		//	  (left) of q1-r1 
		//	- q1 is on the left (right) of q2-r2 and r1 is on the right 
		//	  (left) of q2-r2
		if (((q1r1q2 > TOLL && q1r1r2 < -TOLL) || (q1r1q2 < -TOLL && q1r1r2 > TOLL))
			&& ((q2r2q1 > TOLL && q2r2r1 < -TOLL) || (q2r2q1 < -TOLL && q2r2r1 > TOLL)))
			return IntersectionType::INVALID;
			
		//
		// The intersection point coincides with a vertex of a segment
		// but it is internal to the other one
		//
		// One of the following two statements must hold:
		//	- q2 is on the left (right) of q1-r1, r2 is on the right
		//	  (left) of q1-r1 and either q1 or r1 is collinear with q2 and r2
		//  - q1 is on the left (right) of q2-r2, r1 is on the right
		//	  (left) of q2-r2 and either q2 or r2 is collinear with q1 and r1
		if ((((q1r1q2 > TOLL && q1r1r2 < -TOLL) || (q1r1q2 < -TOLL && q1r1r2 > TOLL))
			&& (((-TOLL <= q2r2q1) && (q2r2q1 <= TOLL)) || ((-TOLL <= q2r2r1) && (q2r2r1 <= TOLL)))) ||
			(((q2r2q1 > TOLL && q2r2r1 < -TOLL) || (q2r2q1 < -TOLL && q2r2r1 > TOLL))
			&& (((-TOLL <= q1r1q2) && (q1r1q2 <= TOLL)) || ((-TOLL <= q1r1r2) && (q1r1r2 <= TOLL)))))
			return IntersectionType::INVALID;
			
		//
		// Only remaining scenarios: the segments share a vertex
		//
		return IntersectionType::VALID;
	}
	
	
	//
	// Three-dimensional methods
	//
	
	Point2Tri gutility::inTri3d(const point3d & P, 
		const point3d & A, const point3d & B, const point3d & C)
	{
		// Compute the normal to the triangle and the RHS 
		// of the equation of the plane the triangle lies in
		auto N = ((B - A)^(C - B)).normalize();
		auto D = N*A;
		
		//
		// Test if the point belongs to the plane of the triangle
		//
		
		auto dist = abs(N*P - D);
		if (dist > TOLL)
			return Point2Tri::EXTERN;
			
		//
		// The point belongs to the plane of the triangle
		//
		
		// Extract the maximum coordinate of N
		UInt z = N.getMaxCoor();
		UInt x = (z+1) % 3;
		UInt y = (z+2) % 3;
		
		// Project all points onto the "xy"-plane
		point2d p(P[x],P[y]);
		point2d a(A[x],A[y]);
		point2d b(B[x],B[y]);
		point2d c(C[x],C[y]);
		
		// Check belonging in 2D
		return gutility::inTri2d(p,a,b,c);
	}
	
				
	tuple<Line2Plane, Point2Seg, Real> gutility::intSegPlane
		(const point3d & Q, const point3d & R, const point3d & N, const Real & D)
	{
		auto l2p = Line2Plane::INCIDENT; 
		auto p2s = Point2Seg::ONVERTEX;
		Real t;
		
		// Compute numerator and denumerator of Equation (7.7), p. 228
		auto q_plane = D - Q*N;
		auto den = (R - Q)*N;
		
		//
		// The segment is parallel to the plane
		//
		// This happens iff the denumerator vanishes
		// If the numerator vanishes too: the segment belongs to the plane
		// If the numerator does not vanish: the segment does not belong to the plane
		if ((-TOLL <= den) && (den <= TOLL))
		{
			if ((-TOLL <= q_plane) && (q_plane <= TOLL))
				l2p = Line2Plane::COMPLANAR;
			else
				l2p = Line2Plane::PARALLEL;
				
			return make_tuple(l2p, p2s, t);
		}
		
		//
		// The segment is not parallel to the plane
		//
		
		// Check if the plane and the segment intersect
		// in the querying vertex of the segment
		if ((-TOLL <= q_plane) && (q_plane <= TOLL))
			return make_tuple(l2p, p2s, 0.);
			
		// Check if the plane and the segment intersect
		// in the ray vertex of the segment
		auto r_plane = D - R*N;
		if ((-TOLL <= r_plane) && (r_plane <= TOLL))
			return make_tuple(l2p, p2s, 1.);
		
		t = q_plane/den;
		
		// They intersect one each other iff \$ 0 \leq t \leq 1 \$
		if ((t < -TOLL) || (t > 1.+TOLL))
		{
			p2s = Point2Seg::EXTERN;
			return make_tuple(l2p, p2s, t);
		}
		
		// The intersection point is strictly inside the segment
		// iff \$ 0 < t < 1 \$
		if ((TOLL < t) && (t < 1.-TOLL))
		{
			p2s = Point2Seg::INTERN;
			return make_tuple(l2p, p2s, t);
		}
		
		// Due to floating point arithmetic, t may turn out to be 
		// close to 0 or 1, i.e. $-TOLL \leq t \leq TOLL$ or 
		// $1-TOLL \leq t \leq 1+TOLL$. To avoid any run time issue, 
		// we should handle this situation properly.
		if ((-TOLL <= t) && (t <= TOLL))
			return make_tuple(l2p, p2s, 0.);
		return make_tuple(l2p, p2s, 1.);
	}
	
	
	IntersectionType gutility::intSegTri(const point3d & Q, const point3d & R,
		const point3d & A, const point3d & B, const point3d & C)
	{
		// Compute the normal to the triangle and the RHS 
		// of the equation of the plane the triangle lies in
		auto N = ((B - A)^(C - B)).normalize();
		auto D = N*A;
		
		// Extract the maximum coordinate of N
		UInt z = N.getMaxCoor();
		UInt x = (z+1) % 3;
		UInt y = (z+2) % 3;
		
		//
		// Segment-plane intersection
		//
		
		Line2Plane l2p;
		Point2Seg p2s;
		Real t;
		tie(l2p,p2s,t) = gutility::intSegPlane(Q,R,N,D);
				
		// Necessary condition for the segment to intersect the triangle
		// is that the segment intersects the plane
		if ((l2p == Line2Plane::PARALLEL) || (p2s == Point2Seg::EXTERN))
			return IntersectionType::NONE;
			
		// Project all points onto the "xy"-plane
		point2d q(Q[x],Q[y]);
		point2d r(R[x],R[y]);
		point2d a(A[x],A[y]);
		point2d b(B[x],B[y]);
		point2d c(C[x],C[y]);
		
		//
		// The segment is parallel to the plane
		//
		
		if (l2p == Line2Plane::COMPLANAR)
		{
			// Test intersection of the segment with each edge
			auto qr_ab = gutility::intSegSeg2d(q,r,a,b);
			if (qr_ab == IntersectionType::INVALID)
				return qr_ab;
				
			auto qr_bc = gutility::intSegSeg2d(q,r,b,c);
			if (qr_bc == IntersectionType::INVALID)
				return qr_bc;
				
			auto qr_ca = gutility::intSegSeg2d(q,r,c,a);
			if (qr_ca == IntersectionType::INVALID)
				return qr_ca;
				
			// The segment may be completely within the triangle
			auto q_abc = gutility::inTri2d(q,a,b,c);
			auto r_abc = gutility::inTri2d(r,a,b,c);
			if ((q_abc == Point2Tri::INTERN) || (r_abc == Point2Tri::INTERN))
				return IntersectionType::INVALID;
			 
			// If here, the triangles do not intersect or
			// they intersect in a conformal way
			// Since we do not really care about that,
			// just classify the intersection as VALID
			return IntersectionType::VALID;
		}
		
		//
		// The segment is not parallel to the plane
		//
		
		// Compute the intersection point
		point2d p = q + t*(r - q);
		
		// Find the relative position between the intersection
		// point and the triangle
		auto p2t = gutility::inTri2d(p,a,b,c);
		
		// The segment does not intersect the triangle
		if (p2t == Point2Tri::EXTERN)
			return IntersectionType::NONE;
			
		// The segment intersects the triangle in a conformal way
		if ((p2s == Point2Seg::ONVERTEX) && (p2t != Point2Tri::INTERN))	
			return IntersectionType::VALID;
			
		// Only remaining scenario: the segment intersects the
		// triangle in a non-conformal way
		return IntersectionType::INVALID;
	}
	
	
	IntersectionType gutility::intSegTri(const point3d & Q, const point3d & R,
		const point2d & a, const point2d & b, const point2d & c, 
		const point3d & N, const Real & D, const UInt & x, const UInt & y)
	{
		//
		// Segment-plane intersection
		//
		
		Line2Plane l2p;
		Point2Seg p2s;
		Real t;
		tie(l2p,p2s,t) = gutility::intSegPlane(Q,R,N,D);
		//utility::printLine2Plane(l2p);
		//utility::printPoint2Seg(p2s);
				
		// Necessary condition for the segment to intersect the triangle
		// is that the segment intersects the plane
		if ((l2p == Line2Plane::PARALLEL) || (p2s == Point2Seg::EXTERN))
			return IntersectionType::NONE;
			
		// Project Q and R onto the "xy"-plane
		point2d q(Q[x],Q[y]);
		point2d r(R[x],R[y]);
				
		//
		// The segment is parallel to the plane
		//
		
		if (l2p == Line2Plane::COMPLANAR)
		{
			// Test intersection of the segment with each edge
			auto qr_ab = gutility::intSegSeg2d(q,r,a,b);
			if (qr_ab == IntersectionType::INVALID)
				return qr_ab;
			
			auto qr_bc = gutility::intSegSeg2d(q,r,b,c);
			if (qr_bc == IntersectionType::INVALID)
				return qr_bc;
			
			auto qr_ca = gutility::intSegSeg2d(q,r,c,a);
			if (qr_ca == IntersectionType::INVALID)
				return qr_ca;
			
			// The segment may be completely within the triangle
			auto q_abc = gutility::inTri2d(q,a,b,c);
			auto r_abc = gutility::inTri2d(r,a,b,c);
			if ((q_abc == Point2Tri::INTERN) || (r_abc == Point2Tri::INTERN))
				return IntersectionType::INVALID;
				
			// If here, the triangles do not intersect or
			// they intersect in a conformal way
			// Since we do not really care about that,
			// just classify the intersection as VALID
			return IntersectionType::VALID;
		}
		
		//
		// The segment is not parallel to the plane
		//
		
		// Compute the intersection point
		point2d p = q + t*(r - q);
		
		// Find the relative position between the intersection
		// point and the triangle
		auto p2t = gutility::inTri2d(p,a,b,c);
		//utility::printPoint2Tri(p2t);
		
		// The segment does not intersect the triangle
		if (p2t == Point2Tri::EXTERN)
			return IntersectionType::NONE;
			
		// The segment intersects the triangle in a conformal way
		if ((p2s == Point2Seg::ONVERTEX) && (p2t != Point2Tri::INTERN))	
			return IntersectionType::VALID;
			
		// Only remaining scenario: the segment intersects the
		// triangle in a non-conformal way
		return IntersectionType::INVALID;
	}
} 

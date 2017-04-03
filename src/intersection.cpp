/*!	\file	intersection.cpp
	\brief	Implementations of members of class intersection. */
	
/* 	Note: the class may not correctly handle the case of two
	coinciding triangles. In the non-static interface, this
	is tackled by checking if the two input triangles Id's
	are different. One could argue that two different triangles
	may come to coincide after an operation on the mesh.
	Even if we cannot exclude it, we think this is a really 
	exceptional case, especially in a process of mesh simplification
	by edge collapsing. 
	On the other hand, the static API does not perform any check
	for detecting completely overlapping triangles. Even if not
	necessary for our code (as just explained), this kind of 
	checks may be implemented in future just for the sake of
	completeness. */
	
#include "utility.hpp"
#include "intersection.hpp"

//	Include definitions of inlined members
#ifndef INLINED
#include "inline/inline_intersection.hpp"
#endif

namespace geometry
{
	//
	// Constructor
	//
	
	intersection<Triangle>::intersection(const bmesh<Triangle> * pg) :
		grid(pg)
	{
	}
	
	
	//
	// Interface
	//	
	
	bool intersection<Triangle>::intersect(const point3d & A, const point3d & B, const point3d & C, 
		const point3d & D, const point3d & E, const point3d & F)
	{
		// 
		// Triangle ABC as reference
		//
		
		#ifdef NDEBUG
			// Compute the normal to the triangle and the RHS 
			// of the equation of the plane the triangle lies in
			auto N = ((B - A)^(C - B)).normalize();
			auto RHS = N*A;
		
			// Extract the maximum coordinate of N
			UInt z = N.getMaxCoor();
			UInt x = (z+1) % 3;
			UInt y = (z+2) % 3;
		
			// Project the triangle ABC onto the "xy"-plane
			point2d a(A[x],A[y]);
			point2d b(B[x],B[y]);
			point2d c(C[x],C[y]);
		#endif
		
		// Test intersection of each edge of DEF with ABC
		#ifndef NDEBUG
			auto de_abc = gutility::intSegTri(D,E,A,B,C);
			auto ef_abc = gutility::intSegTri(E,F,A,B,C);
			auto fd_abc = gutility::intSegTri(F,D,A,B,C);
		#else
			auto de_abc = gutility::intSegTri(D,E,a,b,c,N,RHS,x,y);
			//utility::printIntersectionType(de_abc,"de and abc");
			auto ef_abc = gutility::intSegTri(E,F,a,b,c,N,RHS,x,y);
			//utility::printIntersectionType(ef_abc,"ef and abc");
			auto fd_abc = gutility::intSegTri(F,D,a,b,c,N,RHS,x,y);
			//utility::printIntersectionType(fd_abc,"fd and abc");
		#endif
				
		// If at least one segment-triangle intersection
		// is not conformal, the triangle-triangle intersection is 
		// invalid too
		if ((de_abc == IntersectionType::INVALID) ||
			(ef_abc == IntersectionType::INVALID) ||
			(fd_abc == IntersectionType::INVALID))
			return true;
			
		// 
		// Triangle DEF as reference
		//
		
		#ifdef NDEBUG
			// Compute the normal to the triangle and the RHS 
			// of the equation of the plane the triangle lies in
			N = ((E - D)^(F - E)).normalize();
			RHS = N*D;
		
			// Extract the maximum coordinate of N
			z = N.getMaxCoor();
			x = (z+1) % 3;
			y = (z+2) % 3;
		
			// Project the triangle ABC onto the "xy"-plane
			point2d d(D[x],D[y]);
			point2d e(E[x],E[y]);
			point2d f(F[x],F[y]);
		#endif
		
		// Test intersection of each edge of DEF with ABC
		#ifndef NDEBUG
			auto ab_def = gutility::intSegTri(A,B,D,E,F);
			auto bc_def = gutility::intSegTri(B,C,D,E,F);
			auto ca_def = gutility::intSegTri(C,A,D,E,F);
		#else
			auto ab_def = gutility::intSegTri(A,B,d,e,f,N,RHS,x,y);
			//utility::printIntersectionType(ab_def,"ab and def");
			auto bc_def = gutility::intSegTri(B,C,d,e,f,N,RHS,x,y);
			//utility::printIntersectionType(bc_def,"bc and def");
			auto ca_def = gutility::intSegTri(C,A,d,e,f,N,RHS,x,y);
			//utility::printIntersectionType(ca_def,"ca and def");
		#endif
				
		// If at least one segment-triangle intersection
		// is not conformal, the triangle-triangle intersection is 
		// invalid too
		if ((ab_def == IntersectionType::INVALID) ||
			(bc_def == IntersectionType::INVALID) ||
			(ca_def == IntersectionType::INVALID))
			return true;
			
		// If here, the triangles do not intersection one each other
		// or they do in a conformal way
		return false;
	}
				
				
	bool intersection<Triangle>::intersect(const UInt & id1, const UInt & id2) const
	{
		// Extract vertices of first element
		auto el1 = grid->getElem(id1); 
		point3d A(grid->getNode(el1[0]));
		point3d B(grid->getNode(el1[1]));
		point3d C(grid->getNode(el1[2]));
				
		// Extract vertices of first element
		auto el2 = grid->getElem(id2); 
		point3d D(grid->getNode(el2[0]));
		point3d E(grid->getNode(el2[1]));
		point3d F(grid->getNode(el2[2]));
		
		// Call static interface
		return ((id1 != id2) && intersection<Triangle>::intersect(A,B,C,D,E,F));
	}
}



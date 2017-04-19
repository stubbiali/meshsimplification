/*!	\file	projection.cpp
	\brief	Implementations of members of class projection<Triangle>. */
	
#include <cmath>
#include <cstdlib>
#include <limits>
#include <stdexcept>

#include "gutility.hpp"
#include "projection.hpp"

namespace geometry
{	
	//
	// Constructors
	//
	
	projection<Triangle>::projection(const bmesh<Triangle> & bg) :
		meshInfo<Triangle, MeshType::DATA>(bg)
	{
	}
	
	
	//
	// Auxiliary methods
	//
	
	pair<bool,UInt> projection<Triangle>::inTri2d(const point2d & p, const point2d & a, 
		const point2d & b, const point2d & c)
	{
		// Compute signed area of the triangle pab, pbc and pac
		auto pab = ((a - p)^(b - a))[2];
		auto pbc = ((b - p)^(c - b))[2];
		auto pca = ((c - p)^(a - c))[2];
		
		// If the areas do not have all the same sign:
		// the point is external
		if (((pab > TOLL) && (pbc < TOLL) && (pca < TOLL)) ||
			((pab < TOLL) && (pbc > TOLL) && (pca < TOLL)) ||
			((pab < TOLL) && (pbc < TOLL) && (pca > TOLL)) ||
			((pab < TOLL) && (pbc > TOLL) && (pca > TOLL)) ||
			((pab > TOLL) && (pbc < TOLL) && (pca > TOLL)) ||
			((pab > TOLL) && (pbc > TOLL) && (pca < TOLL)))
			return {false, 0};
			
		
		// If the areas are all positive or all negative:
		// the point is internal to the triangle
		if (((pab > TOLL) && (pbc > TOLL) && (pca > TOLL)) ||
			((pab < -TOLL) && (pbc < -TOLL) && (pca < -TOLL)))
			return {true, 0};
			
		// Check how many signed areas are zero
		bool pab_iszero = (-TOLL <= pab) && (pab <= TOLL);
		bool pbc_iszero = (-TOLL <= pbc) && (pbc <= TOLL);
		bool pca_iszero = (-TOLL <= pca) && (pca <= TOLL);
				
		// If two triangles are degenerate, then p coincides with a vertex
		if (pab_iszero && pbc_iszero)
			return {true, 5};
		if (pbc_iszero && pca_iszero)
			return {true, 6};
		if (pca_iszero && pab_iszero)
			return {true, 4};
		
		// If just a triangle is degenerate, then p lies on an edge
		if (pab_iszero)
			return {true, 1};
		if (pbc_iszero)
			return {true, 2};
		if (pca_iszero)
			return {true, 3};
			
		return {};
	}
	
	
	vector<UInt> projection<Triangle>::getNewData2Elem(const UInt & Id, const UInt & pos) const
	{
		// Extract the triangle and its vertices
		auto elem = this->getCPointerToMesh()->getElem(Id);
		
		//
		// The point is inside the triangle
		//
		
		if (pos == 0)
			return {Id};
			
		//
		// The point is on an edge
		//
		
		if (pos == 1)
			return this->getElemsOnEdge(elem[0], elem[1]);	
		if (pos == 2)
			return this->getElemsOnEdge(elem[1], elem[2]);
		if (pos == 3)
			return this->getElemsOnEdge(elem[2], elem[0]);
			
		//
		// The point coincides with a vertex
		//
		
		if (pos == 4)
			return this->connectivity.getNode2Elem(elem[0]).getConnected();
		if (pos == 5)
		 	return this->connectivity.getNode2Elem(elem[1]).getConnected();
		if (pos == 6)
			return this->connectivity.getNode2Elem(elem[2]).getConnected();
			
		//
		// Exception
		//
		
		throw runtime_error("Datum does not belong to any triangle.");
	}
		
		
	//
	// Static interface
	//
	
	tuple<Real, point3d, UInt> projection<Triangle>::project(const point3d & P, 
		const point3d & A, const point3d & B, const point3d & C)
	{
		//
		// Preliminary computations
		//
		
		// Compute normal to the plane defined by the triangle
		// and (signed) distance from the origin
		auto N = ((B - A)^(C - B)).normalize();
		auto D = N*A;
		
		// Get the "xy"-plane
		auto z = N.getMaxCoor();
		UInt x = (z+1) % 3;
		UInt y = (z+2) % 3;
					
		//
		// Project the point onto the plane defined by the triangle
		//
		// Let $Q$ be the projected point, it belongs to the plane, i.e.
		// $N \cdot Q = D$. Then, introducing $t \in \mathcal{R}$ such that
		// $Q = P + t N$, we have $(P + t N) \cdot N = D$.
		// After few calculations, one gets $t = D - P \cdot N$, hence
		// $Q = P + (D - P \cdot N) N$.
		
		auto t = D - P*N;
		auto Q = P + t*N;
				
		//
		// Project all points onto the "xy"-plane
		//
		
		point2d q(Q[x],Q[y]);
		point2d a(A[x],A[y]);
		point2d b(B[x],B[y]);
		point2d c(C[x],C[y]);
		
		//
		// Check if the projection falls within the triangle
		//
		// If so, compute the square distance and return
		
		auto q_abc = projection<Triangle>::inTri2d(q, a, b, c);
		if (q_abc.first)
		{
			if (q_abc.second == 4)
				return make_tuple((P - A)*(P - A), A, 4);
			if (q_abc.second == 5)
				return make_tuple((P - B)*(P - B), B, 5);
			if (q_abc.second == 6)
				return make_tuple((P - C)*(P - C), C, 6);
			return make_tuple((P - Q)*(P - Q), Q, q_abc.second);
		}
			
		//
		// The projection is outside the triangle and the element of the
		// triangle closest to the projection is an edge
		//
		// For each edge, test if the orthogonal projection of the point
		// onto the line of edge falls within the edge itself.
		// To compute the projection $Q'$ of a point $Q$ onto the line
		// $A B$, first note that $Q'$ can be expressed as $Q' = A + t (B - A)$,
		// with $t \in \mathcal{R}$. Then, imposing the orthogonality between
		// $Q Q'$ and $A B$, one gets 
		// $t = \dfrac{(Q - A) \cdot (B - A)}{(B - A) \cdot (B - A)}$.
		// To check if $Q'$ actually falls between $A$ and $B$, check that $A$
		// is on the left (right) of $Q Q'$, while $B$ is on the right (left) of $Q Q'$. 
		
		Real opt_dist(numeric_limits<Real>::max());
		point3d opt_Qp;
		UInt opt_pos;
		
		//
		// Edge AB
		//
		
		{
			// Project Q onto the line AB
			auto t = (Q - A) * (B - A) / ((B - A)*(B - A));
			auto Qp = A + t * (B - A);
			
			// Project Qp onto the "xy"-plane
			point2d qp(Qp[x],Qp[y]);
			
			// Check if qp is within a and b...
			auto qqpa = gutility::getTriArea2d(q, qp, a);
			auto qqpb = gutility::getTriArea2d(q, qp, b);
			
			// ... and if so, update optimal variables
			if (qqpa*qqpb < 0.)
			{
				Real dist = (P - Qp)*(P - Qp);
				if (dist < opt_dist - TOLL)
				{
					opt_dist = dist;
					opt_Qp = Qp;
					opt_pos = 1;
				}
			}
		}
		
		//
		// Edge BC
		//
		
		{
			// Project Q onto the line BC
			auto t = (Q - B) * (C - B) / ((C - B)*(C - B));
			auto Qp = B + t * (C - B);
			
			// Project Qp onto the "xy"-plane
			point2d qp(Qp[x],Qp[y]);
			
			// Check if qp is within b and c...
			auto qqpb = gutility::getTriArea2d(q, qp, b);
			auto qqpc = gutility::getTriArea2d(q, qp, c);
			
			// ... and if so, update optimal variables
			if (qqpb*qqpc < 0.)
			{
				Real dist = (P - Qp)*(P - Qp);
				if (dist < opt_dist - TOLL)
				{
					opt_dist = dist;
					opt_Qp = Qp;
					opt_pos = 2;
				}
			}
		}
		
		//
		// Edge CA
		//
		
		{
			// Project Q onto the line AB
			auto t = (Q - C) * (A - C) / ((A - C)*(A - C));
			auto Qp = C + t * (A - C);
			
			// Project Qp onto the "xy"-plane
			point2d qp(Qp[x],Qp[y]);
			
			// Check if qp is within c and a...
			auto qqpc = gutility::getTriArea2d(q, qp, c);
			auto qqpa = gutility::getTriArea2d(q, qp, a);
			
			// ... and if so, update optimal variables
			if (qqpc*qqpa < 0.)
			{
				Real dist = (P - Qp)*(P - Qp);
				if (dist < opt_dist - TOLL)
				{
					opt_dist = dist;
					opt_Qp = Qp;
					opt_pos = 3;
				}
			}
		}
					
		//
		// The projection is outside the triangle and the point of the
		// triangle closest to the point is a vertex
		//
		
		// Vertex A
		Real qa = (q[0]-a[0])*(q[0]-a[0]) + (q[1]-a[1])*(q[1]-a[1]);
		if (qa < opt_dist)
		{
			opt_dist = qa;
			opt_Qp = A;
			opt_pos = 4;
		}
		
		// Vertex B
		Real qb = (q[0]-b[0])*(q[0]-b[0]) + (q[1]-b[1])*(q[1]-b[1]);
		if (qb < opt_dist)
		{
			opt_dist = qb;
			opt_Qp = B;
			opt_pos = 5;
		}
			
		// Vertex C
		Real qc = (q[0]-c[0])*(q[0]-c[0]) + (q[1]-c[1])*(q[1]-c[1]);
		if (qc < opt_dist)
		{
			opt_dist = qc;
			opt_Qp = C;
			opt_pos = 6;
		}
		
		// Due to floating point arithmetic, the projection may not be computable.
		// In this case, project the point onto the barycenter of the triangle
		if (opt_dist < numeric_limits<Real>::max())
			return make_tuple(opt_dist, opt_Qp, opt_pos);
		auto G = 0.333*(A + B + C);
		return make_tuple((P - G)*(P - G), G, 0);
	}
	
	
	tuple<Real, point3d, UInt> projection<Triangle>::project(const point3d & P, 
		const point3d & A, const point3d & B, const point3d & C,
		const point3d & N, const Real & D, const UInt & x, const UInt & y)
	{		
		//
		// Project the point onto the plane defined by the triangle
		//
		// Let $Q$ be the projected point, it belongs to the plane, i.e.
		// $N \cdot Q = D$. Then, introducing $t \in \mathcal{R}$ such that
		// $Q = P + t N$, we have $(P + t N) \cdot N = D$.
		// After few calculations, one gets $t = D - P \cdot N$, hence
		// $Q = P + (D - P \cdot N) N$.
		
		auto t = D - P*N;
		auto Q = P + t*N;
				
		//
		// Project the projection onto the "xy"-plane
		//
		
		point2d q(Q[x],Q[y]);
		point2d a(A[x],A[y]);
		point2d b(B[x],B[y]);
		point2d c(C[x],C[y]);
				
		//
		// Check if the projection falls within the triangle
		//
		// If so, compute the square distance and return
		
		auto q_abc = projection<Triangle>::inTri2d(q, a, b, c);
		if (q_abc.first)
		{
			if (q_abc.second == 4)
				return make_tuple((P - A)*(P - A), A, 4);
			if (q_abc.second == 5)
				return make_tuple((P - B)*(P - B), B, 5);
			if (q_abc.second == 6)
				return make_tuple((P - C)*(P - C), C, 6);
			return make_tuple((P - Q)*(P - Q), Q, q_abc.second);
		}
			
		//
		// The projection is outside the triangle and the element of the
		// triangle closest to the projection is an edge
		//
		// For each edge, test if the orthogonal projection of the point
		// onto the line of edge falls within the edge itself.
		// To compute the projection $Q'$ of a point $Q$ onto the line
		// $A B$, first note that $Q'$ can be expressed as $Q' = A + t (B - A)$,
		// with $t \in \mathcal{R}$. Then, imposing the orthogonality between
		// $Q Q'$ and $A B$, one gets 
		// $t = \dfrac{(Q - A) \cdot (B - A)}{(B - A) \cdot (B - A)}$.
		// To check if $Q'$ actually falls between $A$ and $B$, check that $A$
		// is on the left (right) of $Q Q'$, while $B$ is on the right (left) of $Q Q'$. 
		
		Real opt_dist(numeric_limits<Real>::max());
		point3d opt_Qp;
		UInt opt_pos;
		
		//
		// Edge AB
		//
		
		{
			// Project Q onto the line AB
			auto t = (Q - A) * (B - A) / ((B - A)*(B - A));
			auto Qp = A + t * (B - A);
			
			// Project Qp onto the "xy"-plane
			point2d qp(Qp[x],Qp[y]);
			
			// Check if qp is within a and b...
			auto qqpa = gutility::getTriArea2d(q, qp, a);
			auto qqpb = gutility::getTriArea2d(q, qp, b);
			
			// ... and if so, update optimal variables
			if (qqpa*qqpb < 0.)
			{
				Real dist = (P - Qp)*(P - Qp);
				if (dist < opt_dist - TOLL)
				{
					opt_dist = dist;
					opt_Qp = Qp;
					opt_pos = 1;
				}
			}
		}
		
		//
		// Edge BC
		//
		
		{
			// Project Q onto the line BC
			auto t = (Q - B) * (C - B) / ((C - B)*(C - B));
			auto Qp = B + t * (C - B);
			
			// Project Qp onto the "xy"-plane
			point2d qp(Qp[x],Qp[y]);
			
			// Check if qp is within b and c...
			auto qqpb = gutility::getTriArea2d(q, qp, b);
			auto qqpc = gutility::getTriArea2d(q, qp, c);
			
			// ... and if so, update optimal variables
			if (qqpb*qqpc < 0.)
			{
				Real dist = (P - Qp)*(P - Qp);
				if (dist < opt_dist - TOLL)
				{
					opt_dist = dist;
					opt_Qp = Qp;
					opt_pos = 2;
				}
			}
		}
		
		//
		// Edge CA
		//
		
		{
			// Project Q onto the line AB
			auto t = (Q - C) * (A - C) / ((A - C)*(A - C));
			auto Qp = C + t * (A - C);
			
			// Project Qp onto the "xy"-plane
			point2d qp(Qp[x],Qp[y]);
			
			// Check if qp is within c and a...
			auto qqpc = gutility::getTriArea2d(q, qp, c);
			auto qqpa = gutility::getTriArea2d(q, qp, a);
			
			// ... and if so, update optimal variables
			if (qqpc*qqpa < 0.)
			{
				Real dist = (P - Qp)*(P - Qp);
				if (dist < opt_dist - TOLL)
				{
					opt_dist = dist;
					opt_Qp = Qp;
					opt_pos = 3;
				}
			}
		}
			
		//
		// The projection is outside the triangle and the point of the
		// triangle closest to the point is a vertex
		//
		
		// Vertex A
		Real qa = (q[0]-a[0])*(q[0]-a[0]) + (q[1]-a[1])*(q[1]-a[1]);
		if (qa < opt_dist - TOLL)
		{
			opt_dist = qa;
			opt_Qp = A;
			opt_pos = 4;
		}
		
		// Vertex B
		Real qb = (q[0]-b[0])*(q[0]-b[0]) + (q[1]-b[1])*(q[1]-b[1]);
		if (qb < opt_dist - TOLL)
		{
			opt_dist = qb;
			opt_Qp = B;
			opt_pos = 5;
		}
			
		// Vertex C
		Real qc = (q[0]-c[0])*(q[0]-c[0]) + (q[1]-c[1])*(q[1]-c[1]);
		if (qc < opt_dist - TOLL)
		{
			opt_dist = qc;
			opt_Qp = C;
			opt_pos = 6;
		}
		
		// Due to floating point arithmetic, the projection may not be computable.
		// In this case, project the point onto the barycenter of the triangle
		if (opt_dist < numeric_limits<Real>::max())
			return make_tuple(opt_dist, opt_Qp, opt_pos);
		auto G = 0.333*(A + B + C);
		return make_tuple((P - G)*(P - G), G, 0);
	}
					
	
	//
	// Non-static interface
	//
	
	pair<point3d, vector<UInt>> projection<Triangle>::project
		(const UInt & datum, const vector<UInt> & elems)
	{
		//
		// Extract data point
		// 
		
		point3d P(this->getCPointerToMesh()->getData(datum));
		
		//
		// Extract triangles
		//
		
		// Declare auxiliary vectors
		vector<point3d> A, B, C;
		#ifdef NDEBUG
			vector<point3d> N;
			vector<Real> D;
			vector<UInt> x, y;
		#endif
		
		// Reserve memory
		A.reserve(elems.size());
		B.reserve(elems.size());
		C.reserve(elems.size());
		#ifdef NDEBUG
			N.reserve(elems.size());
			D.reserve(elems.size());
			x.reserve(elems.size());
			y.reserve(elems.size());
		#endif
		
		// Get information on each triangle
		for (UInt i = 0; i < elems.size(); ++i)
		{
			auto elem = this->getCPointerToMesh()->getElem(elems[i]);
			
			// Extract vertices
			A.emplace_back(this->getCPointerToMesh()->getNode(elem[0]));
			B.emplace_back(this->getCPointerToMesh()->getNode(elem[1]));
			C.emplace_back(this->getCPointerToMesh()->getNode(elem[2]));
			#ifdef NDEBUG
				// Extract normal to the plane defined by the triangle
				// and (signed) distance from the origin
				N.emplace_back(((B[i] - A[i])^(C[i] - B[i])).normalize());
				D.emplace_back(N[i]*A[i]);
				
				// Get the "xy"-plane
				auto z = N[i].getMaxCoor();
				x.emplace_back((z+1) % 3);
				y.emplace_back((z+2) % 3);
			#endif
		}
		
		//
		// Project the point
		//
		
		Real dist, opt_dist(numeric_limits<Real>::max());
		point3d Q, opt_Q;
		UInt opt_id(MAX_NUM_ELEMS);
		UInt pos, opt_pos;
		
		// Loop over all triangles
		for (UInt i = 0; i < elems.size(); ++i)
		{
			// Test projection
			#ifndef NDEBUG
				tie(dist, Q, pos) = project(P, A[i], B[i], C[i]);
			#else
				tie(dist, Q, pos) = project(P, A[i], B[i], C[i], N[i], D[i], x[i], y[i]);
			#endif
			
			// If the projection falls within the triangle,
			// make sure it is the closest triangle to the point.
			if (dist < opt_dist)
			{
				opt_dist = dist;
				opt_Q = Q;
				opt_id = elems[i];
				opt_pos = pos;
			}
		}
		
		// Test if the projection falls within a triangle
		// (only debug mode)
		assert(opt_id < MAX_NUM_ELEMS);
		
		//
		// Update connections
		//
		
		// Get new data-element connections
		auto newData2Elem = getNewData2Elem(opt_id, opt_pos);
		
		// Set new data-element connections
		auto oldData2Elem = this->connectivity.setData2Elem(datum, newData2Elem);
		
		// Update data point location
		this->getPointerToMesh()->setData(datum, opt_Q);
		
		return make_pair(P, oldData2Elem);
	}
	
	
	vector<pair<point3d, vector<UInt>>> projection<Triangle>::project
		(const vector<UInt> & data, const vector<UInt> & elems)
	{
		//
		// Extract data points
		// 
		
		vector<point3d> P;
		P.reserve(data.size());
		for (auto datum : data)
			P.emplace_back(this->getCPointerToMesh()->getData(datum));
		
		//
		// Extract triangles
		//
		
		// Declare auxiliary vectors
		vector<point3d> A, B, C;
		#ifdef NDEBUG
			vector<point3d> N;
			vector<Real> D;
			vector<UInt> x, y;
		#endif
		
		// Reserve memory
		A.reserve(elems.size());
		B.reserve(elems.size());
		C.reserve(elems.size());
		#ifdef NDEBUG
			N.reserve(elems.size());
			D.reserve(elems.size());
			x.reserve(elems.size());
			y.reserve(elems.size());
		#endif
		
		// Get information on each triangle
		for (UInt i = 0; i < elems.size(); ++i)
		{
			auto elem = this->getCPointerToMesh()->getElem(elems[i]);
			
			// Extract vertices
			A.emplace_back(this->getCPointerToMesh()->getNode(elem[0]));
			B.emplace_back(this->getCPointerToMesh()->getNode(elem[1]));
			C.emplace_back(this->getCPointerToMesh()->getNode(elem[2]));
			#ifdef NDEBUG
				// Extract normal to the plane defined by the triangle
				// and (signed) distance from the origin
				N.emplace_back(((B[i] - A[i])^(C[i] - B[i])).normalize());
				D.emplace_back(N[i]*A[i]);
				
				// Get the "xy"-plane
				auto z = N[i].getMaxCoor();
				x.emplace_back((z+1) % 3);
				y.emplace_back((z+2) % 3);
			#endif
		}
		
		//
		// Project the points
		//
		
		vector<pair<point3d, vector<UInt>>> res;
		res.reserve(data.size());
		
		// Loop over all data points
		for (UInt j = 0; j < data.size(); ++j)
		{
			Real dist, opt_dist(numeric_limits<Real>::max());
			point3d Q, opt_Q;
			UInt opt_id(MAX_NUM_ELEMS);
			UInt pos, opt_pos;
			
			//
			// Projection
			//
			// For each data point, loop over all triangles
			for (UInt i = 0; i < elems.size(); ++i)
			{
				// Test projection
				#ifndef NDEBUG
					tie(dist, Q, pos) = project(P[j], A[i], B[i], C[i]);
				#else
					tie(dist, Q, pos) = project(P[j], A[i], B[i], C[i], N[i], D[i], x[i], y[i]);
				#endif
			
				// If the projection falls within the triangle,
				// make sure it is the closest triangle to the point.
				if (dist < opt_dist)
				{
					opt_dist = dist;
					opt_Q = Q;
					opt_id = elems[i];
					opt_pos = pos;
				}
			}
		
			// Test if the projection falls within a triangle
			// (only debug mode)
			assert(opt_id < MAX_NUM_ELEMS);
		
			//
			// Update connections
			//
		
			auto newData2Elem = getNewData2Elem(opt_id, opt_pos);
			
			// Set new data-element connections
			auto oldData2Elem = this->connectivity.setData2Elem(data[j], newData2Elem);
		
			// Update data point location
			this->getPointerToMesh()->setData(data[j], opt_Q);
			
			// Update output
			res.emplace_back(P[j], oldData2Elem);
		}
		
		return res;
	}
	
	
	void projection<Triangle>::undo(const vector<UInt> & ids, const vector<pair<point3d, vector<UInt>>> & oldData)
	{
		for (UInt i = 0; i < ids.size(); ++i)
		{
			// Set data point location
			this->getPointerToMesh()->setData(ids[i], oldData[i].first);
			
			// Set data-element connections
			this->connectivity.setData2Elem(ids[i], oldData[i].second);
		}
	}
}

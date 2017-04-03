/*!	\file	main_intersection.cpp
	\brief	A small executable testing some functionality provided
			by the class intersection<Triangle>. */
			
#include <chrono>

#include "gutility.hpp"
#include "intersection.hpp"
#include "meshInfo.hpp"

using namespace geometry;

int main()
{
	using point2d = geoPoint<2>;
	using point3d = geoPoint<3>;
	
	//
	// Test segment-segment intersection (2D)
	//
	
	{
		// First segment
		point2d q1(7,5);
		point2d r1(1,2);
		
		// Second segment
		point2d q2(1,2);
		point2d r2(1.5,3);
		
		// Output
		utility::printIntersectionType(gutility::intSegSeg2d(q1,r1,q2,r2), "Segments");
		cout << endl;
	}
	
	//
	// Test point in triangle (2D)
	//
	
	{
		// The triangle
		point2d a(0,0);
		point2d b(2,0);
		point2d c(1,1);
		
		// The points to test
		point2d p1(1,0.5);
		point2d p2(0.5,0.499999999);
		point2d p3(0.5,0.500000001);
		point2d p4(0.5,0.5);
		point2d p5(c);
		
		// Output
		utility::printPoint2Tri(gutility::inTri2d(p1,a,b,c));
		utility::printPoint2Tri(gutility::inTri2d(p2,a,b,c));
		utility::printPoint2Tri(gutility::inTri2d(p3,a,b,c));
		utility::printPoint2Tri(gutility::inTri2d(p4,a,b,c));
		utility::printPoint2Tri(gutility::inTri2d(p5,a,b,c));		
		cout << endl;
	}

	//
	// Test segment-plane intersection
	//

	{
		string inputfile("mesh/bunny.inp");
		bmesh<Triangle> bm(inputfile);
		
		UInt id1(8356);
		UInt id2(1693);
		
		auto el1 = bm.getElem(id1); 
		point3d A(bm.getNode(el1[0]));
		point3d B(bm.getNode(el1[1]));
		point3d C(bm.getNode(el1[2]));
		
		auto el2 = bm.getElem(id2); 
		point3d D(bm.getNode(el2[0]));
		point3d E(bm.getNode(el2[1]));
		point3d F(bm.getNode(el2[2]));
		
		// Compute N and D
		auto N = ((E-D)^(F-E)).normalize();
		auto RHS = N*D;
		
		// The segment
		point3d Q(A);
		point3d R(B);
		
		// Output
		auto ans = gutility::intSegPlane(Q,R,N,RHS);
		utility::printLine2Plane(get<0>(ans));
		utility::printPoint2Seg(get<1>(ans));
		cout << "t = " << get<2>(ans) << endl;
		cout << endl;
	}

	//
	// Test triangle-triangle intersection (non-static API)
	//
	
	
	{
		// Read mesh
		string inputfile("mesh/brain.inp");
		bmesh<Triangle> bm(inputfile);
		intersection<Triangle> ntr(&bm);
		
		// Id's of triangles to consider
		UInt id1 = 11156;
		UInt id2 = 11206;
		auto el1(bm.getElem(id1));
		cout << bm.getNode(el1[0]) << endl
			<< bm.getNode(el1[1]) << endl
			<< bm.getNode(el1[2]) << endl;
		auto el2(bm.getElem(id2));
		cout << bm.getNode(el2[0]) << endl
			<< bm.getNode(el2[1]) << endl
			<< bm.getNode(el2[2]) << endl;
		
		// Test intersection
		#ifdef NDEBUG
		auto ans = ntr.intersect(id1,id2);
		#else
		using namespace std::chrono;
		high_resolution_clock::time_point start, stop; 
		start = high_resolution_clock::now();
		for (UInt i = 0; i < 1e6; i++)
			auto ans = ntr.intersect(id1,id2);
		#endif
		
		#ifdef NDEBUG
		if (ans)
			cout << "Triangles " << id1 << " and " << id2 << " intersect." << endl;
		else
			cout << "Triangles " << id1 << " and " << id2 << " do not intersect." << endl;
		#else
		stop = high_resolution_clock::now();
		auto duration = duration_cast<milliseconds>(stop-start).count();
		cout << "Elapsed time: " << duration << " ms" << endl;
		#endif
	}

	//
	// Test triangle-triangle intersection (static API)
	//
	// Just work in 2d for the sake of simplicity
	
	{
		// First triangle
		point3d A(1,0,0);
		point3d B(4,0,0);
		point3d C(2.5,3,0);
		
		// Second triangle
		point3d D(0,0,0);
		point3d E(2,0,0);
		point3d F(0,2,0);
		
		// Output
		auto ans = intersection<Triangle>::intersect(A,B,C,D,E,F);
		if (ans)
			cout << "Triangles intersect." << endl;
		else
			cout << "Triangles do not intersect." << endl;
	}
	
	//
	// Test triangle-triangle intersection (non-static API)
	//
	
	{
		// Read mesh
		string inputfile("results/brain_8000_033_033_033_bis.inp");
		bmesh<Triangle> bm(inputfile);
		bmeshInfo<Triangle, MeshType::GEO> news(inputfile);
		intersection<Triangle> ntr(&bm);
		
		// Id's of triangles to consider
		for (UInt i = 0; i < bm.getNumElems()-1; ++i)
		{
			auto ids = news.getTriPatch(i);
			for (auto id : ids)
				bm.setElemInactive(id);
				
			for (UInt j = i+1; j < bm.getNumElems(); ++j)
			{
				if (bm.isElemActive(j))
				{
					auto ans = ntr.intersect(i,j);
					if (ans)
						cerr << "Triangles " << i << " and " << j << " intersect." << endl;
				}
			}
			
			for (auto id : ids)
				bm.setElemActive(id);
		}
	}
}

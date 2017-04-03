/*!	\file	main_geoPoint.cpp
	\brief	A small executable to test the class geoPoint. */
	
#include "geoPoint.hpp"

int main()
{
	using namespace geometry;
	
	//
	// Test constructors
	//
	
	{
		geoPoint<4> p0;
		geoPoint<2> p1(2.,3.4);
		geoPoint<3> p2(2,5.6,7.1);
		geoPoint<3> p3({1.,2.,3.});
		geoPoint<3> p4({1.,2.});
		geoPoint<3> p5({1.,2.,3.,4.});
		geoPoint<2> p6(p5);
		geoPoint<5> p7(p6);
	
		cout << "geoPoint<2> p1(2.,3.4)        = " << p1 << endl;
		cout << "geoPoint<3> p2(2,5.6,7.1)     = " << p2 << endl;
		cout << "geoPoint<3> p3({1.,2.,3.})    = " << p3 << endl;
		cout << "geoPoint<3> p4({1.,2.})       = " << p4 << endl;
		cout << "geoPoint<3> p5({1.,2.,3.,4.}) = " << p5 << endl;
		cout << "geoPoint<2> p6(p5)            = " << p6 << endl;
		cout << "geoPoint<5> p7(p6)            = " << p7 << endl;
		
		cout << endl;
	}
	
	//
	// Test operators
	//
	
	{
		geoPoint<5> p1({2.,4.,6.,8.,10.,12.});
		auto p2 = p1/2;
		auto p3 = p1+p2;
		auto p4 = p1-p2;
		auto p5 = 0.1*p4;
		
		cout << "p1          = " << p1 << endl;
		cout << "p2 = p1/2   = " << p2 << endl;
		cout << "p3 = p1+p2  = " << p3 << endl;
		cout << "p4 = p1-p2  = " << p4 << endl;
		cout << "p5 = 0.1*p4 = " << p5 << endl;
	}
}

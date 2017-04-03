/*! \file	main_point.cpp
	\brief	Small executable to test class point. */

#include <iostream>

#include "point.hpp"

int main()
{
	using namespace geometry;
		
	point p1(1,2,3,1);
	point p2(4,5,6,2);
	point p3 = p1+p2;
	
	cout << p1 << endl;
	cout << p2 << endl;
	cout << p3 << endl;
	cout << 0.1*p3 << endl;
}

/*! \file	main_gutility.cpp
	\brief	A small executable to test class gutility. */
	
#include <iostream>

#include "gutility.hpp"

using namespace geometry;

int main()
{	
	point3d A(1,0,0);
	point3d B(0,1,0);
	point3d C(0,0,1);
	auto M(0.333 * (A+B+C));
	cout << M[0] << " " << M[1] << " " << M[2] << endl;
}

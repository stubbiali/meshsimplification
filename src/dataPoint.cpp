/*!	\file	dataPoint.cpp
	\brief	Implementations of members of class dataPoint. */

#include "dataPoint.hpp"

// Include definitions of inlined members 
#ifndef INLINED
#include "inline/inline_dataPoint.hpp"
#endif

namespace geometry
{
	using namespace std;
	
	//
	// Constructors
	//
	
	dataPoint::dataPoint() : point(), datum(0.)
	{
	}
	
	
	dataPoint::dataPoint(const array<Real,3> & c, const UInt & ID, const UInt & dat) : 
		point(c,ID), datum(dat) 
	{
	}
	
	
	dataPoint::dataPoint(const point & p, Real const & dat) : 
		point(p), datum(dat) 
	{
	}
	
	
	//
	// Operators
	//
	
	dataPoint & dataPoint::operator=(const dataPoint & p)
	{
		// Copy coordinates
		point::operator=(p);
		
		// Copy datum
		datum = p.datum;
		
		return *this;
	}
	
	
	//
	// Print
	//
	
	void dataPoint::print(ostream & out) const
	{
		out << "Point ID: " << Id << endl;
		
		out << "Point coordinates: x = " << coor[0] << endl
			<< "                   y = " << coor[1] << endl
			<< "                   z = " << coor[2] << endl;
			
		out << "Point datum: " << datum << endl;
	}
}



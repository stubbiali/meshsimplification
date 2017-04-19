/*!	\file	point.cpp
	\brief	Implementations of members and friend functions of class point. */

#include "point.hpp"

// Include definitions of inlined members and friend functions
#ifndef INLINED
#include "inline/inline_point.hpp"
#endif

namespace geometry 
{
	//
	// Constructors
	//
	
	point::point(const Real & x, const Real & y, const Real & z, const UInt & ID, const UInt & bond) :
		geoPoint<3>(x,y,z), Id(ID), boundary(bond), active(true)
	{
	}
	

	point::point(const array<Real,3> & c, const UInt & ID, const UInt & bond) :
		geoPoint<3>(c), Id(ID), boundary(bond), active(true)
	{
	}
	
	
	point::point(const geoPoint<3> & gp, const UInt & ID, const UInt & bond) :
		geoPoint<3>(gp), Id(ID), boundary(bond), active(true)
	{
	}
	

	//
	// Operators
	//

	// For copy-assignment operator: just consider the coordinates and the boundary flag, but not the Id.
	// This will be useful when updating the mesh after an edge contraction.
	point & point::operator=(const point & p) 
	{
		// Copy the coordinates
		geoPoint<3>::operator=(p);
		
		// Copy the boundary flag
		boundary = p.boundary;
		
		return *this;
	}
					
	
	ostream & operator<<(ostream & out, const point & p)
	{
		out << "Point " << p.Id << ": "
			<< static_cast<const geoPoint<3> &>(p);
		return out;
	}
}


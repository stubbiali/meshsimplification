/*!	\file	utility.cpp
	\brief	Implementations of static members of class utility. */
	
#include <istream>
#include <sstream>
#include <algorithm>
#include <stdexcept>

#include "utility.hpp"
#include "intersection.hpp"

namespace geometry
{
	string utility::getFileExtension(string filename)
	{
		// Reverse name of the file
		reverse(filename.begin(), filename.end());
				
		// Extract the format
		string format;
		istringstream iss;
		iss.str(filename);
		if (getline(iss, format, '.'))
		{
			reverse(format.begin(), format.end());
			return format;
		}
		else
		{
			reverse(filename.begin(), filename.end());
			throw runtime_error("The file " + filename + " does not have any extension.");
		}
	}
	
	
	void utility::printPoint2Seg(const Point2Seg & p2s, ostream & out)
	{
		if (p2s == Point2Seg::EXTERN)
			out << "The point is outside the segment." << endl;
		else if (p2s == Point2Seg::INTERN)
			out << "The point is (strictly) inside the segment." << endl;
		else
			out << "The point coincides with a vertex of the segment." << endl;
	}
	
	
	void utility::printPoint2Tri(const Point2Tri & p2t, ostream & out)
	{
		if (p2t == Point2Tri::EXTERN)
			out << "The point is outside the triangle." << endl;
		else if (p2t == Point2Tri::INTERN)
			out << "The point is inside the triangle." << endl;
		else if (p2t == Point2Tri::ONEDGE)
			out << "The point is on an edge of the triangle." << endl;
		else
			out << "The point coincides with a vertex of the triangle." << endl;
	}
	
	
	void utility::printLine2Plane(const Line2Plane & l2p, ostream & out)
	{
		if (l2p == Line2Plane::PARALLEL)
			out << "The line and the plane are parallel." << endl;
		else if (l2p == Line2Plane::COMPLANAR)
			out << "The line and the plane are complanar." << endl;
		else 
			out << "The line and the plane are incident." << endl;
	}
	
	
	void utility::printIntersectionType(const IntersectionType & it, const string & elements,
		ostream & out)
	{
		out << elements << " ";
		if (it == IntersectionType::NONE)
			out << "do not intersect." << endl;
		else if (it == IntersectionType::VALID)
			out << "intersect in a conformal way." << endl;
		else
			out << "intersect in a non-conformal way." << endl;
	}
}

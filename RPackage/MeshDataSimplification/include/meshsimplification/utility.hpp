/*! \file	utility.hpp
	\brief	A set of utilities. */
	
#ifndef HH_UTILITY_HH
#define HH_UTILITY_HH

#include <string>
#include <iostream>

#include "inc.hpp"

namespace geometry
{
	/*! Forward declarations of some enum's defined in intersect.hpp
		\sa intersect.hpp */
	enum class Point2Seg;
	enum class Point2Tri;
	enum class Line2Plane;
	enum class IntersectionType;
	
	/*! A class providing some generic utilities as static members. */
	class utility
	{
		public:
			/*! Extract the extension of a file.
				\param filename		name of the file
				\return				file extension (if any) */
			static string getFileExtension(string filename);
			
			/*!	Print the relative position of a point w.r.t. a segment.
				\param p2s	a Point2Seg object
				\param out	output stream */
			static void printPoint2Seg(const Point2Seg & p2s, ostream & out = cout);
			
			/*!	Print the relative position of a point w.r.t. a triangle.
				\param p2t	a Point2Tri object
				\param out	output stream */
			static void printPoint2Tri(const Point2Tri & p2t, ostream & out = cout);
			
			/*!	Print the relative position of a line w.r.t. a plane.
				\param l2p	a Line2Plane object
				\param out	output stream */
			static void printLine2Plane(const Line2Plane & l2p, ostream & out = cout);
			
			/*!	Print the intersection type between two elements.
				\param it		an IntersectionType object
				\param elements	name of the elements (e.g. "Segments", "Triangles")
				\param out		output stream */
			static void printIntersectionType(const IntersectionType & it, const string & elements,
				ostream & out = cout);
	};
}

#endif

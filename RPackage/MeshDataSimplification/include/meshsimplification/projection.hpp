/*!	\file	projection.hpp
	\brief	Class projecting a point on a set of elements. */
	
#ifndef HH_PROJECTION_HH
#define HH_PROJECTION_HH

#include <tuple>

#include "meshInfo.hpp"

namespace geometry
{
	/*!	This is a class projecting a (data) point on a set of elements. 
		Note that projection does not necessarily mean orthogonal
		projection but simply the point minimizing the distance point-element. 
		Both a static and non-static interfaces are provided.
		This is a template class where the template parameter is the shape 
		SHAPE of the elements. 
		The class inherits meshInfo<SHAPE, MeshType::DATA>. 
		
		At the moment, only the specialization for triangular grids
		implemented. Further specializations may be added in future. 
		
		Reference: 
		O’Rourke J. "Computational geometry in C". 
		Cambridge (UK), Cambridge University Press, 1998.
				
		\sa meshInfo.hpp */
	template<typename SHAPE = Triangle>
	class projection : public meshInfo<SHAPE, MeshType::DATA>
	{
	};
	
	/*!	Specialization for triangular grids. */
	template<>
	class projection<Triangle> : public meshInfo<Triangle, MeshType::DATA>
	{
		public:
			//
			// Constructors
			//
			
			/*!	Synthetic default constructor. */
			projection() = default;
			
			/*!	Constructor.
				\param bg	a (base) grid */
			projection(const bmesh<Triangle> & bg);
									
			/*!	Constructor.
				\param args	arguments to forward to mesh 
				
				\sa mesh.hpp */
			template<typename... Args>
			projection(Args... args);
						
			//
			// Static interface
			//
			
			/*!	Project a point onto a triangle (debug mode).
				\param P	the point to project
				\param A	first vertex of the triangle
				\param B	second vertex of the triangle
				\param C	third vertex of the triangle
				\return		the square distance between the point and
							the projected point 
				\return		the projected point 
				\return		0 if the projection falls inside the triangle,
							1, 2 or 3 if the projection falls onto the edge
							AB, BC or CA, respectively, 
							4, 5, 6 if the projection coincides with the vertex
							A, B or C, respectively */
			static tuple<Real, point3d, UInt> project(const point3d & P, 
				const point3d & A, const point3d & B, const point3d & C);
				
			/*!	Project a point on a triangle (release mode).
				\param P	the point to project
				\param A	first vertex of the triangle
				\param B	second vertex of the triangle
				\param C	third vertex of the triangle
				\param N	unit normal to the triangle
				\param D	signed distance between the origin and 
							the plane definied by the triangle
				\param x	the "x"-coordinate
				\param y	the "y"-coordinate
				\return		the square distance between the point and
							the projected point 
				\return		the projected point 
				\return		0 if the projection falls inside the triangle,
							1, 2 or 3 if the projection falls onto the edge
							AB, BC or CA, respectively, 
							4, 5, 6 if the projection coincides with the vertex
							A, B or C, respectively */
			static tuple<Real, point3d, UInt> project(const point3d & P, 
				const point3d & A, const point3d & B, const point3d & C,
				const point3d & N, const Real & D, const UInt & x, const UInt & y);
											
			//
			// Non-static interface
			//
			
			/*!	Project a data point on a set of triangles.
				Ideally, the triangles are the ones involved in an edge collapsing.
				
				\param datum	data point Id
				\param elems	elements Id's
				\return			an STL pair storing the original location of the datum
								and Id's of the elements the datum belonged to */
			pair<point3d, vector<UInt>> project(const UInt & datum, 
				const vector<UInt> & elems);
						
			/*!	Project a set of data points on a set of triangles.
				Ideally, the triangles are the ones involved in an edge collapsing.
				
				\param data		data points Id's
				\param elems	elements Id's
				\return			a vector of STL pairs storing, for each data point, 
								its original location and Id's of the elements the 
								it used to belong */
			vector<pair<point3d, vector<UInt>>> project(const vector<UInt> & data, 
				const vector<UInt> & elems);
				
			/*!	Undo a previous projection.
				This method is useful for undoing temporary mesh modifications,
				e.g. for checking the validity of an edge collapsing or 
				computing the related cost.
				
				\param ids		Id's of data points to update
				\param oldData	information about old data points, i.e. 
								their location and data-element connections;
								ideally, these information come from project() */
			void undo(const vector<UInt> & ids, const vector<pair<point3d, vector<UInt>>> & oldData);
								
		protected:
			//
			// Auxiliary methods
			//
			
			/*!	Check if a 2D point belongs to a triangle.
				\param p	the point
				\param a	first vertex of the triangle
				\param b	second vertex of the triangle
				\param c	thrid vertex of the triangle
				\return		TRUE if the point belongs to the triangle,
							FALSE otherwise
				\return		0 if the point falls inside the triangle,
							1, 2 or 3 if the proint falls onto the edge
							ab, bc or cd, respectively, 
							4, 5, 6 if the point coincides with the vertex
							a, b or c, respectively */
			static pair<bool,UInt> inTri2d(const point2d & p, const point2d & a, const point2d & b, 
				const point2d & c);
			
			/*!	Starting from the belonging to an element, get all elements
				a projection falls within.
				This method should be called after a static version of project().
				
				\param Id	Id of the triangle ABC the projection falls within
							(as given by project())
				\param pos	0 if the projection falls inside the triangle,
							1, 2 or 3 if the projection falls onto the edge
							AB, BC or CA, respectively, 
							4, 5, 6 if the projection coincides with the vertex
							A, B or C, respectively 
				\return		Id's of the elements the projection falls within */			
			vector<UInt> getNewData2Elem(const UInt & Id, const UInt & pos) const;
	};
}

/*!	Include implementations of template class members. */
#include "implementation/imp_projection.hpp"

#endif

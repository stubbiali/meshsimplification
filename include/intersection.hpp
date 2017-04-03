/*!	\file	intersection.hpp
	\brief	A class to test if two equally-shaped elements intersect. */
	
#ifndef HH_INTERSECTION_HH
#define HH_INTERSECTION_HH

#include <memory>
#include <tuple>

#include "gutility.hpp"
#include "mesh.hpp"

namespace geometry
{			
	/*!	This is a template class testing whether two equally-shaped
		elements intersect or not. The template parameter is the 
		shape of the elements. The class provides both a static and
		a non-static interface. 
		
		At the moment, only the specialization for triangles is
		implemented. Further specializations may be added in future. 
		
		Reference: 
		O’Rourke J. "Computational geometry in C". 
		Cambridge (UK), Cambridge University Press, 1998. */
	template<typename SHAPE = Triangle>
	class intersection
	{
	};
	
	/*!	Specialization for triangles. */
	template<>
	class intersection<Triangle>
	{
		private:
			/*!	Pointer to a mesh. 
				We use bmesh class in the declaration so to 
				exploit polymorphism with the class mesh. */
			const bmesh<Triangle> * grid;
			
		public:
			//
			// Constructors
			//
			
			/*!	(Default) constructor.
				\param pg	pointer to a mesh */
			intersection(const bmesh<Triangle> * pg = nullptr);
						
			//
			// Access mesh
			//
			
			/*!	Set mesh pointer.
				\param pg	pointer to a new mesh */
			void setMesh(const bmesh<Triangle> * pg);
						
			//
			// Interface
			//
			
			/*!	Check the intersection between two triangular elements.
				\param id1	Id of first triangle
				\param id2	Id of second triangle
				\return		FALSE if the triangles do not intersect or
							intersect in a conformal way, TRUE otherwise */
			bool intersect(const UInt & id1, const UInt & id2) const;
			
			/*!	Check the intersection between two triangles.
				Note that this is a static interface.
				
				\param A	first vertex of first triangle
				\param B	second vertex of first triangle
				\param C	third verted of first triangle
				\param D	first vertex of second triangle
				\param E	second vertex of second triangle
				\param F	third vertex of second triangle */
			static bool intersect(const point3d & A, const point3d & B, const point3d & C, 
				const point3d & D, const point3d & E, const point3d & F);
	};
}

/*!	Include definitions of inlined members. */
#ifdef INLINED
#include "inline/inline_intersection.hpp"
#endif

#endif

/*!	\file	boundingBox.hpp
	\brief	Class representing the bounding box surrounding a N-dimensional element. */
	
#ifndef HH_BOUNDINGBOX_HH
#define HH_BOUNDINGBOX_HH

#include <iostream>

#include "geoPoint.hpp"
#include "bmeshInfo.hpp"

namespace geometry
{
	/*!	This class stores the extrema of the bounding box surrounding 
		an element. Moreover, it also computes the indices needed for
		a structured data search. However, the characteristic
		dimension of the mesh along each direction is needed, as long
		as the number of cells and its South-West point. Then all these 
		required information are stored as static class attributes. 
		To set them, call the method setup() before any use of this class.
		Note that for this class to properly work, the code should deal
		with just one mesh at a time. */
	template<UInt N>
	class boundingBox
	{
		private:
			//
			// Static attributes
			//
			
			/*!	Mesh (global) North-East and South-West point. */
			static geoPoint<N> NE_global;
			static geoPoint<N> SW_global;
			
			/*! Mesh characteristic dimension along each direction. */
			static array<Real,N> cellSize;
			
			/*! Number of cells along each direction. */
			static array<UInt,N> numCells;
			
			//
			// Non-static attributes
			//
			
			/*!	Element Id. */
			UInt Id;
			
			/*!	Element index. */
			UInt idx;
			
			/*! North-East and South-West (local) points. */
			geoPoint<N> NE;
			geoPoint<N> SW;
			
		public:
			//
			// Constructors
			//
			// We make use of variadic templates which allow 
			// to take an arbitrary number of vertices in input, 
			// so the class should work with any elements, 
			// either two- or three-dimensional.
			
			/*!	Default constructor. */
			boundingBox();
			
			/*!	Constructor converting an index to a boundingBox object.
				This will be useful when searching a bounding box
				in an ordered set (see structuredData class).
				
				\param index	element index */
			boundingBox(const UInt & index);
			
			/*!	Constructor.
				\param args	vertices of the element */			
			template<typename... Args>
			boundingBox(Args... args);
			
			/*!	Constructor.
				\param id	element Id
				\param args	vertices of the element */			
			template<typename... Args>
			boundingBox(const UInt & id, Args... args);
			
			/*!	Copy constructor.
				This method is provided only for \$ DIM \leq N \$.
				
				\param bb	another bounding box */
			template<UInt DIM>
			boundingBox<N>(const boundingBox<DIM> & bb);
			
			//
			// Operators
			//
			
			/*!	Less than operator: comparison based on indices.
				
				\param bb1	first bounding box
				\param bb2	second bounding box
				\return		result */
			template<UInt DIM>
			friend bool operator<(const boundingBox<DIM> & bb1, const boundingBox<DIM> & bb2);
			
			/*!	Equality operator: comparison based on indices.
				
				\param bb1	first bounding box
				\param bb2	second bounding box
				\return		result */
			template<UInt DIM>
			friend bool operator==(const boundingBox<DIM> & bb1, const boundingBox<DIM> & bb2);
			
			/*!	Inequality operator: comparison based on indices.
				
				\param bb1	first bounding box
				\param bb2	second bounding box
				\return		result */
			template<UInt DIM>
			friend bool operator!=(const boundingBox<DIM> & bb1, const boundingBox<DIM> & bb2);
			
			/*!	Output stream operator.
				\param out	output stream
				\param bb	a bounding box
				\return		updated output stream */
			template<UInt DIM>
			friend ostream & operator<<(ostream & out, const boundingBox<DIM> & bb);
			
			//
			// Other friend functions
			//
			
			/*!	Test if two bounding boxes (i.e., two N-dimensional parallelepipeds) intersect.
				\param bb1	first bounding box
				\param bb2	second bounding box
				\return		TRUE if they intersect, FALSE otherwise */
			template<UInt DIM>
			friend bool doIntersect(const boundingBox<DIM> & bb1, const boundingBox<DIM> & bb2); 
			
			//
			// Get methods
			//
				
			/*!	Get element Id.
				\return		the Id */	
			UInt getId() const;
			
			/*!	Get element index.
				\return		the index */	
			UInt getIdx() const;
			
			/*!	Get (local) North-East point.
				\return		the North-East point */	
			geoPoint<N> getNE() const;
			
			/*!	Get (local) South-West point.
				\return		the South-West point */	
			geoPoint<N> getSW() const;
			
			/*!	Get mesh (global) North-East point.
				\return		the mesh North-East point. */
			static geoPoint<N> getGlobalNE();
			
			/*!	Get mesh (global) South-West point.
				\return		the mesh South-West point. */
			static geoPoint<N> getGlobalSW();
			
			/*! Get one size of the cells.
				\param i	component
				\return 	the size */
			static Real getCellSize(const UInt & i);
			
			/*! Get cells sizes.
				\return		array with cells sizes */
			static array<Real,N> getCellSize();
			
			/*! Get number of cells along one direction.
				\param i	direction
				\return		number of cells */
			static UInt getNumCells(const UInt & i);
			
			/*! Get number of cells along each direction.
				\return		array with number of cells along each direction */
			static array<UInt,N> getNumCells();
			
			//
			// Set methods
			//
			// Note: we prevent the user to directly modify the index 
			// and the end-points of the (local) box. These can be modified 
			// only by the class once it will be given new vertices.
			// On the other hand, the user is allowed to modify the parameters
			// of the bounding box surrounding the grid. 
			// Static methods guaranteing the coherency of the static members 
			// are provided in the following.
			
			/*!	Set element Id.
				\param id	the new Id */
			void setId(const UInt & id);
			
			/*!	Update the vertices of the element, then its bounding box.
				\param args	vertices of the elements */
			template<typename... Args>
			void update(Args... args);
			
			/*!	Update the Id and the vertices of the element, then its bounding box.
				\param id	the new Id
				\param args	vertices of the elements */
			template<typename... Args>
			void update(const UInt & id, Args... args);
						
			/*! Set mesh (global) North-East point.
				\param p	the new North-East point */
			static void setGlobalNE(const geoPoint<N> & p);
			
			/*! Set mesh (global) South-West point.
				\param p	the new South-West point */
			static void setGlobalSW(const geoPoint<N> & p);
			
			/*! Set one cells size.
				\param i	component
				\param val	value */
			static void setCellSize(const UInt & i, const Real & val);
			
			/*! Set all cells sizes.
				\param val	array with new cells sizes */
			static void setCellSize(const array<Real,N> & val);
			
			/*! Set number of cells along one direction.
				\param i	component
				\param val	value */
			static void setNumCells(const UInt & i, const UInt & val);
			
			/*! Set number of cells along each direction.
				\param val	array with new number of cells */
			static void setNumCells(const array<UInt,N> & val);
			
			/*! Set static (i.e. global) class members.
				This method should be called BEFORE any use of the class.
								
				\param pne	mesh North-East point
				\param psw	mesh South-West point
				\param dl	cell size */
			static void setup(const geoPoint<N> & pne, const geoPoint<N> & psw, 
				const array<Real,N> & dl);
			
			/*! Set static (i.e. global) class members.
				This method should be called BEFORE any use of the class.
				It is provided only for three-dimensional elements.
				
				\param pne	mesh North-East point
				\param psw	mesh South-West point
				\param dx	cells size along x
				\param dy	cells size along y
				\param dz	cells size along z */
			static void setup(const point3d & pne = point3d(1.),  
				const point3d & psw = point3d(0.), 
				const Real & dx = 1., const Real & dy = 1., const Real & dz = 1.);
				
			/*! Set static (i.e. global) class members.
				This method should be called BEFORE any use of the class.
				It is provided only for three-dimensional elements.
				
				\param news	a bmeshInfo object
				
				\sa bmeshInfo.hpp, meshInfo.hpp */
			template<typename SHAPE, MeshType MT>
			static void setup(const bmeshInfo<SHAPE,MT> & news);
					
		private:
			//
			// Methods to compute (local) North-East and South-West points
			//
			
			/*!	Update the (local) North-East and South-West points 
				based on a vertex.
				
				\param p	the vertex */
			void computeBoundingBoxVertices(const geoPoint<N> & p);
			
			/*!	Set the (local) North-East and South-West points 
				based on an arbitrary number of vertices.
				This method will be called by the (non-default) 
				constructors and update().
				
				\param p	the first vertex
				\param args	all other vertices */
			template<typename... Args>
			void computeBoundingBoxVertices(const geoPoint<N> & p, Args... args);
			
			//
			// Methods to keep static variables coherent
			//     
			
			/*! Update number of cells along one direction. 
				\param i	direction */
			static void updateNumCells(const UInt & i);            
			
			/*! Update number of cells along each direction. */
			static void updateNumCells();
			
			/*! Update cells size along one direction.
				\param i	direction */
			static void updateCellSize(const UInt & i);
			
			/*! Update cells sizes along each direction. */
			static void updateCellSize();
	};
	
	/*!	A couple of typedef's for ease of notation. */
	using bbox2d = boundingBox<2>;
	using bbox3d = boundingBox<3>;
}

/*!	Include definitions of class members and friend functions. */
#include "implementation/imp_boundingBox.hpp"

#endif

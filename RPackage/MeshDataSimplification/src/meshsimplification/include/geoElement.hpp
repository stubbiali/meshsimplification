/*! \file  	geoElement.hpp
	\brief 	Class representing a geometric element (e.g. a triangle) and 
			the set of connected elements (e.g the vertices of the triangle). */
			
#ifndef HH_GEOELEMENT_HH
#define HH_GEOELEMENT_HH

#include <utility>
#include <iterator>

#include "shapes.hpp"

namespace geometry
{
	/*! This is a template class inherithing a shape (e.g. Triangle) and
		representing a mesh entity (e.g. edge, element) connected to a set of vertices (nodes). 
		The vertices are stored through their Id's.
		
		\sa shapes.hpp */
	template<typename SHAPE>
	class geoElement : public SHAPE
	{
		private:
			/*! Number of vertices. */
			static constexpr UInt NV = SHAPE::numVertices;
			
			/*! Id of the element. */
			UInt Id;
			
			/*!	Index of the element. Useful for structured data search. */
			UInt idx;
			
			/*! Geometric Id of the element. */
			UInt geoId;
			
			/*! Id's of the vertices. */
			array<UInt,NV> vertices;
			
			/*! Active flag. */
			bool active;
			
		public:
			//
			// Constructors
			//
			
			/*! (Default) constructor.
				\param id	element Id */
			geoElement(const UInt & id = 0);
			
			/*! Constructor.
				\param v	vertices Id's
				\param id	element Id
				\param gid	geometric Id */
			geoElement(const array<UInt,NV> & v, const UInt & id = 0, const UInt & gid = 0);
			
			/*! Copy constructor.
				\param g	another element */
			geoElement(const geoElement<SHAPE> & g) = default;
			
			//
			// Operators
			//
			
			/*! Copy assignment operator.
				\param g	another geo element
				\return		updated object */
			geoElement & operator=(const geoElement<SHAPE> & g);
			
			/*! Access operator (const version). It allows to access the vertices.
				\param i	position
				\return		i-th vertex Id */
			UInt operator[](const UInt & i) const;
			
			/*! Access operator (non const version). It allows to access the vertices.
				\param i	position
				\return		i-th vertex Id */
			UInt & operator[](const UInt & i);
			
			/*! Less than operator. The connected Id's are sorted before comparison.
				\param g1	the first geo element
				\param g2	the second geo element
				\return		bool saying whether g1 is "less than" g2 or not */
			template<typename S>
			friend bool operator<(const geoElement<S> & g1, const geoElement<S> & g2);
			
			/*! The inequality operator. The connected Id's are sorted before comparison.
				\param g1	the first geo element
				\param g2	the second geo element
				\return		bool saying whether g1 is different from g2 or not */
			template<typename S>
			friend bool operator!=(const geoElement<S> & g1, const geoElement<S> & g2);
		
			/*! The equality operator. The connected Id's are sorted before comparison.
				\param g1	the first geo element
				\param g2	the second geo element
				\return		bool saying whether g1 is equal to g2 or not */
			template<typename S>
			friend bool operator==(const geoElement<S> & g1, const geoElement<S> & g2);
			
			/*! Output stream operator.
				\param out	output stream
				\param elem	a geo element
				\return		updated output stream */
			template<typename S>
			friend ostream & operator<<(ostream & out, const geoElement<S> & g);
			
			//
			// Get methods
			//
			
			/*! Get element Id.
				\return		element Id */
			UInt getId() const;
			
			/*!	Get element index.
				\return		element index */
			UInt getIdx() const;
			
			/*! Get geometric Id.
				\return		geometric Id */
			UInt getGeoId() const;
			
			/*! Get number of vertices.
				\return 	number of vertices */
			CONSTEXPR UInt getNumVertices() const;
			
			/*! Get an array with vertices Id's.
				\return 	Id's of vertices */
			array<UInt,NV> getVertices() const;
			
			/*! Get active flag.
				\return		active flag */
			bool isActive() const;
			
			//
			// Set methods
			//
			
			/*! Set element Id.
				\param ID	the new element Id */
			void setId(const UInt & id);
			
			/*!	Set element index.
				\param index	the new element index */
			void setIdx(const UInt & index);
			
			/*! Set geometric Id.
				\param gid	the new geometric Id */
			void setGeoId(const UInt & gid);
			
			/*! Set all vertices Id's.
				\param v	array with vertices Id's */
			void setVertices(const array<UInt,NV> & v);
			
			/*! Set active flag to true. */
			void setActive();
			
			/*! Set active flag to false. */
			void setInactive();
			
			//
			// Find and replace methods
			//
			
			/*! Find a vertex.
				\param val	Id to search
				\return		boolean saying whether the vertex has been found or not */
			bool find(UInt & val) const;
			
			/*! Find a vertex.
				\param val	Id to search
				\return		pair where the first element is a bool saying
							whether the vertex has been found or not,
							while the second element is an iterator to the element */
			pair<typename array<UInt,NV>::iterator, bool> find(const UInt & val);
						
			/*! Replace a vertex.
				\param oldId	the old Id
				\param newId	the new Id
				\return 		status */
			bool replace(const UInt & oldId, const UInt & newId);
	};
}

/*! Include definitions of members and friend functions. */
#include "implementation/imp_geoElement.hpp"

#endif 

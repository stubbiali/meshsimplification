/*! \file 	point.hpp
	\brief 	Class storing a 3D point which may be, e.g., a node of a mesh. */

#ifndef HH_POINT_HH
#define HH_POINT_HH

#include <iostream>
#include <cmath>
#include <cassert>
#include <vector>

#include "geoPoint.hpp"

namespace geometry 
{		    
	/*! Class storing a node of a mesh and performing different operations on it. */
	class point : public geoPoint<3>
	{
		protected:
			/*! The node Id. */
			UInt Id;
			
			/*! Flag for boundary type:
			 	<ol>
			 	<li>  0 internal node
			 	<li>  1 boundary node
			 	<li>  2 triple (i.e. fixed) node
			 	</ol> */
			UInt boundary;
			
			/*! Active flag. */
			bool active;
	                
		public:
			//
			// Constructors and destructor
			//
			
			/*! (Default) constructor.
				\param x	x-coordinate
				\param y	y-coordinate
				\param z	z-coordinate
				\param ID	point Id
				\param bond	boundary flag */
	    	point(const Real & x = 0., const Real & y = 0., const Real & z = 0., 
	  			const UInt & ID = 0, const UInt & bond = 0);

			/*! Constructor. 
				\param c	array with coordinates
				\param ID	point Id
				\param bond	boundary flag */
		    point(const array<Real,3> & c, const UInt & ID = 0, const UInt & bond = 0);
		    
		    /*!	Constructor.
		    	\param gp	a geometric point
		    	\param ID	point Id
				\param bond	boundary flag */
			point(const geoPoint<3> & gp, const UInt & ID = 0, const UInt & bond = 0);

			/*! Synthetic copy constructor.
				\param p	point */
		    point(const point & p) = default;

			/*! Destructor */
		    virtual ~point() = default;
			
			//
			// Operators
			//
			
			/*! The equality operator. 
				\param p	point */
			virtual point & operator=(const point & p);
			
			/*! Output stream operator.
				\param out	output stream
				\param p	point to print
				\return update output stream */
			friend ostream & operator<<(ostream & out, const point & p);
			
			//
			// Get methods
			//
		
			/*! Get the Id.
				\return the Id */
			UInt getId() const;

			/*! Get boundary flag.
				\return the boundary flag */
			UInt getBoundary() const;
			
			/*! Check if the point is active.
				\return active flag */
			bool isActive() const;
						                
			//
			// Set methods
			//

			/*! Set the Id.
				\param newId	the new Id */
			void setId(const UInt & newId);

			/*! Set boundary information. 
				\param newBond	the new boundary flag */
			void setBoundary(const UInt & newBond);
			
			/*! Set active flag to true. */
			void setActive();
			
			/*! Set active flag to false. */
			void setInactive();
			
			/*!	Set coordinates.
				\param newCoor	the new coordinates. */
			void setCoor(const array<Real,3> & newCoor);
	};
}

/*! Include definitions of inlined members and friend functions. */
#ifdef INLINED
#include "inline/inline_point.hpp"
#endif

#endif



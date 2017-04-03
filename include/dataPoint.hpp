/*! \file 	dataPoint.hpp
	\brief 	Declaration of a class representing a point with an associated datum. */
	
#ifndef HH_DATAPOINT_HH
#define HH_DATAPOINT_HH

#include "point.hpp"

namespace geometry
{	
	/*! Class inheriting point and expanding it adding the datum associated to the point. */
	class dataPoint final : public point
	{
		private:
			/*! The datum. */
			Real datum;
			
		public:
			//
			// Constructors
			//
			
			/*! Default constructor. */
			dataPoint();
			
			/*! Constructor. 
				\param c	array with coordinates 
				\param ID	point Id
				\param dat	datum */
			dataPoint(const array<Real,3> & c, const UInt & ID = 0, const UInt & dat = 0.);
			
			/*! Constructor.
				\param p	point
				\param dat	datum */
			dataPoint(const point & p, Real const & dat = 0.);
			
			/*! Synthetic copy constructor.
				\param p	another point */
			dataPoint(const dataPoint & p) = default;
			
			//
			// Operators
			//
			
			/*! Copy assignment operator.
				\param p	another point */
			dataPoint & operator=(const dataPoint & p);
			
			//
			// Get methods
			//
			
			/*! Get the datum.
				\return 	the datum */
			Real getDatum() const;
			
			//
			// Set methods
			//
			
			/*! Set the datum.
				\param dat	the new datum */
			void setDatum(const Real & dat);
			
		private:
			/*! Print to output the point data.
				\param out	the output string */
			void print(ostream & out) const;
	}; 
}

/*! Include definitions of inlined members. */
#ifdef INLINED
#include "inline/inline_dataPoint.hpp"
#endif

#endif

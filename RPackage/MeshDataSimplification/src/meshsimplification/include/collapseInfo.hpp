/*!	\file	collapseInfo.hpp
	\brief	Class storing all information needed to apply an edge collapse. */
	
#ifndef HH_COLLAPSEINFO_HH
#define HH_COLLAPSEINFO_HH

#include "point.hpp"

namespace geometry
{
	/*!	This is a merely storing class bringing all information for
		applying an edge collapse. 
		Conversely to collapsingEdge, the ordering is based on the 
		Id's of the edge end-points, not on the cost. Then, this
		class should be used by a cost class for keeping track of
		the costs.
		
		\sa collapsingEdge.hpp */
	class collapseInfo
	{
		private:
			/*!	Id's of edge end-points. */
			UInt Id1;
			UInt Id2;
		
			/*!	Collapsing point. */
			point cPoint;
		
			/*!	Collapsing cost. */
			Real cost;
			
		public:
			//
			// Constructor
			//
			
			/*!	Synthetic default constructor. */
			collapseInfo() = default;
			
			/*!	Constructor.
				\param id1	Id of first end-point
				\param id2	Id of second end-point
				\param val	collapsing cost
				\param cp	collapsing point */
			collapseInfo(const UInt & id1, const UInt & id2, const Real & val = 0.,
				const point3d & cp = {0.,0.,0.});
				
			/*!	Constructor.
				\param ids	vector with end-points Id's
				\param val	collapsing cost
				\param cp	collapsing point */
			collapseInfo(const vector<UInt> & ids, const Real & val = 0., 
				const point3d & cp = {0.,0.,0.});
				
			/*!	Synthetic copy constructor. 
				\param cInfo	another collapseInfo object */
			collapseInfo(const collapseInfo & cInfo) = default;
			
			//
			// Operators
			//
			
			/*!	Copy-assignment operator.
				\param cInfo	another collapseInfo object
				\return			the updated current object */
			collapseInfo & operator=(const collapseInfo & cInfo);
				
			/*!	Equality operator: only compare end-points Id's,
				i.e. two collapseInfo objects with the same Id's but 
				different end-points are treated as equal.
				
				\param cInfo1	first collapseInfo object
				\param cInfo2	second collapseInfo object
				\return			result of the comparison */		
			friend bool operator==(const collapseInfo & cInfo1, const collapseInfo & cInfo2);
			
			//
			// Get methods
			//
			
			/*!	Get Id of first end-point.
				\return		Id of first end-point */
			UInt getId1() const;
			
			/*!	Get Id of second end-point.
				\return		Id of second end-point */
			UInt getId2() const;
			
			/*!	Get collapsing point.
				\return		the collapsing point */
			point getCollapsingPoint() const;
			
			/*!	Get collapsing cost.
				\return		the cost */
			Real getCost() const;
			
			//
			// Set methods
			//
			
			/*!	Set Id of first end-point.
				\param id1	the new Id */
			void setId1(const UInt & id1);
			
			/*!	Set Id of second end-point.
				\param id2	the new Id */
			void setId2(const UInt & id2);
			
			/*!	Set collapsing point.
				\param cp	the new point */
			void setCollapsingPoint(const point3d & cp);
					
			/*!	Set collapsing cost.
				\param val	the new cost */	
			void setCost(const Real & val);
	};
}

/*!	Include implementations of inlined class members and friend functions. */
#ifdef INLINED
#include "inline/inline_collapseInfo.hpp"
#endif

#endif

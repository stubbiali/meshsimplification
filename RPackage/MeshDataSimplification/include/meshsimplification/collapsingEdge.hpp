/*!	\file	collapsingEdge.hpp
	\brief	Class storing all information needed to apply an edge collapse. */
	
#ifndef HH_COLLAPSINGEDGE_HH
#define HH_COLLAPSINGEDGE_HH

#include "point.hpp"

namespace geometry
{
	/*!	This is a merely storing class bringing all information for
		applying an edge collapse. 
		Conversely to collapseInfo, the ordering is based on the 
		cost, not on the Id's of the edge end-points. Then, this
		class may be useful for class simplification for ordering
		the edges based on their cost.
		
		\sa collapseInfo.hpp */
	class collapsingEdge
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
			collapsingEdge() = default;
			
			/*!	Constructor.
				\param id1	Id of first end-point
				\param id2	Id of second end-point
				\param val	collapsing cost
				\param cp	collapsing point */
			collapsingEdge(const UInt & id1, const UInt & id2, const Real & val = 0.,
				const point3d & cp = {0.,0.,0.});
				
			/*!	Constructor.
				\param ids	vector with end-points Id's
				\param val	collapsing cost
				\param cp	collapsing point */
			collapsingEdge(const vector<UInt> & ids, const Real & val = 0.,
				const point3d & cp = {0.,0.,0.});
				
			/*!	Synthetic copy constructor. 
				\param cEdge	another collapsingEdge object */
			collapsingEdge(const collapsingEdge & cEdge) = default;
			
			//
			// Operators
			//
			
			/*!	Copy-assignment operator.
				\param cEdge	another collapsingEdge object
				\return			the updated current object */
			collapsingEdge & operator=(const collapsingEdge & cEdge);
				
			/*!	Less than operator: first compare the costs,
				then possibly the Id's.
				
				\param cEdge1	first collapsingEdge object
				\param cEdge2	second collapsingEdge object
				\return			result of the comparison */		
			friend bool operator<(const collapsingEdge & cEdge1, const collapsingEdge & cEdge2);
			
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
#include "inline/inline_collapsingEdge.hpp"
#endif

#endif

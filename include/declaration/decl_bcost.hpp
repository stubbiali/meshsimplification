/*!	\file	decl_bcost.hpp
	\brief 	Declaration of members of class bcost. */
	
#ifndef HH_DECLBCOST_HH
#define HH_DECLBCOST_HH

#include <unordered_set>
#include <tuple>

#include "bmeshOperation.hpp"
#include "collapseInfo.hpp"

namespace geometry
{
	/*!	This is a base class for those classes which compute
		the cost associated with and edge collapsing.
		The Curiously Recurring Template Pattern (CRTP) is used.
		Then, this class defines the common interface
		for all its children, delegated to them the implementation
		of some of its members.
		Note that, as required by the CRTP technique, the
		derived class appears as a template parameter of the class. 
		
		All cost classes contain an ordered set of collapseInfo
		objects. Then, it should be fast to find an edge in the list
		based on its end-points. 
		Conversely, the class simplification will contain a set of 
		collapsingEdge objects ascending-ordered according to their 
		costs. Then, in simplification should be fast to find an edge 
		based on its cost.
		In this sense, bcost and simplification are complementary:
		when simplification needs to find an edge given its end-points,
		first it will ask bcost for the cost, then it will look for
		the cost in its list.
		
		\sa collapseInfo.hpp, collapsingInfo.hpp, simplification.hpp */
	template<typename SHAPE, MeshType MT, typename D>
	class bcost
	{
		protected:
			/*!	Pointer to a bmeshOperation object, i.e. the operator. 
				\sa bmeshOperation.hpp */
			bmeshOperation<SHAPE,MT> * oprtr;
			
			/*!	Store info on collapsing edges through
				an unordered set of collapseInfo objects. */
			unordered_set<collapseInfo> cInfoList;
			
		public:
			//
			// Constructors
			//
			
			/*!	(Default) constructor.
				\param bmo	pointer to a bmeshOperation object */
			bcost(bmeshOperation<SHAPE,MT> * bmo = nullptr);
			
			//
			// Set methods
			//
			
			/*!	Set pointer to bmeshOperation.
				The implementation is delegated to the derived class.
				
				\param bmo	pointer to a bmeshOperation object */
			void setMeshOperation(bmeshOperation<SHAPE,MT> * bmo);
			
			//
			// Access members
			//
			
			/*!	Get list of collapseInfo objects.
				\return		vector of collapseInfo objects */
			vector<collapseInfo> getCollapseInfoList() const;
			
			//
			// Get methods
			//
			
			/*!	Get list of valid collapsing points for an edge.
				Here, their validity is sanctioned according to 
				the boundary flags of the edge end-points.
				The implementation is delegated to the derived class.
				
				\param id1	Id of first end-point of the edge
				\param id2	Id of second end-point of the edge
				\return		vector of valid points */
			vector<point> getPointsList(const UInt & id1, const UInt & id2) const;
			
			/*!	Get cost for collapsing an edge in a point and possibly keep
				track of the resulting cost(s) for future class updates.
				The implementation is delegated to the derived class.
			
				\param id1		Id of first end-point of the edge
				\param id2		Id of second end-point of the edge
				\param p		collapsing point
				\param toKeep	Id's of elements involved in the collapse 
								but not insisting on the edge
				\param toMove	Id's of data points involved in the collapse
				\return			the cost 
				
				Note that the last two parameters may not be required by the
				implementation, this is why they are given default (empty) values.
				However, the user is not exposed to this problem thanks to
				template specialization used in the implementation of the method.
				One may (or may not!) give all the arguments to the interface,
				then the class bcost is in charge of forwarding the right number
				of arguments to the implementation. */
			Real getCost(const UInt & id1, const UInt & id2, const point3d & p, 
				const vector<UInt> & toKeep = {}, const vector<UInt> & toMove = {});
				
			/*!	Get cost for collapsing an edge in a point without keeping
				track of the resulting cost(s) for future class updates. Indeed,
				"f" stands for "fast".
				The implementation is delegated to the derived class.
			
				\param id1		Id of first end-point of the edge
				\param id2		Id of second end-point of the edge
				\param p		collapsing point
				\param toKeep	Id's of elements involved in the collapse 
								but not insisting on the edge
				\param toMove	Id's of data points involved in the collapse
				\return			the cost 
				
				Note that the last two parameters may not be required by the
				implementation, this is why they are given default (empty) values.
				However, the user is not exposed to this problem thanks to
				template specialization used in the implementation of the method.
				One may (or may not!) give all the arguments to the interface,
				then the class bcost is in charge of forwarding the right number
				of arguments to the implementation. */
			Real getCost_f(const UInt & id1, const UInt & id2, const point3d & p, 
				const vector<UInt> & toKeep = {}, const vector<UInt> & toMove = {}) const;
						
			//
			// Updating methods
			//
							
			/*!	Add a collapseInfo object to the list and possible check if the class  
				requires an update.
				The implementation is delegated to the derived class.
				
				\param id1	Id of first end-point of the edge
				\param id2	Id of second end-point of the edge
				\param val	collapsing cost
				\param p	collapsing point */
			void addCollapseInfo(const UInt & id1, const UInt & id2, const Real & val,
				const point3d & p);
				
			/*!	Add a collapseInfo object to the list without checking if the class  
				requires an update. Indeed, "f" stands for "fast".
				The implementation is delegated to the derived class.
				
				\param id1	Id of first end-point of the edge
				\param id2	Id of second end-point of the edge
				\param val	collapsing cost
				\param p	collapsing point */
			void addCollapseInfo_f(const UInt & id1, const UInt & id2, const Real & val,
				const point3d & p);
			
			/*!	Erase a collapseInfo object from the list.
				\param id1	Id of first end-point of the edge
				\param id2	Id of second end-point of the edge
				\return		TRUE if the edge has been found, FALSE otherwise
				\return		previously computed cost associated with the edge */
			pair<bool,Real> eraseCollapseInfo(const UInt & id1, const UInt & id2); 
			
			/*!	Update after an edge collapse.
				This method should be called after having updated the mesh
				and all the connections.
				The implementation is delegated to the derived class.
				Actually, some derived cost classes may not require an update.
				In that case, the implementation would simply do nothing.
				
				\param newId	Id of the collapsing point 
				\param oldId	Id of the node to remove
				\param toRemove	Id's of the elements to remove 
				
				Note that the last two parameters may not be required by the
				implementation, this is why they are given default (empty) values.
				However, the user is not exposed to this problem thanks to
				template specialization used in the implementation of the method.
				One may (or may not!) give all the arguments to the interface,
				then the class bcost is in charge of forwarding the right number
				of arguments to the implementation.
				For further details, see imp_bcost.hpp.*/
			void update(const UInt & newId, const UInt & oldId = 0.,
				const vector<UInt> & toRemove = {});
				
			/*!	Check whether the collapseInfo's list should be re-built.
				The implementation is delegated to the derived class.
				Some derived classes, e.g. OnlyGeo, may never require a re-build. 
				
				\return		TRUE if the collapseInfo's list should be re-built,
							FALSE otherwise */
			bool toUpdate() const;
			
			/*!	Clear the collapseInfo's list.
				The implementation is delegated to the derived class. */
			void clear(); 
	};
}

#endif

/*!	\file	imp_bcost.hpp
	\brief	Implementations of members of class bcost. */
	
#ifndef HH_IMPBCOST_HH
#define HH_IMPBCOST_HH

#include <string>
#include <stdexcept>

namespace geometry
{
	//
	// Constructor
	//
	
	template<typename SHAPE, MeshType MT, typename D>
	bcost<SHAPE,MT,D>::bcost(bmeshOperation<SHAPE,MT> * bmo) :
		oprtr(bmo)
	{
	}
	
	
	// 
	// Set methods
	//
	
	template<typename SHAPE, MeshType MT, typename D>
	INLINE void bcost<SHAPE,MT,D>::setMeshOperation(bmeshOperation<SHAPE,MT> * bmo)
	{
		static_cast<D *>(this)->imp_setMeshOperation(bmo);
	}
	
	
	//
	// Access members
	//
	
	template<typename SHAPE, MeshType MT, typename D>
	INLINE vector<collapseInfo> bcost<SHAPE,MT,D>::getCollapseInfoList() const
	{
		return {cInfoList.cbegin(), cInfoList.cend()};
	}
	
	
	//
	// Get methods
	//
	
	template<typename SHAPE, MeshType MT, typename D>
	INLINE vector<point> bcost<SHAPE,MT,D>::getPointsList(const UInt & id1, 
		const UInt & id2) const
	{
		return static_cast<const D *>(this)->imp_getPointsList(id1, id2);
	}
		
	
	//
	// Updating methods
	//
	
	
	template<typename SHAPE, MeshType MT, typename D>
	INLINE void bcost<SHAPE,MT,D>::addCollapseInfo(const UInt & id1, const UInt & id2, 
		const Real & val, const point3d & p)
	{
		static_cast<D *>(this)->imp_addCollapseInfo(id1, id2, val, p);
	}
	
	
	template<typename SHAPE, MeshType MT, typename D>
	INLINE void bcost<SHAPE,MT,D>::addCollapseInfo_f(const UInt & id1, const UInt & id2, 
		const Real & val, const point3d & p)
	{
		cInfoList.emplace(id1, id2, val, p);
	}
	
	
	template<typename SHAPE, MeshType MT, typename D>
	pair<bool,Real> bcost<SHAPE,MT,D>::eraseCollapseInfo(const UInt & id1, const UInt & id2)
	{
		// Find the edge and before erasing it extract the related cost
		// Correctly handle the case the edge cannot be found
		auto it = cInfoList.find({id1, id2});
		if (it != cInfoList.end())
		{
			auto val = it->getCost();
			cInfoList.erase(it);
			return {true, val};
		}
		return {false, -1};
	}
	
	
	template<typename SHAPE, MeshType MT, typename D>
	INLINE bool bcost<SHAPE,MT,D>::toUpdate() const
	{
		return static_cast<const D *>(this)->imp_toUpdate();
	}
	
	
	template<typename SHAPE, MeshType MT, typename D>
	INLINE void bcost<SHAPE,MT,D>::clear()
	{
		static_cast<D *>(this)->imp_clear();
	}
}

#ifdef INLINED
#include "inline/inline_bcost.hpp"
#endif

#endif

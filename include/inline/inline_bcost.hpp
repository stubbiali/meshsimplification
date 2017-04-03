/*!	\file	inline_bcost.hpp
	\brief	Specialization of some members of class bcost. */
	
#ifndef HH_INLINEBCOST_HH
#define HH_INLINEBCOST_HH

namespace geometry
{
	//
	// Get methods
	//
	
	// Both OnlyGeo<MeshType::GEO> and OnlyGeo<MeshType::DATA> just need
	// the edge end-points and the collapsing point
	template<>
	INLINE Real bcost<Triangle, MeshType::GEO, OnlyGeo<MeshType::GEO>>::getCost
		(const UInt & id1, const UInt & id2, const point3d & p, 
		const vector<UInt> & toKeep, const vector<UInt> & toMove)
	{
		return static_cast<const OnlyGeo<MeshType::GEO> *>(this)
			->imp_getCost(id1, id2, p);
	}
	
	
	template<>
	INLINE Real bcost<Triangle, MeshType::DATA, OnlyGeo<MeshType::DATA>>::getCost
		(const UInt & id1, const UInt & id2, const point3d & p, 
		const vector<UInt> & toKeep, const vector<UInt> & toMove)
	{
		return static_cast<const OnlyGeo<MeshType::DATA> *>(this)
			->imp_getCost(id1, id2, p);
	}
	
	
	// Specialization for DataGeo, requiring also the Id's of the elements 
	// and the data points involved in the collapse
	template<>
	INLINE Real bcost<Triangle, MeshType::DATA, DataGeo>::getCost
		(const UInt & id1, const UInt & id2, const point3d & p, 
		const vector<UInt> & toKeep, const vector<UInt> & toMove)
	{
		return static_cast<DataGeo *>(this)
			->imp_getCost(id1, id2, p, toKeep, toMove);
	}
	
	
	// Both OnlyGeo<MeshType::GEO> and OnlyGeo<MeshType::DATA> just need
	// the edge end-points and the collapsing point
	template<>
	INLINE Real bcost<Triangle, MeshType::GEO, OnlyGeo<MeshType::GEO>>::getCost_f
		(const UInt & id1, const UInt & id2, const point3d & p, 
		const vector<UInt> & toKeep, const vector<UInt> & toMove) const
	{
		return static_cast<const OnlyGeo<MeshType::GEO> *>(this)
			->imp_getCost_f(id1, id2, p);
	}
	
	
	template<>
	INLINE Real bcost<Triangle, MeshType::DATA, OnlyGeo<MeshType::DATA>>::getCost_f
		(const UInt & id1, const UInt & id2, const point3d & p, 
		const vector<UInt> & toKeep, const vector<UInt> & toMove) const
	{
		return static_cast<const OnlyGeo<MeshType::DATA> *>(this)
			->imp_getCost_f(id1, id2, p);
	}
	
	
	// Specialization for DataGeo, requiring also the Id's of the elements 
	// and the data points involved in the collapse
	template<>
	INLINE Real bcost<Triangle, MeshType::DATA, DataGeo>::getCost_f
		(const UInt & id1, const UInt & id2, const point3d & p, 
		const vector<UInt> & toKeep, const vector<UInt> & toMove) const
	{
		return static_cast<const DataGeo *>(this)
			->imp_getCost_f(id1, id2, p, toKeep, toMove);
	}
	
	
	//
	// Updating methods
	//
	
	// Both OnlyGeo<MeshType::GEO> and OnlyGeo<MeshType::DATA> just need
	// the Id of the collapsing point for the update
	template<>
	INLINE void bcost<Triangle, MeshType::GEO, OnlyGeo<MeshType::GEO>>::update
		(const UInt & newId, const UInt & oldId, const vector<UInt> & toRemove)
	{
		static_cast<OnlyGeo<MeshType::GEO> *>(this)->imp_update(newId);
	}
	
	
	template<>
	INLINE void bcost<Triangle, MeshType::DATA, OnlyGeo<MeshType::DATA>>::update
		(const UInt & newId, const UInt & oldId, const vector<UInt> & toRemove)
	{
		static_cast<OnlyGeo<MeshType::DATA> *>(this)->imp_update(newId);
	}
	
	// Specialization for DataGeo, requiring also the Id's of the elements to remove
	template<>
	INLINE void bcost<Triangle, MeshType::DATA, DataGeo>::update(const UInt & newId, const UInt & oldId,
		const vector<UInt> & toRemove)
	{
		static_cast<DataGeo *>(this)->imp_update(newId, toRemove);
	}
}

#endif

/*!	\file	inline_simplification.hpp
	\brief	Specialization of some members of class simplification. */
			
#ifndef HH_INLINESIMPLIFICATION_HH
#define HH_INLINESIMPLIFICATION_HH

namespace geometry
{
	//
	// Get methods
	//
	
	template<MeshType MT, typename CostClass>
	INLINE const mesh<Triangle,MT> * simplification<Triangle, MT, CostClass>::
		getCPointerToMesh() const
	{
		return this->gridOperation.getCPointerToMesh();
	}
			
	
	template<MeshType MT, typename CostClass>
	INLINE const connect<Triangle,MT> * simplification<Triangle, MT, CostClass>::
		getCPointerToConnectivity() const
	{
		return this->gridOperation.getCPointerToConnectivity();
	}
	
	
	template<MeshType MT, typename CostClass>
	INLINE const bmeshOperation<Triangle,MT> * simplification<Triangle, MT, CostClass>::
		getCPointerToMeshOperator() const
	{
		return &this->gridOperation;
	}
		
	
	//
  	// Compute cost and apply collapse
  	//
	
	template<>
	INLINE void simplification<Triangle, MeshType::GEO, OnlyGeo<MeshType::GEO>>::
		getCost(const UInt & id1, const UInt & id2)
	{
		getCost_f(id1, id2);
	}
	
	
	template<>
	INLINE void simplification<Triangle, MeshType::DATA, OnlyGeo<MeshType::DATA>>::
		getCost(const UInt & id1, const UInt & id2)
	{
		getCost_f(id1, id2);
	}
}

#endif

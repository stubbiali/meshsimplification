/*!	\file 	bmeshOperation.hpp
		
	The class bmeshOperation<SHAPE,MT> should inherit
	meshInfo<SHAPE,MT> if MT is MeshType::GEO,
	projection<SHAPE> if MT is MeshType::DATA.
	For this aim, we exploit template typedef's and
	type traits.
	First, we introduce a compile time-evaluated function
	converting a MeshType object in a bool. In particular,
	MeshType::GEO is converted to TRUE, MeshType::DATA to
	FALSE. The function is template in the MeshType object.
	Then, we combine a template typedef with the
	class conditional provided by type_traits.
	Hence, bmeshOperation is ultimately a placeholder for
	either meshInfo or projection.
	
	\sa meshInfo.hpp, projection.hpp, meshOperation.hpp */
	
#ifndef HH_BMESHOPERATION_HH
#define HH_BMESHOPERATION_HH

#include <type_traits>

#include "meshInfo.hpp"
#include "projection.hpp"

namespace geometry
{	
	/*!	constexpr function converting a MeshType object in a bool. 
		\return		TRUE if the template argument is MeshType::GEO,
					FALSE otherwise */
	template<MeshType MT>
	constexpr bool MeshType2Bool();
		
	/*!	Specialization for MeshType::GEO. */
	template<>
	constexpr bool MeshType2Bool<MeshType::GEO>() 
	{
		return true;
	};
	
	/*!	Specialization for MeshType::DATA. */
	template<>
	constexpr bool MeshType2Bool<MeshType::DATA>() 
	{
		return false;
	};
	
	/*!	A template typedef exploiting type traits. */
	template<typename SHAPE, MeshType MT>
	using bmeshOperation = 
		typename conditional<MeshType2Bool<MT>(), meshInfo<SHAPE,MT>, projection<SHAPE>>::type;
}

#endif

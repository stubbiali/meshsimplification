/*!	\file	imp_projection.hpp
	\brief	Implementations of template members of class projection<Triangle>. */
	
#ifndef HH_IMPPROJECTION_HH
#define HH_IMPPROJECTION_HH

namespace geometry
{
	//
	// Constructors
	//
	
	template<typename... Args>
	projection<Triangle>::projection(Args... args) :
		meshInfo<Triangle, MeshType::DATA>(args...)
	{
	}
}

#endif

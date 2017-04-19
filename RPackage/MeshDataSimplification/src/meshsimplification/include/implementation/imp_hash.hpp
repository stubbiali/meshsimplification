/*!	\file	imp_hash.hpp
	\brief	Re-define hash function for user-defined template classes. */
	
#ifndef HH_IMPHASH_HH
#define HH_IMPHASH_HH

namespace std
{
	//
	// Specialization for boundingBox<N>
	//
	
	template<UInt N>
	INLINE size_t hash<boundingBox<N>>::operator()(const boundingBox<N> & bb) const
	{
		return bb.getIdx();
	}
}

#endif

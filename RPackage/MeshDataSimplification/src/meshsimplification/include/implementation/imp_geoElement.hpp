/*! \file  	imp_geoElement.hpp
	\brief 	Definitions of members and friend functions of template class geoElement. */
	
#ifndef HH_IMPGEOELEMENT_HH
#define HH_IMPGEOELEMENT_HH

#include <algorithm>
#include <utility>
#include <iterator>
#include <set>

namespace geometry
{
	//
	// Constructors
	//
	
	template<typename SHAPE>
	geoElement<SHAPE>::geoElement(const UInt & id) : 
		Id(id), idx(0), geoId(0), active(true) 
	{
	}
	
	
	template<typename SHAPE>
	geoElement<SHAPE>::geoElement(const array<UInt,NV> & v, const UInt & id, const UInt & gid) :
		Id(id), idx(0), geoId(gid), vertices(v), active(true) 
	{
	}
	
	
	// 
	// Operators
	//
	
	template<typename SHAPE>
	geoElement<SHAPE> & geoElement<SHAPE>::operator=(const geoElement<SHAPE> & g)
	{
		// Copy index
		idx = g.idx;
		
		// Copy geo Id
		geoId = g.geoId;
		
		// Copy vertices
		copy(g.vertices.cbegin(), g.vertices.cend(), vertices.begin());
		
		// Copy active flag
		active = g.active;
		
		return *this;
	}
	
	
	template<typename SHAPE>
	INLINE UInt geoElement<SHAPE>::operator[](const UInt & i) const
	{
		return vertices[i];
	}
	
	
	template<typename SHAPE>
	INLINE UInt & geoElement<SHAPE>::operator[](const UInt & i)
	{
		return vertices[i];
	}
	
	
	template<typename SHAPE>
	bool operator<(const geoElement<SHAPE> & g1, const geoElement<SHAPE> & g2)
	{
		// Sort vertices Id's
		const set<UInt> s1(g1.vertices.cbegin(), g1.vertices.cend());
		const set<UInt> s2(g2.vertices.cbegin(), g2.vertices.cend());
		
		// Compare
		return (s1 < s2);
	}
	
	
	template<typename SHAPE>
	bool operator==(const geoElement<SHAPE> & g1, const geoElement<SHAPE> & g2)
	{
		// Sort vertices Id's
		const set<UInt> s1(g1.vertices.cbegin(), g1.vertices.cend());
		const set<UInt> s2(g2.vertices.cbegin(), g2.vertices.cend());
		
		// Compare
		return (s1 == s2);
	}
	
	
	template<typename SHAPE>
	bool operator!=(const geoElement<SHAPE> & g1, const geoElement<SHAPE> & g2)
	{
		// Sort vertices Id's
		const set<UInt> s1(g1.vertices.cbegin(), g1.vertices.cend());
		const set<UInt> s2(g2.vertices.cbegin(), g2.vertices.cend());
		
		// Compare
		return (s1 != s2);
	}
	
	
	template<typename SHAPE>
	ostream & operator<<(ostream & out, const geoElement<SHAPE> & g)
	{
		if (g.active)
		{
			out << "Element Id   : " << g.Id << endl;
			out << "Index        : " << g.idx << endl;
			out << "Geometric Id : " << g.geoId << endl;
			out << "Vertices Id's: ";
			for (auto v : g.vertices)
				out << v << " ";
			out << endl;
		}
		else
			out << "Element " << g.Id << " is inactive." << endl;
			
		return out;
	}
	
	
	//
	// Get methods
	//
	
	template<typename SHAPE>
	INLINE UInt geoElement<SHAPE>::getId() const
	{
		return Id;
	}
	
	
	template<typename SHAPE>
	INLINE UInt geoElement<SHAPE>::getIdx() const
	{
		return idx;
	}
	
	
	template<typename SHAPE>
	INLINE UInt geoElement<SHAPE>::getGeoId() const
	{
		return geoId;
	}
	
	
	template<typename SHAPE>
	CONSTEXPR UInt geoElement<SHAPE>::getNumVertices() const
	{
		return NV;
	}
	
	
	template<typename SHAPE>
	INLINE array<UInt,geoElement<SHAPE>::NV> geoElement<SHAPE>::getVertices() const
	{
		return vertices;
	}
	
	
	template<typename SHAPE>
	bool geoElement<SHAPE>::isActive() const
	{
		return active;
	}
	
	
	//
	// Set methods
	//
	
	template<typename SHAPE>
	INLINE void geoElement<SHAPE>::setId(const UInt & id)
	{
		Id = id;
	}
	
	
	template<typename SHAPE>
	INLINE void geoElement<SHAPE>::setIdx(const UInt & index)
	{
		idx = index;
	}
	
	
	template<typename SHAPE>
	INLINE void geoElement<SHAPE>::setGeoId(const UInt & gid)
	{
		geoId = gid;
	}
	
	
	template<typename SHAPE>
	INLINE void geoElement<SHAPE>::setVertices(const array<UInt,NV> & v)
	{
		copy(v.cbegin(), v.cend(), vertices.begin());
	}
	
	
	template<typename SHAPE>
	INLINE void geoElement<SHAPE>::setActive()
	{
		active = true;
	}
	
	
	template<typename SHAPE>
	INLINE void geoElement<SHAPE>::setInactive()
	{
		active = false;
	}
	
	
	//
	// Find and replace methods
	//
	
	template<typename SHAPE>
	INLINE bool geoElement<SHAPE>::find(UInt & val) const
	{
		return (std::find(vertices.cbegin(), vertices.cend(), val) != vertices.cend());
	}
	
	
	template<typename SHAPE>
	pair<typename array< UInt, geoElement<SHAPE>::NV >::iterator, bool> geoElement<SHAPE>::find(const UInt & val)
	{
		auto it = std::find(vertices.begin(), vertices.end(), val);
		return make_pair(it, it != vertices.end());
	}
	
	
	template<typename SHAPE>
	bool geoElement<SHAPE>::replace(const UInt & oldId, const UInt & newId)
	{
		auto it = std::find(vertices.begin(), vertices.end(), oldId);
		if (it != vertices.end())
		{
			*it = newId;
			return true;
		}
		else	return false;
	}
}

#endif	

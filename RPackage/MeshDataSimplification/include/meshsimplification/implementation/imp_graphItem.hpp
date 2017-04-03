/*!	\file	imp_graphItem.hpp
	\brief	Definitions of template friend functions of class graphItem. */
	
#ifndef HH_IMPGRAPHITEM_HH
#define HH_IMPGRAPHITEM_HH

#include <algorithm>

namespace geometry
{
	template<typename... Args>
	set<UInt> set_intersection(const graphItem & g, Args... args)
	{
		// Find the Id's shared by args
		auto s = set_intersection(args...);
		
		// Make the intersection with g
		set<UInt> res;
		set_intersection(g.conn.cbegin(), g.conn.cend(), s.begin(), s.end(), inserter(res, res.begin()));
		
		return res;
	}
	
	
	template<typename... Args>
	set<UInt> set_union(const graphItem & g, Args... args)
	{
		// Find the Id's shared by at least one of args
		auto s = set_union(args...);
		
		// Make the union with g
		set<UInt> res;
		set_union(g.conn.cbegin(), g.conn.cend(), s.begin(), s.end(), inserter(res, res.begin()));
		
		return res;
	}
}

#endif

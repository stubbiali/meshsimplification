/*! \file  	inline_graphItem.hpp
	\brief 	Definitions of inlined members and friend functions of class graphItem. */
	
#ifndef HH_INLINEGRAPHITEM_HH
#define HH_INLINEGRAPHITEM_HH

#include <algorithm>

namespace geometry
{
	//
	// Definitions of inlined members
	//
	
	INLINE UInt graphItem::getId() const 
	{
		return Id;
	}
			
	
	INLINE UInt graphItem::size() const 
	{
		return conn.size();
	}
	
	
	INLINE vector<UInt> graphItem::getConnected() const 
	{
		return vector<UInt>(conn.cbegin(), conn.cend());
	}
	
	
	INLINE bool graphItem::isActive() const 
	{
		return active;
	}
	
	
	INLINE void graphItem::setId(const UInt & ID) 
	{
		Id = ID;
	}
	
	
	INLINE void graphItem::setActive() 
	{
		active = true;
	}
	
	
	INLINE void graphItem::setInactive() 
	{
		active = false;
	}
	
	
	INLINE bool graphItem::find(const UInt & val) const 
	{
		return std::find(conn.cbegin(), conn.cend(), val) != conn.cend();
	}
	
	
	INLINE void graphItem::insert(const UInt & val) 
	{
		conn.insert(val);
	}
	
	
	INLINE void graphItem::insert(const set<UInt> & s)
	{
		set_union(conn.begin(), conn.end(), s.cbegin(), s.cend(), inserter(conn, conn.begin()));
	}
	
	
	INLINE UInt graphItem::erase(const UInt & val) 
	{
		return conn.erase(val);
	}
	
	
	INLINE void graphItem::erase(set<UInt>::iterator it) 
	{
		conn.erase(it);
	}
	
	
	INLINE void graphItem::clear() 
	{
		conn.clear();
	}
	
	
	//
	// Definitions of inlined friend functions
	//
	
	INLINE bool operator<(const graphItem & g1, const graphItem & g2)
	{
		return (g1.conn < g2.conn);
	}
	
	
	INLINE bool operator!=(const graphItem & g1, const graphItem & g2)
	{
		return (g1.conn != g2.conn);
	}
	
	
	INLINE bool operator==(const graphItem & g1, const graphItem & g2)
	{
		return (g1.conn == g2.conn);
	}
	
	
	//
	// Common and uncommon connected
	//
	
	INLINE void set_union(const graphItem & g, set<UInt> & s)
	{
		set_union(g.conn.cbegin(), g.conn.cend(), s.begin(), s.end(), inserter(s, s.end()));
	}
}

#endif

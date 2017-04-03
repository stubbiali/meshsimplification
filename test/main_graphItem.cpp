/*!	\file	main_graphItem.cpp
	\brief	Small executable to test implementation of set_intersection() 
			and set_union() through variadic templates. */
	
#include <iostream>
#include <vector>

#include "graphItem.hpp"

using namespace geometry;

int main()
{
	// Initialize graph items 
	vector<UInt> c1{0,2,3};		
	graphItem g1(c1);	
	vector<UInt> c2{5,2};		
	graphItem g2(c2);
	vector<UInt> c3{2};			
	graphItem g3(c3);
	vector<UInt> c4{2,10,11,12,13,14,15};
	graphItem g4(c4);
	
	// Make intersection
	{
		auto si = set_intersection(g1, g2, g3, g4);
		cout << "set_intersection: { ";
		for (auto it = si.begin(); it != si.end(); it++)
			cout << *it << " ";
		cout << "}" << endl;
	}
	
	// Make union
	{
		auto su = set_union(g1, g2, g3, g4);
		cout << "set_union: { ";
		for (auto it = su.begin(); it != su.end(); it++)
			cout << *it << " ";
		cout << "}" << endl;
	}
	
	// Make union
	{
		set<UInt> s;
		set_union(g1,s);
		set_union(g2,s);
		set_union(g4,s);
		cout << "set_union: { ";
		for (auto it = s.begin(); it != s.end(); it++)
			cout << *it << " ";
		cout << "}" << endl;
	}
	
}

/*!	\file	searchPoint.cpp
	\brief	Implementations of members and friend functions of class searchPoint. */
	
#include <algorithm>

#include "point.hpp"
#include "searchPoint.hpp"

// Include definitions of inlined members and friend functions
#ifndef INLINED
#include "inline/inline_searchPoint.hpp"
#endif

namespace geometry 
{	
	//
	// Definitions of static members
	//
	// These are just default initializations. The user, before using any instances of the 
	// searchPoint class, should call setup() so to set the actual values for static members.
	
	point3d searchPoint::NE_global(1.,1.,1.);
	point3d searchPoint::SW_global(0.,0.,0.);
	array<Real,3> searchPoint::cellSize{{1.,1.,1.}};
	array<UInt,3> searchPoint::numCells{{1,1,1}}; 
	
	//
	// Constructors
	//
	
	searchPoint::searchPoint(const array<UInt,3> & idx, const UInt & ID) : 
		Id(ID), idx(idx) 
	{
		assert(idx < searchPoint::numCells);
	}
	
	
	searchPoint::searchPoint(const Real & x, const Real & y, const Real & z, const UInt & ID) : 
		Id(ID)
	{
		idx[0] = static_cast<UInt>((x - searchPoint::SW_global[0]) / searchPoint::cellSize[0]);
		idx[1] = static_cast<UInt>((y - searchPoint::SW_global[1]) / searchPoint::cellSize[1]);
		idx[2] = static_cast<UInt>((z - searchPoint::SW_global[2]) / searchPoint::cellSize[2]);
		
		// Check
		assert(idx < searchPoint::numCells);
	}
	
	
	searchPoint::searchPoint(const array<Real,3> & c, const UInt & ID) : 
		Id(ID)
	{
		idx[0] = static_cast<UInt>((c[0] - searchPoint::SW_global[0]) / searchPoint::cellSize[0]);
		idx[1] = static_cast<UInt>((c[1] - searchPoint::SW_global[1]) / searchPoint::cellSize[1]);
		idx[2] = static_cast<UInt>((c[2] - searchPoint::SW_global[2]) / searchPoint::cellSize[2]);
		
		// Check
		assert(idx < searchPoint::numCells);
	}
	
	
	searchPoint::searchPoint(const point3d & p) : 
		Id(0)
	{
		idx[0] = static_cast<UInt>((p[0] - searchPoint::SW_global[0]) / searchPoint::cellSize[0]);
		idx[1] = static_cast<UInt>((p[1] - searchPoint::SW_global[1]) / searchPoint::cellSize[1]);
		idx[2] = static_cast<UInt>((p[2] - searchPoint::SW_global[2]) / searchPoint::cellSize[2]);
		
		// Check
		assert(idx < searchPoint::numCells);
	}
	
	
	//
	// Operators
	//
	
	bool operator<(const searchPoint & pA, const searchPoint & pB)
	{
		// Compute scalar index for both points
		auto NA = pA[0] + pA[1]*searchPoint::numCells[0] 
						+ pA[2]*searchPoint::numCells[0]*searchPoint::numCells[1];
		auto NB = pB[0] + pB[1]*searchPoint::numCells[0] 
						+ pB[2]*searchPoint::numCells[0]*searchPoint::numCells[1];
						
		// Compare scalar indices
		return (NA < NB);
	}
	
	
	ostream & operator<<(ostream & out, const searchPoint & p)
	{
		out << "Point ID: " << p.Id << endl;		
		out << "Point indices: " << p.idx[0] << ", " << p.idx[1] << ", " << p.idx[2] << endl;
		return out;
	}
	
	
	//
	// Set methods
	//
	
	void searchPoint::setGlobalNE(const point3d & p)
	{
		// Set the new point
		searchPoint::NE_global = p;
		
		// Update number of cells
		searchPoint::updateNumCells();
	}
	
	
	void searchPoint::setGlobalSW(const point3d & p)
	{
		// Set the new point
		searchPoint::SW_global = p;
		
		// Update number of cells
		searchPoint::updateNumCells();
	}
	
	
	void searchPoint::setCellSize(const UInt & i, const Real & val)
	{
		assert(i <= 3);
		
		// Set the new size
		searchPoint::cellSize[i] = val;
		
		// Update number of cells
		searchPoint::updateNumCells(i);
	}
	
	
	void searchPoint::setCellSize(const array<Real,3> & val)
	{
		// Set the new sizes
		copy(val.cbegin(), val.cend(), searchPoint::cellSize.begin());
		
		// Update number of cells
		searchPoint::updateNumCells();
	}
	
	
	void searchPoint::setNumCells(const UInt & i, const UInt & val)
	{
		assert(i <= 3);
		
		// Set the new number of cells
		searchPoint::numCells[i] = val;
		
		// Update related cells size
		searchPoint::updateCellSize(i);
	}
	
	
	void searchPoint::setNumCells(const array<UInt,3> & val)
	{
		// Set the new numbers of cells
		copy(val.cbegin(), val.cend(), searchPoint::numCells.begin());
		
		// Update related cells size
		searchPoint::updateCellSize();
	}
	
	
	void searchPoint::setup(const point3d & pne, const point3d & psw, 
		const Real & dx, const Real & dy, const Real & dz)
	{
		searchPoint::NE_global = pne;
		searchPoint::SW_global = psw;
		searchPoint::cellSize = {{dx,dy,dz}};
				
		// Compute number of cells
		searchPoint::updateNumCells();
	}
	
	
	//
	// Methods to keep static variables coherent
	// 
	
	void searchPoint::updateNumCells(const UInt & i)
	{
		// Update the number of cells
		searchPoint::numCells[i] = 
			static_cast<UInt>((searchPoint::NE_global[i] - searchPoint::SW_global[i]) 
			/ searchPoint::cellSize[i]);
		
		// Check it is not zero
		if (searchPoint::numCells[i] == 0)
			searchPoint::numCells[i] = 1;
			
		// Update size
		searchPoint::updateCellSize(i);
	}
	
	
	void searchPoint::updateNumCells()
	{
		for (size_t i = 0; i < 3; ++i)
			searchPoint::updateNumCells(i);
	}
	
	
	void searchPoint::updateCellSize(const UInt & i)
	{
		assert(searchPoint::numCells[i] > 0);
		
		searchPoint::cellSize[i] = (searchPoint::NE_global[i] - searchPoint::SW_global[i]) / searchPoint::numCells[i];
	}
	
	
	void searchPoint::updateCellSize()
	{
		for (size_t i = 0; i < 3; ++i)
			searchPoint::updateCellSize(i);
	}
}



/*!	\file	imp_searchPoint.hpp
	\brief	Implementations of template members of class searchPoint. */
	
#ifndef HH_IMPSEARCHPOINT_HH
#define HH_IMPSEARCHPOINT_HH

#include <tuple>

namespace geometry
{
	template<typename SHAPE, MeshType MT>
	void searchPoint::setup(const bmeshInfo<SHAPE,MT> & news)
	{
		// Compute North-East and South-West point of the grid
		tie(searchPoint::NE_global, searchPoint::SW_global) = 
			news.getBoundingBoxVertices();
		
		// Get cells size
		searchPoint::cellSize = news.getCellSize();
		
		// Update number of cells
		searchPoint::updateNumCells();
	}
}

#endif

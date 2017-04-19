/*!	\file	boundingBox.cpp
	\brief	Specialization of some members of class boundingBox. */
			
#include "boundingBox.hpp"

namespace geometry
{
	//
	// Set methods
	//
	
	// Specialization for three-dimensional elements
	template<>
	void boundingBox<3>::setup(const point3d & pne, const point3d & psw, 
		const Real & dx, const Real & dy, const Real & dz)
	{
		// Set global North-East and South-West points
		boundingBox::NE_global = pne;
		boundingBox::SW_global = psw;
		
		// Set cell size
		boundingBox::cellSize = {{dx,dy,dz}};
		
		// Update number of cells along each direction
		boundingBox::updateNumCells();
	}
}





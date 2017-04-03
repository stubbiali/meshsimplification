/*!	\file	structuredData.cpp
	\brief	Specialization of some members of class structuredData. */
	
#include "structuredData.hpp"

namespace geometry
{
	//
	// Get methods
	//
	
	// Specialization for triangular grids
	template<>
	bbox3d structuredData<Triangle>::getBoundingBox(const UInt & Id) const
	{
		auto elem = grid->getElem(Id);
		return {grid->getNode(elem[0]), grid->getNode(elem[1]),
			grid->getNode(elem[2])};
	}
	
	
	// Specialization for quadrilateral grids
	template<>
	bbox3d structuredData<Quad>::getBoundingBox(const UInt & Id) const
	{
		auto elem = grid->getElem(Id);
		return {grid->getNode(elem[0]), grid->getNode(elem[1]),
			grid->getNode(elem[2]), grid->getNode(elem[3])};
	}
	
	
	//
	// Modify set of bounding boxes
	//
	
	// Specialization for triangular grids
	template<>
	void structuredData<Triangle>::update(const vector<UInt> & ids)
	{
		for (auto id : ids)
		{
			//
			// Remove old bounding box
			//
			
			// Extract element index
			auto idx = grid->getElem(id).getIdx();
			
			// Extract all elements having that index...
			auto range = boxes.equal_range(idx);
			
			// ... Then find and remove the desired one
			auto it_old = find_if(range.first, range.second,
				[id](const bbox3d & bb){ return bb.getId() == id; });
			if (it_old != range.second)
				boxes.erase(it_old);
			
			//
			// Insert new bounding box
			//
			
			auto elem = grid->getElem(id);
			auto A(grid->getNode(elem[0])), B(grid->getNode(elem[1])), C(grid->getNode(elem[2]));
			auto it_new = boxes.emplace(id, A, B, C);
			
			// Update grid
			grid->setIdx(id, it_new->getIdx());
				
			//
			// Check if the structure requires an update
			//
			
			torefresh = torefresh || 
				(abs(B[0] - A[0]) > 1.3 * bbox3d::getCellSize(0)) || 
				(abs(C[0] - B[0]) > 1.3 * bbox3d::getCellSize(0)) ||
				(abs(A[0] - C[0]) > 1.3 * bbox3d::getCellSize(0)) ||
				(abs(B[1] - A[1]) > 1.3 * bbox3d::getCellSize(1)) || 
				(abs(C[1] - B[1]) > 1.3 * bbox3d::getCellSize(1)) ||
				(abs(A[1] - C[1]) > 1.3 * bbox3d::getCellSize(1)) ||
				(abs(B[2] - A[2]) > 1.3 * bbox3d::getCellSize(2)) || 
				(abs(C[2] - B[2]) > 1.3 * bbox3d::getCellSize(2)) ||
				(abs(A[2] - C[2]) > 1.3 * bbox3d::getCellSize(2));
		}
	}
	
	
	// Specialization for quadrilateral grids
	template<>
	void structuredData<Quad>::update(const vector<UInt> & ids)
	{
		for (auto id : ids)
		{
			//
			// Remove old bounding box
			//
			
			// Extract element index
			auto idx = grid->getElem(id).getIdx();
			
			// Extract all elements having that index...
			auto range = boxes.equal_range(idx);
			
			// ... Then find and remove the desired one
			auto it_old = find_if(range.first, range.second,
				[id](const bbox3d & bb){ return bb.getId() == id; });
			if (it_old != range.second)
				boxes.erase(it_old);
			
			//
			// Insert new bounding box
			//
			
			auto elem = grid->getElem(id);
			auto A(grid->getNode(elem[0])), B(grid->getNode(elem[1])), 
				C(grid->getNode(elem[2])), D(grid->getNode(elem[3]));
			auto it_new = boxes.emplace(id, A, B, C, D);
			
			// Update grid
			grid->setIdx(id, it_new->getIdx());
			
			//
			// Check if the structure requires an update
			//
			
			torefresh = torefresh || 
				(abs(B[0] - A[0]) > 1.3 * bbox3d::getCellSize(0)) || 
				(abs(C[0] - B[0]) > 1.3 * bbox3d::getCellSize(0)) ||
				(abs(D[0] - C[0]) > 1.3 * bbox3d::getCellSize(0)) ||
				(abs(A[0] - D[0]) > 1.3 * bbox3d::getCellSize(0)) ||
				(abs(B[1] - A[1]) > 1.3 * bbox3d::getCellSize(1)) || 
				(abs(C[1] - B[1]) > 1.3 * bbox3d::getCellSize(1)) ||
				(abs(D[1] - C[1]) > 1.3 * bbox3d::getCellSize(1)) ||
				(abs(A[1] - D[1]) > 1.3 * bbox3d::getCellSize(1)) ||
				(abs(B[2] - A[2]) > 1.3 * bbox3d::getCellSize(2)) || 
				(abs(C[2] - B[2]) > 1.3 * bbox3d::getCellSize(2)) ||
				(abs(D[2] - C[2]) > 1.3 * bbox3d::getCellSize(2)) ||
				(abs(A[2] - D[2]) > 1.3 * bbox3d::getCellSize(2));
		}
	}
	
	
	// Specialization for triangular grids
	template<>
	void structuredData<Triangle>::update_f(const vector<UInt> & ids)
	{
		for (auto id : ids)
		{
			//
			// Remove old bounding box
			//
			
			// Extract element index
			auto idx = grid->getElem(id).getIdx();
			
			// Extract all elements having that index...
			auto range = boxes.equal_range(idx);
			
			// ... Then find and remove the desired one
			auto it_old = find_if(range.first, range.second,
				[id](const bbox3d & bb){ return bb.getId() == id; });
			if (it_old != range.second)
				boxes.erase(it_old);
			
			//
			// Insert new bounding box
			//
			
			auto elem = grid->getElem(id);
			auto it_new = boxes.emplace(id, grid->getNode(elem[0]), 
				grid->getNode(elem[1]), grid->getNode(elem[2]));
			
			// Update grid
			grid->setIdx(id, it_new->getIdx());
		}
	}
	
	
	// Specialization for quadrilateral grids
	template<>
	void structuredData<Quad>::update_f(const vector<UInt> & ids)
	{
		for (auto id : ids)
		{
			//
			// Remove old bounding box
			//
			
			// Extract element index
			auto idx = grid->getElem(id).getIdx();
			
			// Extract all elements having that index...
			auto range = boxes.equal_range(idx);
			
			// ... Then find and remove the desired one
			auto it_old = find_if(range.first, range.second,
				[id](const bbox3d & bb){ return bb.getId() == id; });
			if (it_old != range.second)
				boxes.erase(it_old);
			
			//
			// Insert new bounding box
			//
			
			auto elem = grid->getElem(id);
			auto it_new = boxes.emplace(id, grid->getNode(elem[0]), 
				grid->getNode(elem[1]), grid->getNode(elem[2]), grid->getNode(elem[3]));
			
			// Update grid
			grid->setIdx(id, it_new->getIdx());
		}
	}
}

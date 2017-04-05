/*!	\file	imp_structuredData.hpp
	\brief	Implementations of members of class structuredData. */
	
#ifndef HH_IMPSTRUCTUREDDATA_HH
#define HH_IMPSTRUCTUREDDATA_HH

#include <algorithm>

#include "searchPoint.hpp"

namespace geometry
{
	//
	// Constructors
	//
	
	template<typename SHAPE>
	structuredData<SHAPE>::structuredData(bmesh<SHAPE> * pg) :
		grid(pg), torefresh(false)
	{
		// Build bounding boxes
		if (grid != nullptr)
			refresh(bmeshInfo<SHAPE>(*grid));
	}
	
	
	template<typename SHAPE>
	template<MeshType MT>
	structuredData<SHAPE>::structuredData(bmeshInfo<SHAPE,MT> & news) :
		grid(news.getPointerToMesh()), torefresh(false)
	{
		// Build bounding boxes
		refresh(news);
	}
	
	
	//
	// Operators
	//
	
	template<typename S>
	ostream & operator<<(ostream & out, const structuredData<S> & sd)
	{
		for (auto it = sd.boxes.cbegin(); it != sd.boxes.cend(); ++it)
			out << *it << endl;
		return out;
	}
	
	
	//
	// Get methods
	//
	
	template<typename SHAPE>
	bbox3d structuredData<SHAPE>::getBoundingBox(const UInt & Id) const
	{
		static_assert(((SHAPE::numVertices == 3) || (SHAPE::numVertices == 4)),
			"getBoundingBox(), then the entire class, "
			"provided only for triangular and quadrilateral grids.");
		return {};
	}
	
	
	// Declare specialization for triangular grids
	template<>
	bbox3d structuredData<Triangle>::getBoundingBox(const UInt & Id) const;
	
	
	// Declare specialization for quadrilateral grids
	template<>
	bbox3d structuredData<Quad>::getBoundingBox(const UInt & Id) const;
		
	
	template<typename SHAPE>
	INLINE unordered_multiset<bbox3d> structuredData<SHAPE>::getBoundingBox() const
	{
		return boxes;
	}
	
	
	template<typename SHAPE>
	vector<UInt> structuredData<SHAPE>::getNeighbouringElements(const UInt & Id) const
	{
		// Implementation provide only for triangular and quadrilateral grids
		static_assert(((SHAPE::numVertices == 3) || (SHAPE::numVertices == 4)),
			"getIntersectingBoundingBoxes(), then the entire class, "
			"provided only for triangular and quadrilateral grids.");
			
		//
		// Create bounding box around the element and extract
		// North-West and South-West points, also as searchPoint
		//
		// As for getBoundingBox(): it is probably faster (or at least easier)
		// to re-build the bounding box rather than extracting it from boxes	
		
		auto box = getBoundingBox(Id);
		auto p_NE = box.getNE();
		auto p_SW = box.getSW();
		searchPoint sp_NE(p_NE);
		searchPoint sp_SW(p_SW);
				
		//
		// Find intersecting boxes
		//
		// We scan all the cells intersecting the reference box
		// plus an extra layer. This should ensure that all
		// possible intersecting elements are taken into account
		// since an element cannot span more than one cell.
					
		unordered_set<UInt> res;
		
		// Determine indices range
		UInt i_start = (sp_SW[0] == 0) ? 0 : sp_SW[0]-1;
		UInt i_stop = sp_NE[0]+1;
		UInt j_start = (sp_SW[1] == 0) ? 0 : sp_SW[1]-1;
		UInt j_stop = sp_NE[1]+1;
		UInt k_start = (sp_SW[2] == 0) ? 0 : sp_SW[2]-1;
		UInt k_stop = sp_NE[2]+1;
						
		for (UInt i = i_start; i <= i_stop; ++i)
			for (UInt j = j_start; j <= j_stop; ++j)
				for (UInt k = k_start; k <= k_stop; ++k)
				{
					// Compute scalar index
					UInt idx(i + j * bbox3d::getNumCells(0) + 
						k * bbox3d::getNumCells(0) * bbox3d::getNumCells(1));
												
					// Extract all boxes having that index
					auto range = boxes.equal_range(idx);
					
					// Out of these boxes, keep only the active ones 
					// actually intersecting the reference bounding box							
					for (auto it = range.first; it != range.second; ++it)
						if (grid->isElemActive(it->getId()) && doIntersect(box, *it))
							res.insert(it->getId());
				}
		
		// Remove Id of reference element from the set,
		// then convert to a vector
		return {res.cbegin(), res.cend()};
	}
	
	
	template<typename SHAPE>
	vector<UInt> structuredData<SHAPE>::getNeighbouringElements(const point3d & P) const
	{
		// Implementation provide only for triangular and quadrilateral grids
		static_assert(((SHAPE::numVertices == 3) || (SHAPE::numVertices == 4)),
			"getIntersectingBoundingBoxes(), then the entire class, "
			"provided only for triangular and quadrilateral grids.");
			
		// Compute indices for the point within the structure		
		searchPoint sp_P(P);
						
		//
		// Find triangles the point may belong to
		//
		// We return all the triangles associated with the cell P
		// falls within, plus an extra layer. This should ensure 
		// that all elements which P may belong to are taken into 
		// account since an element cannot span more than one cell.
					
		unordered_set<UInt> res;
		
		// Determine indices range
		UInt i_start = (sp_P[0] == 0) ? 0 : sp_P[0]-1;
		UInt i_stop = sp_P[0]+1;
		UInt j_start = (sp_P[1] == 0) ? 0 : sp_P[1]-1;
		UInt j_stop = sp_P[1]+1;
		UInt k_start = (sp_P[2] == 0) ? 0 : sp_P[2]-1;
		UInt k_stop = sp_P[2]+1;
						
		for (UInt i = i_start; i <= i_stop; ++i)
			for (UInt j = j_start; j <= j_stop; ++j)
				for (UInt k = k_start; k <= k_stop; ++k)
				{
					// Compute scalar index
					UInt idx(i + j * bbox3d::getNumCells(0) + 
						k * bbox3d::getNumCells(0) * bbox3d::getNumCells(1));
												
					// Extract all boxes having that index
					auto range = boxes.equal_range(idx);
					
					// Extract boxes Id's						
					for (auto it = range.first; it != range.second; ++it)
						if (grid->isElemActive(it->getId()))
							res.insert(it->getId());
				}
		
		return {res.cbegin(), res.cend()};
	}
	
	
	//
	// Set methods
	//
	
	template<typename SHAPE>
	void structuredData<SHAPE>::setMesh(bmesh<SHAPE> * pg)
	{
		// Set the mesh
		grid = pg;
		
		// (Re-)build bounding boxes
		refresh(bmeshInfo<SHAPE>(*grid));
	}
	
	
	template<typename SHAPE>
	template<MeshType MT>
	void structuredData<SHAPE>::setMesh(bmeshInfo<SHAPE,MT> & news)
	{
		// Set the mesh
		grid = news.getPointerToMesh();
		
		// (Re-)build bounding boxes
		refresh(news);
	}
	
	
	//
	// Modify set of bounding boxes
	//
	
	template<typename SHAPE>
	void structuredData<SHAPE>::erase(const vector<UInt> & ids)
	{
		for (auto id : ids)
		{
			// Extract element index
			auto idx = grid->getElem(id).getIdx();
			
			// Extract all elements having that index...
			auto range = boxes.equal_range(idx);
			
			// ... Then find and remove the desired one
			auto it = find_if(range.first, range.second,
				[id](const bbox3d & bb){ return bb.getId() == id; });
			boxes.erase(it);
		}
	}
	
	
	template<typename SHAPE>
	void structuredData<SHAPE>::update(const vector<UInt> & ids)
	{
		static_assert(((SHAPE::numVertices == 3) || (SHAPE::numVertices == 4)),
			"update(), then the entire class, "
			"provided only for triangular and quadrilateral grids.");
	}
	
	
	// Declare specialization for triangular grids
	template<>
	void structuredData<Triangle>::update(const vector<UInt> & ids);
	
	
	// Declare specialization for quadrilateral grids
	template<>
	void structuredData<Quad>::update(const vector<UInt> & ids);
	
	
	template<typename SHAPE>
	void structuredData<SHAPE>::update_f(const vector<UInt> & ids)
	{
		static_assert(((SHAPE::numVertices == 3) || (SHAPE::numVertices == 4)),
			"update(), then the entire class, "
			"provided only for triangular and quadrilateral grids.");
	}
	
	
	// Declare specialization for triangular grids
	template<>
	void structuredData<Triangle>::update_f(const vector<UInt> & ids);
	
	
	// Declare specialization for quadrilateral grids
	template<>
	void structuredData<Quad>::update_f(const vector<UInt> & ids);
	
	
	template<typename SHAPE>
	INLINE void structuredData<SHAPE>::update(const vector<UInt> & toRemove, 
		const vector<UInt> & toKeep)
	{
		erase(toRemove);
		update(toKeep);
	}
	
	
	template<typename SHAPE>
	INLINE void structuredData<SHAPE>::update_f(const vector<UInt> & toRemove, 
		const vector<UInt> & toKeep)
	{
		erase(toRemove);
		update_f(toKeep);
	}
	
	
	//
	// Refresh methods
	//
	
	template<typename SHAPE>
	INLINE bool structuredData<SHAPE>::toRefresh() const
	{
		return torefresh;
	}
	
	template<typename SHAPE>
	template<MeshType MT>
	void structuredData<SHAPE>::refresh(const bmeshInfo<SHAPE,MT> & news)
	{
		static_assert(((SHAPE::numVertices == 3) || (SHAPE::numVertices == 4)),
			"refresh(), then the entire class, provided only for triangular "
			"and quadrilateral grids.");
	}
	
	
	// Specialization for triangular grids 
	template<>
	template<MeshType MT>
	void structuredData<Triangle>::refresh(const bmeshInfo<Triangle,MT> & news)
	{
		// Reset torefresh flag
		torefresh = false;
		
		//
		// Set static members of class searchPoint 
		// and bbox3d (i.e. boundingBox<3>)
		//
		
		searchPoint::setup(news);
		bbox3d::setup(news);
		
		//
		// Create the bounding box surrounding each element
		//
		
		boxes.clear();
		for (UInt id = 0; id < grid->getElemsListSize(); ++id)
		{
			if (grid->isElemActive(id))
			{
				// Extract element
				auto elem = grid->getElem(id);
			
				// Build bounding box
				auto it = boxes.emplace(id, grid->getNode(elem[0]), 
					grid->getNode(elem[1]), grid->getNode(elem[2]));
				
				// Set element index
				grid->setIdx(id, it->getIdx());
			}
		}
	}
	
	
	// Specialization for quadrilateral grids 
	template<>
	template<MeshType MT>
	void structuredData<Quad>::refresh(const bmeshInfo<Quad,MT> & news)
	{
		// Reset torefresh flag
		torefresh = false;
		
		//
		// Set static members of class searchPoint 
		// and bbox3d (i.e. boundingBox<3>)
		//
		
		searchPoint::setup(news);
		bbox3d::setup(news);
		
		//
		// Create the bounding box surrounding each element
		//
		
		boxes.clear();
		for (UInt id = 0; id < grid->getElemsListSize(); ++id)
		{
			if (grid->isElemActive(id))
			{
				// Extract element
				auto elem = grid->getElem(id);
			
				// Build bounding box
				auto it = boxes.emplace(id, grid->getNode(elem[0]), 
					grid->getNode(elem[1]), grid->getNode(elem[2]), grid->getNode(elem[3]));
				
				// Set element index
				grid->setIdx(id, it->getIdx());
			}
		}
	}
}

#endif

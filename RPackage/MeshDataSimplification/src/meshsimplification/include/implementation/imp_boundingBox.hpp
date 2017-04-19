/*!	\file	imp_boundingBox.hpp
	\brief	Implementations of template members of class boundingBox. */
	
/*
	Note: some methods are provided only for three dimensional elements. 
	To accomplish this, we add some static_assert's.
	However, in debugging phase we might force the compiler to generate the 
	code for all methods of template classes. Then, to enable code
	generation also for non three-dimensional elements, all static_assert's 
	are enabled only in release mode. */
	
#ifndef HH_IMPBOUNDINGBOX_HH
#define HH_IMPBOUNDINGBOX_HH

#include <limits>
#include <numeric>

namespace geometry
{
	//
	// Definitions of static members
	//
	// These are just default initializations. The user, before using any instances of the 
	// searchPoint class, should call setup() so to set the actual values for static members.
	
	template<UInt N>
	geoPoint<N> boundingBox<N>::NE_global(1.);
	
	template<UInt N>
	geoPoint<N> boundingBox<N>::SW_global(0.);
	
	template<UInt N>
	array<Real,N> boundingBox<N>::cellSize;
	
	template<UInt N>
	array<UInt,N> boundingBox<N>::numCells;
	
	
	//
	// Constructors
	//
	
	template<UInt N>
	boundingBox<N>::boundingBox() :
		Id(0), idx(0), NE(1.), SW(0.)
	{
	}
	
	
	template<UInt N>
	boundingBox<N>::boundingBox(const UInt & index) :
		Id(0), idx(index), NE(1.), SW(0.)
	{
	}
	
	
	template<UInt N>
	template<typename... Args>
	boundingBox<N>::boundingBox(Args... args) :
		Id(0), idx(0), NE(numeric_limits<Real>::lowest()), SW(numeric_limits<Real>::max())	
	{
		// Compute (local) North-East and South-West points
		computeBoundingBoxVertices(args...);
		
		// Compute barycenter of bounding box
		auto bar = 0.5*(NE + SW);
		
		// Compute index
		for (UInt i = 0; i < N; ++i) 
		{
			// "Local" index along i-th direction
			auto idx_I = static_cast<UInt>((bar[i] - boundingBox::SW_global[i])
				/ boundingBox::cellSize[i]);
				
			// Update "global" (i.e. scalar) index
			idx += idx_I * 
				accumulate(boundingBox<N>::numCells.cbegin(), 
				boundingBox<N>::numCells.cbegin() + i, 1, multiplies<UInt>());
		}
								
		// Check (olny debug mode): the barycenter must fall
		// within the mesh (global) bounding box
		#ifndef NDEBUG
		auto ncells = accumulate(boundingBox<N>::numCells.cbegin(),
			boundingBox<N>::numCells.cend(), 1, multiplies<UInt>());
		assert(idx < ncells);
		#endif
	} 
	
	
	// Specialization for three-dimensional elements
	template<>
	template<typename... Args>
	boundingBox<3>::boundingBox(Args... args) :
		Id(0), idx(0), NE(numeric_limits<Real>::lowest()), SW(numeric_limits<Real>::max())	
	{
		// Compute (local) North-East and South-West points
		computeBoundingBoxVertices(args...);
		
		// Compute barycenter of bounding box
		auto bar = 0.5*(NE + SW);
		
		// Compute indices along all directions 
		auto idx_I = static_cast<UInt>((bar[0] - boundingBox::SW_global[0])
			/ boundingBox::cellSize[0]);
		auto idx_J = static_cast<UInt>((bar[1] - boundingBox::SW_global[1])
			/ boundingBox::cellSize[1]);
		auto idx_K = static_cast<UInt>((bar[2] - boundingBox::SW_global[2])
			/ boundingBox::cellSize[2]);
					
		// Compute index
		idx = idx_I + idx_J*boundingBox::numCells[0]
			+ idx_K*boundingBox::numCells[0]*boundingBox::numCells[1];
			
		// Check (olny debug mode): the barycenter must fall
		// within the mesh (global) bounding box
		assert(idx < boundingBox::numCells[0]*boundingBox::numCells[1]*boundingBox::numCells[2]);
	} 
	
	
	template<UInt N>
	template<typename... Args>
	boundingBox<N>::boundingBox(const UInt & id, Args... args) :
		Id(id), idx(0), NE(numeric_limits<Real>::lowest()), SW(numeric_limits<Real>::max())	
	{
		// Compute (local) North-East and South-West points
		computeBoundingBoxVertices(args...);
		
		// Compute barycenter of bounding box
		auto bar = 0.5*(NE + SW);
		
		// Compute index
		for (UInt i = 0; i < N; ++i) 
		{
			// "Local" index along i-th direction
			auto idx_I = static_cast<UInt>((bar[i] - boundingBox::SW_global[i])
				/ boundingBox::cellSize[i]);
				
			// Update "global" (i.e. scalar) index
			idx += idx_I * 
				accumulate(boundingBox<N>::numCells.cbegin(), 
				boundingBox<N>::numCells.cbegin() + i, 1, multiplies<UInt>());
		}
								
		// Check (olny debug mode): the barycenter must fall
		// within the mesh (global) bounding box
		#ifndef NDEBUG
		auto ncells = accumulate(boundingBox<N>::numCells.cbegin(),
			boundingBox<N>::numCells.cend(), 1, multiplies<UInt>());
		assert(idx < ncells);
		#endif
	} 
	
	
	// Specialization for three-dimensional elements
	template<>
	template<typename... Args>
	boundingBox<3>::boundingBox(const UInt & id, Args... args) :
		Id(id), idx(0), NE(numeric_limits<Real>::lowest()), SW(numeric_limits<Real>::max())
	{
		// Compute (local) North-East and South-West points
		computeBoundingBoxVertices(args...);
		
		// Compute barycenter of bounding box
		auto bar = 0.5*(NE + SW);
		
		// Compute indices along all directions
		auto idx_I = static_cast<UInt>((bar[0] - boundingBox::SW_global[0])
			/ boundingBox::cellSize[0]);
		auto idx_J = static_cast<UInt>((bar[1] - boundingBox::SW_global[1])
			/ boundingBox::cellSize[1]);
		auto idx_K = static_cast<UInt>((bar[2] - boundingBox::SW_global[2])
			/ boundingBox::cellSize[2]);
			
		// Compute index
		idx = idx_I + idx_J*boundingBox::numCells[0]
			+ idx_K*boundingBox::numCells[0]*boundingBox::numCells[1];
			
		// Check (olny debug mode): the barycenter must fall
		// within the mesh (global) bounding box
		//assert(idx < boundingBox::numCells[0]*boundingBox::numCells[1]*boundingBox::numCells[2]);
	} 
	
	
	template<UInt N>
	template<UInt DIM>
	boundingBox<N>::boundingBox(const boundingBox<DIM> & bb) :
		Id(), idx(bb.getIdx()), NE(bb.getNE()), SW(bb.getSW())
	{
		static_assert(DIM <= N,
			"Copy constructor not provided for input bounding boxes " 
			"having a greater dimension than the calling one.");
	}
	
	
	//
	// Operators
	//
	
	template<UInt N>
	INLINE bool operator<(const boundingBox<N> & bb1, const boundingBox<N> & bb2)
	{
		return (bb1.idx < bb2.idx);
	}
	
	
	template<UInt N>
	INLINE bool operator==(const boundingBox<N> & bb1, const boundingBox<N> & bb2)
	{
		return (bb1.idx == bb2.idx);
	}
	
	
	template<UInt N>
	INLINE bool operator!=(const boundingBox<N> & bb1, const boundingBox<N> & bb2)
	{
		return (bb1.idx != bb2.idx);
	}
	
	
	template<UInt N>
	INLINE ostream & operator<<(ostream & out, const boundingBox<N> & bb)
	{
		out << bb.Id << " " << bb.idx << " ";
		out << bb.NE;
		out << " " << bb.SW;
		return out;
	}
	
	
	//
	// Other friend functions
	//
	
	template<UInt N>
	INLINE bool doIntersect(const boundingBox<N> & bb1, const boundingBox<N> & bb2)
	{
		return (((bb2.SW < bb1.SW) && (bb1.SW < bb2.NE))
			|| ((bb1.SW < bb2.SW) && (bb2.SW < bb1.NE))
			|| ((bb1.SW < bb2.NE) && (bb2.SW < bb1.NE)));
	}
	
	
	//
	// Get methods
	//
	
	template<UInt N>
	INLINE UInt boundingBox<N>::getId() const
	{
		return Id;
	}
	
	
	template<UInt N>
	INLINE UInt boundingBox<N>::getIdx() const
	{
		return idx;
	}
	
	
	template<UInt N>
	INLINE geoPoint<N> boundingBox<N>::getNE() const
	{
		return NE;
	}
	
	
	template<UInt N>
	INLINE geoPoint<N> boundingBox<N>::getSW() const
	{
		return SW;
	}
	
	
	template<UInt N>
	INLINE geoPoint<N> boundingBox<N>::getGlobalNE()
	{
		return boundingBox::NE_global;
	}
	
	
	template<UInt N>
	INLINE geoPoint<N> boundingBox<N>::getGlobalSW()
	{
		return boundingBox::SW_global;
	}
	
	
	template<UInt N>
	INLINE Real boundingBox<N>::getCellSize(const UInt & i)
	{
		return boundingBox::cellSize[i];
	}
	
	
	template<UInt N>
	INLINE array<Real,N> boundingBox<N>::getCellSize()
	{
		return boundingBox::cellSize;
	}
	
	
	template<UInt N>
	INLINE UInt boundingBox<N>::getNumCells(const UInt & i)
	{
		return boundingBox::numCells[i];
	}
	
	
	template<UInt N>
	INLINE array<UInt,N> boundingBox<N>::getNumCells()
	{
		return boundingBox::numCells;
	}
	
	
	//
	// Set methods
	//
	
	template<UInt N>
	INLINE void boundingBox<N>::setId(const UInt & id)
	{
		Id = id;
	}
	
	
	template<UInt N>
	template<typename... Args>
	void boundingBox<N>::update(Args... args)
	{
		// Reset 
		idx = 0;
		NE.reset(numeric_limits<Real>::lowest());
		SW.reset(numeric_limits<Real>::max());
		
		// Update bounding box vertices 
		// according to new element vertices
		computeBoundingBoxVertices(args...);
		
		// Compute barycenter of bounding box
		auto bar = 0.5*(NE + SW);
		
		// Update index
		for (UInt i = 0; i < N; ++i) 
		{
			// "Local" index along i-th direction
			auto idx_I = static_cast<UInt>((bar[i] - boundingBox::SW_global[i])
				/ boundingBox::cellSize[i]);
				
			// Update "global" (i.e. scalar) index
			idx += idx_I * 
				accumulate(boundingBox<N>::numCells.cbegin(), 
				boundingBox<N>::numCells.cbegin() + i, 1, multiplies<UInt>());
		}
								
		// Check (olny debug mode): the barycenter must fall
		// within the mesh (global) bounding box
		#ifndef NDEBUG
		auto ncells = accumulate(boundingBox<N>::numCells.cbegin(),
			boundingBox<N>::numCells.cend(), 1, multiplies<UInt>());
		assert(idx < ncells);
		#endif
	}
	
	
	// Specialization for three-dimensional elements
	template<>
	template<typename... Args>
	void boundingBox<3>::update(Args... args)
	{
		// Reset 
		NE.reset(numeric_limits<Real>::lowest());
		SW.reset(numeric_limits<Real>::max());
		
		// Update bounding box vertices 
		// according to new element vertices
		computeBoundingBoxVertices(args...);
		
		// Compute barycenter of bounding box
		auto bar = 0.5*(NE + SW);
		
		// Update indices along all directions
		UInt idx_I = static_cast<UInt>((bar[0] - boundingBox::SW_global[0])
			/ boundingBox::cellSize[0]);
		UInt idx_J = static_cast<UInt>((bar[1] - boundingBox::SW_global[1])
			/ boundingBox::cellSize[1]);
		UInt idx_K = static_cast<UInt>((bar[2] - boundingBox::SW_global[2])
			/ boundingBox::cellSize[2]);
			
		// Update index
		idx = idx_I + idx_J*boundingBox::numCells[0]
			+ idx_K*boundingBox::numCells[0]*boundingBox::numCells[1];
			
		// Check (olny debug mode): the barycenter must fall
		// within the mesh (global) bounding box
		assert(idx < boundingBox::numCells[0]*boundingBox::numCells[1]*boundingBox::numCells[2]);
	}
	
	
	template<UInt N>
	template<typename... Args>
	void boundingBox<N>::update(const UInt & id, Args... args)
	{
		// Reset 
		idx = 0;
		NE.reset(numeric_limits<Real>::lowest());
		SW.reset(numeric_limits<Real>::max());
		
		// Set Id
		Id = id;
		
		// Update bounding box vertices 
		// according to new element vertices
		computeBoundingBoxVertices(args...);
		
		// Compute barycenter of bounding box
		auto bar = 0.5*(NE + SW);
		
		// Update index
		for (UInt i = 0; i < N; ++i) 
		{
			// "Local" index along i-th direction
			auto idx_I = static_cast<UInt>((bar[i] - boundingBox::SW_global[i])
				/ boundingBox::cellSize[i]);
				
			// Update "global" (i.e. scalar) index
			idx += idx_I * 
				accumulate(boundingBox<N>::numCells.cbegin(), 
				boundingBox<N>::numCells.cbegin() + i, 1, multiplies<UInt>());
		}
								
		// Check (only debug mode): the barycenter must fall
		// within the mesh (global) bounding box
		#ifndef NDEBUG
		auto ncells = accumulate(boundingBox<N>::numCells.cbegin(),
			boundingBox<N>::numCells.cend(), 1, multiplies<UInt>());
		assert(idx < ncells);
		#endif
	}
	
	
	// Specialization for three-dimensional elements
	template<>
	template<typename... Args>
	void boundingBox<3>::update(const UInt & id, Args... args)
	{
		// Reset 
		NE.reset(numeric_limits<Real>::lowest());
		SW.reset(numeric_limits<Real>::max());
		
		// Set Id
		Id = id;
		
		// Update bounding box vertices 
		// according to new element vertices
		computeBoundingBoxVertices(args...);
		
		// Compute barycenter of bounding box
		auto bar = 0.5*(NE + SW);
		
		// Update indices along all directions
		UInt idx_I = static_cast<UInt>((bar[0] - boundingBox::SW_global[0])
			/ boundingBox::cellSize[0]);
		UInt idx_J = static_cast<UInt>((bar[1] - boundingBox::SW_global[1])
			/ boundingBox::cellSize[1]);
		UInt idx_K = static_cast<UInt>((bar[2] - boundingBox::SW_global[2])
			/ boundingBox::cellSize[2]);
			
		// Update index
		idx = idx_I + idx_J*boundingBox::numCells[0]
			+ idx_K*boundingBox::numCells[0]*boundingBox::numCells[1];
			
		// Check (olny debug mode): the barycenter must fall
		// within the mesh (global) bounding box
		assert(idx < boundingBox::numCells[0]*boundingBox::numCells[1]*boundingBox::numCells[2]);
	}
	
	
	template<UInt N>
	void boundingBox<N>::setGlobalNE(const geoPoint<N> & p)
	{
		// Update global North-East point
		boundingBox::NE_global = p;
		
		// Update number of cells along each direction
		boundingBox::updateNumCells();
	}
	
	
	template<UInt N>
	void boundingBox<N>::setGlobalSW(const geoPoint<N> & p)
	{
		// Update global South-West point
		boundingBox::SW_global = p;
		
		// Update number of cells along each direction
		boundingBox::updateNumCells();
	}
	
	
	template<UInt N>
	void boundingBox<N>::setCellSize(const UInt & i, const Real & val)
	{
		assert(i < N);
		
		// Update cell size along i-th direction
		boundingBox::cellSize[i] = val;
		
		// Update number of cells along i-th direction
		boundingBox::updateNumCells(i);	
	}
	
	
	template<UInt N>
	void boundingBox<N>::setCellSize(const array<Real,N> & val)
	{
		// Update cell size 
		boundingBox::cellSize = val;
		
		// Update number of cells along each direction
		boundingBox::updateNumCells();	
	}
	
	
	template<UInt N>
	void boundingBox<N>::setNumCells(const UInt & i, const UInt & val)
	{
		assert(i < N);
		
		// Update number of cells along i-th direction
		boundingBox::numCells[i] = val;
		
		// Update cell size along i-th direction
		boundingBox::updateCellSize(i);	
	}
	
	
	template<UInt N>
	void boundingBox<N>::setNumCells(const array<UInt,N> & val)
	{
		// Update number of cells
		boundingBox::numCells = val;
		
		// Update cell size along each direction
		boundingBox::updateCellSize();	
	}
	
	
	template<UInt N>
	void boundingBox<N>::setup(const geoPoint<N> & pne, const geoPoint<N> & psw, 
		const array<Real,N> & dl)
	{
		// Set global North-East and South-West points
		boundingBox::NE_global = pne;
		boundingBox::SW_global = psw;
		
		// Set cell size
		boundingBox::cellSize = dl;
		
		// Update number of cells
		boundingBox::updateNumCells();
	}
	
	
	template<UInt N>
	void boundingBox<N>::setup(const point3d & pne, const point3d & psw, 
		const Real & dx, const Real & dy, const Real & dz)
	{
		#ifdef NDEBUG
		static_assert(N == 3,
			"This method is provided only for three-dimensional elements.");
		#endif
	}
	
	
	// Declare specialization for three-dimensional elements
	template<>
	void boundingBox<3>::setup(const point3d & pne, const point3d & psw, 
		const Real & dx, const Real & dy, const Real & dz);
		
	
	template<UInt N>
	template<typename SHAPE, MeshType MT>
	void boundingBox<N>::setup(const bmeshInfo<SHAPE,MT> & news)
	{
		#ifdef NDEBUG
		static_assert(N == 3,
			"This method is provided only for three-dimensional elements.");
		#endif
	}
	
	
	// Specialization for three-dimensional elements
	template<>
	template<typename SHAPE, MeshType MT>
	void boundingBox<3>::setup(const bmeshInfo<SHAPE,MT> & news)
	{
		// Set global North-East and South-West points
		tie(boundingBox::NE_global, boundingBox::SW_global) =
			news.getBoundingBoxVertices();
				
		// Set cell size
		boundingBox::cellSize = news.getCellSize();
		
		// Update number of cells along each direction
		boundingBox::updateNumCells();
	}
	
	
	//
	// Methods to compute (local) North-East and South-West points
	//
	
	template<UInt N>
	void boundingBox<N>::computeBoundingBoxVertices(const geoPoint<N> & p)
	{
		// Update North-East and South-West points
		// according to p
		for (UInt i = 0; i < N; ++i)
		{
			if (p[i] > NE[i])
				NE[i] = p[i];
							
			if (p[i] < SW[i])
				SW[i] = p[i];
		}
	}
		
		
	template<UInt N>
	template<typename... Args>
	void boundingBox<N>::computeBoundingBoxVertices(const geoPoint<N> & p, Args... args)
	{
		// Update North-East and South-West points
		// according to p, then iterate the procedure
		computeBoundingBoxVertices(p);
		computeBoundingBoxVertices(args...);
	}
	
	
	//
	// Methods to keep static variables coherent
	//
	
	template<UInt N>
	void boundingBox<N>::updateNumCells(const UInt & i)
	{
		// Update number of cells
		boundingBox::numCells[i] = 
			static_cast<UInt>((boundingBox::NE_global[i] - boundingBox::SW_global[i]) 
			/ boundingBox::cellSize[i]);
			
		// Check it is not zero
		if (boundingBox::numCells[i] == 0)
			boundingBox::numCells[i] = 1;
			
		// Update cell size
		boundingBox::updateCellSize(i);
	}
	
	
	template<UInt N>
	void boundingBox<N>::updateNumCells()
	{
		for (UInt i = 0; i < N; ++i)
			boundingBox::updateNumCells(i);
	}
	
	
	template<UInt N>
	void boundingBox<N>::updateCellSize(const UInt & i)
	{
		assert(i < N);
		
		boundingBox::cellSize[i] = 
			(boundingBox::NE_global[i] - boundingBox::SW_global[i]) / boundingBox::numCells[i];
	}
	
	
	template<UInt N>
	void boundingBox<N>::updateCellSize()
	{
		for (UInt i = 0; i < N; ++i)
			boundingBox::updateCellSize(i);
	}
}

#endif

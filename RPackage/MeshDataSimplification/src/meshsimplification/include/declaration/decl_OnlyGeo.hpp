/*!	\file	decl_OnlyGeo.hpp
	\brief 	Declaration of members of class OnlyGeo. */
	
#ifndef HH_DECLONLYGEO_HH
#define HH_DECLONLYGEO_HH

#include <utility>

namespace geometry
{
	/*!	This class computes the geometric cost function associated
		with the collapsing of an edge as described in Section 2.2.1
		of the paper by Dassi et al.
		It can be applied on grids with or without distributed data.
		This is why MeshType still appears as template parameter.
		The class inherits the base class bcost according to the
		Curiously Recurring Template Pattern (CRTP).
		Since this class should only provide the implementation for
		the bcost class, all methods are protected. */
	template<MeshType MT>
	class OnlyGeo final : public bcost<Triangle, MT, OnlyGeo<MT>>
	{
		/*!	The base class is a friend class. */
		friend class bcost<Triangle, MT, OnlyGeo<MT>>;
		
		private:
			/*!	Vector of Q matrices for all nodes.
				Denoting by $\pi_v$ the set of planes identified by
				the triangles sharing the node $v$, the matrix Q is 
				defined as $Q = \sum_{\rho \in \pi_v} \rho \rho^T$,
				with $\rho = [a b c d]^T$ and $a^2 + b^2 + c^2 = 1$, 
				where $ax + by + cz + d = 0$ is the equation of the 
				plane. Note that Q is a symmetric matrix, then the
				number of elements to store reduces to 10.
				For each node, these elements are stored in an STL
				array as the following:
				[Q(0,0) Q(0,1) Q(0,2) Q(0,3) Q(1,1) Q(1,2) Q(1,3) 
				Q(2,2) Q(2,3) Q(3,3)]
				In other words, we store only the upper triangular
				part following a row-major policy. */
			vector<array<Real,10>> Qs;
			
		public:
			//
			// Constructors
			//
			
			/*!	(Default) constructor.
				\param bmo	pointer to a bmeshOperation object */
			OnlyGeo(bmeshOperation<Triangle,MT> * bmo = nullptr);
			
			//
			// Access members
			//
			
			/*!	Get Q matrix associated with a node.
				\param id	node Id
				\return		the matrix reshaped in a (10x1) vector */
			array<Real,10> getQMatrix(const UInt & id) const;
			
			/*!	Get list of Q matrices for all nodes.
				\return		vector of Q matrices reshaped in (10x1) vectors*/
			vector<array<Real,10>> getQs() const;
			
		protected:
			//
			// Handle list of Q matrices
			//
			
			/*!	Compute the matrix K associated with the plane of a triangle,
				i.e. $K = \rho \rho^T$ with $\rho = [a b c d]^T$ and
				$a^2 + b^2 + c^2 = 1$, where $ax + by + cz + d = 0$ is the
				equation of the plane.
				Exploiting the symmetry of K, only 10 out of 16 elements
				need to be computed. 
				
				\param id	element Id
				\return		the matrix K reshaped in a (16x1) vector */
			array<Real,10> getKMatrix(const UInt & id) const;
			
			/*! Build the matrix Q for each node. */
			void buildQs();
						
			//
			// Set methods
			//
			
			/*!	Set pointer to bmeshOperation.
				This method provides the implementation of the method 
				setMeshOperation() of bcost.
				
				\param bmo	pointer to a bmeshOperation object */
			void imp_setMeshOperation(bmeshOperation<Triangle,MT> * bmo);
			
			//
			// Get methods
			//
			
			/*!	For an edge, get the collapsing point minimizing the
				geometric const function.
				
				\param id1	Id of first end-point of the edge
				\param id2	Id of second end-point of the edge 
				\return		a bool saying if the optimum point does exist
							and, if so, the optimum point itself */
			pair<bool,point> getOptimumPoint(const UInt & id1, const UInt & id2) const;
			
			/*!	Get list of valid collapsing points for an edge.
				Here, their validity is sanctioned according to 
				the boundary flags of the edge end-points.
				This method provides the implementation of the method 
				getPointsList() of bcost.
				
				\param id1	Id of first end-point of the edge
				\param id2	Id of second end-point of the edge
				\return		vector of valid points */
			vector<point> imp_getPointsList(const UInt & id1, const UInt & id2) const;
			
			/*!	Get cost for collapsing an edge in a point.
				The method supposes the connections have already been
				modified to accomplish the collapse.
				This method provides the implementation of the method 
				getCost() of bcost.
				
				\param id1	Id of first end-point of the edge
				\param id2	Id of second end-point of the edge
				\param p	collapsing point
				\return		the cost */
			Real imp_getCost(const UInt & id1, const UInt & id2, const point3d & p) const;
			
			/*!	Get cost for collapsing an edge in a point.
				The method supposes the connections have already been
				modified to accomplish the collapse.
				This method provides the implementation of the method 
				getCost_f() of bcost.
				As it is implemented, this method performs the same operations
				of getCost().
				
				\param id1	Id of first end-point of the edge
				\param id2	Id of second end-point of the edge
				\param p	collapsing point
				\return		the cost */
			Real imp_getCost_f(const UInt & id1, const UInt & id2, const point3d & p) const;
			
			//
			// Updating methods
			//
			
			/*!	Add a collapseInfo object to the list.
				This method provides the implementation of the method 
				addCollapseInfo() of bcost.
				Actually, as it is implemented this method is equal to
				the method addCollapseInfo_f() of bcost.
				
				\param id1	Id of first end-point of the edge
				\param id2	Id of second end-point of the edge
				\param val	collapsing cost
				\param p	collapsing point */
			void imp_addCollapseInfo(const UInt & id1, const UInt & id2, 
				const Real & val, const point3d & p);
				
			/*!	Update the list of Q matrices after an edge collapse.
				This method should be called after having updated the mesh
				and all the connections.
				It provides the implementation of the method update() of bcost.
				
				\param newId	Id of the collapsing point */
			void imp_update(const UInt & newId);
				
			/*!	Check whether the collapseInfo's list should be re-built.
				This method provides the implementation of the method 
				toUpdate() of bcost.
				Actually, the class OnlyGeo never requires the costs to be re-computed,
				then this method always return FALSE. 
				
				\return		FALSE */
			CONSTEXPR bool imp_toUpdate() const;
			
			/*!	Clear the collapseInfo's list.
				This method provides the implementation of the method 
				clear() of bcost. */
			void imp_clear(); 
	};
}

#endif

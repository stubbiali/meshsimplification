/*!	\file	decl_DataGeo.hpp 
	\brief 	Declaration of members of class DataGeo. */
	
#ifndef HH_DECLDATAGEO_HH
#define HH_DECLDATAGEO_HH

#include <tuple>

namespace geometry
{
	/*!	This class computes the geometric, data displacement and 
		data distribution cost functions associated with an edge
		collapse as described in Section 2.2.1, of the paper by 
		Dassi et al. These functions are then combined to give the
		final cost, as described in Section 2.3.
		It can be applied only on grids with distributed data.
		The class inherits the base class bcost according to the
		Curiously Recurring Template Pattern (CRTP).
		Since this class should only provide the implementation for
		the bcost class, all methods are protected. */
	class DataGeo final : public bcost<Triangle, MeshType::DATA, DataGeo>
	{
		/*!	The base class is a friend class. */
		friend class bcost<Triangle, MeshType::DATA, DataGeo>;
		
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
			
			/*!	Original location of data points. */
			vector<point3d> dataOrigin;
						
			/*!	Quantity of information for each element.
				For its formal definition, see Equation (5) of Dassi et al. */
			vector<Real> qoi;
			
			/*!	Current number of elements. */
			UInt numElems;
			
			/*!	Average quantity of information. */
			Real qoi_mean;
			
			/*!	Maximum values over the entire mesh for the geometric,
				data displacement and data distribution cost functions.
				These will be used to normalize the three contributions
				to the collapse cost before combining them. */ 
			array<Real,3> maxCost;
			
			/*!	Weights for the geometric, data displacement and data 
				distribution cost functions. */
			array<Real,3> weight;
			
			/*!	Variables keeping track, for each edge, of the minimum for 
				each component. These values will be useful when checking 
				whether the class requires an update. */
			Real min_geo, min_disp, min_equi;
			
			/*!	Boolean saying whether the costs should be re-computed
				because the maxima have significantly changed. */
			bool to_update;
			
		public:
			//
			// Constructors
			//
			
			/*!	(Default) constructor.
				\param bmo	pointer to a bmeshOperation object
				\param a	coefficient for geometric cost function
				\param b	coefficient for data displacement cost function
				\param c	coefficient for data distribution cost function */
			DataGeo(bmeshOperation<Triangle, MeshType::DATA> * bmo = nullptr,
				const Real & a = 1./3., const Real & b = 1./3., const Real & c = 1./3.);
				
			/*!	Constructor.
				\param a	coefficient for geometric cost function
				\param b	coefficient for data displacement cost function
				\param c	coefficient for data distribution cost function */
			DataGeo(const Real & a, const Real & b, const Real & c);
				
			//
			// Access members
			//
			// 
			
			/*!	Get Q matrix associated with a node.
				\param id	node Id
				\return		the matrix reshaped in a (10x1) vector */
			array<Real,10> getQMatrix(const UInt & id) const;
			
			/*!	Get list of Q matrices for all nodes.
				\return		vector of Q matrices reshaped in (10x1) vectors*/
			vector<array<Real,10>> getQs() const;
			
			/*!	Get the original location of a data point.
				\param id	data point Id
				\return		the location */ 
			point3d getOriginalDataPointLocation(const UInt & id) const;
			
			/*!	Get quantity of information for an element.
				\param id	element Id
				\return		element quantity of information */
			Real getQuantityOfInformation(const UInt & id) const;
			
			/*!	Get number of elements.
				\return		number of elements */
			UInt getNumElems() const;
			
			/*!	Get quantity of information averaged over the mesh.
				\return 	average quantity of information */
			Real getAverageQuantityOfInformation() const;
			
			/*!	Get maximum over the entire mesh for a cost function.
				\param i	geometric (i = 0), data displacement (i = 1)
							or data distribution (i = 2) cost 
				\return		maximum cost */
			Real getMaxCost(const UInt & i) const;
						
			/*!	Get coefficient associated with a cost function.
				\param i	geometric (i = 0), data displacement (i = 1)
							or data distribution (i = 2) cost
				\return		the coefficient */
			Real getWeight(const UInt & i) const;
			
			/*!	Set coefficient associated with a cost function.
				\param i	geometric (i = 0), data displacement (i = 1)
							or data distribution (i = 2) cost
				\return		the new coefficient */
			void setWeight(const UInt & i, const Real & val);
						
		protected:
			//
			// Initialization and update of class-specific members
			//
			// These methods are in charge of handling the lists of Q matrices
			// and quantity of informations and computing the normalizing factors
			
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
			
			/*!	Update Q matrices for the points involved in an edge collapse.
				\param newId	Id of collapsing point */
			void updateQs(const UInt & newId);
			
			/*!	Build list of original locations for data points. */
			void getOriginalDataPointsLocations();
			
			/*!	Compute quantity of information for each element.
				Then average over the entire mesh too. */
			void buildQuantityOfInformation();
			
			/*!	After an edge collapse, update list of quantity of information and 
				average quantity of information.
				
				\param newId	Id of the collapsing point
				\param toRemove	Id's of the elements to remove */ 
			void updateQuantityOfInformation(const UInt & newId, const vector<UInt> & toRemove);
			
			/*!	Get geometric, data displacement and data distribution costs
				for an edge collapse. The components are not normalized.
				
				\param id1		first end-point of the edge
				\param id2		second end-point of the edge
				\param p		collapsing point
				\param toKeep	Id's of elements involved in the collapse
								but not insisting on the edge
				\param toMove	Id's of data points involved in the collapse */
			tuple<Real,Real,Real> getDecomposedCost(const UInt & id1, const UInt & id2,
				const point3d & p, const vector<UInt> & toKeep, const vector<UInt> & toMove) const;
			
			/*!	Among all possible collapsing points, get maximum geometric,
				data displacement and data distribution cost function and
				possibly update the global maxima.
				The components are not normalized.
				
				\param id1		first end-point of the edge
				\param id2		second end-point of the edge */
			void getMaximumCosts(const UInt & id1, const UInt & id2);
			
			/*!	Get maximum geometric, data displacement and data distribution
				cost function over the entire mesh. */
			void getMaximumCosts();
						
			//
			// Set methods
			//
			
			/*!	Set pointer to bmeshOperation.
				This method provides the implementation of the method 
				setMeshOperation() of bcost.
				
				\param bmo	pointer to a bmeshOperation object */
			void imp_setMeshOperation(bmeshOperation<Triangle, MeshType::DATA> * bmo);
						
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
						
			/*!	Get cost for collapsing an edge in a point and keep track 
				of the components for future updating checks.
				The method supposes the connections have already been
				modified to accomplish the collapse.
				This method provides the implementation of the method 
				getCost() of bcost.
				
				\param id1		Id of first end-point of the edge
				\param id2		Id of second end-point of the edge
				\param p		collapsing point
				\param toKeep	Id's of elements involved in the collapse
								but not insisting on the edge
				\param toMove	Id's of the data points involved in the collapse
				\return			the cost */
			Real imp_getCost(const UInt & id1, const UInt & id2, const point3d & p,
				const vector<UInt> & toKeep, const vector<UInt> & toMove);
				
			/*!	Get cost for collapsing an edge in a point.
				The method supposes the connections have already been
				modified to accomplish the collapse.
				This method provides the implementation of the method 
				getCost() of bcost.
				
				\param id1		Id of first end-point of the edge
				\param id2		Id of second end-point of the edge
				\param p		collapsing point
				\param toKeep	Id's of elements involved in the collapse
								but not insisting on the edge
				\param toMove	Id's of the data points involved in the collapse
				\return			the cost */
			Real imp_getCost_f(const UInt & id1, const UInt & id2, const point3d & p,
				const vector<UInt> & toKeep, const vector<UInt> & toMove) const;
				
			//
			// Updating methods
			//
			
			/*!	Add a collapseInfo object to the list and check if the costs
				should be re-computed because the maxima have significantly changed.
				This method provides the implementation of the method 
				addCollapseInfo() of bcost.
				
				\param id1	Id of first end-point of the edge
				\param id2	Id of second end-point of the edge
				\param val	collapsing cost
				\param p	collapsing point */
			void imp_addCollapseInfo(const UInt & id1, const UInt & id2, const Real & val,
				const point3d & p);
				
			/*!	Update the list of Q matrices after an edge collapse.
				This method should be called after having updated the mesh
				and all the connections.
				It provides the implementation of the method update() of bcost.
				
				\param newId	Id of the collapsing point 
				\param toRemove	elements insisting on the edge, then to remove */
			void imp_update(const UInt & newId, const vector<UInt> & toRemove);
			
			/*!	Check if the costs should be re-computed because the maxima
				have significantly changed. 
				This method provides the implementation of the method 
				toUpdate() of bcost.
				
				\return 	TRUE if the costs should be re-computed,
							FALSE otherwise */
			bool imp_toUpdate() const;
			
			/*!	Clear the collapseInfo's list.
				This method provides the implementation of the method 
				clear() of bcost. */
			void imp_clear(); 
	};
}

#endif

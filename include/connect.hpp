/*! \file	connect.hpp
	\brief	A class creating the connections for a mesh. */
	
#ifndef HH_CONNECT_HH
#define HH_CONNECT_HH

#include "bconnect.hpp"

namespace geometry
{
	/*!	Forward declaration of class bmeshInfo. */
	template<typename SHAPE, MeshType MT>
	class bmeshInfo;
	
	/*!	Forward declaration of class meshInfo. */
	template<typename SHAPE, MeshType MT>
	class meshInfo;
	
	/*!	Forward declaration of class projection. */
	template<typename SHAPE>
	class projection;
		
	/*! This is a wrapper class for bconnect, which it inherits.
		It takes two template parameters:
		<ol>
		<li> SHAPE: shape of the elements;
		<li> MT: 	mesh type, i.e. with or without distributed data;
					default is GEO.
		<\ol>
		When MT is set to GEO (i.e. no data): the class just inherits bconnect
		without adding or overriding members.
		When MT is set to DATA (i.e. with data): the class inherits bconnect and 
		adds members for handling data distributed over the mesh. In particular,
		the connections data-element and element-data are built and maintained.
		
		\sa bconnect.hpp */
	template<typename SHAPE, MeshType MT = MeshType::GEO>
	class connect final : public bconnect<SHAPE,MT>
	{
	};
	
	/*! Partial specialization for grids without distributed data. */
	template<typename SHAPE>
	class connect<SHAPE, MeshType::GEO> final : public bconnect<SHAPE, MeshType::GEO> 
	{
		/*! bmeshInfo is a friend class. */
		friend class bmeshInfo<SHAPE, MeshType::GEO>;
		
		/*! meshInfo is a friend class. */
		friend class meshInfo<SHAPE, MeshType::GEO>;
		
		/*! projection is a friend class. */
		friend class projection<SHAPE>;
			
		public:		
			//
			// Constructor
			//
			
			/*!	Synthetic default constructor. */
			connect() = default;
			
			/*! Constructor.
				\param bg	a (base) grid */
			connect(const bmesh<SHAPE> & bg);
			
			/*!	Constructor.
				\param file	path to input mesh */
			connect(const string & file);
									
			/*!	Constructor specifically designed for the R interface.
				\param nds	#nodes-by-dim Eigen matrix storing the coordinates
							of the nodes
				\param els	#elements-by-NV Eigen matrix storing for each element
							the Id's of its vertices */
			connect(const MatrixXd & nds, const MatrixXi & els);
	};
	
	/*! Partial specialization for grids with distributed data. */
	template<typename SHAPE>
	class connect<SHAPE, MeshType::DATA> final : public bconnect<SHAPE, MeshType::DATA> 
	{
		/*! bmeshInfo is a friend class. */
		friend class bmeshInfo<SHAPE, MeshType::DATA>;
		
		/*! meshInfo is a friend class. */
		friend class meshInfo<SHAPE, MeshType::DATA>;
		
		/*! projection is a friend class. */
		friend class projection<SHAPE>;
		
		private:
			/*! Data-element connections. */
			vector<graphItem> data2elem;
			
			/*! Element-data connections. */
			vector<graphItem> elem2data;
						
		public:
			//
			// Constructor
			//
			
			/*!	Synthetic default constructor. */
			connect() = default;
			
			/*! Constructor.
				\param bg	a bmesh object
				
				\sa bmesh.hpp */
			connect(const bmesh<SHAPE> & bg);
			
			/*!	Constructor.
				Note that when the mesh is passed via file, the data locations
				are supposed to coincide with the grid nodes.
				
				\param file	path to input mesh 
				\param val	observations*/
			connect(const string & file, const vector<Real> & val = {});
									
			/*!	Constructor specifically designed for the R interface. 
				The data locations are supposed to coincide with the grid nodes
				and the observations are set to zero.
				
				\param nds	#nodes-by-dim Eigen matrix storing the coordinates
							of the nodes
				\param els	#elements-by-NV Eigen matrix storing for each element
							the Id's of its vertices */
			connect(const MatrixXd & nds, const MatrixXi & els);
			
			/*!	Constructor specifically designed for the R interface.
				The data locations are supposed to coincide with the grid nodes
				while the observations are specified by the user.
				
				\param nds	#nodes-by-dim Eigen matrix storing the coordinates
							of the nodes
				\param els	#elements-by-NV Eigen matrix storing for each element
							the Id's of its vertices 
				\param val	#nodes-by-1 Eigen array with data observations */
			connect(const MatrixXd & nds, const MatrixXi & els, const VectorXd & val);
			
			/*!	Constructor specifically designed for the R interface.
				Both the data locations and values are specified by the user.
				Note that this constructor is provided only for triangular grids.
				
				\param nds	#nodes-by-3 Eigen matrix storing the coordinates
							of the nodes
				\param els	#elements-by-3 Eigen matrix storing for each element
							the Id's of its vertices 
				\param loc	#data-by-3 Eigen matrix storing the coordinates of
							data locations
				\param val	#data-by-1 Eigen array with data observations */
			connect(const MatrixXd & nds, const MatrixXi & els, const MatrixXd & loc,
				const VectorXd & val = VectorXd());
			
			//
			// Initialize and clear connections
			//
			
			/*! Initialize data-element connections. 
				This method supposes the data points coincide with the nodes, 
				then data-element connections coincide with node-element connections.
				In case data points and nodes do not coincide, see buildData2Elem_p(). */
			void buildData2Elem();
			
			/*!	Initialize data-element connections by finding, for each
				data point, the triangle(s) it belongs too. This pedantic (this is why
				the "_p" in the name) is surely more expensive than buildData2Elem(),
				however it allows to deal with data non located at the grid nodes. 
				Note that this method is provided only for triangular grids. */
			void buildData2Elem_p();
			
			/*! Initialize element-data connections. */
			void buildElem2Data();
			
			/*! Initialize element-data connections. This version should be 
				prefered whenever data locations do not coincide with grid nodes. */
			void buildElem2Data_p();
					
			/*! Re-build all connections. */
			virtual void refresh();
			
			/*! Clear all connections and the set of edges. */
			virtual void clear();
			
			//
			// Modify connections
			//
			
			/*! Remove an element from data-element connections.
				\param Id	Id of the element to remove */
			void eraseElemInData2Elem(const UInt & Id);
			
			/*! Remove elements from data-element connections.
				\param ids	vector with Id's of elements to remove */
			void eraseElemInData2Elem(const vector<UInt> & ids);
			
			/*! Insert an element in data-element connections.
				\param Id	Id of the element to insert */
			void insertElemInData2Elem(const UInt & Id);
						
			/*! Insert elements in data-element connections.
				\param ids	vector with Id's of elements to insert */
			void insertElemInData2Elem(const vector<UInt> & ids);
			
			/*! Remove a datum from element-data connections.
				\param Id	Id of the datum to remove */
			void eraseDataInElem2Data(const UInt & Id);
			
			/*! Remove data from element-data connections.
				\param ids	vector with Id's of data to remove */
			void eraseDataInElem2Data(const vector<UInt> & ids);
			
			/*! Insert a datum in element-data connections.
				\param Id	Id of the datum to insert */
			void insertDataInElem2Data(const UInt & Id);
						
			/*! Insert data in element-data connections.
				\param ids	vector with Id's of data to insert */
			void insertDataInElem2Data(const vector<UInt> & ids);
			
			// 
			// Get methods
			//
			
			/*! Get data-element connections for a datum.
				\param Id	datum Id
				\return		the connections */
			graphItem getData2Elem(const UInt & Id) const;
			
			/*! Get data-element connections for all data.
				\return		vector of connections */
			vector<graphItem> getData2Elem() const;
			
			/*! Get element-data connections for an element.
				\param Id	element Id
				\return		the connections */
			graphItem getElem2Data(const UInt & Id) const;
			
			/*! Get element-data connections for all elements.
				\return		vector of connections */
			vector<graphItem> getElem2Data() const;
						
			//
			// Set methods
			//
			
			/*! Set data-element connections for a datum.
				Before setting the new connections, it calls eraseDataInElem2Data().
				After having set the new connections, it calls insertDataInElem2Data().
				It returns the old connections.
				
				\param Id			datum Id
				\param newConn		new connections 
				\return				old connections
				
				\sa eraseDataInElem2Data(), insertDataInElem2Data() */
			vector<UInt> setData2Elem(const UInt & Id, const set<UInt> & newConn);
			
			/*! Set data-element connections for a datum.
				Before setting the new connections, it calls eraseDataInElem2Data().
				After having set the new connections, it calls insertDataInElem2Data().
				It returns the old connections.
				
				\param Id			datum Id
				\param newConn		new connections 
				\return				old connections
				
				\sa eraseDataInElem2Data(), insertDataInElem2Data() */
			vector<UInt> setData2Elem(const UInt & Id, const vector<UInt> & newConn);
			
			/*! Set data-element connections for a datum.
				Before setting the new connections, it calls eraseDataInElem2Data().
				After having set the new connections, it calls insertDataInElem2Data().
				It returns the old connections.
				
				\param newData2Elem	new graph item 
				\return				old connections
				
				\sa eraseDataInElem2Data(), insertDataInElem2Data() */
			graphItem setData2Elem(const graphItem & newData2Elem);
			
			/*! Set data-element connections for all data.
				This is particularly useful if the data do not initially coincide with nodes.
				
				\param newData2Elem	vector of connections */
			void setData2Elem(const vector<graphItem> & newData2Elem);
		
		protected:
			//
			// Auxiliary refresh methods
			//
			
			/*!	Update data-element connections after a mesh refresh.
				\param old2new	old-to-new map for elements Id's */
			void refreshData2Elem(map<UInt,UInt> old2new);
	};
}

/*! Include implementations of class members. */
#include "implementation/imp_connect.hpp"

#endif

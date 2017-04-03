/*! \file	imp_mesh.hpp
	\brief	Definitions of members and friend functions of class mesh. */
	
#ifndef HH_IMPMESH_HH
#define HH_IMPMESH_HH

#include <algorithm>

namespace geometry
{
	//
	// Constructors (MeshType::GEO)
	//
	
	template<typename SHAPE>
	mesh<SHAPE, MeshType::GEO>::mesh(const bmesh<SHAPE> & bm) :
		bmesh<SHAPE>(bm)
	{
	}
	
	
	template<typename SHAPE>
	mesh<SHAPE, MeshType::GEO>::mesh(const UInt & nNodes, const UInt & nElems) :
		bmesh<SHAPE>(nNodes, nElems)
	{
	}
	
	
	template<typename SHAPE>
	mesh<SHAPE, MeshType::GEO>::mesh(const vector<point> & nds, const vector<geoElement<SHAPE>> & els) :
		bmesh<SHAPE>(nds, els)
	{
	}
	
	
	template<typename SHAPE>
	mesh<SHAPE, MeshType::GEO>::mesh(const string & filename) :
		bmesh<SHAPE>(filename)
	{
	}
	
	
	//
	// Constructors (MeshType::DATA)
	//
	
	template<typename SHAPE>
	mesh<SHAPE, MeshType::DATA>::mesh(const bmesh<SHAPE> & bm) :
		bmesh<SHAPE>(bm)
	{
		// Fill data points list with points coinciding 
		// with the nodes and associated to a null datum
		data.reserve(this->nodes.size());
		for (auto node : this->nodes)
			data.emplace_back(node);
	}
	
	
	template<typename SHAPE>
	mesh<SHAPE, MeshType::DATA>::mesh(const vector<point> & nds, 
		const vector<geoElement<SHAPE>> & els, vector<Real> val) :
		bmesh<SHAPE>(nds,els) 
	{
		// If the provided number of values is smaller than 
		// the number of nodes, extend val by zero
		val.resize(this->nodes.size());
		
		// Fill the vector of data points
		data.reserve(this->nodes.size());
		for (UInt i = 0; i < this->nodes.size(); ++i)
			data.emplace_back(this->nodes[i],val[i]);
	}
	
	
	template<typename SHAPE>
	mesh<SHAPE, MeshType::DATA>::mesh(const vector<point> & nds, 
		const vector<geoElement<SHAPE>> & els, const vector<dataPoint> & dat) :
		bmesh<SHAPE>(nds,els), data(dat.cbegin(), dat.cend())
	{
		// If the data list is empty, fill it with data points 
		// coinciding with the nodes and associated to a null datum
		if (data.empty())
		{
			data.reserve(this->nodes.size());
			for (auto node : this->nodes)
				data.emplace_back(node);
		}
	}
	
	
	template<typename SHAPE>
	mesh<SHAPE, MeshType::DATA>::mesh(const string & filename) :
		bmesh<SHAPE>(filename)
	{
		// Fill data points list with points coinciding 
		// with the nodes and associated to a null datum
		data.reserve(this->nodes.size());
		for (auto node : this->nodes)
			data.emplace_back(node);
	}
	
	
	//
	// Operators
	//
	
	template<typename SHAPE>
	mesh<SHAPE, MeshType::DATA> & mesh<SHAPE, MeshType::DATA>::operator=
		(const bmesh<SHAPE> & bm)
	{
		// Copy nodes and elemens
		bmesh<SHAPE>::operator=(bm);
		
		// Update list of data points
		data.clear();
		data.reserve(this->nodes.size());
		for (auto node : this->nodes)
			data.emplace_back(node);
			
		return *this;
	}
	
	
	template<typename SHAPE>
	mesh<SHAPE, MeshType::DATA> & mesh<SHAPE, MeshType::DATA>::operator=
		(const mesh<SHAPE, MeshType::DATA> & m)
	{
		// Copy nodes and elemens
		bmesh<SHAPE>::operator=(m);
		
		// Update list of data points
		data.clear();
		data.reserve(this->nodes.size());
		for (auto node : this->nodes)
			data.emplace_back(node);
		
		return *this;
	}
	
	
	template<typename SHAPE>
	ostream & operator<<(ostream & out, const mesh<SHAPE, MeshType::DATA> & m)
	{
		// Print lists of nodes and elements
		out << static_cast<const bmesh<SHAPE> &>(m);
		
		// Print list of data points
		out << "List of " << m.getNumData() << " data points:" << endl;
		for (auto dat : m.data)
			out << dat << endl;
			
		return out;
	}
	
	
	//
	// Get methods
	//
	
	template<typename SHAPE>
	INLINE dataPoint mesh<SHAPE, MeshType::DATA>::getData(const UInt & Id) const
	{
		return data[Id];
	}
			
	
	template<typename SHAPE>
	INLINE UInt mesh<SHAPE, MeshType::DATA>::getNumData() const
	{
		return data.size();
	}
	
	
	//
	// Set methods
	//
	
	template<typename SHAPE>
	INLINE void mesh<SHAPE, MeshType::DATA>::setData(const UInt & Id, const array<Real,3> & coor)
	{
		data[Id].setCoor(coor);
	}
	
	
	template<typename SHAPE>
	INLINE void mesh<SHAPE, MeshType::DATA>::setData(const UInt & Id, const point3d & p)
	{
		data[Id].setCoor(p.getCoor());
	}
	
	
	template<typename SHAPE>
	INLINE void mesh<SHAPE, MeshType::DATA>::setData(const UInt & Id, const Real & val)
	{
		data[Id].setDatum(val);
	}
	
	
	template<typename SHAPE>
	INLINE void mesh<SHAPE, MeshType::DATA>::setData(const UInt & Id, 
		const array<Real,3> & coor, const Real & val)
	{
		data[Id].setCoor(coor);
		data[Id].setDatum(val);
	}
	
	
	template<typename SHAPE>
	INLINE void mesh<SHAPE, MeshType::DATA>::resizeData(const UInt & numData)
	{
		data.resize(numData);
	}
	
	
	template<typename SHAPE>
	INLINE void mesh<SHAPE, MeshType::DATA>::reserveData(const UInt & numData)
	{
		data.reserve(numData);
	}
	
	
	//
	// Insert, erase and clear methods
	//
	
	template<typename SHAPE>
	INLINE void mesh<SHAPE, MeshType::DATA>::insertData(const array<Real,3> & coor, const Real & val)
	{
		data.emplace_back(coor, data.size(), val);
	}
	
	
	template<typename SHAPE>
	void mesh<SHAPE, MeshType::DATA>::eraseData(const UInt & Id)
	{
		// Erase
		auto it = data.begin() + Id;
		data.erase(it);
		
		// Update id's
		setUpDataIds();
	}
	
	
	template<typename SHAPE>
	void mesh<SHAPE, MeshType::DATA>::clear()
	{
		// Clear nodes and elements list
		bmesh<SHAPE>::clear();
		
		// Clear data points list
		data.clear();
	}
	
	
	//
	// Update Id's
	//
	
	template<typename SHAPE>
	void mesh<SHAPE, MeshType::DATA>::setUpDataIds()
	{
		for (UInt Id = 0; Id < data.size(); ++Id)
			data[Id].setId(Id);
	}	
	
	
	template<typename SHAPE>
	void mesh<SHAPE, MeshType::DATA>::print_inp(const string & filename) const
	{
	}
	
	
	// Declare specialization for triangular grids
	template<>
	void mesh<Triangle, MeshType::DATA>::print_inp(const string & filename) const;
	
	
	// Declare specialization for quadrilateral grids
	template<>
	void mesh<Quad, MeshType::DATA>::print_inp(const string & filename) const;
}

#endif

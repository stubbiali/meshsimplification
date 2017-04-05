/*! \file	imp_bmesh.hpp
	\brief	Definitions of members and friend functions of class bmesh. 
			For the specializations of members for reading the mesh
			from a .imp file: see bmesh.cpp. */
	
#ifndef HH_IMPBMESH_HH
#define HH_IMPBMESH_HH

#include <algorithm>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <istream>
#include <array>
#include <map>

#include "utility.hpp"

namespace geometry
{
	//
	// Constructors
	//
	
	template<typename SHAPE>
	bmesh<SHAPE>::bmesh(const UInt & nNodes, const UInt & nElems) :
		numNodes(nNodes), numElems(nElems)
	{
		assert(numNodes < MAX_NUM_NODES);
		assert(numElems < MAX_NUM_ELEMS);
		
		nodes.reserve(numNodes);
		elems.reserve(numElems);
	}
	
	
	template<typename SHAPE>
	bmesh<SHAPE>::bmesh(const vector<point> & nds, const vector<geoElement<SHAPE>> & els) :
		numNodes(nds.size()), numElems(els.size()),
		nodes(nds.cbegin(), nds.cend()), elems(els.cbegin(), els.cend())
	{
	}
	
	
	template<typename SHAPE>
	bmesh<SHAPE>::bmesh(const string & filename)
	{
		// Extract file extension
		auto format = utility::getFileExtension(filename);
		
		// Switch the format
		if (format == "inp")
			read_inp(filename);
		else if (format == "vtk")
			read_vtk(filename);
		else if (format == "obj")
			read_obj(filename);
		else
			throw runtime_error("Format " + format + " not known.");
	}
	
	
	template<typename SHAPE>
	bmesh<SHAPE>::bmesh(const MatrixXd & nds, const MatrixXi & els) :
		numNodes(nds.rows()), numElems(els.rows())
	{
		// Check dimensions
		if ((nds.cols() != 3) || (els.cols() != NV))
			throw runtime_error("Dimensions check failed; first input matrix should have "
				"3 columns, while the second input matrix " + to_string(NV) + " columns.");
				
		// Initialize nodes list
		nodes.reserve(numNodes);
		for (UInt i = 0; i < numNodes; ++i)
			nodes.emplace_back(nds(i,0), nds(i,1), nds(i,2), i);
			
		// Initialize elements list
		elems.reserve(numElems);
		array<UInt,NV> ids;
		for (UInt i = 0; i < numElems; ++i)
		{
			for (UInt j = 0; j < NV; ++j)
				ids[j] = UInt(els(i,j));
			elems.emplace_back(ids, i);
		}
	}
		
	
	//
	// Operators
	//
	
	template<typename SHAPE>
	bmesh<SHAPE> & bmesh<SHAPE>::operator=(const bmesh<SHAPE> & bm)
	{
		// Clear nodes and elements lists
		nodes.clear();
		elems.clear();
		
		// Set the new capacities
		nodes.reserve(bm.getNumNodes());
		elems.reserve(bm.getNumElems());
		
		// Copy
		copy(bm.nodes.cbegin(), bm.nodes.cend(), nodes.begin());
		copy(bm.elems.cbegin(), bm.elems.cend(), elems.begin());
		
		// Update number of nodes and elements
		numNodes = nodes.size();
		numElems = elems.size();
		
		return *this;
	}
	
	
	template<typename SHAPE>
	ostream & operator<<(ostream & out, const bmesh<SHAPE> & bm)
	{
		// Print list of nodes			
		out << "List of " << bm.getNumNodes() << " nodes:" << endl;
		for (auto node : bm.nodes)
			out << node << endl;
			
		// Print list of elements
		out << "List of " << bm.getNumElems() << " elements:" << endl;
		for (auto elem : bm.elems)
			out << elem << endl;
			
		return out;
	}
	
	
	//
	// Get methods
	//
	
	template<typename SHAPE>
	INLINE point bmesh<SHAPE>::getNode(const UInt & Id) const
	{
		return nodes[Id];
	}
	
	
	template<typename SHAPE>
	INLINE vector<point> bmesh<SHAPE>::getNodes() const
	{
		return nodes;
	}
	
	
	template<typename SHAPE>
	INLINE geoElement<SHAPE> bmesh<SHAPE>::getElem(const UInt & Id) const
	{
		return elems[Id];
	}
	
	
	template<typename SHAPE>
	INLINE vector<geoElement<SHAPE>> bmesh<SHAPE>::getElems() const
	{
		return elems;
	}
	
	
	template<typename SHAPE>
	INLINE UInt bmesh<SHAPE>::getNumNodes() const
	{
		return numNodes;
	}
	
	
	template<typename SHAPE>
	INLINE UInt bmesh<SHAPE>::getNodesListSize() const
	{
		return nodes.size();
	}
	
	
	template<typename SHAPE>
	INLINE UInt bmesh<SHAPE>::getNumElems() const
	{
		return numElems;
	}
	
	
	template<typename SHAPE>
	INLINE UInt bmesh<SHAPE>::getElemsListSize() const
	{
		return elems.size();
	}
	
	
	template<typename SHAPE>
	INLINE bool bmesh<SHAPE>::isNodeActive(const UInt & Id) const
	{
		return nodes[Id].isActive();
	}
	
	
	template<typename SHAPE>
	INLINE bool bmesh<SHAPE>::isElemActive(const UInt & Id) const
	{
		return elems[Id].isActive();
	}
	
	
	//
	// Set methods
	//
	
	template<typename SHAPE>
	INLINE void bmesh<SHAPE>::setNode(const UInt & Id, const point & p)
	{
		nodes[Id] = p;
	}
	
	
	template<typename SHAPE>
	INLINE void bmesh<SHAPE>::setElem(const UInt & Id, const geoElement<SHAPE> & g)
	{
		elems[Id] = g;
	}
	
	
	template<typename SHAPE>
	void bmesh<SHAPE>::resizeNodes(const UInt & nNodes)
	{
		nodes.resize(nNodes);
		
		// Update number of (active) nodes
		numNodes = 0;
		for (UInt i = 0; i < nodes.size(); ++i)
		{
			if (nodes[i].isActive())
				++numNodes;
		}
	}
	
	
	template<typename SHAPE>
	void bmesh<SHAPE>::reserveNodes(const UInt & nNodes)
	{
		nodes.reserve(nNodes);
		
		// Update number of (active) nodes
		numNodes = 0;
		for (UInt i = 0; i < nodes.size(); ++i)
		{
			if (nodes[i].isActive())
				++numNodes;
		}
	}
	
	
	template<typename SHAPE>
	void bmesh<SHAPE>::resizeElems(const UInt & nElems)
	{
		elems.resize(nElems);
		
		// Update number of (active) elements
		numElems = 0;
		for (UInt i = 0; i < elems.size(); ++i)
		{
			if (elems[i].isActive())
				++numElems;
		}
	}
	
	
	template<typename SHAPE>
	void bmesh<SHAPE>::reserveElems(const UInt & nElems)
	{
		elems.reserve(nElems);
		
		// Update number of (active) elements
		numElems = 0;
		for (UInt i = 0; i < elems.size(); ++i)
		{
			if (elems[i].isActive())
				++numElems;
		}
	}
		
	
	template<typename SHAPE>
	INLINE void bmesh<SHAPE>::setBoundary(const UInt & Id, const UInt & bound)
	{
		nodes[Id].setBoundary(bound);
	}
	
	
	template<typename SHAPE>
	INLINE void bmesh<SHAPE>::setNodeActive(const UInt & Id)
	{
		if (!(nodes[Id].isActive()))
		{
			nodes[Id].setActive();
			++numNodes;
		}
	}
	
	
	template<typename SHAPE>
	INLINE void bmesh<SHAPE>::setNodeInactive(const UInt & Id)
	{
		if (nodes[Id].isActive())
		{
			nodes[Id].setInactive();
			--numNodes;
		}
	}
	
	
	template<typename SHAPE>
	INLINE void bmesh<SHAPE>::setIdx(const UInt & Id, const UInt & idx)
	{
		elems[Id].setIdx(idx);
	}
	
	
	template<typename SHAPE>
	INLINE void bmesh<SHAPE>::setElemActive(const UInt & Id)
	{
		if (!(elems[Id].isActive()))
		{
			elems[Id].setActive();
			++numElems;
		}
	}
	
	
	template<typename SHAPE>
	INLINE void bmesh<SHAPE>::setElemInactive(const UInt & Id)
	{
		if (elems[Id].isActive())
		{
			elems[Id].setInactive();
			--numElems;
		}
	}
	
	
	//
	// Insert and replace methods
	//
	
	template<typename SHAPE>
	INLINE void bmesh<SHAPE>::insertNode(const array<Real,3> & coor, const UInt & bound)
	{
		nodes.emplace_back(coor, nodes.size(), bound);
		++numNodes;
	}
	
	
	template<typename SHAPE>
	INLINE void bmesh<SHAPE>::insertElem(const array<UInt,NV> & vert, const UInt & geoId)
	{
		elems.emplace_back(vert, elems.size(), geoId);
		++numElems;
	}
		
	
	template<typename SHAPE>
	INLINE void bmesh<SHAPE>::replaceVertex(const UInt & elemId, const UInt & oldId, const UInt & newId)
	{
		elems[elemId].replace(oldId,newId);
	}
	
	
	//
	// Erase and clear methods
	//
	
	template<typename SHAPE>
	void bmesh<SHAPE>::eraseNode(const UInt & Id)
	{
		// Find the node
		auto it = nodes.begin() + Id;
		
		// Before remove it, update number of nodes
		if (it->isActive())
			--numNodes;
		nodes.erase(it);
		
		// Update id's
		setUpNodesIds();
	}
	
	
	template<typename SHAPE>
	void bmesh<SHAPE>::eraseElem(const UInt & Id)
	{
		// Find the element
		auto it = elems.begin() + Id;
		
		// Before removing it, update number of elements
		if (it->isActive())
			--numElems;
		elems.erase(it);
		
		// Update id's
		setUpElemsIds();
	}
	
	
	template<typename SHAPE>
	void bmesh<SHAPE>::clear()
	{
		// Clear nodes
		numNodes = 0;
		nodes.clear();
		
		// Clear elements
		numElems = 0;
		elems.clear();
	}
	
	
	//
	// Refresh methods
	//
	
	template<typename SHAPE>
	pair<map<UInt,UInt>, map<UInt,UInt>> bmesh<SHAPE>::refresh()
	{
		// Map from old to new nodes id's
		map<UInt,UInt> nodes_old2new;
		map<UInt,UInt> elems_old2new;
		
		// Temporary list of (active) nodes and elements
		vector<point> tmp_nodes;
		vector<geoElement<SHAPE>> tmp_elems;

		// Reserve memory 
		tmp_nodes.reserve(numNodes);
		tmp_elems.reserve(numElems);
		
		//
		// Build temporary list of nodes
		//
		
		// Counter
		UInt count(0);
		
		// Loop on the nodes
		for (UInt i = 0; i < nodes.size(); ++i)
		{
			if (nodes[i].isActive())
			{
				// Put the node in the temporary list and set the Id
				tmp_nodes.push_back(nodes[i]);
				tmp_nodes[count].setId(count);

				// Update old-to-new map and counter
				nodes_old2new[i] = count;
				++count;
			}
		}
		
		//
		// Build temporary list of elements
		//
		
		// Reset counter
		count = 0;
		
		// Loop on the elements
		for (UInt i = 0; i < elems.size(); ++i)
		{
			if (elems[i].isActive())
			{
				// Extract the vertices and apply old-to-new map
				array<UInt, bmesh<SHAPE>::NV> ids;
				for (UInt j = 0; j < bmesh<SHAPE>::NV; ++j)
					ids[j] = nodes_old2new[elems[i][j]];
				
				// Extract the geometrical Id
				auto gId = elems[i].getGeoId();

				// Insert in the temporary list of elements
				tmp_elems.emplace_back(ids, count, gId);
				
				// Update old-to-new map and counter
				elems_old2new[i] = count;
				++count;
			}
		}
		
		//
		// Set new lists of nodes and elements
		//
		
		// Nodes
		nodes.clear();
		nodes.reserve(numNodes);
		copy(tmp_nodes.cbegin(), tmp_nodes.cend(), back_inserter(nodes));
		
		// Elements
		elems.clear();
		elems.reserve(numElems);
		copy(tmp_elems.cbegin(), tmp_elems.cend(), back_inserter(elems));
		
		return {nodes_old2new, elems_old2new};
	}
	
	
	//
	// Read mesh from file
	//
	
	template<typename SHAPE>
	void bmesh<SHAPE>::read_inp(const string & filename)
	{
		ifstream file(filename);
		if (file.is_open())
		{
			string line;
						
			// Get number of nodes and elements
			getline(file,line);
			static_cast<stringstream>(line) >> numNodes >> numElems;
			
			// Assert
			assert(numNodes < MAX_NUM_NODES);
			assert(numElems < MAX_NUM_ELEMS);
			
			// Reserve memory
			nodes.reserve(numNodes);
			elems.reserve(numElems);
			
			// Insert nodes
			UInt Id;
			array<Real,3> coor; 
			for (UInt n = 0; n < numNodes && getline(file,line); ++n)
			{
				// Extract coordinates
				static_cast<stringstream>(line)	>> Id 
												>> coor[0] 
												>> coor[1] 
												>> coor[2];
												
				// Insert at back
				nodes.emplace_back(coor,n);
			}
			
			// Insert elements
			UInt geoId;
			string foo; 
			array<UInt,NV> vert;
			for (UInt n = 0; n < numElems && getline(file,line); ++n)
			{
				// Extract geometric Id
				stringstream ss(line);
				ss >> Id >> geoId >> foo;
				
				// Extract vertices Id's
				// They need to be made compliant with a zero-based indexing
				for (auto & v : vert)
				{
					ss >> v;
					--v;
				}
				
				// Insert at back							
				elems.emplace_back(vert, n, geoId);
			}
			
			// Close the file
			file.close();
		}
		else
			throw runtime_error(filename + " can not be opened.");
	}
	
	
	template<typename SHAPE>
	void bmesh<SHAPE>::read_vtk(const string & filename)
	{
		// Note: we suppose the file does not include
		// any empty file, so in case they are present,
		// please remove them before calling this methods.
		
		ifstream file(filename);
		if (file.is_open())
		{
			string line, foo;
			
			// Disregard the first four lines
			for (UInt i = 0; i < 4; ++i)
				 getline(file,line);
				 
			//
			// Import nodes
			//
						
			// Get number of nodes
			getline(file,line);
			static_cast<stringstream>(line) >> foo >> numNodes;
						
			// Assert
			assert(numNodes < MAX_NUM_NODES);
			
			// Reserve memory
			nodes.reserve(numNodes);
			
			// Insert nodes
			// We take into account also the case of 
			// multiple points on the same line
			UInt id(0);
			array<Real,3> coor; 
			while ((id < numNodes) && getline(file,line))
			{
				stringstream ss(line);
													
				while (!(ss.eof()))
				{
					// Extract coordinates and insert at back
					ss >> coor[0] >> coor[1] >> coor[2];
					nodes.emplace_back(coor,id);
					++id;
					
					// To handle the case of a space before "\n"					
					ss.get();
					ss.peek();
				}
			}
			
			//
			// Import elements
			//
			
			// Get number of elements
			getline(file,line);
			static_cast<stringstream>(line) >> foo >> numElems;
			
			// Assert
			assert(numElems < MAX_NUM_ELEMS);
			
			// Reserve memory
			elems.reserve(numElems);
			
			UInt geoId; 
			array<UInt,NV> vert;
			for (UInt id = 0; id < numElems && getline(file,line); ++id)
			{
				// Extract geometric Id
				stringstream ss(line);
				ss >> geoId;
				
				// Extract vertices Id's
				// They do not need to be made compliant 
				// with a zero-based indexing
				for (auto & v : vert)
					ss >> v;
				
				// Insert at back							
				elems.emplace_back(vert, id, geoId);
			}
			
			// Close the file
			file.close();
		}
		else
			throw runtime_error(filename + " can not be opened.");
	}
	
	
	template<typename SHAPE>
	void bmesh<SHAPE>::read_obj(const string & filename)
	{
		ifstream file(filename);
		if (file.is_open())
		{
			string line;
			
			// 
			// Get nodes
			//
						
			// Get number of nodes
			char foo_c;
			Real foo_r;
			getline(file,line);
			static_cast<stringstream>(line) >> foo_c 
				>> foo_r >> foo_r >> foo_r >> foo_r >> foo_r 
				>> numNodes;
			
			// Assert
			assert(numNodes < MAX_NUM_NODES);
						
			// Reserve memory
			nodes.reserve(numNodes);
						
			// Insert nodes
			UInt Id;
			array<Real,3> coor; 
			for (UInt n = 0; n < numNodes && getline(file,line); ++n)
			{
				// Extract coordinates
				static_cast<stringstream>(line)	>> coor[0] 
												>> coor[1] 
												>> coor[2];
												
				// Insert at back
				nodes.emplace_back(coor,n);
			}
			
			// Skip useless part
			getline(file,line);
			for (UInt n = 0; n < numNodes && getline(file,line); ++n)
				continue;
			getline(file,line);
				
			//
			// Get elements
			//
			
			// Get number of elements and reserve memory
			getline(file,line);
			static_cast<stringstream>(line) >> numElems;
			elems.reserve(numElems);
			
			// Assert
			assert(numElems < MAX_NUM_ELEMS);
			
			// Skip useless part
			getline(file,line);
			getline(file,line);
			
			UInt lines_to_skip(ceil(numElems / 8.));
							
			for (UInt n = 0; n < lines_to_skip && getline(file,line); ++n)
				continue;
				
			getline(file,line);
			
			// Insert elements
			UInt n(0), id(0); 
			array<UInt,NV> vert;
			while (getline(file,line))
			{
				stringstream ss(line);
				for (UInt j = 0; j < 8; ++j, ++n)
				{
					// Get vertex index
					ss >> vert[n % 3];
										
					// Add a new element
					if (n % 3 == 2)
					{
						elems.emplace_back(vert, id, 0);
						++id;
					}
				}
			}
						
			// Close the file
			file.close();
		}
		else
			throw runtime_error(filename + " can not be opened.");
	}
	
	
	//
	// Print
	//
		
	template<typename SHAPE>
	void bmesh<SHAPE>::print(const string & filename)
	{
		// Extract file extension
		auto format = utility::getFileExtension(filename);
				
		// Check if the mesh needs a refresh
		if ((numNodes < nodes.size()) || (numElems < elems.size()))
			refresh();
		
		// Switch the format
		if ((format == "inp") || (format == "txt"))
			print_inp(filename);
		else
			throw runtime_error("Format " + format + " not known.");
	}
	
	
	template<typename SHAPE>
	void bmesh<SHAPE>::print_inp(const string & filename) const
	{
	}
	
	
	// Declare specialization for triangular grids
	template<>
	void bmesh<Triangle>::print_inp(const string & filename) const;
	
	
	// Declare specialization for quadrilateral grids
	template<>
	void bmesh<Quad>::print_inp(const string & filename) const;
	
	//
	// Update Id's
	//
	
	template<typename SHAPE>
	void bmesh<SHAPE>::setUpNodesIds()
	{
		for (UInt i = 0; i < nodes.size(); ++i)
			nodes[i].setId(i);
	}
	

	template<typename SHAPE>
	void bmesh<SHAPE>::setUpElemsIds()
	{
		for (UInt i = 0; i < elems.size(); ++i)
			elems[i].setId(i);
	}
}

#endif

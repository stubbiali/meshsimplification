/*!	\file	bmesh.cpp
	\brief	Specialization of some members of class bmesh. */
	
#include "bmesh.hpp"

namespace geometry
{
	//
	// Print
	//
	
	// Specialization for triangular grids
	template<>
	void bmesh<Triangle>::print_inp(const string & filename) const
	{
		// Note: we suppose the mesh has been refreshed
		// before print method has been called
		
		// Open the file
		ofstream file(filename);
		
		if (file.is_open())
		{			
			// Set number of significant figures
			file.precision(11);
			
			// Preamble
			file << numNodes << " "
				 << numElems << " " 
				 << "0 0 0" << endl;
				 
			// Print nodes
			for (auto node : nodes)
				file << node.getId()+1 	<< " "
					 << node[0] 		<< " "
					 << node[1] 		<< " "
					 << node[2] 		<< endl;
					 
			// Print elements
			for (auto elem : elems)
				file << elem.getId()+1 	<< " "
					 << elem.getGeoId() << "  "
					 << "tri" 			<< "  "
					 << elem[0]+1		<< "  "
					 << elem[1]+1		<< "  "
					 << elem[2]+1		<< endl;
					 
			// Close the file
			file.close();
		}
		else
			throw runtime_error(filename + " can not be opened.");
	}
	
	
	// Specialization for quadrilateral grids
	template<>
	void bmesh<Quad>::print_inp(const string & filename) const
	{
		// Note: we suppose the mesh has been refreshed
		// before print method has been called
		
		// Open the file
		ofstream file(filename);
		
		if (file.is_open())
		{
			// Preamble
			file << numNodes << " "
				 << numElems << " " 
				 << "0 0 0" << endl;
				 
			// Print nodes
			for (auto node : nodes)
				file << node.getId()+1 	<< " "
					 << node[0] 		<< " "
					 << node[1] 		<< " "
					 << node[2] 		<< endl;
					 
			// Print elements
			for (auto elem : elems)
				file << elem.getId()+1 	<< " "
					 << elem.getGeoId() << "  "
					 << "quad" 			<< "  "
					 << elem[0] 		<< "  "
					 << elem[1] 		<< "  "
					 << elem[2] 		<< "  " 
					 << elem[3]			<< endl;
					 
			// Close the file
			file.close();
		}
		else
			throw runtime_error(filename + " can not be opened.");
	}
}

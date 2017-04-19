/*!	\file	meshInfo.cpp
	\brief	Specialization of some members of class meshInfo. */
	
#include "meshInfo.hpp"

namespace geometry
{
	//
	// Print in different format
	//
	
	// Specialization for triangular grids
	template<>
	void meshInfo<Triangle, MeshType::DATA>::print_inp(const string & filename) const
	{
		// Open the file
		ofstream file(filename);
		
		if (file.is_open())
		{			
			// Set number of significant figures
			file.precision(11);
			
			// Preamble
			file << this->connectivity.grid.getNumNodes() << " "
				 << this->connectivity.grid.getNumElems() << " " 
				 << "0 1 0" << endl;
				 
			// Print nodes
			auto nodes = this->connectivity.grid.getNodes();
			for (auto node : nodes)
				file << node.getId()+1 	<< " "
					 << node[0] 		<< " "
					 << node[1] 		<< " "
					 << node[2] 		<< endl;
					 
			// Print elements
			auto elems = this->connectivity.grid.getElems();
			for (auto elem : elems)
				file << elem.getId()+1 	<< " "
					 << 0				<< "  "
					 << "tri" 			<< "  "
					 << elem[0]+1		<< "  "
					 << elem[1]+1		<< "  "
					 << elem[2]+1		<< endl;
					 
			// Print quantity of information for each element
			file << "1 1" << endl;
			file << "QOI, -" << endl;
			for (auto elem : elems)
				file << elem.getId()+1 << " "
					 << getQuantityOfInformation(elem.getId()) << endl;
					 
			// Close the file
			file.close();
		}
		else
			throw runtime_error(filename + " can not be opened.");
	}
	
	
	// Specialization for quadrilateral grids
	template<>
	void meshInfo<Quad, MeshType::DATA>::print_inp(const string & filename) const
	{
		// Open the file
		ofstream file(filename);
		
		if (file.is_open())
		{			
			// Set number of significant figures
			file.precision(11);
			
			// Preamble
			file << this->connectivity.grid.getNumNodes() << " "
				 << this->connectivity.grid.getNumElems() << " " 
				 << "0 0 0" << endl;
				 
			// Print nodes
			auto nodes = this->connectivity.grid.getNodes();
			for (auto node : nodes)
				file << node.getId()+1 	<< " "
					 << node[0] 		<< " "
					 << node[1] 		<< " "
					 << node[2] 		<< endl;
					 
			// Print elements
			auto elems = this->connectivity.grid.getElems();
			for (auto elem : elems)
				file << elem.getId()+1 	<< " "
					 << getQuantityOfInformation(elem.getId()) << "  "
					 << "quad" 			<< "  "
					 << elem[0]+1		<< "  "
					 << elem[1]+1		<< "  "
					 << elem[2]+1		<< "  "
					 << elem[3]+1		<< endl;
					 
			// Close the file
			file.close();
		}
		else
			throw runtime_error(filename + " can not be opened.");
	}
}

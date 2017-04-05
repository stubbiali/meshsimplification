/*!	\file	RcppSimplification.cpp
	\brief	Implementations of methods of class RcppSimplification. */
	
#include <set>
#include "RcppSimplification.hpp"

//
// Constructors
//

RcppSimplification::RcppSimplification(const CharacterVector & file, 
	const double & wgeom, const double & wdisp, const double & wequi) :
	simplifier(as<string>(file), wgeom, wdisp, wequi)
{
}


RcppSimplification::RcppSimplification(const NumericMatrix & nds, 
	const IntegerMatrix & els,
	const double & wgeom, const double & wdisp, const double & wequi) :
	simplifier(as<Map<MatrixXd>>(nds), as<Map<MatrixXi>>(els),
		wgeom, wdisp, wequi)
{
}


RcppSimplification::RcppSimplification(const NumericMatrix & nds, 
	const IntegerMatrix & els, const NumericMatrix & loc,
	const double & wgeom, const double & wdisp, const double & wequi) :
	simplifier(as<Map<MatrixXd>>(nds), as<Map<MatrixXi>>(els),
		as<Map<MatrixXd>>(loc), wgeom, wdisp, wequi)
{
}


RcppSimplification::RcppSimplification(const NumericMatrix & nds, 
	const IntegerMatrix & els, const NumericMatrix & loc, const NumericVector & val,
	const double & wgeom, const double & wdisp, const double & wequi) :
	simplifier(as<Map<MatrixXd>>(nds), as<Map<MatrixXi>>(els),
		as<Map<MatrixXd>>(loc), as<Map<VectorXd>>(val), wgeom, wdisp, wequi)
{
}


//
// Get methods
//

NumericMatrix RcppSimplification::getNodes() const
{
	// Extract number of nodes
	auto numNodes(this->simplifier.getCPointerToMesh()->getNumNodes());
			
	// Fill a NumericMatrix with the nodes
	NumericMatrix out(numNodes,3);
	for (UInt i = 0; i < numNodes; ++i)
	{	
		auto node(this->simplifier.getCPointerToMesh()->getNode(i));
		out(i,0) = node[0];
		out(i,1) = node[1];
		out(i,2) = node[2];
	}
	
	return out;
}


IntegerMatrix RcppSimplification::getEdges() const
{
	// Get list of edges
	auto edges(this->simplifier.getCPointerToConnectivity()->getEdges());
			
	// Fill an IntegerMatrix with the edges
	IntegerMatrix out(edges.size(),2);
	for (UInt i = 0; i < edges.size(); ++i)
	{	
		out(i,0) = edges[i][0]+1;
		out(i,1) = edges[i][1]+1;
	}
	
	return out;
}


IntegerMatrix RcppSimplification::getElems() const
{
	// Extract number of elements as vector<geoElement<Triangle>>
	UInt numElems(this->simplifier.getCPointerToMesh()->getNumElems());
	
	// Fill an IntegerMatrix with the elements
	IntegerMatrix out(numElems,3);
	for (UInt i = 0; i < numElems; ++i)
	{
		auto elem(this->simplifier.getCPointerToMesh()->getElem(i));
		out(i,0) = elem[0]+1;
		out(i,1) = elem[1]+1;
		out(i,2) = elem[2]+1;
	}
	
	return out;
}


NumericMatrix RcppSimplification::getDataLocations() const
{
	// Extract number of data points
	UInt numData(this->simplifier.getCPointerToMesh()->getNumData());
	
	// Fill a NumericMatrix with data locations
	NumericMatrix out(numData,3);
	for (UInt i = 0; i < numData; ++i)
	{
		auto data(this->simplifier.getCPointerToMesh()->getData(i));
		out(i,0) = data[0];
		out(i,1) = data[1];
		out(i,2) = data[2];
	}
	
	return out;
}


NumericVector RcppSimplification::getObservations() const
{
	// Extract number of data points
	UInt numData(this->simplifier.getCPointerToMesh()->getNumData());
	
	// Fill a NumericVector with the observations
	NumericVector out(numData);
	for (UInt i = 0; i < numData; ++i)
		out(i) = double(this->simplifier.getCPointerToMesh()->getData(i).getDatum());
		
	return out;
}


NumericVector RcppSimplification::getQuantityOfInformation() const
{
	// Extract number of elements
	UInt numElems(this->simplifier.getCPointerToMesh()->getNumElems());
	
	// Fill a NumericVector with the quantity of information for each element
	NumericVector out(numElems);
	for (UInt i = 0; i < numElems; ++i)
		out[i] = this->simplifier.getCPointerToMeshOperator()
			->getQuantityOfInformation(i);
			
	return out;
}


IntegerVector RcppSimplification::getElemsOnEdge(const int & id1, const int & id2) const
{
	// Get elements insisting on the edge
	auto elems(this->simplifier.getCPointerToMeshOperator()->getElemsOnEdge(id1-1,id2-1));
		
	return wrap(elems);
}

List RcppSimplification::getMeshQuadraticFEM() const
{
	// TODO
}


//
// Auxiliary functions
//

List getMeshLinearFEM(const NumericMatrix & nodes, const IntegerMatrix & elems)
{
	// Check on dimensions
	if (elems.ncol() != 6)
		throw runtime_error("Number of nodes per triangle must be 6.");
		
	// Extract Id's of the vertices of the mesh
	// These are stored in the first three columns of elems
	set<UInt> vertices;
	for (UInt i = 0; i < elems.nrow(); ++i)
	{
		vertices.insert(elems(i,0));
		vertices.insert(elems(i,1));
		vertices.insert(elems(i,2));
	}
	
	// Build list of vertices
	// In the meanwhile, create map from old to new indices
	NumericMatrix newnodes(vertices.size(),3);
	map<UInt,UInt> old2new;
	UInt row(0);
	for (auto id : vertices)
	{
		newnodes(row,0) = nodes(id-1,0);
		newnodes(row,1) = nodes(id-1,1);
		newnodes(row,2) = nodes(id-1,2);
		old2new[id] = row+1;
		++row;
	}
	
	// Update Id's in the list of elements
	IntegerMatrix newelems(elems.nrow(),3);
	for (UInt i = 0; i < elems.nrow(); ++i)
	{
		newelems(i,0) = old2new[elems(i,0)];
		newelems(i,1) = old2new[elems(i,1)];
		newelems(i,2) = old2new[elems(i,2)];
	}
	
	return List::create(Named("nodes") = newnodes, 
		Named("triangles") = newelems);
}





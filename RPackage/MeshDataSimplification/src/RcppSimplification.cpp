/*!	\file	RcppSimplification.cpp
	\brief	Implementations of methods of class RcppSimplification. */
	
#include "RcppSimplification.hpp"

RcppSimplification::RcppSimplification(const CharacterVector & file, 
	const double & wgeom, const double & wdisp, const double & wequi) :
	simplifier(as<string>(file), wgeom, wdisp, wequi)
{
}


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


IntegerMatrix RcppSimplification::getElems() const
{
	// Extract number of elements as vector<geoElement<Triangle>>
	UInt numElems(this->simplifier.getCPointerToMesh()->getNumElems());
	
	// Fill an IntegerMatrix with the elements
	IntegerMatrix out(numElems,3);
	for (UInt i = 0; i < numElems; ++i)
	{
		auto elem(this->simplifier.getCPointerToMesh()->getElem(i));
		out(i,0) = elem[0];
		out(i,1) = elem[1];
		out(i,2) = elem[2];
	}
	
	return out;
}


NumericMatrix RcppSimplification::getData() const
{
	// Extract number of data points
	UInt numData(this->simplifier.getCPointerToMesh()->getNumData());
	
	// Fill a NumericMatrix with data
	NumericMatrix out(numData,4);
	for (UInt i = 0; i < numData; ++i)
	{
		auto data(this->simplifier.getCPointerToMesh()->getData(i));
		out(i,0) = data[0];
		out(i,1) = data[1];
		out(i,2) = data[2];
		out(i,3) = data.getDatum();
	}
	
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

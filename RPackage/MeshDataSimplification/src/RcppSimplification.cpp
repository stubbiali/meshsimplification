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
		out(i,0) = edges[i][0];
		out(i,1) = edges[i][1];
	}
	
	return out;
}


NumericMatrix RcppSimplification::getEndPoints() const
{
	// Get list of edges
	auto edges(this->simplifier.getCPointerToConnectivity()->getEdges());
	
	// Extract end-points for each edge and store them 
	// in a #edges-by-6 matrix, where the first (last) three
	// columns store the coordinates of the first (second) end-point
	NumericMatrix out(edges.size(),6);
	for (UInt i = 0; i < edges.size(); ++i)
	{
		// First end-point
		auto P(this->simplifier.getCPointerToMesh()->getNode(edges[i][0]));
		out(i,0) = P[0];
		out(i,1) = P[1];
		out(i,2) = P[2];
		
		// Second end-point
		auto Q(this->simplifier.getCPointerToMesh()->getNode(edges[i][1]));
		out(i,3) = Q[0];
		out(i,4) = Q[1];
		out(i,5) = Q[2];
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


NumericMatrix RcppSimplification::getElemsVertices() const
{
	// Fill a (4*#triangles-1)-by-3 matrix where:
	// - the (4*i-3)-th row stores the coordinates of the 
	//   first vertex of the i-th triangle
	// - the (4*i-2)-th row stores the coordinates of the 
	//   second vertex of the i-th triangle
	// - the (4*i-1)-th row stores the coordinates of the 
	//   third vertex of the i-th triangle
	// - the 4*i-th row stores NA's if i < #triangles
	auto nt(this->simplifier.getCPointerToMesh()->getNumElems());
	NumericMatrix out(4*nt-1,3);
	for (UInt i = 0; i < nt; ++i)
	{
		// Extract triangle
		auto t(this->simplifier.getCPointerToMesh()->getElem(i));
		
		// Extract first vertex
		auto V1(this->simplifier.getCPointerToMesh()->getNode(t[0]));
		out(4*i,0) = V1[0];
		out(4*i,1) = V1[1];
		out(4*i,2) = V1[2];
		
		// Extract second vertex
		auto V2(this->simplifier.getCPointerToMesh()->getNode(t[1]));
		out(4*i+1,0) = V2[0];
		out(4*i+1,1) = V2[1];
		out(4*i+1,2) = V2[2];
		
		// Extract third vertex
		auto V3(this->simplifier.getCPointerToMesh()->getNode(t[2]));
		out(4*i+2,0) = V3[0];
		out(4*i+2,1) = V3[1];
		out(4*i+2,2) = V3[2];
		
		if (i < nt-1)
		{
			// Set last row of the chunk to NA's
			out(4*i+3,0) = NA_REAL;
			out(4*i+3,1) = NA_REAL;
			out(4*i+3,2) = NA_REAL;
		}
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
	auto elems(this->simplifier.getCPointerToMeshOperator()->getElemsOnEdge(id1,id2));
		
	return wrap(elems);
}


List RcppSimplification::getQuadraticFEMesh() const
{
	// Allocate memory for new lists of nodes and triangles
	auto numVerts(simplifier.getCPointerToMesh()->getNumNodes());
	auto numElems(simplifier.getCPointerToMesh()->getNumElems());
	auto numEdges(simplifier.getCPointerToConnectivity()->getNumEdges());
	NumericMatrix newnodes(numVerts+numEdges,3);
	IntegerMatrix newelems(numElems,6);
	
	// Insert vertices in the list of nodes
	for (UInt i = 0; i < numVerts; ++i)
	{
		auto node(simplifier.getCPointerToMesh()->getNode(i));
		newnodes(i,0) = node[0];
		newnodes(i,1) = node[1];
		newnodes(i,2) = node[2];
	}
	
	// Insert vertices in the list of elements
	for (UInt i = 0; i < numElems; ++i)
	{
		auto elem(simplifier.getCPointerToMesh()->getElem(i));
		newelems(i,0) = elem[0];
		newelems(i,1) = elem[1];
		newelems(i,2) = elem[2];
	}
	
	// Go through all edges
	auto edges(simplifier.getCPointerToConnectivity()->getEdges());
	for (UInt i = 0; i < numEdges; ++i)
	{
		// Shortcuts
		UInt id1(edges[i][0]), id2(edges[i][1]);
		UInt row(numVerts+i-1);
		
		// Add mid-point to the list of nodes
		auto P(simplifier.getCPointerToMesh()->getNode(id1));
		auto Q(simplifier.getCPointerToMesh()->getNode(id2));
		newnodes(row,0) = 0.5*(P[0]+Q[0]);
		newnodes(row,1) = 0.5*(P[1]+Q[1]);
		newnodes(row,2) = 0.5*(P[2]+Q[2]);
		
		// Extract triangles sharing the edge
		auto trs(simplifier.getCPointerToMeshOperator()->getElemsOnEdge(id1,id2));
		
		// Associated the new node to each triangle
		// Vertices and nodes are ordered as described at 
		// https://www.cs.cmu.edu/~quake/triangle.highorder.html
		for (auto id : trs)
		{
			auto tr(simplifier.getCPointerToMesh()->getElem(id));
			if ((tr[1] == id1 && tr[2] == id2) || (tr[1] == id2 && tr[2] == id1))
				newelems(id,3) = row;
			if ((tr[2] == id1 && tr[0] == id2) || (tr[2] == id2 && tr[0] == id1))
				newelems(id,4) = row;
			if ((tr[0] == id1 && tr[1] == id2) || (tr[0] == id2 && tr[1] == id1))
				newelems(id,5) = row;
		}
	}
	
	return List::create(Named("nodes") = newnodes, 
		Named("triangles") = newelems);
}


//
// Auxiliary functions
//

List getLinearFEMesh(const NumericMatrix & nodes, const IntegerMatrix & elems)
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
		newnodes(row,0) = nodes(id,0);
		newnodes(row,1) = nodes(id,1);
		newnodes(row,2) = nodes(id,2);
		old2new[id] = row;
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





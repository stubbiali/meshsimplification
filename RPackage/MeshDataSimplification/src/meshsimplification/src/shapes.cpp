/*!	\file	shapes.cpp
	\brief	Definitions of static members of shapes-defining classes. */

#include "shapes.hpp"

namespace geometry 
{
	//
	// simplePoint
	//
	
	const ReferenceShapes simplePoint::Shape = ReferenceShapes::POINT; 
	const ReferenceGeometry simplePoint::Geometry = ReferenceGeometry::VERTEX;
	
	
	//
	// Line
	//
	
	const ReferenceShapes Line::Shape = ReferenceShapes::LINE; 
	const ReferenceGeometry Line::Geometry = ReferenceGeometry::EDGE;
	const array<UInt, Line::numEdges*Line::numVertices> Line::edgeConn = {{0, 1}};
	
	
	//
	// Triangle
	//
	
	const ReferenceShapes Triangle::Shape = ReferenceShapes::TRIANGLE; 
	const ReferenceGeometry Triangle::Geometry = ReferenceGeometry::FACE;
	const array<UInt, Triangle::numEdges*Triangle::numVerticesPerEdge> Triangle::edgeConn = {{	0, 1, 
											 											  		1, 2, 
											 											  		2, 0}};
											 
	const array<UInt, Triangle::numFaces*Triangle::numVerticesPerFace> Triangle::faceConn = {{0, 1, 2}};

	
	//
	// Quad
	//
	
	const ReferenceShapes Quad::Shape = ReferenceShapes::QUAD; 
	const ReferenceGeometry Quad::Geometry = ReferenceGeometry::FACE;
	const array<UInt, Quad::numEdges*Quad::numVerticesPerEdge> Quad::edgeConn = {{	0, 1, 
																				 	1, 2, 
											 									 	2, 3,
											 										3, 0}};
											 
	const array<UInt, Quad::numFaces*Quad::numVerticesPerFace> Quad::faceConn = {{0, 1, 2, 3}};


	//
	// Tetra
	//
	
	const ReferenceShapes Tetra::Shape = ReferenceShapes::TETRA; 
	const ReferenceGeometry Tetra::Geometry = ReferenceGeometry::VOLUME;
	const array<UInt, Tetra::numEdges*Tetra::numVerticesPerEdge> Tetra::edgeConn = {{	0, 1, 
											 											1, 2, 
											 											2, 0,
											 											0, 3,
											 											1, 3,
											 											2, 3}};
											 
	const array<UInt, Tetra::numFaces*Tetra::numVerticesPerFace> Tetra::faceConn = {{	0, 1, 2, 
																						3, 1, 2,
																						0, 3, 2,
																						0, 1, 3}};
	
	
	//
	// Hexa
	//
	
	const ReferenceShapes Hexa::Shape = ReferenceShapes::HEXA; 
	const ReferenceGeometry Hexa::Geometry = ReferenceGeometry::VOLUME;
	const array<UInt, Hexa::numEdges*Hexa::numVerticesPerEdge> Hexa::edgeConn = {{	0, 1,
									 												1, 2, 
																					2, 3, 
																					3, 0, 
																					0, 4, 
																					1, 5, 
																					2, 6, 
																					3, 7, 
																					4, 5, 
																					5, 6, 
																					6, 7,
																					7, 4}};

	const array<UInt, Hexa::numFaces*Hexa::numVerticesPerFace> Hexa::faceConn = {{	0, 1, 2, 3,
																					0, 1, 5, 4,
																					0, 3, 7, 4,
																					1, 2, 6, 5,
																					2, 3, 7, 6,
																					4, 5, 6, 7}};
									 
}	
								 

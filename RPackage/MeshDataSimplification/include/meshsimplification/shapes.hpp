/*! \file 	shapes.hpp
	\brief 	Set of structs describing different geometric shapes used throughout the library. */

#ifndef HH_SHAPES_HH
#define HH_SHAPES_HH

#include <array>

#include "inc.hpp"

namespace geometry 
{
	//
	// Enumerations
	//

	/*! Enumerations listing all geometric shapes used in the library. */
	enum class ReferenceShapes {POINT, LINE, TRIANGLE, QUAD, HEXA, PRISM, TETRA};
	enum class ReferenceGeometry {VERTEX, EDGE, FACE, VOLUME};

	/*! Intersection types. */
	enum class intersectionType {NOTINTERSEC, INTERSEC, CONSECUTIVE, CONTENT, COINCIDE};

	/*! Relative positions for a point with respect to a generic geometric shape. */
	enum class positionType {OUTSIDE, INSIDE, ONBOUNDARY};
	
	// 
	// Structs
	//

	/*! A point. */
	struct simplePoint 
	{
		using BShape = simplePoint;
		static const ReferenceShapes Shape; 
		static const ReferenceGeometry Geometry;
		static constexpr UInt dim = 0;   
		static constexpr UInt numVertices = 1;
		static constexpr UInt numFaces = 0;
		static constexpr UInt numEdges = 0;	 
	};

	/*! A line. */
	struct Line 
	{
		using BShape = simplePoint;
		static const ReferenceShapes Shape; 
		static const ReferenceGeometry Geometry;
		static constexpr UInt dim = 1;
		static constexpr UInt numFaces = 0;
		static constexpr UInt numEdges = 1;
		static constexpr UInt numVertices = 2;
		static constexpr UInt numVerticesPerEdge = 2;
		static const array<UInt, numEdges*numVerticesPerEdge> edgeConn;
	};

	/*! A triangle. */
	struct Triangle 
	{
		using BShape = Line;
		static const ReferenceShapes Shape; 
		static const ReferenceGeometry Geometry;
		static constexpr UInt dim = 2;
		static constexpr UInt numVertices = 3;
		static constexpr UInt numFaces = 1;
		static constexpr UInt numEdges = numVertices;
		static constexpr UInt numVerticesPerEdge = 2;
		static constexpr UInt numVerticesPerFace = 3;
		static const array<UInt, numEdges*numVerticesPerEdge> edgeConn;
		static const array<UInt, numFaces*numVerticesPerFace> faceConn;
	};

	/*! A quadrilateral. */
	struct Quad 
	{
		using BShape = Line;
		static const ReferenceShapes Shape; 
		static const ReferenceGeometry Geometry;
		static constexpr UInt dim = 2;
		static constexpr UInt numVertices = 4;
		static constexpr UInt numFaces = 1;
		static constexpr UInt numEdges = numVertices;
		static constexpr UInt numVerticesPerEdge = 2;
		static constexpr UInt numVerticesPerFace = 4;
		static const array<UInt, numEdges*numVerticesPerEdge> edgeConn;
		static const array<UInt, numFaces*numVerticesPerFace> faceConn;
	};

	/*! A tetrahedron. */
	struct Tetra 
	{
		using BShape = Triangle;
		static const ReferenceShapes Shape; 
		static const ReferenceGeometry Geometry;
		static constexpr UInt dim = 3;
		static constexpr UInt numVertices = 4;
		static constexpr UInt numFaces = 4;
		static constexpr UInt numEdges = numFaces + numVertices - 2;
		static constexpr UInt numVerticesPerEdge = 2;
		static constexpr UInt numVerticesPerFace = 3;
		static constexpr UInt numVerticesPerVolume = 4;
		static const array<UInt, numEdges*numVerticesPerEdge> edgeConn;
		static const array<UInt, numFaces*numVerticesPerFace> faceConn;
	};

	/*! A hexahedron. */
	struct Hexa 
	{
		using BShape = Quad;
		static const ReferenceShapes Shape; 
		static const ReferenceGeometry Geometry;
		static constexpr UInt dim = 3;
		static constexpr UInt numFaces = 6;
		static constexpr UInt numVertices = 8;
		static constexpr UInt numEdges = numFaces + numVertices - 2;
		static constexpr UInt numVerticesPerEdge = 2;
		static constexpr UInt numVerticesPerFace = 4;
		static constexpr UInt numVerticesPerVolume = 8;
		static const array<UInt, numEdges*numVerticesPerEdge> edgeConn;
		static const array<UInt, numFaces*numVerticesPerFace> faceConn;
	};
}

#endif 




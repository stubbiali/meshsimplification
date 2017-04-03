/*! \file	instantiation.cpp
	\brief	Explicitly generate the code for all members of template classes.
			This is useful for debugging. */

/*
#include "inc.hpp"
using namespace geometry;

#include "geoElement.hpp"
template class geoElement<Triangle>;

#include "bmesh.hpp"
template class bmesh<Triangle>;
template class bmesh<Quad>;

#include "mesh.hpp"
template class mesh<Triangle>;
template class mesh<Triangle, MeshType::DATA>;
template class mesh<Quad>;
template class mesh<Quad, MeshType::DATA>;

#include "bconnect.hpp"
template class bconnect<Triangle, MeshType::GEO>;
template class bconnect<Triangle, MeshType::DATA>;
template class bconnect<Quad, MeshType::GEO>;
template class bconnect<Quad, MeshType::DATA>;

#include "connect.hpp"
template class connect<Triangle>;
template class connect<Triangle, MeshType::DATA>;
template class connect<Quad>;
template class connect<Quad, MeshType::DATA>;

#include "bmeshInfo.hpp"
#ifndef NDEBUG
template class bmeshInfo<Triangle, MeshType::GEO>;
template class bmeshInfo<Triangle, MeshType::DATA>;
template class bmeshInfo<Quad, MeshType::GEO>;
template class bmeshInfo<Quad, MeshType::DATA>;
#endif

#include "meshInfo.hpp"
#ifndef NDEBUG
template class meshInfo<Triangle>;
template class meshInfo<Triangle, MeshType::DATA>;
template class meshInfo<Quad>;
template class meshInfo<Quad, MeshType::DATA>;
#endif

#include "boundingBox.hpp"
#ifndef NDEBUG
template class boundingBox<2>;
template class boundingBox<3>;
template class boundingBox<10>;
#endif

#include "structuredData.hpp"
template class structuredData<Triangle>;
template class structuredData<Quad>;

#include "OnlyGeo.hpp"
template class bcost<Triangle, MeshType::GEO, OnlyGeo<MeshType::GEO>>;
template class bcost<Triangle, MeshType::DATA, OnlyGeo<MeshType::DATA>>;
template class OnlyGeo<MeshType::GEO>;
template class OnlyGeo<MeshType::DATA>;

#include "simplification.hpp"
template class simplification<Triangle, MeshType::GEO, OnlyGeo<MeshType::GEO>>;
*/


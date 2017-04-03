/*!	\file	bcost.cpp
	\brief	Specialization of some members of class bcost. */
	
#include "bcost.hpp"

//
// Include implementations of inlined members.
// Note that the derived classes need to be included too
// to avoid issues at linking phase.
// Indeed, as required by the CRTP technique, bcost is templated
// in its derived class. Then, if one needs to specialize any
// member of bcost based on the derived class, the derived class
// declaration must be visible, i.e. it has to precede the 
// implementations of bcost members. This is accomplished by including
// decl_OnlyGeo.hpp and decl_DataGeo.hpp in bcost.hpp.
// However, when inlining is disable, i.e. when this source file is not empty,
// implementations of template derived classes must be available to avoid
// undefined reference issues at linking stage. This is why OnlyGeo.hpp
// and DataGeo.hpp are included. Note that the latter is not strictly
// necessary since DataGeo is a non-template class.

#ifndef INLINED
#include "OnlyGeo.hpp"
#include "DataGeo.hpp"
#include "inline/inline_bcost.hpp"
#endif


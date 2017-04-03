/*!	\file	bcost.hpp
	\brief	Base class for classes computing the cost associated
			with an edge collapsing. CRTP paradigm is used. */
	
#ifndef HH_BCOST_HH
#define HH_BCOST_HH

/*!	Include declarations of class members. */
#include "declaration/decl_bcost.hpp"

/*!	Include declarations of derived class OnlyGeo and DataGeo.

	Indeed, as required by the CRTP technique, bcost is templated
	in its derived class. Then, if one needs to specialize any
	member of bcost based on the derived class, the derived class
	declaration must be visible, i.e. it has to precede the 
	implementations of bcost members. */
#include "declaration/decl_OnlyGeo.hpp"
#include "declaration/decl_DataGeo.hpp"

/*!	Include implementations of inlined class members. */
#ifdef INLINED
#include "inline/inline_bcost.hpp"
#endif

/*!	Include implementations of non-inlined class members. */
#include "implementation/imp_bcost.hpp"

#endif

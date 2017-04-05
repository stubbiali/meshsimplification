/*!	\file	inline_RcppSimplification.hpp
	\brief	Implementations of inline methods for class RcppSimplification. */
	
#ifndef HH_INLINERCPPSIMPLIFICATION_HH
#define HH_INLINERCPPSIMPLIFICATION_HH

//
// Get methods
//

inline int RcppSimplification::getNumNodes() const
{
	return simplifier.getCPointerToMesh()->getNumNodes();
}


inline int RcppSimplification::getNumElems() const
{
	return simplifier.getCPointerToMesh()->getNumElems();
}


inline int RcppSimplification::getNumData() const
{
	return simplifier.getCPointerToMesh()->getNumData();
}


//
// Run the simplification
//

inline void RcppSimplification::simplificate(const int & numNodesMax, 
	const CharacterVector & file)
{
	simplifier.simplificate(numNodesMax, true, as<string>(file));
}

#endif

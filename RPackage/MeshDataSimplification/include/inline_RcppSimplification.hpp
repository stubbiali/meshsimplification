/*!	\file	inline_RcppSimplification.hpp
	\brief	Implementations of inline methods for class RcppSimplification. */
	
#ifndef HH_INLINERCPPSIMPLIFICATION_HH
#define HH_INLINERCPPSIMPLIFICATION_HH

inline void RcppSimplification::simplificate(const int & numNodesMax, 
	const CharacterVector & file)
{
	simplifier.simplificate(numNodesMax, true, as<string>(file));
}

#endif

/*!	\file	main_simplification.cpp
	\brief	Executable running the iterative mesh simplification process. Settings are hard-coded. */
	
#include <chrono>
	
#include "simplification.hpp"

int main()
{
	using namespace geometry;
	using namespace std::chrono;
	
	// Path to input
	string iFile("mesh/pawn.inp");
	string oFile("results/pawn_test.inp");
	
	// Desired number of nodes
	UInt numNodesMax(1400);
	
	// Simplificate!
	#ifdef NDEBUG
	high_resolution_clock::time_point start = high_resolution_clock::now();
	#endif
	
	simplification<Triangle, MeshType::DATA, DataGeo> simplifier(iFile);
	simplifier.simplificate(numNodesMax, true, oFile);
		
	#ifdef NDEBUG
	high_resolution_clock::time_point stop = high_resolution_clock::now();
	auto dif = duration_cast<milliseconds>(stop-start).count();
	cout << "Total elapsed time: " << dif << " ms" << endl;
	#endif
}

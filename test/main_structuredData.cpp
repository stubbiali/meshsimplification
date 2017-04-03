/*!	\file	main_structuredData.cpp
	\brief	Small executable testing class structuredData. */
	
#include <chrono>

#include "structuredData.hpp"
#include "meshInfo.hpp"

int main()
{
	using namespace geometry;
	using namespace std::chrono;
	
	#ifdef NDEBUG
	high_resolution_clock::time_point start, stop; 
	start = high_resolution_clock::now();
	#endif
	
	// Import mesh and create connections
	string inputfile("mesh/brain.inp");
	meshInfo<Triangle, MeshType::DATA> news(inputfile);
		
	// Create structured data
	structuredData<Triangle> sd(news);
	
	// Extract intersecting bounding boxes
	{
	auto ids = sd.getNeighbouringElements(35219);
	for (auto id : ids)
			cout << id << " ";
	cout << endl;
	}
	
	{
	auto ids = sd.getNeighbouringElements(35506);
	for (auto id : ids)
			cout << id << " ";
	cout << endl;
	}
	
	#ifdef NDEBUG
	stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop-start).count();
	cout << "Elapsed time: " << duration << " ms" << endl << endl;
	#endif
}

/*!	\file	connect.cpp
	\brief	Specialization for some members of class connect. */
	
#ifdef NDEBUG
#include <chrono>
#endif
	
#include "connect.hpp"
#include "structuredData.hpp"
#include "gutility.hpp"

namespace geometry
{
	//
	// Constructor
	//
	
	// Specialization for triangular grids
	template<>
	connect<Triangle, MeshType::DATA>::connect(const MatrixXd & nds, const MatrixXi & els, 
		const MatrixXd & loc, const VectorXd & val) :
		bconnect<Triangle, MeshType::DATA>(nds, els, loc, val)
	{
		// Build data-element and element-data connections
		buildData2Elem_p();
		buildElem2Data_p();
	}
	
	
	//
	// Initialize and clear connections
	//
	
	// Specialization for triangular grids
	template<>
	void connect<Triangle, MeshType::DATA>::buildData2Elem_p()
	{		
		#ifdef NDEBUG
			using namespace std::chrono;
			high_resolution_clock::time_point start = high_resolution_clock::now();
		#endif
		
		// Clear memory
		data2elem.clear();
		
		// Allocate memory for data-element connections
		data2elem.reserve(this->grid.getNumData());
		
		// Initialize class for structured data search
		structuredData<Triangle> sd(&this->grid);
		
		#ifdef NDEBUG
			UInt barWidth(40);
		#endif
						
		// Go through all data points
		for (UInt i = 0; i < this->grid.getNumData(); ++i)
		{
			// Variables to keep track of the closest triangle,
			// i.e. the one minimizing the distance between the
			// point and its barycenter.
			// This is to take into account also the case where
			// the point can not be associated with any triangle,
			// either due to faults in input mesh or errors during
			// computations
			Real d_opt(numeric_limits<Real>::max());
			UInt id_opt(this->grid.getNumNodes());
			point3d M_opt;
			bool toassociate(true);
			
			// Get triangles the data point may belong to
			auto ids( sd.getNeighbouringElements(this->grid.getData(i)) );
			
			// Test belonging for each triangle; when done, exit
			for (UInt j = 0; j < ids.size() && toassociate; ++j)
			{
				auto el(this->grid.getElem(ids[j]));
				auto res( gutility::inTri3d_v(this->grid.getData(i),
					this->grid.getNode(el[0]), this->grid.getNode(el[1]),
					this->grid.getNode(el[2])) );
					
				// Test if the points belong to the triangle, in case set 
				// data-element connections accordingly
				if (get<0>(res) > 0)
				{
					// The point is strictly inside the triangle
					if (get<0>(res) == 1)
						data2elem.emplace_back(vector<UInt>({ids[j]}), i);
											
					// The point lies onto the first edge
					if (get<0>(res) == 2)
						data2elem.emplace_back(
							set_intersection(this->node2elem[el[0]], this->node2elem[el[1]]), i);
							
					// The point lies onto the second edge
					if (get<0>(res) == 3)
						data2elem.emplace_back(
							set_intersection(this->node2elem[el[1]], this->node2elem[el[2]]), i);
							
					// The point lies onto the third edge
					if (get<0>(res) == 4)
						data2elem.emplace_back(
							set_intersection(this->node2elem[el[2]], this->node2elem[el[0]]), i);
							
					// The point coincides with the first vertex
					if (get<0>(res) == 5)
						data2elem.emplace_back(this->node2elem[el[0]].getConnected(), i);
						
					// The point coincides with the second vertex
					if (get<0>(res) == 6)
						data2elem.emplace_back(this->node2elem[el[1]].getConnected(), i);
						
					// The point coincides with the third vertex
					if (get<0>(res) == 7)
						data2elem.emplace_back(this->node2elem[el[2]].getConnected(), i);
																
					toassociate = false;
				}
				else 
				{
					// If the point falls outside the triangle, check if it is
					// the closest so far
					if (get<1>(res) < d_opt)
					{
						d_opt = get<1>(res);
						id_opt = ids[j];
						M_opt = get<2>(res);
					}
				}
			}
			
			// If the point has not been associated yet, move the point to the 
			// barycenter of the closest triangle and set connections accordingly		
			if (data2elem.size() == i)
			{
				this->grid.setData(i, M_opt);
				data2elem.emplace_back(vector<UInt>(id_opt), i);	
			}
			
			// Update progress bar
			#ifdef NDEBUG
				Real progress((i+1) / static_cast<Real>(this->grid.getNumData()));
				cout << "Initialize data-element connections  [";
				UInt pos(barWidth * progress);
				for (UInt i = 0; i < barWidth; ++i) 
				{
					if (i < pos) 
						cout << "=";
					else if (i == pos) 
						cout << ">";
					else 
						cout << " ";
				}
				cout << "] " << UInt(progress * 100.0) << " %\r";
				cout.flush();
			#endif
		}
		#ifdef NDEBUG
			cout << endl;
			high_resolution_clock::time_point stop = high_resolution_clock::now();
			auto dif = duration_cast<milliseconds>(stop-start).count();
			cout << "Initialization of data-element connections completed in " << dif/1000 << " seconds." << endl;
		#endif
	}
}

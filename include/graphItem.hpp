/*! \file 	graphItem.hpp
	\brief 	A class representing the element of a generic graph. */
	
#ifndef HH_GRAPHITEM_HH
#define HH_GRAPHITEM_HH

#include <iostream>
#include <set>
#include <utility>
#include <vector>

#include "inc.hpp"

namespace geometry
{
	/*! This class represents an element of a generic graph.
		The element is characterized by an Id and the Id's of the connected
		elements are stored in a STL set. In this way, the order in which
		the connected elements are inserted does not matter. 
		Connected Id's are ascendig-ordered.  
		Every operation is carefully performed not to give rise to 
		duplicated in connected Id's set.
		
		This class is useful for representing items whose connected elements
		may change throughout the code but whose order does not matter. */
	class graphItem
	{
		private:
			/*! Element Id. */
			UInt Id;
		
			/*! Id's of connected elements. */
			set<UInt> conn;
		
			/*! Flag
				<ol>
				<li> TRUE if this is an active node of the graph
				<li> FALSE if this is not an active node of the graph
				<\ol> */
			bool active;
						
		public:						
			//
			// Constructors
			//
									
			/*! (Default) constructor.
				\param ID	element Id */
			graphItem(const UInt & ID = 0);
			
			/*! Constructor.
				\param ID	element Id 
				\param c	ID's of connected elements */
			graphItem(const vector<UInt> & c, const UInt & ID = 0);
			
			/*! Constructor.
				\param ID	element Id 
				\param s	ID's of connected elements */
			graphItem(const set<UInt> & c, const UInt & ID = 0);
			
			/*! Synthetic copy constructor.
				\param g	another graph item */
			graphItem(const graphItem & g) = default;
						
			//
			// Operators
			//
			
			/*! Copy assignment operator.
				\param g	another graph item
				\return		updated object */
			graphItem & operator=(const graphItem & g);
						
			/*! Less than operator. 
				\param g1	the first graph item
				\param g2	the second graph item
				\return		bool saying whether g1 is "less than" g2 or not */
			friend bool operator<(const graphItem & g1, const graphItem & g2);
			
			/*! The inequality operator. 
				\param g1	the first graph item
				\param g2	the second graph item
				\return		bool saying whether g1 is different from g2 or not */
			friend bool operator!=(const graphItem & g1, const graphItem & g2);
		
			/*! The equality operator. 
				\param g1	the first graph item
				\param g2	the second graph item
				\return		bool saying whether g1 is equal to g2 or not */
			friend bool operator==(const graphItem & g1, const graphItem & g2);
			
			/*! Output stream operator.
				\param out	output stream
				\param g	a graph item
				\return		updated output stream */
			friend ostream & operator<<(ostream & out, const graphItem & g);
			
			//
			// Get methods
			//
			
			/*! Get element Id.
				\return		element Id */
			UInt getId() const;
			
			/*! Get number of connected elements.
				\return 	number of connected elements */
			UInt size() const;
			
			/*! Get a set with the Id's of connected elements.
				\return 	Id's of connected elements */
			vector<UInt> getConnected() const;
						
			/*! Get active flag.
				\return		active flag */
			bool isActive() const;
			
			//
			// Set methods
			//
			
			/*! Set element Id.
				\param ID	the new element Id */
			void setId(const UInt & ID);
						
			/*! Set all connected Id's.
				\param v	vector with connected Id's */
			void setConnected(const vector<UInt> & v);
			
			/*! Set active flag to true. */
			void setActive();
			
			/*! Set active flag to false. */
			void setInactive();
			
			//
			// Find, insert, replace and erase methods
			//
			
			/*! Find a connected Id.
				\param val	Id to search
				\return		TRUE if the Id has been found, FALSE otherwise */
			bool find(const UInt & val) const;
						
			/*! Find a connected Id.
				\param val	Id to search
				\return		pair where the first element is a bool saying
							whether the item has been found or not,
							while the second element is an iterator to the element */
			pair<set<UInt>::iterator,bool> find(const UInt & val);
						
			/*! Insert a new Id to the connected elements.
				\param val	value to insert */
			void insert(const UInt & val);
			
			/*! Insert a set of new Id's to the connected elements.
				\param s	set to insert */
			void insert(const set<UInt> & s);
			
			/*! Insert a vector of new Id's to the connected elements.
				\param v	vector to insert */
			void insert(const vector<UInt> & v);
			
			/*! Replace a connected.
				\param oldId	the old Id
				\param newId	the new Id
				
				Note: 	if oldId is not present in the list of connected Id's, 
						the method tries to insert newId anyway. */
			void replace(const UInt & oldId, const UInt & newId);
									
			/*! Erase a connected element.
				\param val	Id to erase 
				\return		number of removed elements */
			UInt erase(const UInt & val);
			
			/*! Erase a connected element.
				\param it	iterator to the element to erase */
			void erase(set<UInt>::iterator it);
			
			/*! Clear the set with connected Id's. */
			void clear();
			
			//
			// Common and uncommon connected
			//
			
			/*! Find the connected Id's shared by two graph items.
				\param g1	first graph item
				\param g2	second graph item
				\return		set of common Id's */
			friend set<UInt> set_intersection(const graphItem & g1, const graphItem & g2);
			
			/*! Find the connected Id's shared by an arbitrary number of items.
				\param g	first graph item
				\param args	all other graph items
				\return 	set of common Id's */
			template<typename... Args>
			friend set<UInt> set_intersection(const graphItem & g, Args... args);
			
			/*! Find the connected Id's shared by two graph items and store them in a vector.
				\param g1	first graph item
				\param g2	second graph item 
				\param v	vector to fill */
			friend void set_intersection(const graphItem & g1, const graphItem & g2, vector<UInt> & v);
			
			/*! Find the connected Id's shared by at least one of two graph items.
				\param g1	first graph item
				\param g2	second graph item
				\return		set of common and uncommon Id's */
			friend set<UInt> set_union(const graphItem & g1, const graphItem & g2);
			
			/*! Find the connected Id's shared by at least one of an arbitrary number of items.
				\param g	first graph item
				\param args	all other graph items
				\return 	set of common and uncommon Id's */
			template<typename... Args>
			friend set<UInt> set_union(const graphItem & g, Args... args);
			
			/*! Extend a set of Id's by adding the connected Id's of a graphItem.
				\param g	the graph item
				\param s	the set */
			friend void set_union(const graphItem & g, set<UInt> & s);
			
			/*! Find the connected Id's shared by at least one of two graph items and store them in a vector.
				\param g1	first graph item
				\param g2	second graph item 
				\param v	vector to fill */
			friend void set_union(const graphItem & g1, const graphItem & g2, vector<UInt> & v);
			
			/*! Find the Id's connected to the first item but not to the second one.
				\param g1	first graph item
				\param g2	second graph item
				\return		set of desired Id's */
			friend set<UInt> set_difference(const graphItem & g1, const graphItem & g2);
			
			/*! Find the Id's connected to the first item but not to the second one and store them in a vector.
				\param g1	first graph item
				\param g2	second graph item 
				\param v	vector to fill */
			friend void set_difference(const graphItem & g1, const graphItem & g2, vector<UInt> & v);
			
			/*! Find the connected Id's not shared by two undirected graph items.
				\param g1	first graph item
				\param g2	second graph item
				\return		set of uncommon Id's */
			friend set<UInt> set_symmetric_difference(const graphItem & g1, const graphItem & g2);
			
			/*! Find the connected Id's not shared by two undirected graph items and store them in a vector.
				\param g1	first graph item
				\param g2	second graph item 
				\param v	vector to fill */
			friend void set_symmetric_difference(const graphItem & g1, const graphItem & g2, vector<UInt> & v);			
	};
}

/*! Include definitions of inlined members and friend functions. */
#ifdef INLINED
#include "inline/inline_graphItem.hpp"
#endif

/*! Include definitions of template friend functions. */
#include "implementation/imp_graphItem.hpp"

#endif

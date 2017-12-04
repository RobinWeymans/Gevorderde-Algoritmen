#pragma once

#include <memory>
using std::unique_ptr;

namespace prioriteitswachtrijen {

	template<class T, class D>
	class PairingKnoop;

	template<class T, class D>
	class PairingHeap : public unique_ptr<PairingKnoop<T, D>> {
		using unique_ptr<PairingKnoop<T, D>>::unique_ptr;
	public:
		PairingHeap() {}
		PairingHeap(PairingHeap<T, D>&& p) : unique_ptr<PairingKnoop<T, D>>(std::move(p)) {}
		PairingHeap(PairingKnoop<T, D>* k) : unique_ptr<PairingKnoop<T, D>>(k) {}
		PairingHeap& operator=(PairingHeap<T, D>&& p) { unique_ptr<PairingKnoop<T, D>>::operator=(std::move(p)); return *this; }

		void put(const T&, const D&);
		void merge(PairingHeap<T, D>& b);
		D& min() { return (*this)->data; }
	private:


	};

	template<class T, class D>
	class PairingKnoop {
		friend class PairingHeap<T, D>;
	public:
		PairingKnoop(const T& sl, const D& d) : sleutel(sl), data(d){}
	private:
		T sleutel;
		D data;

		PairingHeap<T, D> links, broer;
	};

	template<class T, class D> 
	void PairingHeap<T, D>::put(const T& sl, const D& d) {
		merge(std::move(PairingHeap<T, D>(new PairingKnoop<T, D>(sl, d))));
	}

	template<class T, class D>
	void PairingHeap<T, D>::merge(PairingHeap<T, D>& b) {
		if (*this == nullptr) swap(b);
		if (b == nullptr) return;

		if ((*this)->sleutel > b->sleutel) swap(b); // Als de wortel van b kleiner is als onze heap, dan kan je de twee omwisselen
		b->broer = std::move((*this)->links); // Broer van b laten wijzen naar de linkse deelboom van this
		(*this)->links = std::move(b); // Linkse deelboom van this laten wijzen naar b

	}

}


/* Voorbeelden (Zoals ik het hierboven heb geprogrammeerd. Geen idee of da ook echt klopt.)

	1.
	insert(1)		insert(2)
		
		1				1
					   /
				      2

	2.
	insert(3)		insert(4)
		
		3				3
					   /
					  4
	
	3.
		1				3				1
	   /		+	   /		=	   /
	  2				  4				  3 - 2
									 /
									4

	4.
			1							1
		   /				5		   /
		  3 - 2		+	   /	=	  5 - 3 - 2
		 /				  6			 /	 /
		4							6	4
													5	  3
	Als we nu uit 1 gaan verwijderen, dan gaan we  / en  /  mergen. Vervolgens wordt dit resultaat gemerged met 2.
												  6		4
	

					3		|      3     				2
	  3     5      /		|     /					   /
	 /  +  /  =   5 - 4		|    5 - 4  +   2  =	  3
	4     6		 /			|   /					 /
				6			|  6					5 - 4
												   /
												  6
*/
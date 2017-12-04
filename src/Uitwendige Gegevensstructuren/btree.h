#pragma once

#include "../Util/schijf.h"
using util::Schijf;
using util::blokindex;


#include <algorithm>
#include <iostream>
#include <vector>
#include <map>
#include <queue>
using std::endl;
using std::cerr;
using std::ostream;

namespace uitwendige_gegevensstructuren {

	//Dit bestand bevat code en hoofdingen van
	template<class T, class D, unsigned int m>
	class Btree;
	template<class T, class D, unsigned int m>
	class Bknoop;
	//betekenis m: zie cursus (Het maximum aantal sleutels dat een knoop kan bevatten vooraleer het wordt opgesplitst.)

	template<class T, class D, unsigned int m>
	class Bknoop {
		friend class Btree<T, D, m>;
	public:
		Bknoop<T, D, m>();
		~Bknoop<T, D, m>();
		Bknoop<T, D, m>& operator=(const Bknoop<T, D, m>& b);

		T sleutel[m];
		D data[m];
		blokindex index[m + 1];
		unsigned int k;
		bool isblad;
	private:
		static int crashtest;
		void add(const T& sleutel, const D& data, blokindex l, blokindex r);
		void split(Bknoop&);
		void init();
	};



	template<class T, class D, unsigned int m>
	class Btree {
		typedef Bknoop<T, D, m> Knoop;
	public:
		Btree(Schijf<Knoop>& s);

		void add(const T&, const D&);
		void incr(const T&);
		bool contains(const T&);
		std::pair<T, D> getMax();
		std::vector<std::pair<T, D>> getMax(int);
	private:
		Schijf<Knoop>& schijf;
		Knoop wortel;
		blokindex wortelindex;
	};

#pragma region "Bknoop implementatie"
	//Default constructor klasse Bknoop
	template<class T, class D, unsigned int m>
	Bknoop<T, D, m>::Bknoop<T, D, m>() : k(0), isblad(true) {
		init();
	}
	//Destructor klasse Bknoop
	template<class T, class D, unsigned int m>
	Bknoop<T, D, m>::~Bknoop<T, D, m>() {
	}
	//Copy operator klasse Bknoop
	template<class T, class D, unsigned int m>
	Bknoop<T, D, m>& Bknoop<T, D, m>::operator=(const Bknoop<T, D, m>& b) {
		k = b.k;
		isblad = b.isblad;
		for (unsigned int i = 0; i < m; i++) {
			sleutel[i] = b.sleutel[i];
			data[i] = b.data[i];
		}
		if (!isblad) {
			for (unsigned int i = 0; i <= k; i++) {
				index[i] = b.index[i];
			}
		}
		return *this;
	}
	//Toevoegfuntie van gegevens in een knoop
	template<class T, class D, unsigned int m>
	void Bknoop<T, D, m>::add(const T& sl, const D& d, blokindex l, blokindex r) {
		//Denk eraan dat deze funtie enkel wordt opgeroepen op een blad (normaal gezien), zodat er niets met de indexen moet gebeuren.
		//Als ze toch wordt uitgevoerd op een knoop dat geen blad is, dan moet je er zelf voor zorgen dat de indexen goed worden gezet.

		//Check of het de sleutel er al niet in zit.
		int i = 0;
		while (i < k && sleutel[i] != sl) i++;
		if (sleutel[i] == sl) return; //Terugkeren als de knoop er al in zat.

		//Alle zaken opschuiven, inclusief indexen.
		for (i = k; i > 0 && sleutel[i-1] >= sl; i--) {
			sleutel[i] = sleutel[i - 1];
			data[i] = data[i - 1];
			index[i+1] = index[i];
		}
		k++;
		sleutel[i] = sl;
		data[i] = d;
		index[i] = l;
		index[i+1] = r;
	}
	//Splitst een knoop op in drie knopen, waarvan de ouder moet geschreven worden op de plek waar de oorspronkelijke knoop stond.
	template<class T, class D, unsigned int m>
	void Bknoop<T, D, m>::split(Bknoop& y) {
		std::copy(sleutel + m / 2 + 1, sleutel + m, y.sleutel);
		std::copy(data + m / 2 + 1, data+ m, y.data);
		std::copy(index + m / 2 + 1, index + m + 1, y.index);
		k = m / 2;
		y.k = m - (m / 2) - 1;
		y.isblad = isblad;
	}
	//Zet alle waarden op nul
	template<class T, class D, unsigned int m>
	void Bknoop<T, D, m>::init() {
		for (int i = k; i < m; i++) {
			sleutel[i] = T();
			data[i] = D();
		}
		//Indexen juist zetten
		for (int i = k + 1; i < m + 1; i++)
			index[i] = 0;
		if (k == 0) index[0] = 0;
	}
#pragma endregion

#pragma region "Btree implementatie"

	//Constructor van een Btree. Heeft een schijf nodig als parameter die een echte harde schijf simuleert.
	template<class T, class D, unsigned int m>
	Btree<T, D, m>::Btree<T, D, m>(Schijf<Knoop>& s) : schijf(s) {
		wortel.k = 0;
		wortel.isblad = true;
		wortelindex = schijf.schrijf(wortel);
	}
	//Voegt sleutel en data toe aan de Btree
	template<class T, class D, unsigned int m>
	void Btree<T, D, m>::add(const T& sl, const D& d) {
		/* UITLEG
			Door de boom lopen naar het element dat sl als sleutel heeft.
			Hierbij houden we steeds de knoop en zijn voorloper bij.

			Als de knoop waar we voorbij lopen vol zit, dan splitsen we die op.
			Het middenste element gaat naar boven. Daar is altijd plek, want anders zou 
			ze zijn opgesplitst in de vorige stap. 

			Als we zijn aangekomen in een blad waar de knoop geplaatst moet worden, voegen we die simpelweg toe.
			Wanneer hierdoor de knoop vol zou graken, splitsen we deze opnieuw op en sturen we het middelste
			element terug naar de knoop erboven. (Waar opnieuw altijd plaats is.)

			Dit is dus een top down approach.

		*/

		//Initialisatie van de hulpvariabelen.
		Knoop prev;
		blokindex prev_index = wortelindex;
		prev.init();
		schijf.lees(prev, wortelindex);
		prev.init();
		bool added = false; //Wordt op true gezet zodra de sleutel en bijhorende data in de btree zit.

		//Special case: de WORTEL
		//Dit is eigenlijk een beetje slecht geprogrammeerd. De reden is dat de prev knoop tijdens de eerste iteratie niets zou bevatten, en x de wortel van de Btree bevat.
		//Maar als je dan moet splitsen, dan krijg je problemen. Daarom heb ik dit als appart geval genomen. (Er is wss wel een andere oplossing..)
		if (prev.k == m) {
			Knoop newRoot, y;
			prev.split(y);

			blokindex r = schijf.schrijf(y);
			newRoot.add(prev.sleutel[m / 2], prev.data[m / 2], prev_index, r);
			newRoot.isblad = false;
			wortelindex = schijf.schrijf(newRoot);
			prev.init();
			schijf.herschrijf(prev, prev_index);
			prev = newRoot;
			prev.init();
		} else {
			if (prev.isblad) {
				prev.add(sl, d, 0, 0);
				schijf.herschrijf(prev, prev_index);
				added = true;
			}
		}

		int i = 0;
		while (i < prev.k && prev.sleutel[i] < sl) i++;

		while (!added) {
			if (prev.index[i] != 0) {
				Knoop x;
				blokindex x_index = prev.index[i];
				schijf.lees(x, x_index);
				if (!x.isblad) { //Als de knoop geen blad is moet je controleren of de knoop vol zit.
					if (x.k == m) { //Knoop x zit vol dus splitsen.
						Knoop y;													//
						x.split(y);													//
						int y_index = schijf.schrijf(y);							//
						prev.add(x.sleutel[m/2], x.data[m/2], x_index, y_index);	//	DUPLICATE CODE
						schijf.herschrijf(prev, prev_index);						//
						x.init();													//
						schijf.herschrijf(x, x_index);								//
					} else {
						prev = x;
						prev_index = x_index;
					}
				} else { //Als de knoop een blad is moet je de knoop hier toevoegen.
					x.add(sl, d, 0, 0);
					schijf.herschrijf(x, x_index);
					added = true;

					if (x.k == m) { //Als het toevoegen dit blad heeft opgevuld moet je het splitsen.
						Knoop y;													//
						x.split(y);													//
						int y_index = schijf.schrijf(y);							//
						prev.add(x.sleutel[m/2], x.data[m/2], x_index, y_index);	//	DUPLICATE CODE
						prev.init();												//
						schijf.herschrijf(prev, prev_index);						//
						x.init();													//
						schijf.herschrijf(x, x_index);								//
					}
				}
				i = 0;
				while (i < prev.k && prev.sleutel[i] < sl) i++;
			}
		}
		schijf.lees(wortel, wortelindex);
	}

	//Incrementeerd het element met sleutel sl met 1. Doet eigenlijk boom[sl]++ maar ik heb geen idee hoe je de subscript- en increment operator (operator[],operator++) daarvoor moet overloaden.
	template<class T, class D, unsigned int m>
	void Btree<T, D, m>::incr(const T& sl) {
		int next = wortelindex;
		while (next != 0) {
			Knoop k;
			schijf.lees(k, next);
			int i = 0;
			while (i < k.k && k.sleutel[i] != sl) i++;
			if (i < k.k && k.sleutel[i] == sl) { //Als hij in deze knoop zit
				k.data[i]++;
				schijf.herschrijf(k, next);
				return;
			}else {
				i = 0;
				while (i<k.k && k.sleutel[i] < sl) i++;
				next = k.index[i];
			}
		}
	}

	//Functie die teruggeeft of een sleutel zich al dan niet in de boom bevind.
	template<class T, class D, unsigned int m>
	bool Btree<T, D, m>::contains(const T& sl) {
		blokindex next = wortelindex;
		while ( next != 0 ) {
			Knoop k;
			schijf.lees(k, next);
			int i = 0;
			while (i < k.k && k.sleutel[i] != sl) i++;
			if ( i < k.k && k.sleutel[i] == sl) {
				return true;
			}else {
				int i = 0; 
				while (i<k.k && k.sleutel[i] < sl) i++;
				next = k.index[i];
			}
		}
		return false;
	}

	//Zoekt de knoop met de grootste data en geeft die terug in een std::pair<T, D> (dus zowel sleutel en data)
	template<class T, class D, unsigned int m>
	std::pair<T, D> Btree<T, D, m>::getMax() {
		std::queue<blokindex> w;
		w.push(wortelindex);
		std::pair<T, D> max;
		while (w.front() != 0) {
			Knoop k;
			schijf.lees(k, w.front());
			w.pop();

			for (int i=0; i<=k.k; i++)	
				w.push(k.index[i]);
			
			for (int i = 0; i < k.k; i++)
				if (max.second < k.data[i])
					max = std::pair<T, D>(k.sleutel[i], k.data[i]);
		}
		return max;
	}

	//Zoekt de i knopen met de grootste data en geeft die terug in een vector van std::pair<T, D>
	//Exact dezelfde code
	template<class T, class D, unsigned int m>
	std::vector<std::pair<T, D>> Btree<T, D, m>::getMax(int aantal) {
		std::queue<blokindex> w;
		w.push(wortelindex);
		std::vector<std::pair<T, D>> max_vec;
		while (w.front() != 0) {
			Knoop k;
			schijf.lees(k, w.front());
			w.pop();

			for (int i = 0; i <= k.k; i++)
				w.push(k.index[i]);

			for (int i = 0; i < k.k; i++) {
				max_vec.push_back(std::pair<T, D>(k.sleutel[i], k.data[i]));
				std::sort(max_vec.begin(), max_vec.end(), [](std::pair<T, D> a, std::pair<T, D> b) { return b.second < a.second; });
				max_vec.resize(aantal);
			}
				
		}
		return max_vec;
	}


#pragma endregion

}

/* Code die ik gebruikt het om deze header file te testen.

int main() {
	Schijf<Bknoop<std::string, int, 3>> schefke;
	Btree < std::string, int, 3> boom(schefke);


	std::ifstream file("Resources (iobestanden)\\donquijote.txt");

	std::string s;
	int i = 0;
	while (file >> s) {
		i++;
		std::transform(s.begin(), s.end(), s.begin(), ::tolower);

		if (boom.contains(s))	boom.incr(s);
		else					boom.add(s, 1);

	}

	for (auto p : boom.getMax(100))
		std::cout << p.first << " komt " << p.second << " keer voor." << std::endl;

	return 0;
}


*/
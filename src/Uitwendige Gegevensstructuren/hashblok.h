#pragma once

#include <algorithm>
#include <vector>
using std::vector;

#ifndef HASHBLOKGROOTTE
#define HASHBLOKGROOTTE 5
#endif // !HASHBLOKGROOTTE

namespace uitwendige_gegevensstructuren {

	template<class T>
	class HashBlok {
	public:
		HashBlok() : volgend(0) {}

		bool isFull() { return hashvalues.size() == HASHBLOKGROOTTE; }
		HashBlok<T> split(size_t);
		void add(size_t h, T v) { hashvalues.push_back(h); values.push_back(v); }
		bool contains(const T& sl) {
			return std::find(values.begin(), values.end(), sl) != values.end();
		}
	private:
		vector<size_t>	hashvalues;
		vector<T>		values;
		blokindex		volgend; //Niet gebruikt bij extendible hashing. Dit is voor de overflow paginas.
	};

	template<class T>
	HashBlok<T> HashBlok<T>::split(size_t gsb) {
		HashBlok<T> b;
		//Van achter naar voor
		for (int i = hashvalues.size() - 1; i >= 0; i--) {
			if (hashvalues[i] & (1 << (gsb - 1))) {
				//Voeg ze toe in blok b
				b.hashvalues.push_back(hashvalues[i]);
				b.values.push_back(values[i]);
				//Verwijder hem uit deze koop
				//std::iter_swap(b.hashvalues.begin()+i, b.hashvalues.end())
				hashvalues.erase(hashvalues.begin() + i);
				values.erase(values.begin() + i);
			}
		}
		return b;
	}
}
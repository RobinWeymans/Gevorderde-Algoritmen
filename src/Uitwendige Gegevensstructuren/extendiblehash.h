#pragma once

#include "hashblok.h"
using uitwendige_gegevensstructuren::HashBlok;
#include <functional>
#include "../Util/schijf.h"
using util::blokindex;
using util::Schijf;
#include <vector>
using std::vector;
#include <iostream>
#include <algorithm>

namespace uitwendige_gegevensstructuren {

	template<class T>
	class ExtendibleHash {
		typedef HashBlok<T> Blok;
	public:
		ExtendibleHash(Schijf<Blok>& s);
		bool contains(const T& sl);
		void add(const T&);
	private:
		std::hash<T>		hash;
		Schijf<Blok>&		schijf;
		size_t				gsb; //global significant bits
		vector<size_t>		lsb; //local significant bits
		vector<blokindex>	indexen;

		void extend();
	};

	template<class T>
	ExtendibleHash<T>::ExtendibleHash(Schijf<Blok>& s) : schijf(s), gsb(1), indexen(1 << gsb), lsb(1 << gsb) {
		indexen[0] = schijf.schrijf(Blok());
		indexen[1] = schijf.schrijf(Blok());
		lsb[0] = 1;
		lsb[1] = 1;
	}

	template<class T>
	bool ExtendibleHash<T>::contains(const T& sl) {
		size_t index = hash(value) & ((1 << gsb) - 1);
		Blok b;
		schijf.lees(b, indexen[index]);
		return b.contains(sl);
	}

	template<class T>
	void ExtendibleHash<T>::add(const T& value) {
		size_t hashvalue = hash(value);
		size_t index = hashvalue & ((1 << gsb) - 1);
		
		Blok b;
		schijf.lees(b, indexen[index]);
		if (b.contains(value)) return;

		if (b.isFull()) {
			if (gsb == lsb[index]) extend();
			lsb[index]++;
			b.add(hashvalue, value);
			Blok c = b.split(lsb[index]);
			schijf.herschrijf(b, indexen[index]);
			indexen[hashvalue & ((1 << gsb) - 1)] = schijf.schrijf(c);
			lsb[hashvalue & ((1 << gsb) - 1)]++;
		} else {
			b.add(hashvalue, value);
			schijf.herschrijf(b, indexen[index]);
		}
	}
	
	template<class T>
	void ExtendibleHash<T>::extend() {
		gsb++;
		indexen.resize(1 << gsb); // De grootte van de vector vergroten
		lsb.resize(1 << gsb);
		for (size_t i = indexen.size() / 2; i < indexen.size(); i++) {
			indexen[i] = indexen[i - indexen.size() / 2];
			lsb[i] = lsb[i - indexen.size() / 2];
		}
	}

}

/* Code gebruikt om extendible hashing te testen. En da bleek te lukken. Verder niet veel aandacht aan besteed.
int main() {
	Schijf<HashBlok<string>> schijf;
	ExtendibleHash<string> tabel(schijf);

	tabel.add("Hallo");
	tabel.add("dit");
	tabel.add("is");
	tabel.add("een");
	tabel.add("test");
	tabel.add("en");
	tabel.add("ik");
	tabel.add("hoop");
	tabel.add("dat");
	tabel.add("het");
	tabel.add("probeersel");
	tabel.add("gelukt");
	tabel.add("is");
	tabel.add(".");

	return 0;
}


*/
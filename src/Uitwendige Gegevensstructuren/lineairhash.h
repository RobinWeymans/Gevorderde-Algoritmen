#pragma once

#include "../Util/schijf.h"
using util::blokindex;
using util::Schijf;

#include "hashblok.h"

namespace uitwendige_gegevensstructuren {

	//Hiervoor had ik ECHT geen goesting nimeer.. FuCkDiT
	template<class T>
	class LineairHash {
		typedef HashBlok<T> Blok;
	public:
		LineairHash(Schijf<Blok>& s);
	private:
	};

	//Constructor van een LineairHash
	template<class T>
	LineairHash<T>::LineairHash(Schijf<Blok>& s) {

	}



}
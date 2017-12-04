#pragma once

#include "graaf.h"

namespace grafen {

	template<RichtType RT, class Knoopdata>
	class GraafMetKnoopdata :public virtual Graaf<RT> {
	public:
		// Construeert een graaf met gegeven RichtType, en lijst van Knoopdata;
		template<class InputIterator>
		GraafMetKnoopdata(InputIterator start, InputIterator eind);
		GraafMetKnoopdata(int aantalknopen) :Graaf<RT>(aantalknopen) {};

		virtual int voegKnoopToe();
		virtual int voegKnoopToe(const Knoopdata&);

		const Knoopdata* geefKnoopdata(int knoopnr) const;
		Knoopdata* geefKnoopdata(int knoopnr);
		virtual void wis();
		virtual void schrijfKnoop(std::ostream &os, int k) const;
	protected:
		//datavelden
		std::vector<Knoopdata>  knoopdatavector;
	};
	template<RichtType RT, class Knoopdata>
	template<class InputIterator>
	GraafMetKnoopdata<RT, Knoopdata>::GraafMetKnoopdata(InputIterator start, InputIterator eind)
		:Graaf<RT>(0) {
		for (; start != eind; start++)
			voegKnoopToe(*start);
	}

	template<RichtType RT, class Knoopdata>
	int GraafMetKnoopdata<RT, Knoopdata>::voegKnoopToe() {
		return this->voegKnoopToe(Knoopdata());
	}

	template<RichtType RT, class Knoopdata>
	int GraafMetKnoopdata<RT, Knoopdata>::voegKnoopToe(const Knoopdata& kd) {
		int ret = Graaf<RT>::voegKnoopToe();
		knoopdatavector.push_back(kd);
		return ret;
	}

	template<RichtType RT, class Knoopdata>
	const Knoopdata* GraafMetKnoopdata<RT, Knoopdata>::geefKnoopdata(int knoopnummer) const {
		this->controleerKnoopnummer(knoopnummer);
		return &knoopdatavector[knoopnummer];
	}

	template<RichtType RT, class Knoopdata>
	Knoopdata* GraafMetKnoopdata<RT, Knoopdata>::geefKnoopdata(int knoopnummer) {
		this->controleerKnoopnummer(knoopnummer);
		return &knoopdatavector[knoopnummer];
	}


	template<RichtType RT, class Knoopdata>
	void GraafMetKnoopdata<RT, Knoopdata>::wis() {
		Graaf<RT>::wis();
		knoopdatavector.clear();
	}


	template<RichtType RT, class Knoopdata>
	void GraafMetKnoopdata<RT, Knoopdata>::schrijfKnoop(std::ostream &os, int k) const {
		os << "knoop " << k << "(Data: " << knoopdatavector[k] << "):" << std::endl;
		for (std::map<int, int>::const_iterator it = this->knopen[k].begin(); it != this->knopen[k].end(); ++it) {
			os << "  ->" << it->first;
			this->schrijfVerbinding(os, it->second);
		}
	}

}
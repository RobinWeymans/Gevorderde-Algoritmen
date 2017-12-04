#pragma once

#include "graaf.h"

namespace grafen {

	template<RichtType RT, class Knoopdata, class Takdata>
	class GraafMetKnoopEnTakdata :public GraafMetKnoopdata<RT, Knoopdata>,	public GraafMetTakdata<RT, Takdata> {
	public:
		template<class InputIterator>
		GraafMetKnoopEnTakdata(InputIterator start, InputIterator eind) : GraafMetKnoopdata<RT, Knoopdata>(start, eind) {};
		GraafMetKnoopEnTakdata() : GraafMetKnoopdata<RT, Knoopdata>() {};
		GraafMetKnoopEnTakdata(int aantalknopen) : GraafMetKnoopdata<RT, Knoopdata>(aantalknopen) {};
		virtual void wis() {
			GraafMetKnoopdata<RT, Knoopdata>::wis();
			this->takdatavector.clear();
		}
	};

}
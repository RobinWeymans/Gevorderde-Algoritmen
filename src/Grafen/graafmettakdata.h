#pragma once

#include <queue>
#include <map>
#include <utility>

#include "graaf.h"
#include "../Util/pad.h"
using util::Pad;

namespace grafen {

	template<RichtType RT, class Takdata>
	class GraafMetTakdata : virtual public Graaf<RT> {
	public:
		// Construeert een graaf met gegeven RichtType en aantal knopen (default 0), zonder verbindingen.
		GraafMetTakdata(int n = 0) :Graaf<RT>(n) {};
		//Copy constructor
		GraafMetTakdata(const GraafMetTakdata<RT, Takdata>& g) : Graaf<RT>(g), takdatavector(g.takdatavector){}
		//Noot: toevoegfunctie zonder takdata op te geven kan alleen gebruikt als de klasse
		//      Takdata een defaultconstructor heeft.
		virtual int voegVerbindingToe(int van, int naar);
		virtual int voegVerbindingToe(int van, int naar, const Takdata&);
		//Noot: verwijderVerbinding wordt ongewijzigd overgenomen van Graaf!

		//TakData vrijgeven (eventueel voor wijziging). Geeft nullpointer als tak niet bestaat
		//Noot: pointers teruggegeven door geefTakdata worden ongeldig
		//door toevoegen van een tak.
		const Takdata* geefTakdata(int van, int naar) const;
		Takdata* geefTakdata(int van, int naar);

		virtual void wis();
		
		// Schrijft de gegevens van de verbinding met verbindingsnummer v naar outputstream os.
		virtual void schrijfVerbinding(std::ostream &os, int v) const;

		// Een pad optellen of aftrekken van deze graaf.
		GraafMetTakdata<RT, Takdata>& operator+=(const Pad<Takdata>& pad);
		GraafMetTakdata<RT, Takdata>& operator-=(const Pad<Takdata>& pad);

		
	public:
		std::vector<Takdata> takdatavector;
	};

	// -- Implementatie -- //

	template<RichtType RT, class Takdata>
	int GraafMetTakdata<RT, Takdata>::voegVerbindingToe(int van, int naar) {
		return this->voegVerbindingToe(van, naar, Takdata());
	}


	template<RichtType RT, class Takdata>
	int GraafMetTakdata<RT, Takdata>::voegVerbindingToe(int van, int naar, const Takdata& td) {
		bool isnieuwtaknummer = this->vrijgekomenVerbindingsnummers.empty();
		int taknummer = Graaf<RT>::voegVerbindingToe(van, naar);
		if (isnieuwtaknummer)
			takdatavector.push_back(td);
		else
			takdatavector[taknummer] = td;
		return taknummer;
	}



	template<RichtType RT, class Takdata>
	const Takdata* GraafMetTakdata<RT, Takdata>::geefTakdata(int van, int naar) const {
		int taknummer = this->verbindingsnummer(van, naar);
		if (taknummer != -1)
			return &takdatavector[taknummer];
		else
			return 0;
	}

	template<RichtType RT, class Takdata>
	Takdata* GraafMetTakdata<RT, Takdata>::geefTakdata(int van, int naar) {
		int taknummer = this->verbindingsnummer(van, naar);
		if (taknummer != -1)
			return &takdatavector[taknummer];
		else
			return 0;
	}


	template<RichtType RT, class Takdata>
	void GraafMetTakdata<RT, Takdata>::wis() {
		Graaf<RT>::wis();
		takdatavector.clear();
	}

	template<RichtType RT, class Takdata>
	void  GraafMetTakdata<RT, Takdata>::schrijfVerbinding(std::ostream &os, int v) const {
		os << " via " << v << "(Data: " << takdatavector[v] << ")" << std::endl;
	}

	template<RichtType RT, class Takdata>
	GraafMetTakdata<RT, Takdata>& GraafMetTakdata<RT, Takdata>::operator+=(const Pad<Takdata>& pad) {
		for (int i = 0; i + 1 < pad.size(); i++) {
			Takdata* td = geefTakdata(pad[i], pad[i + 1]);
			if(td == nullptr)
				voegVerbindingToe(pad[i], pad[i+1], pad.geefCapaciteit());
			else
				*td += pad.geefCapaciteit();
		}
		return *this;
	}

	template<RichtType RT, class Takdata>
	GraafMetTakdata<RT, Takdata>& GraafMetTakdata<RT, Takdata>::operator-=(const Pad<Takdata>& pad) {
		for (int i = 0; i + 1 < pad.size(); i++)
			*geefTakdata(pad[i], pad[i + 1]) -= pad.geefCapaciteit();
		return *this;
	}

}
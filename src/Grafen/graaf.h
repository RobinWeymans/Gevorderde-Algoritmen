#pragma once
//Evenals de bijbehorende klasse van GraafExcepties.
/**********************************************************************

Class: Graaf

beschrijving: Graaf is de basisklasse voor een hiërarchie van ijlegraafklassen.
Deze hiërarchie zal klassen bevatten voor grafen met data behorend bij
knopen en/of takken. Deze data worden opgeslagen in vectoren,
zodat kan worden opgezocht op index.
Ad knopen:      Deze zijn opeenvolgend genummerd. Knopen kunnen niet individueel verwijderd worden.
Wel kunnen alle knopen samen verwijderd worden (door wis()), en kunnen knopen
één voor één toegevoegd worden.
Ad verbindingen: Deze kunnen naar hartenlust toegevoegd en verwijderd worden. Als een
verbinding wordt verwijderd, wordt het nummer op de stack van
vrijgekomenVerbindingsnummers gezet. Worden verbindingen toegevoegd,
dan worden eerst deze vrijgekomen nummers opnieuw gebruikt, alvorens nieuwe nummers
worden toegevoegd. hoogsteVerbindingsnummer kan nooit verkleinen, en de
vector met takdata zal altijd hoogsteVerbindingsnummer elementen bevatten.


***************************************************************************/
/**********************************************************************

Class: GraafMetTakdata<RichtType, Takdata>

beschrijving: Deelklasse van vorigen, met data bij elke tak.

***************************************************************************/


#include <stdexcept>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <stack>
#include <exception>

#include "richttype.h"
#include "graafexeption.h"

namespace grafen {

	template<RichtType RT>
	class Graaf {
	public:
		typedef std::map<int, int>  Knoop;      // beeldt knoopnummer (van buur) af op verbindingsnummer
												// Construeert een graaf met gegeven RichtType en aantal knopen (default 0), zonder verbindingen.
		Graaf(int n = 0);

		//Copy constructor
		Graaf(const Graaf<RT>& g) : knopen(g.knopen), hoogsteVerbindingsnummer(g.hoogsteVerbindingsnummer), vrijgekomenVerbindingsnummers(g.vrijgekomenVerbindingsnummers){}

		// Geeft true indien het richttype GERICHT is, false indien het ONGERICHT is.
		bool isGericht() const;

		// Voegt een nieuwe 'lege' knoop toe, d.w.z. zonder verbindingen.
		// Geeft knoopnummer van toegevoegde knoop terug (begint bij 0).
		virtual int voegKnoopToe();

		// Voegt verbinding toe tussen knoopnummers 'van' en 'naar'.
		// Gooit GraafExceptie indien verbinding al bestaat of knoopnummers ongeldig zijn.
		// Geeft verbindingsnummer van toegevoegde verbinding terug (begint bij 0).
		// Bij ongerichte graaf wordt terugverbinding ook toegevoegd (met zelfde verbindingnummer!)
		virtual int voegVerbindingToe(int van, int naar);

		// Verwijdert de verbinding tussen knoopnummers 'van' en 'naar', incl. de terugverbinding indien ongericht.
		// Gooit GraafExceptie indien knoopnummers ongeldig zijn.
		// Geen exceptie indien de verbinding niet bestond.
		// Andere verbindingen worden niet 'hernummerd'. Er komen dus mogelijks "gaten" in de verbindingsnummers.
		virtual void verwijderVerbinding(int van, int naar);

		// Geeft het aantal knopen van de graaf.
		int aantalKnopen() const;

		// Geeft het aantal verbindingen van de graaf. 
		// Bij ongerichte graaf worden verbindingen NIET dubbel geteld!
		int aantalVerbindingen() const;

		// Geeft het verbindingsnummer van de verbinding tussen 'van' en 'naar'.
		// Geeft -1 indien die verbinding niet bestaat.
		// Gooit een GraafExceptie indien knoopnummers ongeldig zijn.
		// Opgelet: performantie is O(log(v)) waarbij v aantal verbindingen vanuit 'van' is.
		int verbindingsnummer(int van, int naar) const;

		// Verwijdert alle knopen en verbindingen en herstart de verbindingsnummer
		virtual void wis();

		// Toegang tot de knopen:
		const Knoop &operator[](int i) const { return knopen[i]; }
		Knoop &operator[](int i) { return knopen[i]; }  // deze kan als lvalue gebruikt worden


														// Schrijft de gegevens van de volledige graaf naar outputstream os.
		virtual void schrijf(std::ostream &os) const;

		// Schrijft de gegevens van de knoop met knoopnummer k naar outputstream os.
		virtual void schrijfKnoop(std::ostream &os, int k) const;

		// Schrijft de gegevens van de verbinding met verbindingsnummer v naar outputstream os.
		virtual void schrijfVerbinding(std::ostream &os, int v) const;


	protected:
		// hulpfuncties
		void controleerKnoopnummer(int k) const;   // throw indien k ongeldig
		void voegVerbindingToeInDatastructuur(int van, int naar, int verbindingsnummer);
		void verwijderVerbindingUitDatastructuur(int van, int naar);
	public:
		//datavelden
		std::vector<Knoop>  knopen;
		int                 hoogsteVerbindingsnummer;
		std::stack<int>     vrijgekomenVerbindingsnummers;
	};


	template<RichtType RT>
	std::ostream &operator<<(std::ostream& os, const Graaf<RT>& g);


	// --- implementatie --- //


	template<RichtType RT>
	void Graaf<RT>::controleerKnoopnummer(int k) const {
		if (k < 0 || (size_t)k >= knopen.size())
			throw GraafExceptie("ongeldig knoopnummer");
	}

	template<RichtType RT>
	Graaf<RT>::Graaf(int n) : knopen(n), hoogsteVerbindingsnummer(0) {}

	template<RichtType RT>
	bool Graaf<RT>::isGericht() const { return true; }//voor gerichte graaf

	template<>
	bool Graaf<ONGERICHT>::isGericht() const { return false; }//voor ongerichte graaf


	template<RichtType RT>
	int Graaf<RT>::voegKnoopToe() {
		int n = knopen.size();
		knopen.resize(n + 1); // default constructor voor nieuwe knoop wordt opgeroepen (hier lege map)
		return n;
	}


	template<RichtType RT>
	int Graaf<RT>::voegVerbindingToe(int van, int naar) {
		controleerKnoopnummer(van);
		controleerKnoopnummer(naar);

		if (knopen[van].count(naar) > 0)
		{
			std::ostringstream out;
			out << "verbinding " << van << "-" << naar << " bestaat al";
			throw GraafExceptie(out.str());
		}
		else {
			int verbindingsnummer;
			if (!vrijgekomenVerbindingsnummers.empty()) {
				verbindingsnummer = vrijgekomenVerbindingsnummers.top();
				vrijgekomenVerbindingsnummers.pop();
			}
			else
				verbindingsnummer = hoogsteVerbindingsnummer++;
			voegVerbindingToeInDatastructuur(van, naar, verbindingsnummer);
			return verbindingsnummer;
		}
	}

	template<RichtType RT>//voor gerichte graaf
	void Graaf<RT>::voegVerbindingToeInDatastructuur(int van, int naar, int verbindingsnummer) {
		knopen[van][naar] = verbindingsnummer;
	}

	template<>
	void Graaf<ONGERICHT>::voegVerbindingToeInDatastructuur(int van, int naar, int verbindingsnummer) {
		knopen[van][naar] = verbindingsnummer;
		knopen[naar][van] = verbindingsnummer;
	}

	template<RichtType RT>
	void Graaf<RT>::verwijderVerbinding(int van, int naar) {
		controleerKnoopnummer(van);
		controleerKnoopnummer(naar);
		if (knopen[van].find(naar) != knopen[van].end()) {//verbinding bestaat
			vrijgekomenVerbindingsnummers.push(knopen[van][naar]);
			verwijderVerbindingUitDatastructuur(van, naar);
		}
		// (geen exception indien verbinding niet bestaat)
	}

	template<RichtType RT>
	void Graaf<RT>::verwijderVerbindingUitDatastructuur(int van, int naar) {
		knopen[van].erase(naar);
	}

	template<>
	void Graaf<ONGERICHT>::verwijderVerbindingUitDatastructuur(int van, int naar) {
		knopen[van].erase(naar);
		knopen[naar].erase(van);
	}

	template<RichtType RT>
	int Graaf<RT>::aantalKnopen() const { return knopen.size(); }

	template<RichtType RT>
	int Graaf<RT>::aantalVerbindingen() const {
		return hoogsteVerbindingsnummer - vrijgekomenVerbindingsnummers.size();
	}

	template<RichtType RT>
	int Graaf<RT>::verbindingsnummer(int van, int naar) const {
		controleerKnoopnummer(van);
		controleerKnoopnummer(naar);
		Knoop::const_iterator verbindingit = knopen[van].find(naar);

		if (verbindingit == knopen[van].end())
			return -1;
		else
			return (*verbindingit).second;
	}

	template<RichtType RT>
	void Graaf<RT>::wis() {
		knopen.clear();
		hoogsteVerbindingsnummer = 0;
		while (!vrijgekomenVerbindingsnummers.empty())
			vrijgekomenVerbindingsnummers.pop();
	}

	template<RichtType RT>
	void Graaf<RT>::schrijf(std::ostream &os) const {
		os << "Graaf: " << aantalKnopen() << " knopen en "
			<< aantalVerbindingen() << " verbindingen:" << std::endl;
		for (int k = 0; k < aantalKnopen(); k++)
			schrijfKnoop(os, k);
	}

	template<RichtType RT>
	void Graaf<RT>::schrijfKnoop(std::ostream &os, int k) const {
		os << "knoop " << k << ":" << std::endl;
		for (Knoop::const_iterator it = knopen[k].begin(); it != knopen[k].end(); ++it)
		{
			os << "  ->" << it->first;
			schrijfVerbinding(os, it->second);
		}
	}

	template<RichtType RT>
	void Graaf<RT>::schrijfVerbinding(std::ostream &os, int v) const {
		os << " via " << v << std::endl;
	}

	template<RichtType RT>
	std::ostream &operator<<(std::ostream &os, const Graaf<RT> &g) {
		g.schrijf(os);
		return os;
	}
}
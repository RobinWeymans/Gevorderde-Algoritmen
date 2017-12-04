#pragma once
#include <vector>
#include <map>
#include <stack>

#include "graafexeption.h"
#include "richttype.h"

namespace grafen {
	template<RichtType RT>
	class MultiGraaf {
		typedef std::map<int, std::vector<int>>  Knoop;			// beeldt knoopnummer (van buur) af op de verbindingsnummerS

	public:
		MultiGraaf(int aantalKnopen);
		MultiGraaf(const MultiGraaf<RT>& graaf);
		bool isGericht() const;
		virtual int voegKnoopToe();
		virtual int voegVerbindingToe(int van, int naar);
		virtual void verwijderVerbinding(int van, int naar);
		int aantalKnopen() const;
		int aantalVerbindingen() const;
		std::vector<int>& verbindingsnummers(int van, int naar) const;
		virtual void wis();

		const Knoop &operator[](int i) const { return knopen[i]; }
		Knoop &operator[](int i) { return knopen[i]; }

		virtual void schrijf(std::ostream &os) const;
		virtual void schrijfKnoop(std::ostream &os, int k) const;
	protected:
		void controleerKnoopnummer(int k) const;   // throw indien k ongeldig
		void voegVerbindingToeInDatastructuur(int van, int naar, int verbindingsnummer);
		void verwijderVerbindingUitDatastructuur(int van, int naar);
	private:
		std::vector<Knoop>	knopen;
		int					hoogsteVerbindingsnummer;
		std::stack<int>     vrijgekomenVerbindingsnummers;
	};

	//Overloaden van de output operator
	template<RichtType RT>
	std::ostream &operator<<(std::ostream& os, const MultiGraaf<RT>& g) {
		g.schrijf(os);
		return os;
	}

	//Default Constructor
	template<RichtType RT>
	MultiGraaf<RT>::MultiGraaf(int n) : knopen(n), hoogsteVerbindingsnummer(0) {}
	//Copy constructor
	template<RichtType RT>
	MultiGraaf<RT>::MultiGraaf(const MultiGraaf<RT>& graaf) : knopen(graaf.knopen), hoogsteVerbindingsnummer(graaf.hoogsteVerbindingsnummer), vrijgekomenVerbindingsnummers(graaf.vrijgekomenVerbindingsnummers) {}
	//Getter om na te gaan of de graaf gericht of ongericht is
	bool MultiGraaf<GERICHT>::isGericht() const { return true; }
	bool MultiGraaf<ONGERICHT>::isGericht() const { return false; }
	//Toevoegen van een knoop. Geeft de knoopnr terug.
	template<RichtType RT>
	int MultiGraaf<RT>::voegKnoopToe() {
		int n = knopen.size();
		knopen.resize(n + 1); // default constructor voor nieuwe knoop wordt opgeroepen (hier lege map)
		return n;
	}
	//Toevoegen van een verbinding. Geeft de verbindingsnummer terug.
	template<RichtType RT>
	int MultiGraaf<RT>::voegVerbindingToe(int van, int naar) {
		controleerKnoopnummer(van);
		controleerKnoopnummer(naar);

		int verbindingsnummer;
		if (!vrijgekomenVerbindingsnummers.empty()) {
			verbindingsnummer = vrijgekomenVerbindingsnummers.top();
			vrijgekomenVerbindingsnummers.pop();
		}
		else
			verbindingsnummer = hoogsteVerbindingsnummer++;
		voegVerbindingToeInDatastructuur(van, naar, verbindingsnummer);
		return verbindingsnummer;

		return 0;
	}
	//Verwijderen van een verbinding.
	template<RichtType RT>
	void MultiGraaf<RT>::verwijderVerbinding(int van, int naar) {
		controleerKnoopnummer(van);
		controleerKnoopnummer(naar);
		if (knopen[van].find(naar) != knopen[van].end()) {//verbinding bestaat
			vrijgekomenVerbindingsnummers.push(knopen[van][naar][0]);
			verwijderVerbindingUitDatastructuur(van, naar);
		}
		// (geen exception indien verbinding niet bestaat)
	}
	//Getter van het aantal knopen
	template<RichtType RT>
	int MultiGraaf<RT>::aantalKnopen() const { return knopen.size(); }
	//Getter van het aantal verbindingen
	template<RichtType RT>
	int MultiGraaf<RT>::aantalVerbindingen() const {
		return hoogsteVerbindingsnummer - vrijgekomenVerbindingsnummers.size();
	}
	//Getter om de verbinding(en) tussen twee bepaalde knopen
	template<RichtType RT>
	std::vector<int>& MultiGraaf<RT>::verbindingsnummers(int van, int naar) const {
		controleerKnoopnummer(van);
		controleerKnoopnummer(naar);
		auto verbindingit = knopen[van].find(naar);

		if (verbindingit == knopen[van].end())
			knopen[van][naar] = std::vector<int>;
			return knopen[van][naar];
		else
			return verbindingit->second;
	}
	//Maak de hele multigraaf leeg
	template<RichtType RT>
	void MultiGraaf<RT>::wis() {
		knopen.clear();
		hoogsteVerbindingsnummer = 0;
		while (!vrijgekomenVerbindingsnummers.empty())
			vrijgekomenVerbindingsnummers.pop();
	}
	//Uitschrijven van de graaf
	template<RichtType RT>
	void MultiGraaf<RT>::schrijf(std::ostream &os) const {
		os << "MultiGraaf: " << aantalKnopen() << " knopen en "
			<< aantalVerbindingen() << " verbindingen:" << std::endl;
		for (int k = 0; k < aantalKnopen(); k++)
			schrijfKnoop(os, k);
	}
	//Uitschrijven van een knoop
	template<RichtType RT>
	void MultiGraaf<RT>::schrijfKnoop(std::ostream &os, int k) const {
		os << "knoop " << k << ":" << std::endl;
		for (auto it = knopen[k].begin(); it != knopen[k].end(); ++it){
			os << "  ->" << it->first << " (" << it->second.size() << ") via ";
			for (int i = 0; i < it->second.size(); i++)
				os << (it->second)[i] << " ";
			os << std::endl;
		}
	}
	//Controleren van het knoopnummer
	template<RichtType RT>
	void MultiGraaf<RT>::controleerKnoopnummer(int k) const {
		if (k < 0 || (size_t)k >= knopen.size())
			throw GraafExceptie("ongeldig knoopnummer");
	}
	//Hulpfuncties die de verbinding ook effectief toevoegt/verwijdert in de datastructuur
	void MultiGraaf<GERICHT>::voegVerbindingToeInDatastructuur(int van, int naar, int verbindingsnummer) {
		knopen[van][naar].push_back(verbindingsnummer);
	}
	void MultiGraaf<ONGERICHT>::voegVerbindingToeInDatastructuur(int van, int naar, int verbindingsnummer) {
		knopen[van][naar].push_back(verbindingsnummer);
		knopen[naar][van].push_back(verbindingsnummer);
	}
	void MultiGraaf<GERICHT>::verwijderVerbindingUitDatastructuur(int van, int naar) {
		knopen[van][naar].pop_back(); //Maakt niet uit welke van de verbindingen je wegdoet
	}
	void MultiGraaf<ONGERICHT>::verwijderVerbindingUitDatastructuur(int van, int naar) {
		knopen[van][naar].pop_back();
		knopen[naar][van].pop_back();
		if (knopen[van][naar].empty()) {
			knopen[van].erase(naar);
			knopen[naar].erase(van);
		}
	}

}
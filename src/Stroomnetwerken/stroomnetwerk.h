#pragma once

#include "../Grafen/graaf.h"
#include "../Grafen/graafmettakdata.h"
using namespace grafen;

namespace stroomnetwerken {

	template<class T>
	class Stroomnetwerk : virtual public GraafMetTakdata<grafen::GERICHT, T> {
	public:
		Stroomnetwerk(const GraafMetTakdata<GERICHT, T>& gr) : Graaf<GERICHT>(gr), Graaf<GERICHT, T>(gr) {};
		Stroomnetwerk(const GraafMetTakdata<GERICHT, T>& gr, int van, int naar) : Graaf<GERICHT>(gr), Graaf<GERICHT, T>(gr), source(van), sink(naar) {}

		void maximaleStroom();


		//Functie die elke knoop C in twee verschillende knopen splitst: Cin en Cout.
		//Alle verbindingen naar C gaan nu naar Cin
		//Alle verbindingen vanaf C vertrekken nu vanaf Cout
		//Er bestaan een verbinding van Cin naar Cout
		void ontdubbel();
		friend ostream& operator<<(ostream& os, const Stroomnetwerk<T>& s) {
			os << "Graaf: " << s.aantalKnopen() << " knopen en " << s.aantalVerbindingen() << " verbindingen:" << std::endl;
			for (int k = 0; k < s.aantalKnopen(); k++) {
				os << "knoop " << k << ":" << std::endl;
				for (Knoop::const_iterator it = s.knopen[k].begin(); it != s.knopen[k].end(); ++it){
					if (s.takdatavector[it->second] != 0) {
						os << "  ->" << it->first;
						s.schrijfVerbinding(os, it->second);
					}
				}
			}
			return os;
		}
	private:
		int source, sink;

	};

	template<class T>
	void Stroomnetwerk<T>::maximaleStroom() {
		Graaf<GERICHT, T> restnetwerk(*this);
		//Nu het restnetwerk is aangemaakt mag ik alle takdata op nul zetten.
		for (int i = 0; i < takdatavector.size(); i++)
			takdatavector[i] = T();

		while (true) {
			Pad<int> pad;
			Vergrotendpadzoeker<T> pz(restnetwerk, source, sink, pad);
			if (pad.empty()) return;


			//Restnetwerk aanpassen
			restnetwerk -= pad;
			pad.reverse();
			restnetwerk += pad;
			pad.reverse();

			//Resultaat aanpassen (gaat niet met dezelfde operatoren.)
			for (int i = 0; i + 1 < pad.size(); i++) {
				T* takdata = geefTakdata(pad[i], pad[i + 1]);
				if (takdata == nullptr)
					*geefTakdata(pad[i + 1], pad[i]) -= pad.geefCapaciteit();
				else
					*geefTakdata(pad[i], pad[i + 1]) += pad.geefCapaciteit();
			}
		} //while
	}

	template<class T>
	void Stroomnetwerk<T>::ontdubbel() {
		int aantal_knopen = knopen.size();
		for (int Cin = 0; Cin < aantal_knopen; Cin++) {
			int Cout = voegKnoopToe();
			//Alle verbindingen naar C gaan nu naar Cin

			//Alle verbindingen vanaf C gaan nu vanaf Cout
			knopen[Cout] = knopen[Cin];
			knopen[Cin].clear();

			//Verbinding Cin -> Cout
			voegVerbindingToe(Cin, Cout, INT_MAX);

		}
	}

}

//Code waarmee ik dit sharminkel heb getest. Voorbeeld vanuit de labos
//int main() {
//	GraafMetTakdata<RichtType::GERICHT, int> graaf(8);
//	graaf.voegVerbindingToe(0, 1, 7);
//	graaf.voegVerbindingToe(0, 2, 10);
//	graaf.voegVerbindingToe(3, 0, 2);
//	graaf.voegVerbindingToe(2, 1, 3);
//	graaf.voegVerbindingToe(2, 3, 5);
//	graaf.voegVerbindingToe(1, 4, 4);
//	graaf.voegVerbindingToe(2, 5, 6);
//	graaf.voegVerbindingToe(3, 6, 6);
//	graaf.voegVerbindingToe(4, 5, 6);
//	graaf.voegVerbindingToe(6, 5, 2);
//	graaf.voegVerbindingToe(7, 4, 1);
//	graaf.voegVerbindingToe(5, 7, 8);
//	graaf.voegVerbindingToe(6, 7, 7);
//
//
//	Stroomnetwerk<int> s(graaf, 0, 7);
//	s.maximaleStroom();
//
//	std::cout << s << std::endl;
//
//
//}
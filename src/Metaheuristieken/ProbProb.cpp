#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <vector>
#include <cstdlib>
#include <time.h>
#include "probprob.h"
#include "..\Grafen\graafmettakdata.h"
using namespace std;
using grafen::GraafMetTakdata;
using grafen::RichtType;

/**********************************************************************

Class: Dom

beschrijving: klasse die een domme implementatie geeft van Probprob
ze probeert een getal te vinden met een gegeven aantal cijfers dat zo klein mogelijk is.

***************************************************************************/
namespace metaheuristieken {

	class Dom2 : public ProbProb<int> {
	public:
		Dom2(GraafMetTakdata<RichtType::GERICHT, int> graaf) : graaf{ graaf } {};
		virtual bool isOplossing(const vector<int>& deeloplossing) {
			return deeloplossing.size() == graaf.aantalKnopen();
		};
		virtual double f(const vector<int>& oplossing) {
			int totaal = 0;
			for (int i = 0; i<oplossing.size() - 1 && i<6; i++) {
				int begin = oplossing[i];
				int volgende = oplossing[i + 1];
				int getal = *graaf.geefTakdata(begin, volgende);
				totaal += getal;
			}
			return totaal;
		};
		//gegeven een deeloplossing geeft deze functie een vector met mogelijke voortzettingen
		//samen met hun heuristische waarde
		virtual vector<pair<int, double> > geefVoortzetting(const vector<int>& deeloplossing) {
			vector<pair<int, double> > voortzetting;
			int van;
			if (deeloplossing.size() == 0) {
				van = 0;
			}
			else {
				van = deeloplossing[deeloplossing.size() - 1];
			}

			// bestemmingsnr , verbindingsnr
			std::map<int, int> m = graaf[van];
			vector<int> keys;
			for (std::map<int, int>::iterator it = m.begin(); it != m.end(); ++it) {
				int naar = it->first;
				int getal = *graaf.geefTakdata(van, naar);
				if (zitErNogNietIn(deeloplossing, naar)) {
					voortzetting.emplace_back(naar, getal);
				}
			}
			return voortzetting;
		};
		bool zitErNogNietIn(const vector<int>& deeloplossing, int naar) {
			for (int i = 0; i < deeloplossing.size(); i++) {
				if (deeloplossing[i] == naar) {
					return false;
				}
			}
			return true;
		}

	private:
		GraafMetTakdata<RichtType::GERICHT, int> graaf;
	};


	class Dom :public ProbProb<char> {
	public:
		Dom(int aantal) :aantal{ aantal } {};
	protected:
		virtual bool isOplossing(const vector<char>& deeloplossing) {
			return deeloplossing.size() == aantal;
		};
		virtual double f(const vector<char>& oplossing) {
			int totaal = oplossing[0] - '0';
			for (int i = 1; i < oplossing.size() && i < 7; i++) {
				totaal *= 10;
				totaal += oplossing[i] - '0';
			}
			return totaal;
		};
		//gegeven een deeloplossing geeft deze functie een vector met mogelijke voortzettingen
		//samen met hun heuristische waarde
		virtual vector<pair<char, double> > geefVoortzetting(const vector<char>& deeloplossing) {
			vector<pair<char, double> > voortzetting;
			for (int i = 0; i < 9; i++) {
				voortzetting.emplace_back('0' + i, 10 - i);
			}
			return voortzetting;
		};
		int aantal;
	};

	int main2(int argc, char *argv[]) {
		srand(time(0));
		Dom dom(19);
		vector<char> optimum = dom.geefOptimum(2000);
		cout << "Eindoplossing:\n";
		for (auto&& c : optimum) {
			cout << " " << c;
		}
		cout << "\n";
		int x;
		cin >> x;
		return 0;
	}
}
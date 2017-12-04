#pragma once

#include <map>
#include <vector>
#include <set>
#include "probprob.h"
#include "..\Grafen\graafmettakdata.h"
#include "..\Grafen\BellmanFord.h"

namespace metaheuristieken {

	class Dom2 : public ProbProb<int> {
	public:
		Dom2(grafen::GraafMetTakdata<grafen::RichtType::GERICHT, int> graaf) : graaf( graaf ) {};
		virtual bool isOplossing(const vector<int>& deeloplossing) override {
			if (deeloplossing.size() >= graaf.aantalKnopen()) {
				std::set<int> bezocht;
				for (int i : deeloplossing) {
					bezocht.insert(bezocht.begin(), i);
				}
				return bezocht.size() == graaf.aantalKnopen();
			}
			return false;
		};
		virtual double f(const vector<int>& oplossing) {
			int totaal = 0;
			for (int i = 0; i<oplossing.size() - 1 && i<6; i++)
				totaal += afstanden[oplossing[i]][oplossing[i+1]];
			return totaal;
		};
		//gegeven een deeloplossing geeft deze functie een vector met mogelijke voortzettingen
		//samen met hun heuristische waarde
		virtual vector<pair<int, double> > geefVoortzetting(const vector<int>& deeloplossing) {
			vector<pair<int, double> > voortzetting;
			int van;
			if (deeloplossing.size() == 0) {
				van = 58;
			}
			else {
				van = deeloplossing[deeloplossing.size() - 1];

			}
			std::map<int, bool> aanwz;
			for (int i = 0; i < deeloplossing.size(); i++) {
				aanwz[deeloplossing[i]] = true;
			}

			// bestemmingsnr , verbindingsnr
			std::map<int, int> m = graaf[van];

			/*
			//Als eerste stop ik zijn buren (degene die er nog niet in zitten) in de voortzetting
			for (std::map<int, int>::iterator it = m.begin(); it != m.end(); ++it) {
				int naar = it->first;
				int getal = *graaf.geefTakdata(van, naar);
				if (!aanwz[naar]) {
					voortzetting.emplace_back(naar, getal);
					aanwz[naar] = true;
				}
			}*/


			//Vervolgens de rest van de knopen. Twee opeenvolgende bestemmingen moeten namelijk niet rechtstreeks verbonden zijn met elkaar.
			if (afstanden.find(van) == afstanden.end()) {
				std::vector<int> pad;
				std::vector<int> distances;
				grafen::BellmanFord(graaf, van, pad, distances);
				afstanden[van] = distances;
			}
			for (int naar = 0; naar < graaf.aantalKnopen(); naar++) {
				if (aanwz[naar] == false) {
					//De kostprijs berekenen om vanuit 'van' naar deze knoop te gaan.
					voortzetting.emplace_back(naar, afstanden[van][naar]);
					aanwz[naar] = true;

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
		grafen::GraafMetTakdata<grafen::RichtType::GERICHT, int> graaf;
		std::map<int, std::vector<int>> afstanden;
	};

}
/*
int main()
{

	pugi::xml_document doc;
	if (!doc.load_file("Resources (iobestanden)\\treingraaf.xml"))
		return -1;

	pugi::xml_node netwerk = doc.first_child();
	int aantal_knopen = std::distance(netwerk.children("knooppunt").begin(), netwerk.children("knooppunt").end());

	GraafMetTakdata<RichtType::GERICHT, int> graaf(aantal_knopen);
	std::map<std::string, int> nummer_str_int;
	std::map<int, std::string> coord_naam;
	int iDo = 0; //Gewoon een tellerke
	for (auto&& knooppunt : netwerk.children()) {
		std::string nummer = knooppunt.attribute("nummer").value();

		if (nummer_str_int.find(nummer) == nummer_str_int.end()) {	//Zit hij al gemapt op een knoopnummer?
			nummer_str_int[nummer] = iDo++;							//Toevoegen
		}

		int van = nummer_str_int.find(nummer)->second;				//Nu zit hij der zeker in, dus kan je de nummer opvragen.
		coord_naam[van] = nummer;

		for (auto&& route : knooppunt.children()) {
			std::string doel = route.attribute("doel").value();
			if (nummer_str_int.find(doel) == nummer_str_int.end()) //analoog aan hierboven.
				nummer_str_int[doel] = iDo++;
			int naar = nummer_str_int.find(doel)->second;
			std::string afstand = route.attribute("afstand").value();
			//cout << afstand;
			graaf.voegVerbindingToe(van, naar, atoi(afstand.c_str()));
		}
	}
	//cout << graaf;


	srand(time(0));
	metaheuristieken::Dom2 dom(graaf);
	vector<int> optimum = dom.geefOptimum(200000);
	cout << "Eindoplossing: \n";
	for (auto&& c : optimum) {
		cout << " " << coord_naam[c];
	}
	cout << "\n";

	double d = 0;
	for (int i = 0; i < optimum.size(); i++) {
		d += optimum[i];
	}

	std::cout << d << std::endl;

	int x;
	std::cin >> x;

	return 0;
}
*/
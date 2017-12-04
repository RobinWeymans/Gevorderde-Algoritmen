#pragma once

#include <stack>
#include "graafmettakdata.h"

namespace grafen {
	class Kosaraju {
	public:
		Kosaraju(Graaf<RichtType::GERICHT>& graaf) : graaf(graaf), omgekeerd(graaf.aantalKnopen()) {}
		void bepaalSCC() {
			//Omgekeerde graaf bepalen door gewoon alle verbindingen om te draaien
			for (int i = 0; i < graaf.aantalKnopen(); i++)
				for (auto it = graaf[i].begin(); it != graaf[i].end(); it++)
					omgekeerd.voegVerbindingToe(it->first, i);


			std::stack<int> afgewerkt;
			std::vector<bool> bezocht(graaf.aantalKnopen(), false);

			//Diepte eerst zoeken in omgekeerde graaf, en postorder opslaan
			for (int i = 0; i < omgekeerd.aantalKnopen(); i++) {
				if (!bezocht[i]) recursie(i, afgewerkt, bezocht); //Hulpfunctie omdat het niet lukte met een expliciete stack
			}

			//Diepte eerst zoeken in originele graaf, en sla componenten op in twee dimensionale vector SCCs
			bezocht = std::vector<bool>(graaf.aantalKnopen(), false);
			int vuller = 0;
			while (!afgewerkt.empty()) {
				if (!bezocht[afgewerkt.top()]) {

					std::vector<int> SCC;
					std::stack<int> S;
					S.push(afgewerkt.top());

					while (!S.empty()) {
						int k = S.top(); S.pop();

						if (!bezocht[k]) {
							bezocht[k] = true;
							SCC.push_back(k);
							for (auto it = graaf[k].begin(); it != graaf[k].end(); it++) {
								S.push(it->first);

							}
						}
					}
					SCCs.push_back(SCC);
				}
				afgewerkt.pop();
			}

		}
	private:
		void recursie(int knoopnr, std::stack<int>& afgewerkt, std::vector<bool>& bezocht) {
			if (!bezocht[knoopnr]) {
				bezocht[knoopnr] = true;
				for (auto it = omgekeerd[knoopnr].begin(); it != omgekeerd[knoopnr].end(); it++)
					recursie(it->first, afgewerkt, bezocht);
				afgewerkt.push(knoopnr);
			}
		}
		Graaf<RichtType::GERICHT> graaf;
		Graaf<RichtType::GERICHT> omgekeerd;

	public:
		std::vector< std::vector<int> > SCCs; //Strongly connected components

	};
}
/*
int main()
{
	Graaf<GERICHT> G(11);
	G.voegVerbindingToe(0, 1);
	G.voegVerbindingToe(1, 2);
	G.voegVerbindingToe(2, 0);
	G.voegVerbindingToe(1, 3);
	G.voegVerbindingToe(3, 4);
	G.voegVerbindingToe(4, 5);
	G.voegVerbindingToe(5, 3);
	G.voegVerbindingToe(6, 5);
	G.voegVerbindingToe(6, 7);
	G.voegVerbindingToe(7, 8);
	G.voegVerbindingToe(8, 9);
	G.voegVerbindingToe(9, 6);
	G.voegVerbindingToe(9, 10);

	Kosaraju k(G);
	k.bepaalSCC();

	return 0;
}
*/
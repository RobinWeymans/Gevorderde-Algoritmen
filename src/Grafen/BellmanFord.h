#pragma once

#include "graafmettakdata.h"
#include <deque>

#define MET_QUEUE 0


namespace grafen {
	/*
		Belangrijk voordeel van BellmanFord tov Dijkstra is dat deze ook overweg kan met negatieve takdata
		Nadeel is dan weer dat het trager is.
	*/
	class BellmanFord {

	public:
		BellmanFord(GraafMetTakdata<GERICHT, int>& G, int s, std::vector<int>& P, std::vector<int>& D) {
			D = std::vector<int>(G.aantalKnopen(), 1000); //Distances
			P = std::vector<int>(G.aantalKnopen(), -1); //Parent
			P[s] = 0;
			D[s] = 0;

#if MET_QUEUE
			std::stack<int> S;
			S.push(s);
			while (!S.empty()) {
				int current = S.top(); S.pop();

				for (auto it = G[current].begin(); it != G[current].end(); it++) {
					if (D[it->first] > D[current] + *G.geefTakdata(current, it->first)) {
						D[it->first] = D[current] + *G.geefTakdata(current, it->first);
						P[it->first] = current;
						S.push(it->first);
					}
				}
			}
#else
			//Deze methode is blijkbaar (hoewel complexer) net iets beter volgens de cursus.
			std::deque<int> deque;
			std::vector<bool> zatInDeque(G.aantalKnopen(), false);
			deque.push_back(s);
			zatInDeque[s] = true;
			while (!deque.empty()) {
				int current = deque.front(); deque.pop_front();

				for (auto it = G[current].begin(); it != G[current].end(); it++) {
					if (D[it->first] > D[current] + *G.geefTakdata(current, it->first)) {
						D[it->first] = D[current] + *G.geefTakdata(current, it->first);
						P[it->first] = current;

						if (zatInDeque[it->first])  deque.push_front(it->first);
						else						deque.push_back(it->first);
					}
				}

			}
#endif

		}

	};
}

/*

int main(){
	//Voorbeeldje van den tushar.
	GraafMetTakdata<GERICHT, int> G(5);
	G.voegVerbindingToe(0, 1, 4);
	G.voegVerbindingToe(0, 2, 5);
	G.voegVerbindingToe(1, 2, -3);
	G.voegVerbindingToe(2, 4, 4);
	G.voegVerbindingToe(0, 3, 8);
	G.voegVerbindingToe(3, 4, 2);
	G.voegVerbindingToe(4, 3, 1);

	std::vector<int> PATH;
	BellmanFord B(G, 0, PATH);

	for (int i = 0; i < PATH.size(); i++) {
		printf("%i ", PATH[i]);
	}

	return 0;
}


*/
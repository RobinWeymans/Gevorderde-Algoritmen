#pragma once

#include <map>
#include <list>
#include "multigraaf.h"

/*
	Klasse die het eulercircuit berekend.
*/
namespace grafen {
	class Hierholzer {

	public:
		Hierholzer(MultiGraaf<ONGERICHT>& graaf) {
			//Controleren of de graaf wel een Euler-circuit heeft
			int oneven = 0;
			int van, naar;
			for (int i = 0; i < graaf.aantalKnopen(); i++) {
				int teller = 0;
				for (auto it = graaf[i].begin(); it != graaf[i].end(); it++)
					teller += it->second.size();

				if (teller % 2 != 0) {
					oneven++;
					if (oneven == 1)
						van = i;
					else
						naar = i;
				}
			}

			//Als er geen verbindingen zijn met oneven aantal verbindingen, dan is knoop 0 het begin en eindpunt, en is het euler circuit volledig gesloten.
			if (oneven == 0) {
				van = naar = 0;
				oneven = 2; //Zodat we ook door het volgende if statement graken.
			}
			//Als er exact twee knopen zijn met een oneven aantal verbindingen, dan zijn deze twee knopen begin en eindpunt van het euler circuit
			if (oneven == 2) {
				
				std::stack<int> S;
				std::vector<bool> visited(graaf.aantalVerbindingen(), false);
				S.push(van);
				while (!S.empty()) {

					int allesBezochtKnoop = -1, allesBezochtEdge = -1;
					for (auto it = graaf[S.top()].begin(); it != graaf[S.top()].end() && allesBezochtKnoop == -1; it++) {
						for (int i = 0; i < it->second.size(); i++) {
							if (visited[it->second[i]] == false) {
								allesBezochtKnoop = it->first;
								allesBezochtEdge = it->second[i];
							}
						}
					}
							 
					if (allesBezochtKnoop == -1) {
						Path.push_back(S.top());
						S.pop();
					}
					else {
						S.push(allesBezochtKnoop);
						visited[allesBezochtEdge] = true;
					}

				}

			}
		}
		
	private:
		std::list<int> Path;
	};
}


/*

int main()
{
MultiGraaf<ONGERICHT> G(16);
G.voegVerbindingToe(0, 1);
G.voegVerbindingToe(0, 3);
G.voegVerbindingToe(5, 1);
G.voegVerbindingToe(2, 1);
G.voegVerbindingToe(4, 1);
G.voegVerbindingToe(2, 4);
G.voegVerbindingToe(2, 5);
G.voegVerbindingToe(3, 2);


Hierholzer H(G);

return 0;
}

*/
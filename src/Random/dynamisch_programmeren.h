#pragma once

#include <fstream>
#include <vector>
#include <map>

int bruggenprobleem() {
	//Informatie inlezen
	std::map<int, std::pair<int, int>> bruggen;
	std::ifstream file("versnick.dat");
	int count;
	file >> count;

	for (int van = 0; van<count; van++) {
		int bribe, naar;
		file >> bribe >> naar;
		bruggen[van] = std::pair<int, int>(naar, bribe);
	}


	//Oplossing zou 43004 moeten zijn
	std::vector<int> deeloplossingen;
	for (int rechts = 0; rechts < count; rechts++) {
		int max = 0;

		for (int i = 0; i <rechts; i++) {
			if (deeloplossingen[i] > max && bruggen[rechts].first > bruggen[i].first)
				max = deeloplossingen[i];
		}
		deeloplossingen.push_back(max + bruggen[rechts].second);
	}

	int max = 0;
	for (int i = 0; i < deeloplossingen.size(); i++)
		if (max < deeloplossingen[i])
			max = deeloplossingen[i];

	return max;
}


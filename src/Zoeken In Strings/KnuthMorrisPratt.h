#pragma once

#include <vector>
#include <queue>
#include <iostream>
#include <string>

typedef unsigned int uint;
typedef unsigned char uchar;

namespace zoeken_in_strings {

	class KnuthMorrisPratt {
	public:
		KnuthMorrisPratt(const uchar* naald, uint naaldlengte) : naald(naald), naaldlengte(naaldlengte), prefixtabel(naaldlengte, -1) {
			bereken_prefix();
		}
		void zoek(std::queue<int>& plaats, const uchar* hooiberg, uint hooiberglengte);
	private:

		void bereken_prefix();

		const uchar* naald;
		int naaldlengte;

		std::vector<int> prefixtabel;

	};

	void KnuthMorrisPratt::zoek(std::queue<int>& plaats, const uchar* hooiberg, uint hooiberglengte) {
		int m = 0, i = 0;

		while (m < hooiberglengte) {
			while (i != -1 && (i == naaldlengte || naald[i] != hooiberg[m])) i = prefixtabel[i];
			i++; m++;
			if (i == naaldlengte) plaats.push(m - i);
		}


	}

	void KnuthMorrisPratt::bereken_prefix() {
		//prefixtabel berekenen
		for (int i = 1; i < naaldlengte; i++) {
			int pos = prefixtabel[i - 1];
			while (pos > 0 && naald[pos] != naald[i - 1]) pos = prefixtabel[pos];
			prefixtabel[i] = pos + 1;
		}
		//KMP prefixtabel berekenen op basis van de vorige -- the right way
		for (int i = 2; i < naaldlengte; i++)
			while (prefixtabel[i] > 0 && naald[i] == naald[prefixtabel[i]])
				prefixtabel[i] = prefixtabel[prefixtabel[i]];


		for (int i = 0; i < prefixtabel.size(); i++)
			std::cout << *(naald + i) << ' ' << prefixtabel[i] << std::endl;
		std::cout << std::endl;

	}
}

/*
	int main() {
		std::cout << "Geef een string in: " << endl;
		string teZoeken;
		std::getline(cin, teZoeken);
		ifstream shakespeare("Resources (iobestanden)\\Shakespeare.txt");
		string content( (std::istreambuf_iterator<char>(shakespeare) ), (std::istreambuf_iterator<char>() ) );
		KnuthMorrisPratt k((const uchar*)teZoeken.c_str(), (uint)teZoeken.size());
		queue<int> plaats;
		k.zoek(plaats, (const uchar*) content.c_str(), content.size() );
		std::cout << plaats.size() << std::endl;

		return 0;
	}
*/

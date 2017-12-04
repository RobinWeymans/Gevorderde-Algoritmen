#pragma once

#include <queue>
#include <iostream>

typedef unsigned char uchar;
typedef unsigned int uint;

#define GROOTTE_ALFABET 256

namespace zoeken_in_strings {

	class BoyerMooreHorspool {
	public:
		BoyerMooreHorspool(const uchar* naald, uint naaldlengte);
		void zoek(std::queue<int>& plaats, const uchar* hooiberg, uint hooiberglengte);
	private:
		void fill_bcr();


		const uchar* naald;
		int naaldlengte;

		std::vector<int> bad_char_rule;
	};

	BoyerMooreHorspool::BoyerMooreHorspool(const uchar* naald, uint naaldlengte) : naald(naald), naaldlengte(naaldlengte), bad_char_rule(GROOTTE_ALFABET, naaldlengte) {
		fill_bcr();
	}
	void BoyerMooreHorspool::zoek(std::queue<int>& plaats, const uchar* hooiberg, uint hooiberglengte) {
		int skip = 0;
		while (hooiberglengte - skip >= naaldlengte) {
			int i = naaldlengte - 1;
			while (hooiberg[skip + i] == naald[i]) {
				if (i == 0)	std::cout << "gevonden" << std::endl;
				i--;
			}
			skip += bad_char_rule[hooiberg[skip + naaldlengte - 1]];
		}
	}
	void BoyerMooreHorspool::fill_bcr() {
		for (int i = 0; i < naaldlengte-1; i++)
			bad_char_rule[naald[i]] = naaldlengte - i - 1;
	}

}
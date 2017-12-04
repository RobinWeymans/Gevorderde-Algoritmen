#pragma once

#include <queue>

typedef unsigned char uchar;
typedef unsigned int uint;

#define GROOTTE_ALFABET 256

namespace zoeken_in_strings {

	class BoyerMoore {
	public:
		BoyerMoore(const uchar* naald, uint naaldlengte);
		void zoek(std::queue<int>& plaats, const uchar* hooiberg, uint hooiberglengte);
	private:
		void fill_bcr();


		const uchar* naald;
		int naaldlengte;

		std::vector<std::vector<int>> bad_char_rule;
	};

	BoyerMoore::BoyerMoore(const uchar* naald, uint naaldlengte) : naald(naald), naaldlengte(naaldlengte), bad_char_rule(naaldlengte) {
		fill_bcr();
	}
	void BoyerMoore::zoek(std::queue<int>& plaats, const uchar* hooiberg, uint hooiberglengte) {
		int skip = 0;
		while (hooiberglengte - skip >= naaldlengte) {
			int i = naaldlengte - 1;
			while (hooiberg[skip + i] == naald[i] && i > 0)
				i--;
			if (i == 0) {std::cout << "gevonden" << std::endl; i++;	}
			skip += i - bad_char_rule[i][hooiberg[skip+i]];
		}
	}
	void BoyerMoore::fill_bcr() {
		bad_char_rule[0] = std::move(std::vector<int>(GROOTTE_ALFABET, 0));
		for (int i = 1; i < naaldlengte; i++) {
			bad_char_rule[i] = std::move(std::vector<int>(bad_char_rule[i - 1])); //Kopieke pakke van de vorige
			bad_char_rule[i][naald[i - 1]] = i - 1;
		}
	}

}
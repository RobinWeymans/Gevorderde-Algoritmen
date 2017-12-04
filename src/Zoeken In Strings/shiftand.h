#pragma once


#include "../Util/bitpatroon.h"
#include <queue>
typedef unsigned int uint;
typedef unsigned char uchar;

using util::Bitpatroon;

class ShiftAnd {


public:
	enum MODE { EXACT, INSERT, DELETE, REPLACE, ALL };
	//naaldlengte moet kleiner dan patroonlengte zijn,
	//anders wordt een exceptie opgegooid.
	ShiftAnd(const uchar* naald, uint _naaldlengte, uint fuzzyness, MODE m);
	std::queue<const uchar*> zoek(const uchar* hooiberg, uint hooiberglengte);
private:
	util::Bitpatroon letterpatroon[256];
	uint naaldlengte;
	uint fuzzyness;
	MODE mode;
};


ShiftAnd::ShiftAnd(const uchar* naald, uint _naaldlengte, uint _fuzzyness=0, MODE m= EXACT) : naaldlengte(_naaldlengte), fuzzyness(_fuzzyness), mode(m) {
	for (uint i = 0; i < _naaldlengte; i++)
		letterpatroon[naald[i]] |= util::Bitpatroon::eenbit(i);

	if (mode == EXACT)
		fuzzyness = 0;
	
}

std::queue<const uchar*> ShiftAnd::zoek(const uchar* hooiberg, uint hooiberglengte) {
	std::queue<const uchar*> ret;
	std::vector<util::Bitpatroon> D(fuzzyness+1);

	for (int i = 1; i <= fuzzyness; i++)
		D[i] = D[i - 1].shiftrechts(1) | Bitpatroon::eenbit(0);

	for (int i = 0; i < hooiberglengte; i++) {
		util::Bitpatroon oldD = D[0];
		std::cout << "letter: "<< hooiberg[i] << std::endl;
		D[0] = (D[0].shiftrechts(1) | util::Bitpatroon::eenbit(0)) & letterpatroon[hooiberg[i]];
		std::cout << D[0] << std::endl;
		
		for (int f = 1; f <= fuzzyness; f++) {
			util::Bitpatroon tmp = D[f];
			switch (mode) {
			case INSERT:
				//std::cout << oldD << " | " << " ( " << (D[f].shiftrechts(1) | util::Bitpatroon::eenbit(0)) << " & " << letterpatroon[hooiberg[f]] << " ) " << std::endl;
				D[f] = oldD | ((D[f].shiftrechts(1) | util::Bitpatroon::eenbit(0)) & letterpatroon[hooiberg[i]]);
				break;
			case DELETE:
				//std::cout << (D[f - 1].shiftrechts(1) | Bitpatroon::eenbit(0)) << " | ( " << (D[f].shiftrechts(1) | Bitpatroon::eenbit(0)) << " & " <<  letterpatroon[hooiberg[i]] << " ) " << std::endl;
				D[f] = (D[f - 1].shiftrechts(1) | Bitpatroon::eenbit(0)) | ((D[f].shiftrechts(1) | Bitpatroon::eenbit(0)) & letterpatroon[hooiberg[i]]);
				break;
			case REPLACE:
				//std::cout << (oldD.shiftrechts(1) | Bitpatroon::eenbit(0)) << " | ( " << (D[f].shiftrechts(1) | Bitpatroon::eenbit(0)) << " & " << letterpatroon[hooiberg[i]] << " ) " << std::endl;
				D[f] = (oldD.shiftrechts(1) | Bitpatroon::eenbit(0)) | ((D[f].shiftrechts(1) | Bitpatroon::eenbit(0)) & letterpatroon[hooiberg[i]]);
				break;
			case ALL:
				D[f] = oldD | (D[f - 1].shiftrechts(1) | Bitpatroon::eenbit(0)) | (oldD.shiftrechts(1) | Bitpatroon::eenbit(0)) | ((D[f].shiftrechts(1) | Bitpatroon::eenbit(0)) & letterpatroon[hooiberg[i]]);
				break;
			}
			//D[i] = ((oldD | (D[i] & letterpatroon[hooiberg[i]])).shiftrechts(1) | util::Bitpatroon::eenbit(0));
			std::cout << D[f] << std::endl;
			oldD = tmp;
		}
		std::cout << std::endl;

		if (D[fuzzyness].en(util::Bitpatroon::eenbit(naaldlengte-1)))
			std::cout << "gevonden" << std::endl;
	}

	return ret;
}
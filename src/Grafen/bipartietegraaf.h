#pragma once

#include "graaf.h"
#include "graafmettakdata.h"
using grafen::GraafMetTakdata;
using grafen::RichtType;
using grafen::Graaf;

#include <string>
using std::string;

#include <fstream>
using std::ifstream;
namespace grafen {
	class BipartieteGraaf : public virtual GraafMetTakdata<RichtType::GERICHT, int> {
	public:
		BipartieteGraaf() : Graaf<RichtType::GERICHT>() {}

		void lees(const string& filename) {
			ifstream file(filename);
			if (!file.is_open()) {
				std::cerr << "File not found" << std::endl;
				return;
			}

			int aantal_knopen;
			file >> aantal_knopen;

			knopen.clear();
			knopen.resize(aantal_knopen);


			source = voegKnoopToe();
			sink = voegKnoopToe();

			std::cout << aantalKnopen() << std::endl;
			int van, naar;
			while (file >> van >> naar) {
				voegVerbindingToe(van, naar, 1);
				try {
					voegVerbindingToe(source, van, 1);
				}
				catch (GraafExceptie e) { /*std::cout << e.what() << std::endl;*/ }

				try {
					voegVerbindingToe(naar, sink, 1);
				}
				catch (GraafExceptie e) { /*std::cout << e.what() << std::endl;*/ }
			}
		}
		int source, sink;
	private:
		
	};
}

//int main() {
//	BipartieteGraaf graaf;
//	graaf.lees("Resources (iobestanden)\\bipartitegraaf.txt");
//
//	Stroomnetwerk<int> s(graaf, graaf.source, graaf.sink);
//	s.maximaleStroom();
//
//	std::cout << s << std::endl;
//
//	return 0;
//}
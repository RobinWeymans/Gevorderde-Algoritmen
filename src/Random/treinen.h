#pragma once

//Header file met enkel commentaar


/*

	int main(){
		pugi::xml_document doc;
		if (!doc.load_file("Resources (iobestanden)\\treingraaf.xml")) return -1;

		pugi::xml_node netwerk = doc.first_child();
		int aantal_knopen = std::distance(netwerk.children("knooppunt").begin(), netwerk.children("knooppunt").end());

		GraafMetTakdata<GERICHT, int> graaf(aantal_knopen);
		std::map<std::string, int> nummer_str_int;
		int iDo = 0; //Gewoon een tellerke
		for (auto&& knooppunt : netwerk.children()){
			std::string nummer = knooppunt.attribute("nummer").value();

			if (nummer_str_int.find(nummer) == nummer_str_int.end())	//Zit hij al gemapt op een knoopnummer?
				nummer_str_int[nummer] = iDo++;							//Toevoegen
			int van = nummer_str_int.find(nummer)->second;				//Nu zit hij der zeker in, dus kan je de nummer opvragen.

			for (auto&& route : knooppunt.children()) {
				std::string doel = route.attribute("doel").value();
				if (nummer_str_int.find(doel) == nummer_str_int.end()) //analoog aan hierboven.
					nummer_str_int[doel] = iDo++;
				int naar = nummer_str_int.find(doel)->second;

				graaf.voegVerbindingToe(van, naar, 1);

			}
		}

		Stroomnetwerk<int> s(graaf, 58, 80);
		s.ontdubbel(); //Indien deze regel in commentaar wordt gezet krijg je exact hetzelfde (kan dat?)
		s.maximaleStroom();

		int som = 0;
		for (auto it = s.knopen[58].begin(); it != s.knopen[58].end(); it++)
			som += s.takdatavector[it->second];

		std::cout << std::endl << som << std::endl;

		return 0;
	}





*/

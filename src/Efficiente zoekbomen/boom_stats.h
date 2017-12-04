#pragma once

#include <iostream>
#include <vector>
#include <cstdlib>
#include <time.h>
#include "zoekboom11.h"
#include "roodzwartboom.h"
#include "splayboom.h"
#include "../Util/csv.h"
#include "../Util/puntengenerator.h"

using std::vector;
using namespace efficiente_zoekbomen;


int aantalDatapunten = 10000;
int max = 100000;
int zoekmeetpunt = 1;

void treestat() {
	util::CsvData grafiek("dieptedata", ',');
	srand((unsigned int)time(NULL));
	int zoekdiepte = 0;

	//ZOEKBOOM
	//Construeren van de linaire zoekboom
	Zoekboom<int, int>* lineaireZoekboom = new Zoekboom<int, int>();
	for (int i = 0; i < aantalDatapunten; i++) {
		lineaireZoekboom->voegtoe(i, 0);
	}
	std::cout << std::endl << "Potentiaal van een linaire zoekboom met " << aantalDatapunten << " punten: " << lineaireZoekboom->potentiaal() << std::endl;
	delete lineaireZoekboom;

	//Construeren van een random gegenereerde zoekboom
	Zoekboom<int, int>* randomZoekboom = new Zoekboom<int, int>();
	for (int i = 0; i < aantalDatapunten; i++)
		randomZoekboom->voegtoe(rand() % max, 0);
	std::cout << "Potentiaal van een random gegenereerde met ongeveer " << aantalDatapunten << " punten: " << randomZoekboom->potentiaal() << std::endl;
	delete randomZoekboom;


	//Verzamelen van informatie
	vector<double> dieptedata;
	vector<double> potentiaaldata;

	//SPLAYBOOM
	//Construeren van de random splayboom
	Splayboom<double, int>* splayboom;
	Puntgenerator* gen;
	//Zoekoperaties
	//1. Een random aangemaakte boom met random zoekoperaties
	std::cout << '1';
	splayboom = new Splayboom<double, int>();
	for (int i = 0; i < aantalDatapunten; i++)
		splayboom->voegtoe(rand() % max, 0);

	for (int i = 0; i < aantalDatapunten; i++) {
		if (i % zoekmeetpunt == 0) {
			int diepte = 0;
			splayboom->zoek(rand() % max, diepte);
			dieptedata.push_back(diepte);
			potentiaaldata.push_back(splayboom->potentiaal());
		}
	}
	grafiek.voegDataToe(dieptedata);
	grafiek.voegDataToe(potentiaaldata);
	dieptedata.clear();
	potentiaaldata.clear();
	delete splayboom;

	//2. Een random aangemaakte boom met random zoekoperaties. Echter, de kans dat je een waarde kiest hangt af van de plaats: je hebt 99,9 % kans dat je gezochte sleutel in de laatste 0,1 % van de range zit.
	std::cout << '2';
	splayboom = new Splayboom<double, int>();
	for (int i = 0; i < aantalDatapunten; i++)
		splayboom->voegtoe(rand() % max, 0);

	for (int i = 0; i < aantalDatapunten; i++) {
		if (i % zoekmeetpunt == 0) {
			int diepte = 0;
			splayboom->zoek(rand() % (max / 1000), diepte);
			dieptedata.push_back(diepte);
			potentiaaldata.push_back(splayboom->potentiaal());
		}
	}
	grafiek.voegDataToe(dieptedata);
	grafiek.voegDataToe(potentiaaldata);
	dieptedata.clear();
	potentiaaldata.clear();
	delete splayboom;

	//3. Een in volgorde aangemaakte boom met random zoekoperaties.
	std::cout << '3';
	splayboom = new Splayboom<double, int>();
	for (int i = 0; i < aantalDatapunten; i++)
		splayboom->voegtoe(i, 0);

	for (int i = 0; i < aantalDatapunten; i++) {
		if (i % zoekmeetpunt == 0) {
			int diepte = 0;
			splayboom->zoek(rand() % max, diepte);
			dieptedata.push_back(diepte);
			potentiaaldata.push_back(splayboom->potentiaal());
		}
	}
	grafiek.voegDataToe(dieptedata);
	grafiek.voegDataToe(potentiaaldata);
	dieptedata.clear();
	potentiaaldata.clear();
	delete splayboom;

	//4. Een in volgorde aangemaakte boom met random zoekoperaties. Echter, de kans dat je een waarde kiest hangt af van de plaats: je hebt 99,9 % kans dat je gezochte sleutel in de laatste 0,1 % van de range zit.
	std::cout << '4';
	splayboom = new Splayboom<double, int>();
	for (int i = 0; i < aantalDatapunten; i++)
		splayboom->voegtoe(i, 0);

	for (int i = 0; i < aantalDatapunten; i++) {
		if (i % zoekmeetpunt == 0) {
			int diepte = 0;
			splayboom->zoek(rand() % (max / 1000), diepte);
			dieptedata.push_back(diepte);
			potentiaaldata.push_back(splayboom->potentiaal());
		}
	}
	grafiek.voegDataToe(dieptedata);
	grafiek.voegDataToe(potentiaaldata);
	dieptedata.clear();
	potentiaaldata.clear();
	delete splayboom;

	//5. Een met de Puntgenerator aangemaakte boom met random zoekoperties.
	std::cout << '5';
	splayboom = new Splayboom<double, int>();
	gen = new Puntgenerator();
	double mini = 0, maxi = 0;
	for (int i = 0; i < aantalDatapunten; i++) {
		double d = gen->geefpunt();
		if (d < mini) mini = d;
		if (d > maxi) maxi = d;
		splayboom->voegtoe(d, 0);

	}

	for (int i = 0; i < aantalDatapunten; i++) {
		if (i % zoekmeetpunt == 0) {
			int diepte = 0;
			splayboom->zoek(((rand() % 100) / 100) * (maxi - mini) + mini, diepte);
			dieptedata.push_back(diepte);
			potentiaaldata.push_back(splayboom->potentiaal());
		}
	}
	grafiek.voegDataToe(dieptedata);
	grafiek.voegDataToe(potentiaaldata);
	dieptedata.clear();
	potentiaaldata.clear();
	mini = 0, maxi = 0;
	delete splayboom;
	delete gen;

	//6. Een met de Puntgenerator aangemaakte boom met random zoekoperaties. Echter, de kans dat je een waarde kiest hangt af van de plaats: je hebt 99,9 % kans dat je gezochte sleutel in de laatste 0,1 % van de range zit.
	std::cout << '6';
	splayboom = new Splayboom<double, int>();
	gen = new Puntgenerator();
	for (int i = 0; i < aantalDatapunten; i++)
		splayboom->voegtoe(gen->geefpunt(), 0);

	for (int i = 0; i < aantalDatapunten; i++) {
		if (i % zoekmeetpunt == 0) {
			int diepte = 0;
			splayboom->zoek(((rand() % 100) / 1000.0) * (maxi - mini) + mini, diepte);
			dieptedata.push_back(diepte);
			potentiaaldata.push_back(splayboom->potentiaal());
		}
	}
	grafiek.voegDataToe(dieptedata);
	grafiek.voegDataToe(potentiaaldata);
	dieptedata.clear();
	potentiaaldata.clear();
	delete splayboom;
	delete gen;

}
#pragma once

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <queue>
#include <stack>
#include <memory>
#include <string>

using std::ostream;
using std::unique_ptr;

#define prettyprintwidth 500
#define prettyprintheight 500

/**********************************************************************

Class: Zoekboom

beschrijving: Binaire zoekboom waarin geen duplicaatsleutels zijn toegestaan.

***************************************************************************/

namespace efficiente_zoekbomen {

	template <class Sleutel, class Data>
	class Zoekknoop;

	template <class Sleutel, class Data>
	class Zoekboom : public unique_ptr<Zoekknoop<Sleutel, Data>> {
		using unique_ptr<Zoekknoop<Sleutel, Data>>::unique_ptr;
	public:
		Zoekboom() {}
		Zoekboom(Zoekboom<Sleutel, Data>&& z) : unique_ptr<Zoekknoop<Sleutel, Data>>(move(z)) {}
		Zoekboom& operator=(Zoekboom&& z);
		friend std::ostream& operator<<(std::ostream& os, const Zoekboom<Sleutel, Data>& boom) { boom.schrijf();	return os; }

		virtual void voegtoe(const Sleutel& sleutel, const Data& data);

		const Zoekboom<Sleutel, Data>* min() const;
		const Zoekboom<Sleutel, Data>* max() const;

		int diepte() const;
		double gemiddeldediepte() const;
		void roteer(bool links);
		void schrijf() const;
		void maakonevenwichtig();
		void maakevenwichtig();
		double potentiaal();
	protected:
		//zoekfunctie zoekt sl en geeft de boom in waaronder de sleutel zit (eventueel een lege boom als de sleutel
		//ontbreekt) en de pointer naar de ouder (als die bestaat, anders nulpointer).
		//noot: alhoewel de functie niets verandert aan de boom is ze geen const functie.
		double potentiaal(int&);
		void zoek(const Sleutel& sleutel, Zoekknoop<Sleutel, Data>*& ouder, Zoekboom<Sleutel, Data>*& plaats);
		void gemiddeldediepte(int& som, int& aantal) const;
		void maakevenwichtig(Zoekboom<Sleutel, Data>& z, std::vector<std::pair<Sleutel, Data> >& v, int start, int einde);
		int schrijf(bool is_left, int offset, int depth, char s[prettyprintheight][prettyprintwidth]) const;
		std::vector<std::pair<Sleutel, Data> > vectorize();

	};

	template <class Sleutel, class Data>
	class Zoekknoop {
		friend class Zoekboom<Sleutel, Data>;
	public:
		Sleutel& getSleutel() { return sleutel; }
		Zoekboom<Sleutel, Data>& getChild(bool linkerkind) { return linkerkind ? links : rechts; }

	protected:
		Sleutel sleutel;
		Data data;
		Zoekknoop<Sleutel, Data>* ouder;
		Zoekboom<Sleutel, Data> links, rechts;
		Zoekknoop() : ouder(0) {}
		Zoekknoop(const Sleutel& sl, const Data& d) : ouder(0), sleutel(sl), data(d) {}

	};

#pragma region Implementatie Zoekboom
	//Move operator
	template<class Sleutel, class Data>
	Zoekboom<Sleutel, Data>& Zoekboom<Sleutel, Data>::operator=(Zoekboom&& z) {
		unique_ptr<Zoekknoop<Sleutel, Data>>::operator=(move(z));
		return *this;
	}

	//Toevoegen van data in de zoekboom.
	template <class Sleutel, class Data>
	void Zoekboom<Sleutel, Data>::voegtoe(const Sleutel& sleutel, const Data& data) {
		Zoekboom<Sleutel, Data>* plaats;
		Zoekknoop<Sleutel, Data>* ouder;
		zoek(sleutel, ouder, plaats);
		if (!*plaats) {
			Zoekboom<Sleutel, Data> nieuw(new Zoekknoop<Sleutel, Data>(sleutel, data));
			nieuw->ouder = ouder;
			*plaats = move(nieuw);
		}

	};

	//Het zoeken van de kleinste sleutel in de zoekboom. Geeft een pointer terug naar een zoekboom waar deze kleinste knoop de wortel van is.
	template <class Sleutel, class Data>
	const Zoekboom<Sleutel, Data>* Zoekboom<Sleutel, Data>::min() const {
		const Zoekboom<Sleutel, Data>* loper = this;
		while (*loper && (*loper)->links)
			loper = &(*loper)->links;
		return loper;
	}

	//Het zoeken van de grootste sleutel in de zoekboom. Geeft een pointer terug naar een zoekboom waar deze grootste knoop de wortel van is.
	template <class Sleutel, class Data>
	const Zoekboom<Sleutel, Data>* Zoekboom<Sleutel, Data>::max() const {
		const Zoekboom<Sleutel, Data>* loper = this;
		while (*loper && (*loper)->rechts)
			loper = &(*loper)->rechts;
		return loper;
	}

	//Bepalen van de diepte van de laagste knoop in de boom.
	template <class Sleutel, class Data>
	int Zoekboom<Sleutel, Data>::diepte() const {
		return *this ? 1 + std::max((*this)->links.diepte(), (*this)->rechts.diepte()) : 0;
	}

	//Geeft de gemiddelde diepte van de boom terug. Maakt gebruik van de recursieve functie gemiddeldediepte(int&,int&) om het aantal knopen en de totale diepte te bepalen.
	template <class Sleutel, class Data>
	double Zoekboom<Sleutel, Data>::gemiddeldediepte() const {
		int som = 0, aantal = 0;
		gemiddeldediepte(som, aantal);
		return (double)som / aantal;
	}
	template <class Sleutel, class Data>
	void Zoekboom<Sleutel, Data>::gemiddeldediepte(int& som, int& aantal) const {
		if (*this == 0)
			return;

		aantal++;
		som += diepte();

		(*this)->links.gemiddeldediepte(som, aantal);
		(*this)->rechts.gemiddeldediepte(som, aantal);
	}

	//Maakt de boom zo onevenwichtig mogelijk door de knopen uit de boom te halen, te sorteren en vervolgens ze in volgorde weer toe te voegen aan de boom.
	template <class Sleutel, class Data>
	void Zoekboom<Sleutel, Data>::maakonevenwichtig() {
		std::vector<std::pair<Sleutel, Data> > v = vectorize();
		std::sort(v.begin(), v.end());

		Zoekboom<Sleutel, Data> nieuw;
		for (int i = 0; i < v.size(); i++) {
			nieuw.voegtoe(v[i].first, v[i].second);
		}
		*this = std::move(nieuw);
	}

	//Maakt de boom zo evenwichtig mogelijk door de knopen uit de boom te halen, te sorteren en vervolgens telkens de middelste toe te voegen. (Analoog met binary search)
	template <class Sleutel, class Data>
	void Zoekboom<Sleutel, Data>::maakevenwichtig() {
		std::vector<std::pair<Sleutel, Data> > v = vectorize();
		std::sort(v.begin(), v.end());

		Zoekboom<Sleutel, Data> nieuw;
		maakevenwichtig(nieuw, v, 0, v.size() - 1);
		*this = std::move(nieuw);
	}
	template <class Sleutel, class Data>
	void Zoekboom<Sleutel, Data>::maakevenwichtig(Zoekboom<Sleutel, Data>& z, std::vector<std::pair<Sleutel, Data> >& v, int start, int einde) {
		if (start <= einde) {
			int index = start + (einde - start) / 2;
			z.voegtoe(v[index].first, v[index].second);
			maakevenwichtig(z, v, start, index - 1);
			maakevenwichtig(z, v, index + 1, einde);
		}
	}

	//Schrijfmethode om de boom proper uit te printen.
#pragma region PrettyPrint

/*
	Niet mijn code. Als je veel knopen hebt is dit niet aangeraden, dan gaat het bijna altijd de mist in. (en eigenlijk ist bij grote bomen ook niet intressant...)
	Gevonden op: http://stackoverflow.com/questions/801740/c-how-to-draw-a-binary-tree-to-the-console
*/
	template <class Sleutel, class Data>
	void Zoekboom<Sleutel, Data>::schrijf() const {
		char s[prettyprintheight][prettyprintwidth];
		char ss[prettyprintwidth];
		for (int i = 0; i < prettyprintheight; i++) {
			sprintf(ss, "%%%is", prettyprintwidth - 1);
			sprintf(s[i], ss, " ");
		}


		schrijf(false, 0, 0, s);

		for (int i = 0; i < prettyprintheight && !lijnisleeg(s[i]); i++) {
			printf("%s\n", s[i]);
		}
	}

	bool lijnisleeg(char* lijn) {
		char* loper = lijn;
		for (int i = 0; i < prettyprintwidth - 1; i++) {
			if (*loper != ' ')
				return false;
			loper++;
		}
		return true;
	}
	template <class Sleutel, class Data>
	int Zoekboom<Sleutel, Data>::schrijf(bool is_left, int offset, int depth, char s[prettyprintheight][prettyprintwidth]) const {
		char b[prettyprintwidth];
		int width = 5;

		if (!*this) return 0;
		sprintf(b, "(%03i)", (*this)->sleutel);

		int left = (*this)->links.schrijf(1, offset, depth + 1, s);
		int right = (*this)->rechts.schrijf(0, offset + left + width, depth + 1, s);

		for (int i = 0; i < width; i++)
			s[2 * depth][offset + left + i] = b[i];
		if (depth && is_left) {
			for (int i = 0; i < width + right; i++)
				s[2 * depth - 1][offset + left + width / 2 + i] = '-';
			s[2 * depth - 1][offset + left + width / 2] = '+';
			s[2 * depth - 1][offset + left + width + right + width / 2] = '+';
		}
		else if (depth && !is_left) {
			for (int i = 0; i < left + width; i++)
				s[2 * depth - 1][offset - width / 2 + i] = '-';

			s[2 * depth - 1][offset + left + width / 2] = '+';
			s[2 * depth - 1][offset - width / 2 - 1] = '+';
		}
		return left + width + right;
	}
#pragma endregion

	//Hulpfunctie die de boom doorloopt en elke knoop in een vector stopt. Wordt gebruikt in maakevenwichtig() en maakonevenwichtig();
	template <class Sleutel, class Data>
	std::vector<std::pair<Sleutel, Data> > Zoekboom<Sleutel, Data>::vectorize() {
		std::vector<std::pair<Sleutel, Data> > v;
		std::stack<Zoekboom<Sleutel, Data>* > s;
		Zoekboom<Sleutel, Data>* hulp;

		s.push(this);
		while (!s.empty()) {
			hulp = s.top();
			s.pop();
			v.push_back(std::pair<Sleutel, Data>((*hulp)->sleutel, (*hulp)->data));

			if ((*hulp)->links)	s.push(&(*hulp)->links);
			if ((*hulp)->rechts) s.push(&(*hulp)->rechts);

		}
		return v;
	}

	//Roteren van de boom naar links (true) of naar rechts (false)
	template <class Sleutel, class Data>
	void Zoekboom<Sleutel, Data>::roteer(bool links) {
		Zoekboom<Sleutel, Data> hulp(std::move((*this)->getChild(!links)));
		hulp.swap(*this);
		hulp.swap((*this)->getChild(links));
		hulp.swap((*this)->getChild(links)->getChild(!links));
		//ouderpointers juist zetten
		(*this)->ouder = (*this)->getChild(links)->ouder;
		(*this)->getChild(links)->ouder = this->get();
		if ((*this)->getChild(links)->getChild(!links))
			(*this)->getChild(links)->getChild(!links)->ouder = (*this)->getChild(links).get();
	}

	//Zoeken in de boom naar een knoop met een bepaalde sleutel. 
	template <class Sleutel, class Data>
	void Zoekboom<Sleutel, Data>::zoek(const Sleutel& sleutel, Zoekknoop<Sleutel, Data>*& ouder, Zoekboom<Sleutel, Data>*& plaats) {
		plaats = this;
		while (*plaats && (*plaats)->sleutel != sleutel) {
			ouder = plaats->get();
			plaats = &(*plaats)->getChild((*plaats)->sleutel > sleutel);
		};
	};

	//Berekenen van de potentiaal van een boom. Hierbij is potentiaal(boom) = potentiaal(linkerdeelboom) + potentiaal(rechterdeelboom) + ²log(#kinderen + 1)
	template <class Sleutel, class Data>
	double Zoekboom<Sleutel, Data>::potentiaal() {
		int a = 0;
		return potentiaal(a);
	}
	template <class Sleutel, class Data>
	double Zoekboom<Sleutel, Data>::potentiaal(int& a) {
		if (*this) {
			int l = a, r = a;
			double linksepot = (*this)->links.potentiaal(l);
			double rechtsepot = (*this)->rechts.potentiaal(r);
			a = l + r + 1;
			return linksepot + rechtsepot + log2(a);
		}
		else {
			return 0.0;
		}
	}

#pragma endregion

}
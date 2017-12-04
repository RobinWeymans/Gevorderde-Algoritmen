#pragma once

#include <memory>
using std::unique_ptr;
#include <stack>
using std::stack;

#define ROOD		0
#define ZWART		1
#define DUBBELZWART 2

#define prettyprintwidth 500
#define prettyprintheight 500

#ifndef bottomUp
#define bottomUp 0
#endif // !bottomUp

#ifndef showNullLeaves
#define showNullLeaves 0
#endif // !showNullLeaves



namespace efficiente_zoekbomen {

	template <class Sleutel, class Data>
	class RZknoop;

	template <class Sleutel, class Data>
	class RZboom : public unique_ptr<RZknoop<Sleutel, Data>> {
		using unique_ptr<RZknoop<Sleutel, Data>>::unique_ptr;
	public:
		RZboom() {} //Default Constructor
		RZboom(RZboom<Sleutel, Data>&& z) : unique_ptr<RZknoop<Sleutel, Data>>(move(z)) {}
		RZboom& operator=(RZboom&& z);
		friend std::ostream& operator<<(std::ostream& os, const RZboom<Sleutel, Data>& boom) { boom.schrijf();	return os; }

		void voegtoe(const Sleutel&, const Data&);
		void verwijder(const Sleutel&);
		RZboom<Sleutel, Data>* zoek(const Sleutel&);

	protected:
		void zoek(const Sleutel&, RZknoop<Sleutel, Data>*&, RZboom<Sleutel, Data>*&);
		void zoek(const Sleutel&, RZknoop<Sleutel, Data>*&, RZboom<Sleutel, Data>*&, std::stack<bool>&, std::stack<RZboom<Sleutel, Data>*>&);

		RZboom<Sleutel, Data>* min();
		RZboom<Sleutel, Data>* max();
		RZboom<Sleutel, Data>* getSibling() { //Preconditie: ouder moet bestaan
			return (*this)->ouder->links == *this ? &(*this)->ouder->rechts : &(*this)->ouder->links;
		}

		void schrijf() const;
		int schrijf(bool, int, int, char s[prettyprintheight][prettyprintwidth]) const;
		void roteer(bool);
	private:
		bool lijnisleeg(char*) const; //Niet belangrijk (wordt gebruikt om de boom een beetje proper uit te printen..)
	};

	template <class Sleutel, class Data>
	class RZknoop {
		friend class RZboom<Sleutel, Data>;
	public:
		Sleutel& getSleutel() { return sleutel; }
		Data& getData() { return data; }
		RZboom<Sleutel, Data>& getChild(bool linkerkind) { return linkerkind ? links : rechts; }
		RZboom<Sleutel, Data>& getChild() { return links && !links->null ? links : rechts; }
		void setKleur(int kleur) { this->kleur = kleur; }

	protected:
		Sleutel sleutel;
		int kleur; //Kan geen bool zijn want 3 kleuren (rood, zwart & dubbelzwart)
		bool null;
		Data data;

		RZknoop<Sleutel, Data>* ouder;
		RZboom<Sleutel, Data> links, rechts;
		RZknoop() : ouder(0), kleur(1), null(1) {} //Default constructor is een null-knoop
		RZknoop(const Sleutel& sl, const Data& d) : ouder(0), kleur(0), sleutel(sl), data(d), null(0) {
			links = RZboom<Sleutel, Data>(new RZknoop<Sleutel, Data>());
			rechts = RZboom<Sleutel, Data>(new RZknoop<Sleutel, Data>());
		}
	};



#pragma region Implementatie RZboom

	//Move operator
	template<class Sleutel, class Data>
	RZboom<Sleutel, Data>& RZboom<Sleutel, Data>::operator=(RZboom&& z) {
		unique_ptr<RZknoop<Sleutel, Data>>::operator=(move(z));
		return *this;
	}
	//Toevoegen van een knoop
	template<class Sleutel, class Data>
	void RZboom<Sleutel, Data>::voegtoe(const Sleutel& sleutel, const Data& data) {
#if bottomUp
		//Bottom up implementatie

		//Stap 1: Toevoegen zoals bij een gewone zoekboom (maar pad moet bijgehouden worden). Een toegevoegde knoop is standaard ROOD.
		RZboom<Sleutel, Data>* loper = this;
		RZknoop<Sleutel, Data>* ouder = nullptr;
		std::stack<RZboom<Sleutel, Data>* > bomen;
		std::stack<bool					  > richtingen;

		zoek(sleutel, ouder, loper, richtingen, bomen);

		if (!*loper || (*loper)->null) {
			RZboom<Sleutel, Data> nieuw(new RZknoop<Sleutel, Data>(sleutel, data));
			nieuw->ouder = ouder;
			*loper = move(nieuw);
		}
		else {
			return;
		}

		//Stap 2: De zwarte hoogte herstellen.
		RZboom<Sleutel, Data> *p, *g, *u;
		bool cZitLinks, pZitLinks, uIsZwart;

		bomen.pop();
		while (!richtingen.empty()) {
			if (richtingen.size() == 1) {
				richtingen.pop();
			}
			else {
				p = bomen.top();
				bomen.pop();
				g = bomen.top();
				bomen.pop();

				if ((*p)->kleur) //Als de ouder zwart is: GEEN PROBLEEM
					richtingen = std::stack<bool>();

				else {	//Als de ouder zwart is, dan moeten we de zwarte hoogte herstellen
					cZitLinks = richtingen.top();
					richtingen.pop();
					pZitLinks = richtingen.top();
					richtingen.pop();

					//Als het een zig zag situatie is moeten we er een zig zig situatie van maken
					//Een uncle die niet bestaat is hetzelfde als een zwarte nonkel
					uIsZwart = !(*g)->getChild(!pZitLinks) || (*g)->getChild(!pZitLinks)->kleur;
					u = &(*g)->getChild(!pZitLinks);
					if (uIsZwart) { //roteer over grootouder en verander p, g en u van kleur
						if (cZitLinks != pZitLinks) //Zig-zag situatie omvormen naar zig-zig situatie
							p->roteer(!cZitLinks);
						g->roteer(!pZitLinks);
						richtingen = std::stack<bool>();
					}
					else {
						(*g)->setKleur(ROOD);
						(*p)->setKleur(ZWART);
						if (*u) (*u)->setKleur(ZWART);
					}
				}
			}
		}
		(*this)->setKleur(ZWART);
#else
		//Top-down implementatie http://www.eternallyconfuzzled.com/tuts/datastructures/jsw_tut_rbtree.aspx
		RZboom<Sleutel, Data> *grandparent = nullptr, *parent = nullptr, *nieuw = this;
		bool richting = false, last;

		while (true) { //Niet het mooiste, maar anders lukte het niet direct.
			if (*nieuw == nullptr || (*nieuw)->null)
				*nieuw = std::move(RZboom<Sleutel, Data>(new RZknoop<Sleutel, Data>(sleutel, data)));

			if ((*nieuw)->getChild(true)->kleur == ROOD && (*nieuw)->getChild(false)->kleur == ROOD) {	//        Kleuren omdraaien aka COLOR SWITCH
				(*nieuw)->kleur = ROOD;																	//		Z					R
				(*nieuw)->links->kleur = ZWART;															//	  /   \		wordt	  /   \ 
				(*nieuw)->rechts->kleur = ZWART;														//  R       R			Z       Z
			}
			if (nieuw && *nieuw && (*nieuw)->kleur == ROOD && parent && *parent && (*parent)->kleur == ROOD) {	// nieuw en parent mogen niet allebei rood zijn (q is een kind van p)
				if (nieuw == &(*parent)->getChild(last)) //Enkele rotatie rond de grootouder
					grandparent->roteer(!last);
				else {	//Dubbele rotatie. Eerst rond ouder, dan rond grootouder
					parent->roteer(!richting);
					grandparent->roteer(!last);
				}
			}

			if ((*nieuw)->sleutel == sleutel) break;

			//Volgende iteratie voorbereiden
			last = richting;
			richting = (*nieuw)->sleutel > sleutel;
			grandparent = parent;
			parent = nieuw;
			nieuw = &(*nieuw)->getChild(richting);
		}
		(*this)->kleur = ZWART;
#endif
	}
	//Verwijderen van een knoop
	template<class Sleutel, class Data>
	void RZboom<Sleutel, Data>::verwijder(const Sleutel& sleutel) {
#if bottomUp
		//BottomUp implementatie

		//Stap 1: verwijderen zoals bij een gewone BST, wordt vervangen door zijn voorganger (de grootste van zijn linker deelboom). Daarbij is u de te verwijderen knoop, en v de knoop die zijn plaats in neemt.
		std::stack<RZboom<Sleutel, Data>* > bomen;
		std::stack<bool					  > richtingen;
		RZboom<Sleutel, Data>* v = this, *u = nullptr;

		bomen.push(v);
		while (*v && sleutel != (*v)->getSleutel()) {
			bool links = (*v)->getSleutel() > sleutel;
			v = &(*v)->getChild(links);
			richtingen.push(links);
			bomen.push(v);
		}

		if ((*v)->links && !(*v)->links->null && (*v)->rechts && !(*v)->rechts->null) {//De te verwijderen knoop heeft twee kinderen
			u = (*v)->links.max();
			(*v)->sleutel = (*u)->sleutel;
			(*v)->data = (*u)->data;
			richtingen.push(true);
			(*v)->links.verwijder((*(*v)->links.max())->sleutel);
			u = &(*v)->links;
		}
		else {
			bool links = true;
			if ((*v)->getChild(links)->null) links = !links;
			u = &(*v)->getChild(links);

			if ((*v)->kleur == ROOD || (*u)->kleur == ROOD) {
				RZknoop<Sleutel, Data>* hulp = (*v)->ouder;
				*v = std::move(*u);
				(*v)->ouder = hulp;
				(*v)->kleur = ZWART;
			}
			else {
				(*u)->kleur = DUBBELZWART;
				(*u)->ouder = (*v)->ouder;
				v->swap(std::move(*u));
			}
			if (!bomen.empty()) bomen.pop();
			u = v;
		}

		while ((*u)->kleur == DUBBELZWART && (*u)->ouder != 0 && !richtingen.empty()) { //Zolang u dubbelzwart is en hij geen root is
			/*schrijf(); std::cout << std::endl;*/
			bool uZitLinks = &(*u)->ouder->links == u;
			RZboom<Sleutel, Data>* s = u->getSibling();
			bool sIsBlack = (bool)(*s)->kleur;
			if (sIsBlack) {
				bool rBestaat = !(*s)->getChild(true)->kleur || !(*s)->getChild(false)->kleur;
				if (rBestaat) {
					bool rZitLinks = (*s)->getChild(true)->kleur == ROOD;
					RZboom<Sleutel, Data>* r = &(*s)->getChild(rZitLinks);
					if (uZitLinks == rZitLinks) {
						s->roteer(!rZitLinks);
						(*s)->kleur = ZWART;
						(*s)->getChild(!rZitLinks)->kleur = ROOD;
						rZitLinks = !rZitLinks;
					}
					RZboom<Sleutel, Data>* p = bomen.top();
					bomen.pop();
					int pKleur = (*p)->kleur;
					int sKleur = (*s)->kleur;
					p->roteer(uZitLinks);
					(*p)->kleur = ZWART;
					(*r)->kleur = ZWART;
					(*p)->getChild(uZitLinks)->getChild(uZitLinks)->kleur = ZWART;
				}
				else {
					RZboom<Sleutel, Data>* p = bomen.top();
					bomen.pop();
					richtingen.pop();
					(*p)->kleur++;
					(*u)->kleur = ZWART;
					(*s)->kleur = ROOD;
					u = p;
				}
			}
			else {
				RZboom<Sleutel, Data>* p = bomen.top();
				p->roteer(uZitLinks);
				std::swap((*p)->kleur, (*p)->getChild(uZitLinks)->kleur);
				bomen.push(&(*p)->getChild(uZitLinks));
				richtingen.push(uZitLinks);
			}

		}
		if ((*u)->ouder == 0)(*this)->setKleur(ZWART);
#else
		//Top down implementatie
		/*
		//Fictieve rode wortel bovenin de boom stoppen.
		RZboom<Sleutel, Data> f(new RZknoop<Sleutel, Data>());
		swap(f);
		(*this)->getChild(true).swap(f);

		//Hulpvariabelen
		bool richting = true, last;
		RZboom<Sleutel, Data> *g = nullptr, *p = this, *c = &(*p)->getChild(richting);
		RZknoop<Sleutel, Data>* gevonden = nullptr;
		if ((*c)->sleutel == sleutel) gevonden = c->get();

		//Loop that ends when the bottom of the tree is reached.
		while (*c != nullptr || (*c)->null == true) {
			if ((*c)->sleutel == sleutel) gevonden = c->get();


		}
		*/

		if (*this && !(*this)->null) { //Als *this naar 'iets' wijst, en 'iets' geen null-knoop is
			//Knoop in de wortel ROOD maken zodat we die rode kleur naar beneden kunnen duwen.
			(*this)->kleur = ROOD;

			//Hulpvariabelen instellen
			int lastrichting, richting = (sleutel <= (*this)->sleutel);
			RZboom<Sleutel, Data> *grandparent = nullptr, *parent = this, *loper = &(*this)->getChild(richting), *sibling = &(*this)->getChild(!richting);
			RZknoop<Sleutel, Data>* gevonden = nullptr;
			if ((*parent)->sleutel == sleutel) gevonden = parent->get();

			//Zoeken naar de te verwijderen knoop die meteen de voorloper/vervanger zoekt.
			while ((*loper)->null == false) {
				//Gevonden knoop bijhouden zodat we verder kunnen zoeken naar de voorloper.
				if ((*loper)->sleutel == sleutel) gevonden = loper->get();

				//Cases bepalen
				if ((*loper)->kleur == ROOD && (*sibling)->kleur == ROOD) {
					(*parent)->kleur = ZWART;
				} else if ((*loper)->getChild(true)->kleur == ROOD || (*loper)->getChild(false)->kleur == ROOD) { //Als één van zijn kinderen ROOD is.
				   //Case 2
				   //roteren over loper naar de kant met het zwarte kind.
					bool links = (*loper)->getChild(true)->kleur == ZWART;
					loper->roteer(links);
					if ((*loper)->sleutel == sleutel) gevonden = loper->get();
					loper = &(*loper)->getChild(links);

				}
				else {//Als zijn kinderen beide ZWART zijn.
					if ((*sibling)->getChild(true)->kleur == ZWART && (*sibling)->getChild(false)->kleur == ZWART) { //Kinderen van zijn sibling zijn beide zwart
						//Color switch
						(*parent)->kleur = ZWART;
						(*parent)->getChild(true)->kleur = ROOD;
						(*parent)->getChild(false)->kleur = ROOD;
					}
					else { //Een van de kinderen is rood
					 //Case 3-4
						if ((*sibling)->getChild(!richting)->kleur == ZWART) {
							//Case 4: dubbele rotatie om het op te lossen
							loper->roteer(!richting);
							parent->roteer(richting);
						}
						else {
							//Case 3: enkele rotatie om het op te lossen
							parent->roteer(richting);
						}
						(*parent)->kleur = ROOD;
						(*parent)->getChild(!richting)->kleur = ZWART;
						(*parent)->getChild(richting)->kleur = ZWART;
						(*parent)->getChild(richting)->getChild(richting)->kleur = ROOD;
					}
				}
				schrijf(); std::cout << std::endl;
				//Hulpvariabelen updaten voor volgend luske
				lastrichting = richting, richting = sleutel <= (*loper)->sleutel;
				grandparent = parent, parent = loper, loper = &(*parent)->getChild(richting), sibling = &(*parent)->getChild(!richting);
			}
			//Als de knoop niet gevonden is, dan stap je gewoon uit de funtie
			if (!gevonden) return;

			//Voorloper (die nu in de parent zit, want loper is een nullknoop geworden) kopieren in de te verwijderen node en vervolgens verwijderen.
			gevonden->sleutel = (*parent)->sleutel;
			gevonden->data = (*parent)->data;
			*parent = std::move(RZboom<Sleutel, Data>(new RZknoop<Sleutel, Data>()));

			//Wortel zeker terug ZWART maken
			(*this)->kleur = ZWART;

		}
#endif
	}
	//Het zoeken van de kleinste sleutel in de zoekboom. Geeft een pointer terug naar een zoekboom waar deze kleinste knoop de wortel van is.
	template <class Sleutel, class Data>
	RZboom<Sleutel, Data>* RZboom<Sleutel, Data>::min() {
		RZboom<Sleutel, Data>* loper = this;
		while (*loper && (*loper)->links && !(*loper)->links->null)
			loper = &(*loper)->links;
		return loper;
	}
	//Het zoeken van de grootste sleutel in de zoekboom. Geeft een pointer terug naar een zoekboom waar deze grootste knoop de wortel van is.
	template <class Sleutel, class Data>
	RZboom<Sleutel, Data>* RZboom<Sleutel, Data>::max() {
		RZboom<Sleutel, Data>* loper = this;
		while (*loper && (*loper)->rechts && !(*loper)->rechts->null)
			loper = &(*loper)->rechts;
		return loper;
	}
	//Roteren naar links (true) of rechts (false) van de huidige boom. Verschil met roteren in gewone zoekbomen: Kleuren worden ook aangepast. De nieuwe root wordt ZWART, de oude wordt ROOD.
	template<class Sleutel, class Data>
	void RZboom<Sleutel, Data>::roteer(bool links) {
		RZboom<Sleutel, Data> hulp(std::move((*this)->getChild(!links)));
		hulp.swap(*this);
		hulp.swap((*this)->getChild(links));
		hulp.swap((*this)->getChild(links)->getChild(!links));
		//ouderpointers juist zetten
		(*this)->ouder = (*this)->getChild(links)->ouder;
		(*this)->getChild(links)->ouder = this->get();
		if ((*this)->getChild(links)->getChild(!links))
			(*this)->getChild(links)->getChild(!links)->ouder = (*this)->getChild(links).get();
		//kleuren aanpassen
		(*this)->kleur = ZWART;
		(*this)->getChild(links)->kleur = ROOD;
	}
	//Zoeken in een rood-zwarte boom is hetzelfde als zoeken in een normale zoekboom
	template<class Sleutel, class Data>
	RZboom<Sleutel, Data>* RZboom<Sleutel, Data>::zoek(const Sleutel& sleutel) {
		RZboom<Sleutel, Data>* plaats;
		RZknoop<Sleutel, Data>* ouder;
		zoek(sleutel, ouder, plaats);
		return plaats;
	}
	//Recursieve hulpfunctie die gebruikt wordt tijdens het zoeken
	template<class Sleutel, class Data>
	void RZboom<Sleutel, Data>::zoek(const Sleutel& sleutel, RZknoop<Sleutel, Data>*& ouder, RZboom<Sleutel, Data>*& plaats) {
		plaats = this;
		while (*plaats && !(*plaats)->null && (*plaats)->sleutel != sleutel) {
			ouder = plaats->get();
			plaats = &(*plaats)->getChild((*plaats)->sleutel > sleutel);
		};
	}
	//Zoekt naar een knoop met een gegeven sleutel, en houd daarbij het pad bij (zowel boom pointers als richtingen)
	template<class Sleutel, class Data>
	void RZboom<Sleutel, Data>::zoek(const Sleutel& sleutel, RZknoop<Sleutel, Data>*& ouder, RZboom<Sleutel, Data>*& loper, std::stack<bool>& richtingen, std::stack<RZboom<Sleutel, Data>*>& bomen) {
		bomen.push(loper);
		while (*loper && !(*loper)->null && (*loper)->getSleutel() != sleutel) {
			bool res = (*loper)->getSleutel() > sleutel;
			ouder = loper->get();
			loper = &(*loper)->getChild(res);
			richtingen.push(res);
			bomen.push(loper);
		}
	}
	
	//Functie om RoodZwarte bomen op een propere manier af te drukken. Niet proberen met extreem grote bomen, want dat gaat toch niet.
	#pragma region RZboom prettyprint
	template<class Sleutel, class Data>
	void RZboom<Sleutel, Data>::schrijf() const {
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
	template<class Sleutel, class Data>
	bool RZboom<Sleutel, Data>::lijnisleeg(char* lijn) const {
		char* loper = lijn;
		for (int i = 0; i < prettyprintwidth - 1; i++) {
			if (*loper != ' ')
				return false;
			loper++;
		}
		return true;
	}
	template<class Sleutel, class Data>
	int RZboom<Sleutel, Data>::schrijf(bool is_left, int offset, int depth, char s[prettyprintheight][prettyprintwidth]) const {
		char b[prettyprintwidth];
		int width = 7;

		if (!*this)
			return 0;

		#if !showNullLeaves
		if ((*this)->null == true && (*this)->kleur != DUBBELZWART)
			return 0;
		#endif

		if ((*this)->null == true) {
			sprintf(b, "%c NULL ", (*this)->kleur == DUBBELZWART ? 'D' : ' ');
		}
		else {
			char c;
			if ((*this)->kleur == ZWART) c = 'Z';
			else if ((*this)->kleur == ROOD) c = 'R';
			else if ((*this)->kleur == DUBBELZWART) c = 'D';
			sprintf(b, "(%c-%03i)", c, (*this)->sleutel);
		}

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

#pragma endregion

}
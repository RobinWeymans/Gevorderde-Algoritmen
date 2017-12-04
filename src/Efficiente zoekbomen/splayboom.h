#pragma once

#include "zoekboom11.h"

namespace efficiente_zoekbomen {

	template <class Sleutel, class Data>
	class Splayboom : public Zoekboom<Sleutel, Data> {
		using Zoekboom<Sleutel, Data>::Zoekboom;
	public:
		void voegtoe(const Sleutel& sleutel, const Data& data) override {
			Zoekboom<Sleutel, Data>::voegtoe(sleutel, data);
			int i = 0;
			splay(sleutel, i);
		}
		void zoek(const Sleutel& sleutel, int& zoekdiepte) {
			splay(sleutel, zoekdiepte);
		}
	protected:
		void splay(const Sleutel& sleutel, int& zd) {
			/*
				1. Zoeken naar de sleutel
				2.  stack bijhouden van onderweg bezochte knopen
				3.  stack bijhouden van de richting die je bent uitgegaan

				Beginsituatie:
					twee stacks bomen en richtingen (zie hierboven). In bomen wordt de pointer plaats al eens gepusht.
					pointer plaats die initieel wijst naar de wortel van de boom
			*/
			std::stack<Zoekboom<Sleutel, Data>* > bomen;
			std::stack<bool						> richtingen;
			Zoekboom<Sleutel, Data>* loper = this;
			bomen.push(loper);

			while (*loper != 0 && (*loper)->getSleutel() != sleutel) {
				bool res = (*loper)->getSleutel() > sleutel;
				loper = &(*loper)->getChild(res);
				richtingen.push(res);
				bomen.push(loper);
				zd++;
			}
			if (!*loper) {
				bomen.pop();
				richtingen.pop();
			}
			/*
				Huidige situatie:
					bomen bevat alle zoekbomen richting de wortel
					richtingen bevat alle richtingen die zijn uitgegaan

				Naar boven splayen.
			*/

			bool cZitLinks, pZitLinks;
			Zoekboom<Sleutel, Data> *p, *g;
			while (!richtingen.empty()) {
				if (richtingen.size() == 1) { // ZIG geval
					cZitLinks = richtingen.top();
					roteer(!cZitLinks);
					richtingen.pop();
				}
				else { //ZIG-ZIG of ZIG-ZAG geval
					cZitLinks = richtingen.top();
					richtingen.pop();
					pZitLinks = richtingen.top();
					richtingen.pop();

					bomen.pop();
					p = bomen.top();
					bomen.pop();
					g = bomen.top();
					if (pZitLinks != cZitLinks) { //ZIG-ZAG geval
						p->roteer(!cZitLinks);
						g->roteer(!pZitLinks);
					}
					else { //ZIG-ZIG geval
						g->roteer(!pZitLinks);
						//Nog eens langs grootouder roteren, want de voormalige p is nu g geworden.
						g->roteer(!cZitLinks);
					}
				}
			}
		}
	};
}
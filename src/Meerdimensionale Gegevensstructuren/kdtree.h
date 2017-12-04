#pragma once

#include "../Util/punt2.h"
#include <memory>
using std::unique_ptr;
#include <queue>
using std::queue;
#include <math.h>
#include <stack>
using std::stack;

namespace meerdimensionale_gegevensstructuren {

	class Knoop2D;

	class Boom2D : public unique_ptr<Knoop2D> {
		using unique_ptr<Knoop2D>::unique_ptr;
	public:
		Boom2D();
		Boom2D(Boom2D&&);
		Boom2D(struct punt2, bool);
		Boom2D& operator=(Boom2D&&);

		void voegtoe(struct punt2&, bool);
		bool contains(const struct punt2&) const;
		int closest(const struct punt2&, struct punt2&);
	private:
		
	};

	class Knoop2D {
		friend class Boom2D;
	public:
		Knoop2D(struct punt2, bool);
		Boom2D& getChild(bool links);
	private:
		bool x; // true als er opgesplitst wordt in de x dimensie, false -> y dimensie
		struct punt2 punt;
		Boom2D links, rechts;
	};

	Boom2D::Boom2D() : unique_ptr<Knoop2D>() {}
	Boom2D::Boom2D(Boom2D&& b) : unique_ptr<Knoop2D>(std::move(b)) {}
	Boom2D::Boom2D(struct punt2 p, bool b) : unique_ptr<Knoop2D>(new Knoop2D(p, b)) {}
	Boom2D& Boom2D::operator=(Boom2D&& b) {
		unique_ptr<Knoop2D>::operator=(std::move(b));
		return *this;
	}
	Knoop2D::Knoop2D(struct punt2 p, bool dim) : punt(p), x(dim) {}
	Boom2D& Knoop2D::getChild(bool l) { return l ? links : rechts;	}
	void Boom2D::voegtoe(struct punt2& p, bool dim=false) {
		if (*this == nullptr) {
			*this = std::move(Boom2D(p, !dim));
		} else {
			bool b = (*this)->x ? (*this)->punt.x > p.x : (*this)->punt.y > p.y;
			(*this)->getChild(b).voegtoe(p, !dim);
		}
	}
	bool Boom2D::contains(const struct punt2& p) const {
		if (*this == nullptr)
			return false;
		else if ((*this)->punt == p)
			return true;
		else {
			bool b = (*this)->x ? (*this)->punt.x > p.x : (*this)->punt.y > p.y;
			return (*this)->getChild(b).contains(p);
		}
	}

	int Boom2D::closest(const struct punt2& p, struct punt2& closest){
		//Zakken in de boom tot als we aankomen in een blad.
		if (*this == nullptr) return 0;

		bool b = !(*this)->x ? (*this)->punt.y > p.y : (*this)->punt.x > p.x;
		int stappen = (*this)->getChild(b).closest(p, closest);

		//Blad bereikt. Vanaf hier gaan we terug naar boven in de recursie.
		//If the current node is closer than the current best, then it becomes the current best.
		if ((*this)->punt.kwadafstand(p) < closest.kwadafstand(p))	closest = (*this)->punt;

		//Check whether there could be any points on the other side of the splitting plane that are closer to the search point than the current best.
		if ((*this)->getChild(!b) != nullptr) {
			bool bb = (*this)->getChild(!b)->x ? abs((*this)->getChild(!b)->punt.x - p.x) < abs(closest.x - p.x) : abs((*this)->getChild(!b)->punt.y - p.y) < abs(closest.y - p.y);
			if (bb) {
				stappen += (*this)->getChild(!b).closest(p, closest);
			}
		}
		return stappen + 1;
	}

}
//int main() {
//	Boom2D boom;
//
//	ifstream file("Resources (iobestanden)\\puntenverz.txt");
//	int x, y;
//	while (file >> x >> y) {
//		boom.voegtoe(struct punt2(x, y));
//	}
//
//	for (int i = 0; i < 200; i += 10) {
//		for (int j = 0; j < 200; j += 10) {
//			struct punt2 p;
//			int a = boom.closest(struct punt2(i, j), p);
//			std::cout << struct punt2(i, j) << " heeft buur " << p << ", gevonden in " << std::setw(5) << a << " doorzochte knopen" << std::endl;
//		}
//	}
//
//	return 0;
//}

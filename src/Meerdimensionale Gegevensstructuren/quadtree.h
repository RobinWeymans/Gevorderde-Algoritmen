#pragma once

#include <memory>
using std::unique_ptr;

namespace meerdimensionale_gegevensstructuren {

	class QuadKnoop;

	class QuadTree : public unique_ptr<QuadKnoop> {
		using unique_ptr<QuadKnoop>::unique_ptr;
	public:
		QuadTree();
		QuadTree(QuadTree&& q);
		QuadTree& operator=(QuadTree&& q);

		void add(double x, double y);
	private:
	};

	class QuadKnoop {
		friend class QuadTree;
	public:
		QuadKnoop() {};
		QuadKnoop(double xco, double yco) : x(xco), y(yco) {}
	private:
		double x, y;
		QuadTree kwadranten[4];
	};

	QuadTree::QuadTree() {}
	QuadTree::QuadTree(QuadTree&& q) : unique_ptr<QuadKnoop>(std::move(q)) {}
	QuadTree& QuadTree::operator=(QuadTree&& q) { unique_ptr<QuadKnoop>::operator=(std::move(q)); return *this; };
	void QuadTree::add(double x, double y) {
		QuadTree* loper = this;
		while ((*loper) != nullptr) {
			if ((*loper)->x < x)
				if ((*loper)->y < y)
					loper = &(*loper)->kwadranten[1];
				else
					loper = &(*loper)->kwadranten[2];
			else
				if ((*loper)->y < y)
					loper = &(*loper)->kwadranten[0];
				else
					loper = &(*loper)->kwadranten[3];
		}
		*loper = std::move(QuadTree(new QuadKnoop(x, y)));
	}

}
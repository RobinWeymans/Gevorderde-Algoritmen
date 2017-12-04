#pragma once

#include <vector>
#include <memory>
using std::unique_ptr;
// Nuttige links: http://www.sanfoundry.com/cpp-program-implement-binomial-heap/

namespace prioriteitswachtrijen {

	template<class T, class D>
	class BinomiaalKnoop;

	template<class T, class D>
	class BinomialQueue : public unique_ptr<BinomiaalKnoop<T,D>>{
		using unique_ptr<BinomiaalKnoop< T, D>>::unique_ptr;
	public:

		BinomialQueue() {}
		BinomialQueue(BinomialQueue<T, D>&& b) : unique_ptr<BinomiaalKnoop<T, D>>(std::move(b)) {}
		BinomialQueue(BinomiaalKnoop<T, D>* b) : unique_ptr<BinomiaalKnoop<T, D>>(b) {}
		BinomialQueue& operator=(BinomialQueue<T, D>&& b) {
			std::unique_ptr<BinomiaalKnoop<T, D>>::operator=(std::move(b)); 
			return *this; 
		}

		void put(const T&, const D&);
		void merge(BinomialQueue<T, D>& b);
		D& min();
		void erase_min();
	private:
		int getOrder() { return (*this) ? 1 + (*this)->links.getOrder() : 0; }
		void simple_merge(BinomialQueue<T, D>& b);
		BinomialQueue<T, D>* knoop_min();
	};

	template<class T, class D>
	class BinomiaalKnoop{
		friend class BinomialQueue<T, D>;
	public:
		BinomiaalKnoop(const T& sl, const D& d) : sleutel(sl), data(d){}
	private:
		T sleutel;
		D data;

		BinomialQueue<T, D> links, broer;
	};

	template<class T, class D>
	void BinomialQueue<T, D>::put(const T& sl, const D& d) {
		merge(std::move(BinomialQueue<T, D>(new BinomiaalKnoop<T, D>(sl, d))));
	}

	template<class T, class D>
	void BinomialQueue<T, D>::merge(BinomialQueue<T, D>& b) {
		simple_merge(b); //Eerst simple merge

		//Daarna de verschillende binomiaalbomen combineren.
		BinomialQueue<T, D> *x = this, *next = &(*this)->broer;
		while (*next != nullptr) {
			if (x->getOrder() != next->getOrder()) {
				x = next, next = &(*next)->broer;
			}else {
				BinomialQueue<T, D> hulp(std::move(*next));
				if ((*x)->sleutel <= hulp->sleutel)
					(*x)->broer.swap(hulp->broer);
				else
					x->swap(hulp);
				(*x)->links.swap(hulp);
				(*x)->links->broer.swap(hulp);
				next = &(*x)->broer;
			}
		}

	}

	template<class T, class D>
	void BinomialQueue<T, D>::simple_merge(BinomialQueue<T, D>& b) {
		if (*this == nullptr) this->swap(b);
		else {
			if (b != nullptr) {
				if ( getOrder() >= b.getOrder())
					this->swap(b);
				(*this)->broer.simple_merge(b);
			}
		}
	}

	template<class T, class D>
	D& BinomialQueue<T, D>::min() {
		return (*knoop_min())->data;
	}

	template<class T, class D>
	BinomialQueue<T, D>* BinomialQueue<T, D>::knoop_min(){
		BinomialQueue<T, D> *min = this;
		for (BinomialQueue<T, D>* loper = &(*min)->broer; *loper != nullptr; loper = &(*loper)->broer)
			if ((*loper)->sleutel < (*min)->sleutel)
				min = loper;
		return min;
	}

	template<class T, class D>
	void BinomialQueue<T, D>::erase_min() {
		//Knoop ervan tussen halen.
		BinomialQueue<T, D> *q = knoop_min();
		BinomialQueue<T, D> b(std::move(*q));
		*q = std::move(b->broer);

		b = std::move(b->links);
		BinomialQueue<T, D> rest;
		while ( b != nullptr) {
			rest = std::move(b->broer);
			merge(b);
			b = std::move(rest);
		}

	}
	
}
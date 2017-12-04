#pragma once

#include <algorithm>
#include <memory>
using std::unique_ptr;

namespace prioriteitswachtrijen {

	template<class T>
	class LeftistKnoop;

	template<class T>
	class LeftistHeap : unique_ptr<LeftistKnoop<T>> {
		using unique_ptr<LeftistKnoop<T>>::unique_ptr;
	public:
		LeftistHeap(){}
		LeftistHeap& operator=(LeftistHeap<T>&& l) {
			unique_ptr<LeftistKnoop<T>>::operator=(std::move(l));
			return *this;
		}
		LeftistHeap(LeftistKnoop<T>* l) : unique_ptr<LeftistKnoop<T>>(l) {
			
		}
		void add(const T& value) {
			add(LeftistHeap<T>(new LeftistKnoop<T>(value)));
		}
		void add(LeftistHeap& l) {
			if (*this == nullptr)
				(*this) = std::move(l);
			else {
				if ((*this)->rechts && (*this)->rechts->value > l->value) //Als de knoop aan de rechterkan groter is dan de wortel van l
					(*this)->rechts.swap(l);
				(*this)->rechts.add(l);
				if ((*this)->rechts.getSValue() > (*this)->links.getSValue())
					(*this)->rechts.swap((*this)->links);
			}
		}
		T& top() { return (*this)->value; }
		void pop() { 
			LeftistHeap<T> r = (*this)->rechts;
			*this = (*this)->links;
			add(r);
		}
	private:
		int getSValue() {
			return *this ? 1 + std::min((*this)->links.getSValue(), (*this)->rechts.getSValue()) : 0;
		}

	};

	template<class T>
	class LeftistKnoop {
		friend class LeftistHeap<T>;
	public:
		LeftistKnoop(const T& v) : value(v) {}
	private:
		T value;
		LeftistHeap<T> links, rechts;
	};
}

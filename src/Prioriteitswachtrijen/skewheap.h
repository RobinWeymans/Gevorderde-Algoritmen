#pragma once

#include <memory>
using std::unique_ptr;

namespace prioriteitswachtrijen {
	template<class T>
	class SkewKnoop;
	template<class T>
	class SkewHeap : public unique_ptr<SkewKnoop<T>> {
		using unique_ptr<SkewKnoop<T>>::unique_ptr;
	public:
		SkewHeap() : unique_ptr<SkewKnoop<T>>() {}
		SkewHeap(SkewKnoop<T>* l) : unique_ptr<SkewKnoop<T>>(l) {}
		SkewHeap& operator=(SkewHeap&& s) {
			unique_ptr<SkewKnoop<T>>::operator=(std::move(s));
			return *this;
		}
		void add(const T& value) {
			add(SkewHeap<T>(new SkewKnoop<T>(value)));
		}
		void add(SkewHeap<T>& s) {
			if (*this == nullptr)
				(*this) = std::move(s);
			else {
				if ((*this)->value > s->value)
					this->swap(s);
				(*this)->links.swap((*this)->rechts);
				(*this)->links.add(s);
			}
		}
	private:
	};

	template<class T>
	class SkewKnoop {
		friend class SkewHeap<T>;
	public:
		SkewKnoop(const T& v) : value(v) {}
	private:
		T value;
		SkewHeap<T> links, rechts;
	};

}
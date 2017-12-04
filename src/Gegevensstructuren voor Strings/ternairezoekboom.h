#pragma once

#include <memory>
using std::unique_ptr;
#include <string>
using std::string;

namespace gegevensstructuren_voor_strings {

	template<int A>
	class TernaireZoekNode;

	template<int A>
	class TernaireZoekboom : public unique_ptr<TernaireZoekNode<A>>{
		using unique_ptr<TernaireZoekNode<A>>::unique_ptr;
	public:
		TernaireZoekboom() : unique_ptr<TernaireZoekNode<A>>() {}
		TernaireZoekboom(TernaireZoekNode<A>* node) :unique_ptr<TernaireZoekNode<A>>(std::move(node)) {}
		TernaireZoekboom& operator=(TernaireZoekboom<A>&& t) { unique_ptr<TernaireZoekNode<A>>::operator=(move(t)); return *this; }

		void add(const string& str) { add(str.c_str()); }
		void add(const char* c_str);
	private:
	};

	template<int A>
	class TernaireZoekNode {
		friend class TernaireZoekboom<A>;
	public:
		TernaireZoekNode(char c): c(c), eos(false) {}
	private:
		char c;
		bool eos;

		TernaireZoekboom<A> links, midden, rechts;
	};
	
	template<int A>
	void TernaireZoekboom<A>::add(const char* c_str) {
		if (*this == nullptr) {
			*this = std::move(TernaireZoekboom<A>(new TernaireZoekNode<A>(*c_str)));
			(*this).add(c_str);
		}
		else {
			if (*c_str == '\0')
				(*this)->eos = true;
			else {
				if (*c_str == (*this)->c) {
					(*this)->midden.add(c_str + 1);
				}
				else if (*c_str < (*this)->c) {
					(*this)->links.add(c_str);
				}else{
					(*this)->rechts.add(c_str);
				}
			}
		}
	}

}

/*
int main()
{
	//Bijzonder slecht getest!!!!
	TernaireZoekboom<256> t;
	t.add("cute");
	t.add("cup");
	t.add("at");

	return 0;
}
*/
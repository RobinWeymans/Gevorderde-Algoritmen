#pragma once

#include <memory>
using std::unique_ptr;
#include <string>
using std::string;
#include <vector>
using std::vector;


namespace gegevensstructuren_voor_strings {

	template<int A>
	class PatriciaNode;

	template<int A>
	class PatriciaTrie : public unique_ptr<PatriciaNode<A>> {
		using unique_ptr<PatriciaNode<A>>::unique_ptr;
	public:
		PatriciaTrie() : unique_ptr<PatriciaNode<A>>() {}
		PatriciaTrie(PatriciaNode<A>* node) :unique_ptr<PatriciaNode<A>>(std::move(node)) {}
		PatriciaTrie& operator=(PatriciaTrie<A>&& t) { unique_ptr<PatriciaNode<A>>::operator=(move(t)); return *this; }

		void add(const string& str) { add(str.c_str()); }
		void add(const char* c_str);

		void schrijf(string s="");
	};
	template<int A>
	class PatriciaNode {
		friend class PatriciaTrie<A>;
	public:
		PatriciaNode(const char* c_str) : s(c_str), isLeaf(true), children(A) {}
		PatriciaNode(const string& s="") : PatriciaNode(s.c_str()) {}
	private:
		string s;
		vector<PatriciaTrie<A>> children;
		bool isLeaf;

	};

	template<int A>
	void PatriciaTrie<A>::add(const char* c_str) {
		if (*this == nullptr)
			*this = std::move(PatriciaTrie<A>(new PatriciaNode<A>(c_str)));
		else{
			int matches = 0;
			while (matches < strlen(c_str) && matches <= (*this)->s.size() && (*this)->s[matches] == *(c_str + matches))	matches++;

			if (matches == (*this)->s.size() && matches < strlen(c_str)) {
				if((*this)->isLeaf)
					(*this)->children[0] = std::move(PatriciaTrie<A>(new PatriciaNode<A>()));
				(*this)->isLeaf = false;
				(*this)->children[(int)*(c_str + matches)].add(c_str + matches);
			}
			else if(matches < (*this)->s.size()) {
				vector<PatriciaTrie<A>> backup(A);
				bool leafBackup = (*this)->isLeaf;
				backup.swap((*this)->children);
				(*this)->children[(int)((*this)->s)[matches]]= std::move(PatriciaTrie<A>(new PatriciaNode<A>((*this)->s.substr(matches))));
				(*this)->children[(int)((*this)->s)[matches]]->children.swap(backup);
				(*this)->children[(int)((*this)->s)[matches]]->isLeaf = leafBackup;
				(*this)->s = (*this)->s.substr(0, matches);
				(*this)->isLeaf = false;
				(*this)->children[(int)*(c_str + matches)].add(c_str + matches);
			}
		}
	}

	template<int A>
	void PatriciaTrie<A>::schrijf(string str = "") {
		if (*this == nullptr) return;
		if ((*this)->isLeaf == true)
			std::cout << str + (*this)->s << std::endl;
		else
			for (int i = 0; i < (*this)->children.size(); i++)
				if((*this)->children[i])
					(*this)->children[i].schrijf(str + (*this)->s);
	}
}

/*int main()
{
	PatriciaTrie<128> t;
	t.add("a");
	t.add("abandon");
	t.add("ability");
	t.add("able");
	t.add("about");
	t.add("about");
	t.add("above");
	t.add("above");
	t.add("abroad");
	t.add("absence");
	t.add("absent");
	t.add("absolute");
	t.add("abstract");
	t.add("abstract");
	t.add("abuse");
	t.add("abuse");
	t.add("abusive");
	t.add("academic");
	t.add("accept");
	t.add("acceptable");
	t.add("acceptance");
	t.add("access");
	t.add("access");
	t.add("accident");
	t.add("accompany");
	t.add("according+to");
	t.add("account");
	t.add("account");
	t.add("accountant");
	t.add("accurate");
	
	t.schrijf();

	return 0;
	}*/
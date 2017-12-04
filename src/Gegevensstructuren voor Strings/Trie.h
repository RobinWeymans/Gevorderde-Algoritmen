#pragma once
#include <memory>
using std::unique_ptr;
#include <vector>
#include <string>
#include <iostream>

#define ALFABET_SIZE 256
#define END_CHARACTER '\0' //Een nullkarakter

/*Class: MEERWEGS TRIE, waarbij in elke knoop een m-wegs beslissing genomen moet worden.*/

namespace gegevensstructuren_voor_strings {
	template<int A> //Grootte van het alfabet
	class TrieNode;

	template<int A>
	class Trie : public unique_ptr<TrieNode<A>> {
		using unique_ptr<TrieNode<A>>::unique_ptr;
	public:
		Trie() {}
		Trie(TrieNode<A>* node) : unique_ptr<TrieNode<A>>(node) {};
		Trie& operator=(Trie&& t);

		void add(const std::string& str);
		void add(const char* c_str);
		bool contains(const std::string& s);
		bool contains(const char* c_str);

		void schrijf(std::string s);
	private:
	};
	template<int A>
	class TrieNode {
		friend class Trie<A>;
	public:
		TrieNode() : children(A) {}
	private:
		std::vector<Trie<A>> children; //Kan ook een gelinkte lijst zijn, zodat je het opslaan van al die nullpointers kan vermijden.
	};

	template<int A>
	Trie<A>& Trie<A>::operator=(Trie<A>&& t) {
		unique_ptr<TrieNode<A>>::operator=(std::move(t));
		return *this;
	}
	template<int A>
	void Trie<A>::add(const std::string& str) {
		add(str.c_str());
	}
	template<int A>
	void Trie<A>::add(const char* c_str) {
		if (!*this)
			*this = std::move(new TrieNode<A>());

		int index = (int)*c_str;
		if (*c_str != END_CHARACTER)
			(*this)->children[index].add(c_str + 1);
		else {
			(*this)->children[index] = std::move(new TrieNode<A>());
		}
	}
	template<int A>
	bool Trie<A>::contains(const std::string& s) {
		return contains(s.c_str());
	}
	template<int A>
	bool Trie<A>::contains(const char* c_str) {
		if (*this == nullptr) return false;
		int index = (int)*c_str;
		if (*c_str == END_CHARACTER && (*this)->children[(int)END_CHARACTER] != nullptr) 
			return true;
		else
			return (*this)->children[index].contains(c_str + 1);
	}
	template<int A>
	void Trie<A>::schrijf(std::string s="") {
		if (*this == nullptr) return;
		for (int i = 0; i < (*this)->children.size(); i++) 
			(*this)->children[i].schrijf(s+ (char)i );
		
		if ((*this)->children[0] != nullptr)
			std::cout << s << std::endl;
	}
}
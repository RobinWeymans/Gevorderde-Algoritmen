#pragma once

#include <memory>
using std::unique_ptr;
#include <vector>

#define END -123
#define GROOTTE_ALFABET 256

namespace indexeren_van_vaste_text {
	class SuffixNode;
	class SuffixTree : public unique_ptr<SuffixNode> {
		using unique_ptr<SuffixNode>::unique_ptr;
	public:
		SuffixTree() : unique_ptr<SuffixNode>() {}
		SuffixTree(char* text, int text_length);
		SuffixTree(SuffixNode* n) : unique_ptr<SuffixNode>(n) {}
		SuffixTree(SuffixTree&& t) : unique_ptr<SuffixNode>(std::move(t)) {}
		SuffixTree& operator=(SuffixTree&& t) { unique_ptr<SuffixNode>::operator=(std::move(t)); return *this; }

	private:
		char* text;
		int text_length;
	};
	class SuffixNode {
		friend class SuffixTree;
	public:
		SuffixNode(int start_, int stop_) : children(GROOTTE_ALFABET) { start = start_; stop = stop_; }
	private:
		int start, stop;
		SuffixTree* suffixLink;
		std::vector<SuffixTree> children;
	};
	SuffixTree::SuffixTree(char* text, int text_length) : text(text), text_length(text_length) {
		//Met dank aan den tushar. Werkt op zijn voorbeeld, maar zou kunnen dat het niet helemaal correct is.
		*this = std::move(SuffixTree(new SuffixNode(-1, text_length)));

		int remaining = 0;
		SuffixTree* activeNode = this; //Initieel root
		int activeEdge = -1;
		int activeLength = 0;

		for (int i = 0; i < text_length; i++) {
			SuffixTree** backup = nullptr;
			//Tushar: "When a fase starts, the first thing we do is increment our remaining, and we also increment end"
			remaining++;
			//Tushar: "Then we go into a while loop, while remaining is greater than zero"
			while (remaining > 0) {
				//Is there a path with x from activeNode
				if (activeLength == 0) {
					if ((*activeNode)->children[text[i]] == nullptr) { //No, so we create one
						(*activeNode)->children[text[i]] = std::move(SuffixTree(new SuffixNode(i, text_length)));
						remaining--;
					} else {
						activeEdge = (*activeNode)->children[text[i]]->start;
						activeLength++;
						break; //Short/Show stopper, whatever hij ook zegt
					}
				} else {
					if ((*activeNode)->children[text[activeEdge]]->stop >= (*activeNode)->children[text[activeEdge]]->start + activeLength) {
						if (text[(*activeNode)->children[text[activeEdge]]->start + activeLength] == text[i]) {
							activeLength++;
							break;
						}
						else {
							(*activeNode)->children[text[activeEdge]]->children[text[i]] = std::move(SuffixTree(new SuffixNode(i, text_length)));
							(*activeNode)->children[text[activeEdge]]->children[text[(*activeNode)->children[text[activeEdge]]->start + activeLength]] = std::move(SuffixTree(new SuffixNode((*activeNode)->children[text[activeEdge]]->start + activeLength, text_length)));
							(*activeNode)->children[text[activeEdge]]->stop = (*activeNode)->children[text[activeEdge]]->start + activeLength - 1;
							(*activeNode)->children[text[activeEdge]]->suffixLink = this;
							if (backup == nullptr)
								backup = &(*activeNode)->children[text[activeEdge]]->suffixLink;
							else {
								*backup = &(*activeNode)->children[text[activeEdge]];
								backup = &(*activeNode)->children[text[activeEdge]]->suffixLink;
							}
							remaining--;
							if ((*activeNode)->start != -1)
								activeNode = (*activeNode)->suffixLink;
							else {
								activeEdge++;
								activeLength--;
							}
						}
					}
					else {
						activeNode = &(*activeNode)->children[text[activeEdge]];
						activeEdge = (*activeNode)->children[text[i]]->start; //Hier denk ik dat er iets kan foutlopen. Maar die case behandeld hij nie in zijn filmke.
						activeLength = 1;
						break;
					}
				}
				
			}
		}
	}
}
/*
int main()
{
	SuffixTree t("xyzxyaxyz$", 10);
	return 0;
}
*/
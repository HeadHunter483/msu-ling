#pragma once
#pragma warning(disable:4996)
#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<unordered_map>
#include<cmath>
#include<algorithm>
#include"morph_definitions.h"

using namespace std;

string strtolower2(string&str);
string strtoupper2(string&str);

class Node {
public:
	bool isroot;			// флаг корня
	bool isdel;				// флаг удаления
	bool isint;				// флаг целого числа
	bool ispred;			// флаг предиката
	bool isfunc;			// флаг семантической функции
	int id;					// номер слова в предложении
	int num;				// слово, переведенное в целое число
	string word;			// название слово
	string init;			// начальная форма слова
	vector<string>morph;	// морфология слова
	vector<Node*>parent;	// вектор родителей слова
	vector<Node*>child;		// вектор детей слова
	vector<string>rel;		// вектор отношений к родителям
	int object_id;			// индекс объекта, если существует соответствующий, иначе -1
	Node() {
		isroot = false;
		isdel = false;
		isint = false;
		ispred = false;
		isfunc = false;
		id = -1;
		num = -1;
		word = "";
		init = "";
		object_id = -1;
	}
	~Node() {
		morph.clear();
		parent.clear();
		child.clear();
		rel.clear();
	}
	int find_morph(string&str) {	// возвращает индекс, если str найдено, иначе -1
		if (this->morph.size()) {
			for (int i = 0; i < this->morph.size(); i++) {
				if (strtolower2(str) == strtolower2(this->morph[i])) {
					return i;
					break;
				}
			}
		}
		return -1;
	}
};

class Tree {
//protected:
public:
	size_t sz;
	vector<vector<Node>>tree;
	Tree();
	Tree(int sent, vector<int>&word);
	~Tree() { tree.clear(); }
	int insert(string&word, string&init, vector<string>&morph, int sent, int par, string&rel, int id);
	virtual int remove(int sent, int word);
	size_t size();
	size_t sent_amt();
	size_t word_amt(int sent);
};

void read_from_file(ifstream&fin, Tree&tree);
void read_from_file2(string&filename, Tree*tree);

void strtolower(string&str);

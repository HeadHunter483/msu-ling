#pragma once
#include "dict.h"

class SemToken {
	bool isfromdict;
	string primary_word;
	vector<string>secondary_word;
	string name;
	DictToken token;
	vector<int>relation;
public:
	void setisfromdict(bool flag) { isfromdict = flag; }
	void setprimaryword(string&word) { primary_word = word; }
	void addsecondaryword(string&word) { secondary_word.push_back(word); }
	void setname(string&word) { name = word; }
	void settoken(DictToken&tok) {
		token.id = tok.id; 
		token.word = tok.word; 
		token.morph = tok.morph; 
		token.parent = tok.parent; 
		if(!tok.morph.empty()) 
			token.prop = tok.prop; 
		if(!tok.list.empty()) 
			token.list = tok.list; 
		if(!tok.func.empty()) 
			token.func = tok.func; 
		if(!tok.propval.empty()) 
			token.propval = tok.propval; }
	void addrelation(int rel) { relation.push_back(rel); }
	size_t getsecondarywordsize() { return secondary_word.size(); }
	string getname() { return name; }
	string getprimaryword() { return primary_word; }
	string getsecondarywordbyid(int id) { if (id < secondary_word.size()) return secondary_word[id]; else return ""; }
	bool fromdict() { return isfromdict; }
	size_t getrelationsize() { return relation.size(); }
	int getrelationbyid(int id) { if (id < relation.size()) return relation[id]; else return -1; }
	DictToken*gettoken() { return &token; }
	SemToken();
	~SemToken();
	string build();
};


class SemTree : protected Tree {
	vector<SemToken>object;					// вектор объектов (в разработке)
	unordered_map<string, Dict*>dict;		// словарь (в разработке)
	int visited;							// количество посещенных вершин
	int cursent;							// номер текущего предложения в обходе
protected:
	int handle_nonlex(Node*cur);			// модуль обработки слов типа nonlex
	int handle_opred(Node*cur);				// модуль обработки слов типа opred
	int handle_num(Node*cur);				// модуль обработки слов типа num
	int handle_noun(Node*cur);				// модуль обработки слов типа noun
	int handle_verb(Node*cur);				// модуль обработки слов типа verb
	int handle_adj(Node*cur);				// модуль обработки слов типа adj
	int DFSstep(Node*cur);
	void semDFS();
	int dictload();
public:
	SemTree();
	~SemTree();
	int analysis();
	string build_result();
};
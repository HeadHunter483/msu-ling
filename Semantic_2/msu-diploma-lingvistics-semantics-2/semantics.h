#pragma once

#include "dict.h"

class SemToken {
	int id;									// id of the SemToken
	bool isfromdict;						// indicator of SemToken is from one of the dictionaries
	string primary_word;					// primary word of SemToken
	vector<string>secondary_word;			// secondary words of SemToken
	string name;							// name of SemToken
	DictToken*token;						// DictToken of SemToken
	vector<int>relation;					// relations to secondary words of SemToken
	string val;								// value of the token
public:
	SemToken();
	SemToken(SemToken&right);
	~SemToken();
	void setId(size_t id);					// set SemToken id
	void setIsFromDict(bool flag);			// set SemToken isfromdict
	void setPrimaryWord(const string&word);		// set SemToken primary_word
	void addSecondaryWord(const string&word);		// add to SemToken secondary_word
	void setName(const string&word);				// set SemToken name
	void setToken(DictToken*tok);			// set SemToken DictToken
	void addRelation(int rel);				// add to SemToken relation
	void setVal(const string&val);				// set SemToken value
	size_t getId();							// get SemToken id
	size_t getSecondaryWordSize();			// get SemToken secondary_word size
	string getName();						// get SemToken name
	string getPrimaryWord();				// get SemToken primary_word
	string getSecondaryWordById(size_t id);	// get SemToken secondary_word by id
	bool fromDict();						// get SemToken isfromdict
	size_t getRelationSize();				// get SemToken relation size
	int getRelationById(size_t id);			// get SemToken relation by id
	DictToken*getToken();					// get SemToken DictToken
	string getVal();						// get SemToken val
	string build();							// build SemToken string representation
	vector<int> getRelations();				// get vector of relations
	vector<string> getSecondaryWords();		// get vector of secondary words
};

struct Node {
	bool isroot;			// indicator of root token
	bool isdel;				// indicator of deleted token
	bool isnum;				// indicator of token representation as number
	int id;					// id of the token
	string word;			// word of the token
	string infinitive;		// infinitive form of the word of the token
	vector<string>morph;	// morphology of the token
	Node*parent;			// pointer to parent of the token
	int parent_id;			// id of the parent of the token
	string relation;		// relation to the parent of the token
	int object_id;			// id of object in semantic tree if exist, else -1
};

class SemTree {
	size_t sentences_amount;				// amount of sentences in text
	vector<size_t>words_amount;				// amount of words in sentence
	vector<vector<Node*>>tree;				// syntax tree of the text (sentences in text:words in sentence)
	unordered_map<string, Dict*>dict;		// map of dictionaries
	vector<SemToken*>object;				// math objects found in tree
	size_t cur_sent;						// current sentence using in analysis

	int dictsLoad();						// loads dictionaries from files defined in "config.ini"; returns 0 if successful, else 1
	int treeLoad(ifstream&fin);				// loads syntax tree defined in "config.ini" (fin); returns 0 if successful, else error code defined in "Headers.h"

	int handleNonlex(Node*cur);				// handling nonlex-type tokens; returns 0 if successful
	int handleNoun(Node*cur);				// handling noun-type tokens; returns 0 if successful
	int handleVerb(Node*cur);				// handling verb-type tokens; returns 0 if successful
	int handleAdj(Node*cur);				// handling adjective-type tokens; returns 0 if successful
	int handleNum(Node*cur);				// handling num-type tokens; returns 0 if successful

	int buildChildRelations(Node*base, Node*cur, int sent, int i);
	int buildRelations();					// build relations between objects
public:
	SemTree();
	SemTree(ifstream&fin);
	~SemTree();
	size_t sentencesSize();					// returns amount of sentences
	size_t wordsSize(size_t sentence_num);	// returns amount of words in sentence #sentence_num
	int semDfs(Node*cur);					// depth first search for syntax tree (one dfs run per sentence)
	int analysis();							// start running semantic analysis of syntax tree
	string buildResult();						// building string representating result of semantic anasysis
};
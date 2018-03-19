#pragma once

#include "func.h"

class DictToken {
	int id;					// id of the token in dictionary
	string word;			// word of the token in dictionary
	string morph;			// morphology of the token in dictionary
	string type;			// type of the token (can be object or value)
	int parent;				// parent of the token in dictionary
	vector<string>prop;		// properties of the token in dictionary
	vector<string>propval;	// values of properties of the token in dictionary
	vector<string>list;		// list of related words of the token in dictionary
	vector<string>func;		// possible semantic function for the token
public:
	DictToken() {
		id = -1;
		word = "";
		morph = "";
		parent = -1;
	}
	~DictToken() {
		if (!prop.empty()) prop.clear();
		if (!propval.empty()) propval.clear();
		if (!list.empty()) list.clear();
		if (!func.empty()) func.clear();
	}

	void setId(int id);									// set id of the token
	void setWord(const string&word);					// set word of the token
	void setMorph(const string&morph);					// set morph of the token
	void setType(const string&type);					// set type of the token
	void setParent(int parent);							// set parent id of the token
	void addProp(const string&prop);					// add prop to prop vector of the token
	void setPropVal(size_t prop_num, const string&val);	// set propval to a prop of the token
	void addList(const string&list_elem);				// add element to the list of related words of the token
	void addFunc(const string&func);					// add element to the list of possible funcs of the token
	int getId();										// get id of the token
	string getWord();									// get word of the token
	string getMorph();									// get morph of the token
	string getType();									// get type of the token
	int getParent();									// get parent id of the token
	vector<string>getProps();							// get all props of the token
	string getPropByNum(size_t num);					// get prop by id
	string getPropByName(const string&name);					// get prop by name
	vector<string>getList();							// get all of the list elements of the token
	vector<string>getFuncs();							// get all of the func elements of the token
	string getPropValByNum(size_t num);					// get propval by prop id
	string getPropValByName(const string&name);			// get propval by prop name
	string getListByNum(size_t num);					// get list element by id
	string getListByName(const string&name);			// get list element by name
	string getFuncByNum(size_t num);					// get func by id
	string getFuncByName(const string&name);			// get func by name
	int getPropSize();									// get size of prop vector
	int getListSize();									// get size of list vector
	int getFuncSize();									// get size of func vector
};

class Dict {
	vector<DictToken>dict;											// vector of DictTokens
public:
	Dict();
	~Dict();
	Dict(const string&filename);
	void read_dict_from_file(const string&filename, Dict*dict);		// reading dictionary data from .xml dictionary file
	int findWord(const string&str);									// returns id of the token in dict if found, else -1
	DictToken*getTokenById(size_t id);								// returns token found by id if found, else empty token
	string findPropInWord(const string&word, const string&prop);	// returns propval of the word prop if found, else empty string
	bool findListInWord(const string&word, const string&list);		// returns element of list field of the world if found, else empty string
	int insert(DictToken&token);									// inserts token in dict; returns 0 if succesful, else 1
	size_t size();													// returns size of dict
	bool empty();													// returns true if dict is empty, else false
};
#include "semantics.h"

SemToken::SemToken() {
	isfromdict = false;
	primary_word = "";
	name = "";
	token = NULL;
	val = "";
}

SemToken::SemToken(SemToken&right) {
	this->id = right.getId();
	this->isfromdict = right.fromDict();
	this->token = right.getToken();
	this->name = right.getName();
	this->primary_word = right.getPrimaryWord();
	this->relation = right.getRelations();
	this->secondary_word = right.getSecondaryWords();
	this->val = right.val;
}

SemToken::~SemToken() {
	this->secondary_word.clear();
	this->relation.clear();
}

void SemToken::setId(size_t id) {
	this->id = id;
}

void SemToken::setIsFromDict(bool flag) { 
	this->isfromdict = flag; 
}

void SemToken::setPrimaryWord(const string&word) {
	this->primary_word = word; 
}

void SemToken::addSecondaryWord(const string&word) {
	this->secondary_word.push_back(word); 
}

void SemToken::setName(const string&word) {
	this->name = word; 
}

void SemToken::setToken(DictToken*tok) {
	this->token = tok;
}

void SemToken::addRelation(int rel) { 
	this->relation.push_back(rel); 
}

size_t SemToken::getId() {
	return this->id;
}

size_t SemToken::getSecondaryWordSize() { 
	return this->secondary_word.size(); 
}

string SemToken::getName() {
	return this->name; 
}

string SemToken::getPrimaryWord() { 
	return this->primary_word; 
}

string SemToken::getSecondaryWordById(size_t id) {
	if (id < this->secondary_word.size()) return this->secondary_word[id];
	else return "";
}

bool SemToken::fromDict() { 
	return this->isfromdict; 
}

size_t SemToken::getRelationSize() { 
	return this->relation.size(); 
}

int SemToken::getRelationById(size_t id) {
	if (id < this->relation.size()) return this->relation[id];
	else return -1;
}

DictToken* SemToken::getToken() { 
	return this->token; 
}

string SemToken::build() {
	string result = "";
	result += this->name;
	result += "=" + this->primary_word + "(";
	for (size_t i = 0; i < this->secondary_word.size(); i++) {
		result += this->secondary_word[i];
		if (i + 1 < this->secondary_word.size() || this->val.size() > 0) {
			result += ",";
		}
	}
	result += this->val;
	result += ")";
	return result;
}

vector<int> SemToken::getRelations() {
	return this->relation;
}
vector<string> SemToken::getSecondaryWords() {
	return this->secondary_word;
}

void SemToken::setVal(const string&val) {
	this->val = val;
}

string SemToken::getVal() {
	return this->val;
}

SemTree::SemTree() {

}

SemTree::SemTree(ifstream&fin) {
	if (this->treeLoad(fin)) {
		this->~SemTree();
	}
	if (this->dictsLoad()) {
		this->~SemTree();
	}
	this->cur_sent = 0;
	this->sentences_amount = this->tree.size();
	this->words_amount.resize(this->sentences_amount);
	for (size_t i = 0; i < this->sentences_amount; i++) {
		this->words_amount[i] = this->tree[i].size();
	}
}

SemTree::~SemTree() {
	this->dict.clear();
	this->tree.clear();
	this->words_amount.clear();
}

int SemTree::dictsLoad() {
	this->dict[NOUN] = new Dict(config["dict_s"]);
	this->dict[VERB] = new Dict(config["dict_v"]);
	this->dict[ADJ] = new Dict(config["dict_a"]);
	for (pair<string, Dict*>elem : this->dict) {
		if (elem.second->empty()) {	// if at least one of the dictionaries is empty, return error code
			cerr << "error: dict[" << elem.first << "] is empty\n";
			return 1;
		}
	}
	return 0;
}

int SemTree::treeLoad(ifstream&fin) {	// reading syntax tree from file
	string str = "", line = "";
	int line_num = 0;
	int rel_pos = 0;
	vector<string>token;
	Node*node;
	vector<Node*> sentence;

	this->sentences_amount = 0;

	while (getline(fin, line)) {
		line_num++;
		if (line == "\n" || line == "" || line == "<end>") {	// condition of writing sentence to the tree
			this->tree.push_back(sentence);
			this->words_amount.push_back(sentence.size());
			this->sentences_amount++;
			sentence.clear();
			if (line == "<end>")
				break;
			continue;
		}
		node = new Node();
		token = str_split(line, '\t');	// splitting read line into parts of syntax token
		try {
			node->id = stoi(token[0]);
		}
		catch (invalid_argument) {
			cerr << "error: wrong data in syntax tree" << endl;
			return WRONG_DATA;
		}
		node->word = token[1];
		node->infinitive = token[2];
		node->morph = str_split(token[4], ' ');
		try {
			node->parent_id = stoi(token[5]) - 1;
			rel_pos = 6;
		}
		catch (invalid_argument) {
			cerr << "error: wrong data in syntax tree" << endl;
			return WRONG_DATA;
		}
		node->relation = token[rel_pos];
		node->object_id = -1;
		sentence.push_back(node);	// pushing node to the sentence
	}
	for (size_t i = 0; i < this->sentences_amount; i++) {
		for (size_t j = 0; j < this->words_amount[i]; j++) {
			if (tree[i][j]->parent_id != -1)	// mapping pointers to Nodes as pointers to parents
				tree[i][j]->parent = tree[i][tree[i][j]->parent_id];
			else
				tree[i][j]->isroot = true;
		}
	}
	return 0;
}

size_t SemTree::sentencesSize() {
	return this->sentences_amount;
}

size_t SemTree::wordsSize(size_t sentence_num) {
	if (sentence_num < sentences_amount)
		return this->words_amount[sentences_amount];
	return 0;
}

int SemTree::handleNonlex(Node*cur) {
	int retval = 0;
	SemToken*tmp = new SemToken();
	Node*parent = NULL;
	
	if (cur->object_id != -1) {
		return 0;
	}
	parent = cur->parent;
	while (parent) {
		if (strtolower(parent->morph[0]) == NOUN) {
			for (size_t i = 0; i < this->object.size(); i++) {
				if (strtolower(this->object[i]->getPrimaryWord()) == parent->infinitive) {
					if ((char*)strstr(strtolower(this->object[i]->getName()).c_str(), strtolower(this->object[i]->getPrimaryWord()).c_str()) != NULL) {
						object[i]->setName(cur->infinitive);
						cur->object_id = i;
						return 0;
					}
				}
			}
			this->handleNoun(parent);
			this->object[object.size() - 1]->setName(cur->infinitive);
			cur->object_id = this->object.size() - 1;
			return 0;
		}
		parent = parent->parent;
	}
	return 0;
}

int SemTree::handleNoun(Node*cur) {
	int retval = 0;
	int word_id = -1;
	int par_w_id = -1;
	int word_count = 0;
	bool found_obj = false;
	bool plural = false;
	string prop_val = "";
	string obj_name = "";
	Node* parent = NULL;
	DictToken*tmpDict = NULL;
	SemToken*tmpSem = NULL;

	if (cur->object_id != -1) {
		return 0;
	}
	word_id = this->dict[NOUN]->findWord(cur->infinitive);
	for (size_t i = 0; i < cur->morph.size(); i++) {
		if (strtolower(cur->morph[i]) == PLURAL) {
			plural = true;
			break;
		}
	}
	if (plural) {
		parent = cur->parent;
		while (parent) {
			found_obj = false;
			par_w_id = dict[NOUN]->findWord(parent->infinitive);
			if (par_w_id != -1) {
				for (size_t i = 0; i < this->object.size(); i++) {
					if (strtolower(this->object[i]->getPrimaryWord()) == strtolower(parent->infinitive)) {
						found_obj = true;
						break;
					}
				}
				if (!found_obj) {
					this->handleNoun(parent);
				}
				prop_val = dict[NOUN]->getTokenById(par_w_id)->getPropValByName(cur->infinitive);
				if (prop_val.size() > 0) {
					try {
						word_count = stoi(prop_val);
					}
					catch (invalid_argument) {
						word_count = 0;
					}
					if (word_count == 0) {
						for (size_t i = 0; i < this->words_amount[cur_sent]; i++) {
							if (this->tree[cur_sent][i]->parent == cur) {
								if (this->tree[cur_sent][i]->morph[0] == NONLEX) {
									word_count++;
								}
							}
						}
					}
					break;
				}
			}
			parent = parent->parent;
		}
	}
	tmpSem = new SemToken();
	if (word_id != -1) {
		tmpDict = this->dict[NOUN]->getTokenById(word_id);
		tmpSem->setIsFromDict(true);
		tmpSem->setToken(tmpDict);
	}
	tmpSem->setPrimaryWord(cur->infinitive);
	tmpSem->setName(cur->infinitive);
	parent = cur->parent;
	/*while (parent) {
		if (parent->object_id != -1) {
			tmpSem->addRelation(parent->object_id);
		}
		parent = parent->parent;
	}*/
	if (word_count == 0) {
		word_count = 1;
	}
	for (int i = 0; i < word_count; i++) {
		obj_name = cur->infinitive + "_" + to_string(i);
		tmpSem->setName(obj_name);
		tmpSem = new SemToken(*tmpSem);
		object.push_back(tmpSem);
	}
	cur->object_id = object.size() - 1;
	return 0;
}

int SemTree::handleVerb(Node*cur) {
	SemToken*tmp;
	int word_id = -1;
	string str = "";
	str = strtoupper(cur->infinitive);
	word_id = dict["v"]->findWord(strtoupper(cur->infinitive));
	if (word_id == -1) {
		return 0;
	}
	tmp = new SemToken();
	tmp->setPrimaryWord(cur->infinitive);
	tmp->setName(cur->infinitive);
	if (word_id > -1) {
		tmp->setIsFromDict(true);
		tmp->setToken(dict["v"]->getTokenById(word_id));
		for (size_t i = 0; i < this->tree[cur_sent].size(); i++) {
			if (this->tree[cur_sent][i]->parent == cur) {
				if (strtolower(this->tree[cur_sent][i]->morph[0]) == NOUN) {
					for (size_t j = 0; j < object.size(); j++)
						if (this->tree[cur_sent][i]->infinitive == object[j]->getPrimaryWord()) {
							tmp->addSecondaryWord(this->tree[cur_sent][i]->infinitive);
							tmp->addRelation(j);
							object[j]->addRelation(object.size());
						}
				}
			}
		}
	}
	object.push_back(tmp);
	return 0;
}

int SemTree::handleAdj(Node*cur) {
	SemToken*tmp = NULL;
	DictToken*tmpDict = NULL;
	Node*parent = NULL;
	int word_id = 0;
	int noun_id = -1;

	word_id = dict[ADJ]->findWord(strtoupper(cur->infinitive));
	if (word_id == -1) {
		return 0;
	}
	if (dict[ADJ]->getTokenById(word_id)->getType() == OPER) {
		return 0;
	}
	parent = cur->parent;
	for (int i = 0; i < 2; i++) {
		if (parent) {
			if (strtolower(parent->morph[0]) == NOUN) {
				if (parent->object_id == -1) {
					handleNoun(parent);
				}
				tmp = this->object[this->object.size() - 1];
				tmp->addSecondaryWord(cur->infinitive);
				cur->object_id = this->object.size() - 1;
				break;
			}
			else {
				parent = parent->parent;
			}
		}
		else {
			return 0;
		}
	}
	
	return 0;
}

int SemTree::handleNum(Node*cur) {
	SemToken* tmpSem = NULL;
	DictToken* tmpDict = NULL;
	Node* parent = NULL;
	int word_id = -1;
	
	parent = cur->parent;
	while (parent) {
		if (strtolower(parent->morph[0]) == ADJ) {	// trying to find adjective parent
			word_id = this->dict[ADJ]->findWord(parent->infinitive);
			if (word_id != -1) {
				tmpDict = this->dict[ADJ]->getTokenById(word_id);
				if (tmpDict->getType() == OPER) {	// trying to find operation-type parent
					if (strtoupper(parent->infinitive) == EQUAL) {
						for (size_t i = 0; i < this->tree[cur_sent].size(); i++) {
							if (strtolower(tree[cur_sent][i]->morph[0]) == NOUN) { // trying to find noun child of operation-type token
								tmpSem = this->object[this->tree[cur_sent][i]->object_id];
								if (tmpSem->getToken()->getType() == VAL) {
									tmpSem->setVal(cur->infinitive);
									cur->object_id = this->tree[cur_sent][i]->object_id;
								}
								break;
							}
						}
					}
					break;
				}
			}
		}
		parent = parent->parent;
	}

	return 0;
}

int SemTree::semDfs(Node*cur) {
	int retval = 0;
	for (size_t i = 0; i < this->tree[cur_sent].size(); i++) {
		if (this->tree[cur_sent][i]->parent == cur) {
			retval = semDfs(this->tree[cur_sent][i]);
			if (retval) {
				return retval;
			}
		}
	}
	if (strtolower(cur->morph[0]) == NONLEX || strtolower(cur->morph[0]) == ABBR) {
		return this->handleNonlex(cur);
	}
	else if (strtolower(cur->morph[0]) == NOUN) {
		return this->handleNoun(cur);
	}
	else if (strtolower(cur->morph[0]) == VERB) {
		return this->handleVerb(cur);
	}
	else if (strtolower(cur->morph[0]) == ADJ) {
		return this->handleAdj(cur);
	}
	else if (strtolower(cur->morph[0]) == NUM) {
		return this->handleNum(cur);
	}
	return 0;
}

int SemTree::buildChildRelations(Node*base, Node*cur, int sent, int i) { // looking for relations in all descendants
	SemToken*tmpSem = NULL;
	DictToken*tmpDict = NULL;
	bool rel_exists = false;
	for (size_t j = 0; j < this->tree[sent].size(); j++) {
		if (tree[sent][j]->parent == cur) {
			if (tree[sent][j]->object_id != -1 && tree[sent][j]->object_id != base->object_id) {
				tmpSem = object[i];
				tmpDict = tmpSem->getToken();
				if (tmpDict->getPropByName(object[tree[sent][j]->object_id]->getPrimaryWord()).size() > 0) { // if in current object (object[i]) found property with primary word of one of the children, add relation to the child
					rel_exists = false;
					for (int rel_id = 0; rel_id < object[tree[sent][j]->object_id]->getRelationSize(); rel_id++) {
						if (object[tree[sent][j]->object_id]->getRelationById(rel_id) == i) {
							rel_exists = true;
							break;
						}
					}
					if (!rel_exists) {
						object[tree[sent][j]->object_id]->addRelation(i);
						object[tree[sent][j]->object_id]->addSecondaryWord(object[i]->getName());
					}
					
				}
			}
			this->buildChildRelations(base, tree[sent][j], sent, i);
		}
	}
	return 0;
}

int SemTree::buildRelations() {
	Node*parent = NULL;
	Node*child = NULL;
	Node*cur = NULL;
	DictToken*tmpDict = NULL;
	SemToken*tmpSem = NULL;
	bool rel_exists = false;
	for (size_t sent = 0; sent < this->sentences_amount; sent++) {
		for (size_t i = 0; i < this->object.size(); i++) {
			for (size_t j = 0; j < this->tree[sent].size(); j++) {
				if (i == tree[sent][j]->object_id) {
					cur = tree[sent][j];
					if (object[i]->fromDict()) {
						tmpDict = object[i]->getToken();
						parent = cur->parent;
						while (parent) { // looking for relations in all ancestors
							if (parent->object_id != -1 && parent->object_id != cur->object_id) {
								if (tmpDict->getPropByName(object[parent->object_id]->getPrimaryWord()).size() > 0) { // if in current object (object[i]) found property with primary word of one of the parents, add relation to the parent
									rel_exists = false;
									for (int rel_id = 0; rel_id < object[parent->object_id]->getRelationSize(); rel_id++){
										if (object[parent->object_id]->getRelationById(rel_id) == i) {
											rel_exists = true;
											break;
										}
									}
									if (!rel_exists) {
										object[parent->object_id]->addRelation(i);
										object[parent->object_id]->addSecondaryWord(object[i]->getName());
									}
									break;
								}
							}
							parent = parent->parent;
						}
					}
					this->buildChildRelations(cur, cur, sent, i);
					/*for (size_t j = 0; j < this->tree[sent].size(); j++) {
						if (tree[sent][j]->parent == parent) {
							if (tree[sent][j]->object_id != -1 && tree[sent][j]->object_id != cur->object_id) {
								tmpSem = object[tree[sent][j]->object_id];
								tmpDict = tmpSem->getToken();
								if (tmpDict->getPropByName(object[tree[sent][j]->object_id]->getPrimaryWord()).size() > 0) {
									tmpSem->addRelation(i);
									tmpSem->addSecondaryWord(object[i]->getName());
								}
								break;
							}
							parent = tree[sent][j];
						}
					}*/
					// break;
				}
			}
		}
	}
	cout << "relations built\n";
	return 0;
}

int SemTree::analysis() {
	int retval = 0;
	cout << "starting semantic analysis...\n";
	for (size_t i = 0; i < this->sentences_amount; i++) {
		cout << "starting processing sentence #" << i + 1 << endl;
		this->cur_sent = i;
		for (size_t j = 0; j < this->words_amount[i]; j++) {
			if (this->tree[i][j]->parent == NULL) {	// finding root node to start dfs
				retval = semDfs(this->tree[i][j]);
				if (retval != 0) {
					return retval;
				}
				break;
			}
		}
	}
	cout << "sentences processing complete\nstarting building relations...\n";
	this->buildRelations();
	cout << "semantic analysis finished\n";
	return retval;
}

string SemTree::buildResult() {
	string result = "";
	for (size_t j = 0; j < this->sentences_amount; j++) {
		result += "sentence #" + to_string(j + 1) + "\n";
		for (size_t i = 0; i < this->object.size(); i++) {
			for (size_t k = 0; k < this->tree[j].size(); k++) {
				if (i == this->tree[j][k]->object_id) {
					result += object[i]->build() + "\n";
					break;
				}
			}
		}
		result += "\n";
	}
	return result;
}

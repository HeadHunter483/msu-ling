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
		node->object_id.push_back(-1);
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
	
	if (cur->object_id[0] != -1) {
		return 0;
	}
	parent = cur->parent;
	while (parent) {
		if (strtolower(parent->morph[0]) == NOUN) {
			for (size_t j = 0; j < this->tree[cur_sent].size(); j++) {
				for (size_t i = 0; i < this->object.size(); i++) {
					for (size_t k = 0; k < tree[cur_sent][j]->object_id.size(); k++) {
						if (tree[cur_sent][j]->object_id[k] == i) {
							if (strtolower(this->object[i]->getPrimaryWord()) == parent->infinitive) {
								if ((char*)strstr(strtolower(this->object[i]->getName()).c_str(), strtolower(this->object[i]->getPrimaryWord()).c_str()) != NULL) {
									object[i]->setName(cur->infinitive);
									cur->object_id[0] = i;
									return 0;
								}
							}
						}
					}
				}
			}
			this->handleNoun(parent);
			for (size_t i = 0; i < this->object.size(); i++) {
				if (strtolower(this->object[i]->getPrimaryWord()) == parent->infinitive) {
					if ((char*)strstr(strtolower(this->object[i]->getName()).c_str(), strtolower(this->object[i]->getPrimaryWord()).c_str()) != NULL) {
						object[i]->setName(cur->infinitive);
						cur->object_id[0] = i;
						return 0;
					}
				}
			}
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
	Node* cur_loc = NULL;
	DictToken*tmpDict = NULL;
	SemToken*tmpSem = NULL;

	if (cur->object_id[0] != -1) {
		return 0;
	}
	word_id = this->dict[NOUN]->findWord(cur->infinitive);
	for (size_t i = 0; i < cur->morph.size(); i++) {
		if (strtolower(cur->morph[i]) == PLURAL) {
			plural = true;
			break;
		}
	}
	if (plural) {	// case of plural noun
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
				word_count = 0;
				for (size_t id = 0; id < tree[cur_sent].size(); id++) {
					if (tree[cur_sent][id]->parent == cur) {
						if (strtolower(tree[cur_sent][id]->morph[0]) == NONLEX) {
							word_count++;
						}
					}
				}
				if (word_count == 0) {
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
	while (parent) {
		for (size_t obj_id = 0; obj_id < parent->object_id.size(); obj_id++) {
			if (parent->object_id[obj_id] != -1) {
				tmpSem->addRelation(parent->object_id[obj_id]);
				break;
			}
		}
		parent = parent->parent;
	}
	for (size_t i = 0; i < tree[cur_sent].size(); i++) {
		bool found_rel = false;
		if (tree[cur_sent][i]->parent == cur) {
			for (size_t obj_id = 0; obj_id < tree[cur_sent][i]->object_id.size(); obj_id++) {
				if (tree[cur_sent][i]->object_id[obj_id] != -1) {
					for (size_t rel_id = 0; rel_id < tmpSem->getRelationSize(); rel_id++) {
						if (tmpSem->getRelationById(rel_id) == tree[cur_sent][i]->object_id[obj_id]) {
							found_rel = true;
							break;
						}
					}
					if (found_rel)
						break;
					tmpSem->addRelation(tree[cur_sent][i]->object_id[obj_id]);
				}
			}
		}
	}

	if (word_count == 0) {
		word_count = 1;
	}
	cur->object_id.resize(word_count);
	for (int i = 0; i < word_count; i++) {
		obj_name = cur->infinitive + "_" + to_string(i);
		tmpSem->setId(object.size());
		tmpSem->setName(obj_name);
		object.push_back(tmpSem);
		cur->object_id[i] = object.size() - 1;
		tmpSem = new SemToken(*tmpSem);
	}
	// cur->object_id[0] = object.size() - 1;
	return 0;
}

Node*SemTree::childRun(Node*cur, Node*base) {
	SemToken*tmpSemCur = NULL, *tmpSemBase = NULL;
	for (size_t i = 0; i < tree[cur_sent].size(); i++) {
		if (tree[cur_sent][i]->parent == cur) {
			this->childRun(tree[cur_sent][i], base);
		}
	}
	if (cur->object_id[0] != -1) {
		for (size_t i = 0; i < object[base->object_id[0]]->getRelationSize(); i++) {
			for (size_t j = 0; j < cur->object_id.size(); j++) {
				if (object[base->object_id[0]]->getRelationById(i) == cur->object_id[j]) {
					object[base->object_id[0]]->addRelation(cur->object_id[j]);
				}
			}
		}
	}
	return NULL;
}

int SemTree::handleVerb(Node*cur) {
	SemToken*tmp;
	int word_id = -1;
	string str = "";
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
					//for (size_t j = 0; j < object.size(); j++) {
						if (tree[cur_sent][i]->object_id[0] != -1) {
							for (size_t k = 0; k < tree[cur_sent][i]->object_id.size(); k++) {
								//if (this->tree[cur_sent][i]->infinitive == object[j]->getPrimaryWord()) {
								tmp->addRelation(tree[cur_sent][i]->object_id[k]);
								// object[j]->addRelation(object.size());
							}
						}
					//}
				}
			}
		}
	}
	object.push_back(tmp);
	cur->object_id[0] = object.size() - 1;
	tmp = new SemToken();
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
				if (parent->object_id[0] == -1) {
					handleNoun(parent);
				}
				tmp = this->object[this->object.size() - 1];
				tmp->addSecondaryWord(cur->infinitive);
				cur->object_id[0] = this->object.size() - 1;
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
	SemToken* tmpSem2 = NULL;
	DictToken* tmpDict = NULL;
	DictToken* tmpDict2 = NULL;
	Node* parent = NULL;
	int word_id = -1;
	bool found_val = false;
	bool skip = false;
	
	parent = cur->parent;
	while (parent) {
		if (strtolower(parent->morph[0]) == ADJ) {	// trying to find adjective parent
			word_id = this->dict[ADJ]->findWord(parent->infinitive);
			if (word_id != -1) {
				tmpDict = this->dict[ADJ]->getTokenById(word_id);
				if (tmpDict->getType() == OPER) {	// trying to find operation-type parent
					if (strtoupper(parent->infinitive) == EQUAL) {
						for (size_t i = 0; i < this->tree[cur_sent].size(); i++) {
							if (tree[cur_sent][i]->parent != parent) {
								continue;
							}
							skip = false;
							found_val = false;
							if (strtolower(tree[cur_sent][i]->morph[0]) == NOUN) { // trying to find noun child of operation-type token
								for (size_t obj_id = 0; obj_id < tree[cur_sent][i]->object_id.size(); obj_id++) {
									tmpSem = this->object[this->tree[cur_sent][i]->object_id[obj_id]];
									tmpDict = tmpSem->getToken();
									if (tmpDict->getType() == VAL) {	// if object is val-type trying to bind value
										if (tmpSem->getVal().size() == 0) {	// if there is no bind value, bind it
											tmpSem->setVal(cur->infinitive);
											cur->object_id[0] = this->tree[cur_sent][i]->object_id[obj_id];
											found_val = true;
										}
									}
									else if (tmpDict->getType() == OBJ) { // if object is obj-type trying to find prop with val-type and bind value to it
										//for (size_t j = 0; j < tmpSem->getRelationSize(); j++) {	// if there is a bind value, then go to the next
										//	if (object[tmpSem->getRelationById(j)]->getVal().size() > 0) {
										//		skip = true;
										//		break;
										//	}
										//}
										if (skip) {
											continue;
										}
										for (size_t i = 0; i < tmpDict->getPropSize(); i++) {
											tmpDict2 = dict[NOUN]->getTokenById(dict[NOUN]->findWord(tmpDict->getPropByNum(i)));
											if (tmpDict2->getType() == VAL) {
												for (size_t j = 0; j < object.size(); j++) { // if there is a bind value, then go to the next
													for (size_t k = 0; k < object[j]->getRelationSize(); k++) {
														if (object[j]->getRelationById(k) == tmpSem->getId()) {
															if (strtolower(object[j]->getPrimaryWord()) == strtolower(tmpDict2->getWord())) {
																skip = true;
																break;
															}
														}
													}
												}
												if (skip) {
													continue;
												}
												tmpSem2 = new SemToken();
												int objNameNum = 0;
												for (size_t w = 0; w < object.size(); w++) {
													if ((char*)strstr(object[w]->getName().c_str(), tmpDict2->getWord().c_str()) != NULL) {
														objNameNum++;
													}
												}
												tmpSem2->setName(strtolower(tmpDict2->getWord()) + "_" + to_string(objNameNum));
												tmpSem2->setId(object.size());
												tmpSem2->setIsFromDict(true);
												tmpSem2->setPrimaryWord(strtolower(tmpDict2->getWord()));
												tmpSem2->setToken(tmpDict2);
												tmpSem2->setVal(cur->infinitive);
												tmpSem2->addRelation(tmpSem->getId());
												object.push_back(tmpSem2);
												// tmpSem->addRelation(object.size() - 1);
												cur->object_id[0] = object.size() - 1;
												found_val = true;
											}
										}
									}
									if (found_val)
										break;
								}
								if (found_val)
									break;
							}
						}
						if (found_val)
							break;
						parent = parent->parent;
						while (parent) {	// searching nouns among ancestors
							skip = false;
							found_val = false;
							if (strtolower(parent->morph[0]) == NOUN) {
								for (size_t obj_id = 0; obj_id < parent->object_id.size(); obj_id++) {
									skip = false;
									this->handleNoun(parent);
									tmpSem = object[parent->object_id[obj_id]];
									if (tmpSem->fromDict()) {
										tmpDict = tmpSem->getToken();
										if (tmpDict->getType() == VAL) {	// if object is val-type trying to bind value
											if (tmpSem->getVal().size() == 0) {	// if there is no bind value, bind it
												tmpSem->setVal(cur->infinitive);
												cur->object_id[0] = parent->object_id[obj_id];
												found_val = true;
											}
										}
										else if (tmpDict->getType() == OBJ) {	// if object is obj-type trying to find prop with val-type and bind value to it
											skip = false;
											//for (size_t j = 0; j < tmpSem->getRelationSize(); j++) {	// if there is a bind value, then go to the next
											//	if (object[tmpSem->getRelationById(j)]->getVal().size() > 0) {
											//		skip = true;
											//		break;
											//	}
											//}
											if (skip) {
												continue;
											}
											for (size_t i = 0; i < tmpDict->getPropSize(); i++) {
												tmpDict2 = dict[NOUN]->getTokenById(dict[NOUN]->findWord(tmpDict->getPropByNum(i)));
												if (tmpDict2->getType() == VAL) {
													for (size_t j = 0; j < object.size(); j++) { // if there is a bind value, then go to the next
														for (size_t k = 0; k < object[j]->getRelationSize(); k++) {
															if (object[j]->getRelationById(k) == tmpSem->getId()) {
																if (strtolower(object[j]->getPrimaryWord()) == strtolower(tmpDict2->getWord())) {
																	skip = true;
																	break;
																}
															}
														}
													}
													if (skip) {
														continue;
													}
													tmpSem2 = new SemToken();
													int objNameNum = 0;
													for (size_t w = 0; w < object.size(); w++) {
														if ((char*)strstr(strtolower(object[w]->getName()).c_str(), strtolower(tmpDict2->getWord()).c_str()) != NULL) { // checking for having objects with standard name
															objNameNum++;
														}
													}
													tmpSem2->setName(strtolower(tmpDict2->getWord()) + "_" + to_string(objNameNum));
													tmpSem2->setId(object.size());
													tmpSem2->setIsFromDict(true);
													tmpSem2->setPrimaryWord(strtolower(tmpDict2->getWord()));
													tmpSem2->setToken(tmpDict2);
													tmpSem2->setVal(cur->infinitive);
													tmpSem2->addRelation(tmpSem->getId());
													object.push_back(tmpSem2);
													// tmpSem->addRelation(object.size() - 1);
													cur->object_id[0] = object.size() - 1;
													found_val = true;
													break;
												}
											}
											if (found_val)
												break;
										}
										if (found_val)
											break;
									}
									else {
										parent = parent->parent;
										continue;
									}
								}
							}
							if (found_val)
								break;
							else {
								parent = parent->parent;
								continue;
							}
						}
						if (found_val)
							break;
						else
							parent = parent->parent;
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
			for (size_t obj_id = 0; obj_id < tree[sent][j]->object_id.size(); obj_id++) {
				for (size_t b_obj_id = 0; b_obj_id < base->object_id.size(); b_obj_id++) {
					if (tree[sent][j]->object_id[obj_id] != -1 && tree[sent][j]->object_id[obj_id] != base->object_id[b_obj_id]) {
						tmpSem = object[i];
						tmpDict = tmpSem->getToken();
						if (tmpDict->getPropByName(object[tree[sent][j]->object_id[obj_id]]->getPrimaryWord()).size() > 0) { // if in current object (object[i]) found property with primary word of one of the children, add relation to the child
							rel_exists = false;
							for (size_t rel_id = 0; rel_id < object[tree[sent][j]->object_id[obj_id]]->getRelationSize(); rel_id++) {
								if (object[tree[sent][j]->object_id[obj_id]]->getRelationById(rel_id) == i) {
									rel_exists = true;
									break;
								}
							}
							if (!rel_exists) {
								object[tree[sent][j]->object_id[obj_id]]->addRelation(i);
								object[tree[sent][j]->object_id[obj_id]]->addSecondaryWord(object[i]->getName());
							}
						}
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
	// here we suggest that all words in sentence are connected somehow
	for (size_t sent = 0; sent < tree.size(); sent++) {	// building relations not built during processing inside each sentence by watching through sentence as a direct list, not a tree
		for (size_t word = 0; word < tree[sent].size(); word++) {
			if (tree[sent][word]->object_id[0] != -1) {
				for (size_t obj = 0; obj < tree[sent][word]->object_id.size(); obj++) {
					tmpSem = object[tree[sent][word]->object_id[obj]];
					if (tmpSem->fromDict()) {
						tmpDict = tmpSem->getToken();
						for (size_t word2 = 0; word2 < tree[sent].size(); word2++) {
							if (tree[sent][word2]->object_id[0] != -1) {
								for (size_t obj2 = 0; obj2 < tree[sent][word2]->object_id.size(); obj2++) {
									if (tmpDict->getPropByName(object[tree[sent][word2]->object_id[obj2]]->getPrimaryWord()).size() > 0) { // if the word can be a property of another word in sentence, then add it
										rel_exists = false;
										for (size_t rel = 0; rel < tmpSem->getRelationSize(); rel++) {
											if (tmpSem->getRelationById(rel) == tree[sent][word2]->object_id[obj2]) {
												rel_exists = true;
												break;
											}
										}
										if (rel_exists) {
											continue;
										}
										tmpSem->addRelation(tree[sent][word2]->object_id[obj2]);
										break;
									}
								}
							}
						}
					}
				}
			}
		}
	}

	for (size_t obj = 0; obj < object.size(); obj++) {	// trying to build relations between words from different sentences
		bool name_set = false;
		if ((char*)strstr(object[obj]->getName().c_str(), object[obj]->getPrimaryWord().c_str()) != NULL) {	// if the word has standard name
			for (int obj2 = obj - 1; obj2 >= 0; --obj2) {
				if (object[obj2]->getPrimaryWord() == object[obj]->getPrimaryWord()) {	// trying to find word with same primary word
					if ((char*)strstr(strtolower(object[obj2]->getName()).c_str(), strtolower(object[obj2]->getPrimaryWord()).c_str()) == NULL) {	// if found word with not standard name, than setting name of it in the first word
						object[obj]->setName(object[obj2]->getName());
						name_set = true;
						break;
					}
				}
			}
			if (name_set)
				continue;
			for (size_t obj2 = obj + 1; obj2 < object.size(); obj2++) {
				if (object[obj2]->getPrimaryWord() == object[obj]->getPrimaryWord()) {
					if ((char*)strstr(strtolower(object[obj2]->getName()).c_str(), strtolower(object[obj2]->getPrimaryWord()).c_str()) == NULL) {
						object[obj]->setName(object[obj2]->getName());
						name_set = true;
						break;
					}
				}
			}
		}
	}

	//for (size_t sent = 0; sent < this->sentences_amount; sent++) {
	//	for (size_t i = 0; i < this->object.size(); i++) {
	//		for (size_t j = 0; j < this->tree[sent].size(); j++) {
	//			for (size_t obj_id = 0; obj_id < tree[sent][j]->object_id.size(); obj_id++) {
	//				if (i == tree[sent][j]->object_id[obj_id]) {
	//					cur = tree[sent][j];
	//					if (object[i]->fromDict()) {
	//						tmpDict = object[i]->getToken();
	//						parent = cur->parent;
	//						while (parent) { // looking for relations in all ancestors
	//							for (size_t p_obj_id = 0; p_obj_id < parent->object_id.size(); p_obj_id++) {
	//								if (parent->object_id[p_obj_id] != -1 && parent->object_id[p_obj_id] != cur->object_id[obj_id]) {
	//									if (tmpDict->getPropByName(object[parent->object_id[p_obj_id]]->getPrimaryWord()).size() > 0) { // if in current object (object[i]) found property with primary word of one of the parents, add relation to the parent
	//										rel_exists = false;
	//										for (size_t rel_id = 0; rel_id < object[parent->object_id[p_obj_id]]->getRelationSize(); rel_id++) {
	//											if (object[parent->object_id[p_obj_id]]->getRelationById(rel_id) == i) {
	//												rel_exists = true;
	//												break;
	//											}
	//										}
	//										if (!rel_exists) {
	//											object[parent->object_id[p_obj_id]]->addRelation(i);
	//											object[parent->object_id[p_obj_id]]->addSecondaryWord(object[i]->getName());
	//										}
	//										break;
	//									}
	//								}
	//							}
	//							parent = parent->parent;
	//						}
	//					}
	//					this->buildChildRelations(cur, cur, sent, i);
	//				}
	//			}
	//		}
	//	}
	//}
	
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
	DictToken*tmpDict = NULL, *tmpDict2 = NULL;
	vector<int>do_not_print;
	bool rel_exists = false;

	for (size_t i = 0; i < object.size(); i++) {
		for (size_t j = i + 1; j < object.size(); j++) {
			if (object[i]->getName() == object[j]->getName()) {
				do_not_print.push_back(j);
			}
		}
	}

	for (size_t i = 0; i < this->object.size(); i++) { // adding relations to all connected words (a.rel has b => adding a to b.rel)
		for (size_t j = 0; j < object[i]->getRelationSize(); j++) {
			rel_exists = false;
			for (size_t k = 0; k < object[object[i]->getRelationById(j)]->getRelationSize(); k++) {
				if (object[object[i]->getRelationById(j)]->getRelationById(k) != -1){
					rel_exists = true;
					break;
				}
			}
			if (rel_exists)
				continue;
			object[object[i]->getRelationById(j)]->addRelation(i);
			
			/*tmpDict = object[i]->getToken();
			if (tmpDict->getPropByName(object[object[i]->getRelationById(j)]->getPrimaryWord()).size() > 0) {
				object[object[i]->getRelationById(j)]->addRelation(i);
				break;
			}*/
		}
	}

	for (size_t i = 0; i < this->object.size(); i++) { // adding secondary words, so we can add it to the arguments of the word
		for (size_t j = 0; j < object[i]->getRelationSize(); j++) {
			tmpDict = object[i]->getToken();
			if (object[object[i]->getRelationById(j)]->getToken()->getPropByName(tmpDict->getWord()).size() > 0) {
				object[i]->addSecondaryWord(object[object[i]->getRelationById(j)]->getName());
			}
			else if (strtolower(object[i]->getToken()->getMorph()) == VERB) {
				object[i]->addSecondaryWord(object[object[i]->getRelationById(j)]->getName());
			}
		}
	}

	for (size_t j = 0; j < this->sentences_amount; j++) {	// building text form of the semantic graph
		result += "sentence #" + to_string(j + 1) + "\n";
		for (size_t i = 0; i < this->object.size(); i++) {
			for (size_t k = 0; k < this->tree[j].size(); k++) {
				if (i == this->tree[j][k]->object_id[0]) {
					bool found = false;
					for (size_t l = 0; l < do_not_print.size(); l++) {
						if (i == do_not_print[l]) {
							found = true;
							break;
						}
					}
					if (found)
						continue;
					result += object[i]->build() + "\n";
					break;
				}
			}
		}
		result += "\n";
	}

	return result;
}

#include "sem.h"

const string dict1Name = "dict3s.xml";
const string dict2Name = "dict3v.xml";
const string dict3Name = "dict3a.xml";


SemToken::SemToken() {
	isfromdict = false;
}

SemToken::~SemToken() {
	this->secondary_word.clear();
}

string SemToken::build() {
	string result = "";
	int rel_count = this->relation.size();
	if (strtolower2(this->token.morph) == NOUN) {
		result += this->primary_word + "(";
		for (int i = 0; i < this->secondary_word.size(); i++) {
			result += this->secondary_word[i];
			if (i != this->secondary_word.size() - 1)
				result += ",";
		}
		result += ")";
	}
	else if (strtolower2(this->token.morph) == VERB) {
		result += this->primary_word + "(";
		for (int i = 0; i < this->secondary_word.size(); i++) {
			result += this->secondary_word[i];
			if (i != this->secondary_word.size() - 1)
				result += ",";
		}
		result += ")";
	}
	return result;
}

string SemTree::build_result() {
	string result = "";
	for (int i = 0; i < object.size(); i++) {
		result += object[i].getprimaryword() + "(";
		result += object[i].getname();
		if (object[i].getsecondarywordsize()) {
			for (int j = 0; j < object[i].getsecondarywordsize() - 1; j++) {
				result += object[i].getsecondarywordbyid(i) + ",";
			}
			result += object[i].getsecondarywordbyid(object[i].getsecondarywordsize() - 1);
		}
		if (object[i].getrelationsize()) {
			//if (object[i].getsecondarywordsize()) {
			//	result += ",";
			//}
			result += ",";
			for (int j = 0; j < object[i].getrelationsize() - 1; j++) {
				result += object[i].getrelationbyid(j) + ",";
			}
			if (object[i].getrelationsize()) {
				result += object[object[i].getrelationbyid(object[i].getrelationsize() - 1)].getname();
			}
		}
		result += ")\n";
	}
	//for (int i = 0; i < object.size(); i++)
	//	result += object[i].build() + "\n";
	return result;
}

SemTree::SemTree() : Tree(){
	if (dictload()) {
		this->~SemTree();
	}
}

SemTree::~SemTree(){
	object.clear();
	this->dict.clear();
}

int SemTree::dictload() {
	dict[NOUN] = new Dict(dict1Name);
	dict[VERB] = new Dict(dict2Name);
	dict[ADJ] = new Dict(dict3Name);
	for (pair<string, Dict*>elem : dict) {
		if (elem.second->dict.empty()) {						// если любой из словарей пуст после загрузки, то вернуть 1
			return 1;
		}
	}
	return 0;
}

int SemTree::handle_nonlex(Node*cur) {								// модуль работы со словами с морфологией handle_nonlex (слова, отсутвтующие в словаре MyStem)
	
	//if (cur->id != 0) {												// если у слова отсутсвует родитель, чьим обозначением может быть данное слово
	//	for (int j = cursent; j >= 0; j--) {
	//		for (int i = cur->id - 1; i >= 0; i--) {				// ищем предыдущие упоминания данного слова в тексте
	//			if (tree[j][i].init == cur->init) {					// если находим, то проверяем родителя
	//				if (tree[j][i].parent[0]->ispred) {				// если родитель является предикатом, то заменяем данное слово на предикат
	//					cur->word = tree[j][i].parent[0]->word;
	//					cur->ispred = true;
	//					return 0;
	//				}
	//			}
	//		}
	//	}
	//}
	//if (cur->parent[0]->morph[0] == NOUN) {
	//	if (dict[NOUN]->findWord(cur->parent[0]->init) == 0) {		// если выполняется условие, то заменяем родителя на предикат и поднимаемся, минуя родителя
	//		cur->ispred = true;
	//		cur->parent[0]->ispred = true;
	//		cur->word = cur->init + " ( " + cur->init + " )";
	//		cur->parent[0]->word = cur->word;
	//		return 1;
	//	}
	//}
	
	SemToken tmp;
	int word_id = -1;
	for (int i = 0; i < object.size(); i++) {
		if (cur->word == object[i].getname()) {							// если объект с данным обозначением уже создан, то выходим из модуля
			return 0;
		}
	}

	if (strtolower2(cur->parent[0]->morph[0]) == NOUN) {							// если родитель данного слова существительное, то разбираем дальше
		for (int i = 0; i < object.size(); i++) {
			if (object[i].getprimaryword() == cur->parent[0]->init) {	// если слово, соответствующее родителю данного, уже встречалось, то разбираем дальше
				if (strstr(strtolower2(object[i].getname()).c_str(), strtolower2(object[i].getprimaryword()).c_str())) { // если название объекта стандартное, то заменяем на данное слово
					object[i].setname(cur->word);
					//object[i].addsecondaryword(cur->init);
					cur->object_id = i;
				}
				return 0;
			}
		}
		handle_noun(cur->parent[0]);
		object[object.size() - 1].setname(cur->init);
		//object[object.size() - 1].addsecondaryword(cur->init);
		cur->object_id = object.size() - 1;
	}

	return 1;
}

int SemTree::handle_opred(Node*cur) {
	SemToken tmp;
	int word_id = dict[ADJ]->findWord(strtoupper2(cur->init));
	if (word_id == -1){
		return 0;
	}
	tmp.setisfromdict(true);
	tmp.settoken(dict[NOUN]->dict[word_id]);
	tmp.setprimaryword(cur->init);
	tmp.setname(cur->init);
	Node*parent = NULL;
	parent = cur->parent[0];
	for (int i = 0; i < 2; i++) {
		if (parent) {
			if (strtolower2(parent->morph[0]) == NOUN) {
				handle_noun(parent);
				tmp.setname(cur->parent[0]->init);
				if (parent->object_id != -1) {
					tmp.addrelation(parent->object_id);
				}
				parent = parent->parent[0];
				break;
			}
			else {
				parent = parent->parent[0];
			}
		}
		else {
			return 0;
		}
	}
	object.push_back(tmp);
	cur->object_id = object.size() - 1;
	return 0;
}

int SemTree::handle_noun(Node*cur) {
	int word_id = 0, word_count = 1;
	SemToken tmp;
	if (cur->object_id != -1) {
		return 0;
	}
	word_id = dict[NOUN]->findWord(strtoupper2(cur->init));
	if (cur->find_morph((string)PLURAL) != -1) {
		int par_w_id; 
		Node*parent = cur->parent[0];
		while (parent) {
			par_w_id = dict[NOUN]->findWord(strtoupper2(parent->init));
			if (par_w_id != -1) {
				string prop_val = dict[NOUN]->dict[par_w_id].get_propval(cur->init);
				if (prop_val != "") {
					word_count = stoi(prop_val);
				}
				break;
			}
			parent = parent->parent[0];
		}
	}
	if (word_id > -1) {
		tmp.setisfromdict(true);
		tmp.settoken(dict[NOUN]->dict[word_id]);
	}
	tmp.setprimaryword(cur->init);
	tmp.setname(cur->init);
	Node*parent = cur->parent[0];
	while (parent) {
		if (parent->object_id != -1) {
			tmp.addrelation(parent->object_id);
		}
		parent = parent->parent[0];
	}
	for (int i = 0; i < word_count; i++) {
		tmp.setname(cur->init + "_" + to_string(i));
		object.push_back(tmp);
	}
	cur->object_id = object.size() - 1;
	return 0;
}

int SemTree::handle_num(Node*cur) {
	return 0;
}

int SemTree::handle_adj(Node*cur) {
	return 0;
}

int SemTree::handle_verb(Node*cur) {
	SemToken tmp;
	int word_id = -1;
	string str = "";
	str = strtoupper2(cur->init);
	word_id = dict["v"]->findWord(strtoupper2(cur->init));
	if (word_id == -1) {
		return 0;
	}
	tmp.setprimaryword(cur->init);
	tmp.setname(cur->init);
	if (word_id > -1) {
		tmp.setisfromdict(true);
		tmp.settoken(dict["v"]->dict[word_id]);
		for (int i = 0; i < cur->child.size(); i++) {
			if (strtolower2(cur->child[i]->morph[0]) == NOUN) {
				for (int j = 0; j < object.size(); j++)
					if (cur->child[i]->init == object[j].getprimaryword()) {
						tmp.addsecondaryword(cur->child[i]->init);
						tmp.addrelation(j);
						object[j].addrelation(object.size());
					}
			}
		}
	}
	object.push_back(tmp);
	return 0;
}

int SemTree::DFSstep(Node*cur) {
	//int result = 0;
	//for (int i = 0; i < cur->child.size(); i++) {
	//	result = dfsstep(cur->child[i]);
	//	if (result != 0) {
	//		visited++;
	//		return result - 1;									// если dfsstep возвращает число отличное от 0, то поднимаемся на result вершин вверх
	//	}
	//}
	//visited++;
	//if (cur->rel[0] == "punc" || cur->rel[0] == "sochin") {		// если находим знаки препинания или союзы, то удаляем
	//	cur->isdel = true;
	//	return 0;
	//}
	//if (cur->morph[0] == "handle_nonlex") {							// если слово отсутствует в словаре, то оно может быть обозначением
	//	return handle_nonlex(cur);
	//}
	//if (cur->rel[0] == "handle_opred") {
	//	return handle_opred(cur);
	//}
	int return_code = 0;
	visited++;
	for (int i = 0; i < cur->child.size(); i++) {
		return_code = DFSstep(cur->child[i]);
		if (return_code == 1) {
			cur->object_id = object.size() - 1;
		}
	}
	if (strtolower2(cur->rel[0]) == "punc" || strtolower2(cur->rel[0]) == "sochin" || strtolower2(cur->morph[0]) == PRONOUN) {
		return 0;
	}
	else if (strtolower2(cur->morph[0]) == NONLEX || strtolower2(cur->rel[0]) == "atrib") {
		return this->handle_nonlex(cur);
		//return 0;
	}
	else if (strtolower2(cur->rel[0]) == "opred") {
		return this->handle_opred(cur);
	}
	else if (strtolower2(cur->morph[0]) == NUM) {
		this->handle_num(cur);
		return 0;
	}
	else if (strtolower2(cur->morph[0]) == NOUN) {
		return this->handle_noun(cur);
	}
	else if (strtolower2(cur->morph[0]) == VERB || strtolower2(cur->morph[0]) == PARTIC || strtolower2(cur->morph[0]) == GERUND) {
		this->handle_verb(cur);
		return 0;
	}
	return 0;
}

void SemTree::semDFS() {
	for (int i = 0; i < this->tree.size(); i++) {
		cursent = i;
		for (int j = 0; j < tree[i].size(); j++) {
			if (tree[i][j].isroot) {
				visited = 0;
				DFSstep(&tree[i][j]);
				break;
			}
		}
	}
}

int SemTree::analysis() {
	semDFS();
	return 0;
}
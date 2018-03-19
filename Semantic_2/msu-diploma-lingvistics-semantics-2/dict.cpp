#include "dict.h"

void DictToken::setId(int id) {
	this->id = id;
}

void DictToken::setWord(const string&word) {
	this->word = word;
}

void DictToken::setMorph(const string&morph) {
	this->morph = morph;
}

void DictToken::setParent(int parent) {
	this->parent = parent;
}

void DictToken::addProp(const string&prop) {
	this->prop.push_back(prop);
}

void DictToken::setPropVal(size_t prop_num, const string&val) {
	this->propval.resize(prop.size());
	if (prop_num < propval.size())
		this->propval[prop_num] = val;
	else
		throw WRONG_DATA;
}

void DictToken::addList(const string&list_elem) {
	this->list.push_back(list_elem);
}

void DictToken::addFunc(const string&func) {
	this->func.push_back(func);
}

int DictToken::getId() {
	return this->id;
}

string DictToken::getWord() {
	return this->word;
}

string DictToken::getMorph() {
	return this->morph;
}

int DictToken::getParent() {
	return this->parent;
}

vector<string>DictToken::getProps() {
	return this->prop;
}

string DictToken::getPropByNum(size_t num) {
	if (num < this->prop.size())
		return prop[num];
	else
		return "";
}

string DictToken::getPropByName(const string&name) {
	for (size_t i = 0; i < this->prop.size(); i++)
		if (strtoupper(this->prop[i]) == strtoupper(name))
			return name;
	return "";
}

vector<string>DictToken::getList() {
	return this->list;
}

vector<string>DictToken::getFuncs() {
	return this->func;
}

string DictToken::getPropValByNum(size_t num) {
	if (num < this->propval.size())
		return this->propval[num];
	else
		return "";
}

string DictToken::getPropValByName(const string&name) {
	for (size_t i = 0; i < prop.size(); i++) {
		if (prop[i] == strtoupper(name)) {
			return propval[i];
		}
	}
	return "";
}

string DictToken::getListByNum(size_t num) {
	if (num < this->list.size())
		return this->list[num];
	else
		return "";
}

string DictToken::getListByName(const string&name) {
	for (size_t i = 0; i < list.size(); i++) {
		if (list[i] == name) {
			return list[i];
		}
	}
	return "";
}

string DictToken::getFuncByNum(size_t num) {
	if (num < this->func.size())
		return this->func[num];
	else
		return "";
}

string DictToken::getFuncByName(const string&name) {
	for (size_t i = 0; i < func.size(); i++) {
		if (func[i] == name) {
			return func[i];
		}
	}
	return "";
}

int DictToken::getPropSize() {
	return this->prop.size();
}

int DictToken::getListSize() {
	return this->list.size();
}

int DictToken::getFuncSize() {
	return this->func.size();
}

void DictToken::setType(const string&type) {
	this->type = type;
}

string DictToken::getType() {
	return this->type;
}

Dict::Dict() {

}

Dict::~Dict() {
	dict.clear();
}

Dict::Dict(const string&filename) {
	try {
		read_dict_from_file(filename, this);
	}
	catch (int err) {
		if (err == WRONG_DATA)
			cerr << "error: wrong data in \"" << filename << "\"";
	}
}

void Dict::read_dict_from_file(const string&filename, Dict*dict) {
	int line_num = 0;
	int token_num = 0;

	string str = "";
	string str2 = "";

	// dict.xml tags
	const string token = "<token";
	const string word = "<word>";
	const string word_end = "</word>";
	const string morph = "<morph>";
	const string morph_end = "</morph>";
	const string type = "<type>";
	const string type_end = "</type>";
	const string parent = "<parent>";
	const string parent_end = "</parent>";
	const string prop = "<prop>";
	const string prop_end = "</prop>";
	const string list = "<list>";
	const string list_end = "</list>";
	const string func = "<func>";
	const string func_end = "</func>";
	const string token_end = "</token>";
	const string dict_end = "</dict>";

	ifstream fin;
	DictToken tmp;

	fin.open(filename);
	if (!fin) throw FILE_OPEN_ERROR;
	cout << "starting reading from \"" << filename << "\"..." << endl;

	while (str != dict_end) {
		line_num++;
		fin >> str;
		str2 = "";
		if (str == token) {	// reading token id field
			fin >> str;
			for (size_t i = 4; i < str.length(); i++) { // getting value of id field
				if (str[i] < 48 || str[i] > 57)
					break;
				str2.push_back(str[i]);
			}
			try {
				tmp.setId(stoi(str2));
			}
			catch (invalid_argument) {
				cerr << "error: wrong representation of syntax tree (id field) in \"" << filename << "\" at line: " << line_num << endl;
				throw WRONG_DATA;
			}
			// cout << "reading token #" << str2 << endl;
		}
		else if (strstr(str.c_str(), word.c_str())) { // reading word field
			for (size_t i = word.length(); i < str.length() - word_end.length(); i++) {
				str2.push_back(str[i]);
			}
			tmp.setWord(strtoupper(str2));
		}
		else if (strstr(str.c_str(), morph.c_str())) { // reading morph field
			for (size_t i = morph.length(); i < str.length() - morph_end.length(); i++) {
				str2.push_back(str[i]);
			}
			tmp.setMorph(strtoupper(str2));
		}
		else if (strstr(str.c_str(), type.c_str())) { // reading type field
			for (size_t i = type.length(); i < str.length() - type_end.length(); i++) {
				str2.push_back(str[i]);
			}
			tmp.setType(strtoupper(str2));
		}
		else if (strstr(str.c_str(), parent.c_str())) { // reading parent field
			for (size_t i = parent.length(); i < str.length() - parent_end.length(); i++) {
				str2.push_back(str[i]);
			}
			try {
				tmp.setParent(stoi(str2));
			}
			catch (invalid_argument) {
				cerr << "error: wrong representation of syntax tree (parent field) in \"" << filename << "\" at line: " << line_num << endl;
				throw WRONG_DATA;
			}
		}
		else if (str == prop) { // reading prop field
			int prop_num = 0;
			fin >> str2;
			while (str2 != prop_end) {
				str = "";
				for (size_t i = 0; i < str2.length(); i++) { // reading each prop in prop field
					if (str2[i] == '=') {
						tmp.addProp(str);
						str = "";
						i++;
						for (; i < str2.length(); i++) {
							if ((str2[i] < '0' || str2[i] > '9') && str2[i] != 'N') {
								break;
							}
							str.push_back(str2[i]);
						}
						break;
					}
					str.push_back(str2[i]);
				}
				tmp.setPropVal(prop_num, strtoupper(str));
				prop_num++;
				fin >> str2;
			}
		}
		else if (str == list) { // reading list field
			fin >> str2;
			while (str2 != list_end) {
				tmp.addList(strtoupper(str2));
				fin >> str2;
			}
		}
		else if (str == func) { // reading func field
			fin >> str2;
			while (str2 != func_end) {
				if (strstr(str2.c_str(), ";")) {
					for (size_t i = 0; i < str2.length() - 1; i++) {
						str[i] = str2[i];
					}
				}
				tmp.addFunc(strtoupper(str2));
				fin >> str2;
			}
		}
		else if (str == token_end) { // adding element to a dict
			token_num++;
			dict->insert(tmp);
			tmp.~DictToken();
		}
	}
	for (int i = 0; i < this->dict.size(); i++) {
		for (int j = 0; j < this->dict.size(); j++) {
			if (this->dict[i].getId() == this->dict[j].getParent()) {
				int size_of_child = this->dict[j].getPropSize();
				int size_of_parent = this->dict[i].getPropSize();
				for (int k = 0; k < size_of_child; k++) {
					for (int l = 0; l < size_of_parent; l++) {
						if (this->dict[i].getPropByNum(l) != this->dict[j].getPropByNum(k)) {
							this->dict[j].addProp(this->dict[i].getPropByNum(l));
							this->dict[j].setPropVal(this->dict[j].getPropSize() - 1, this->dict[i].getPropByNum(l));
						}
					}
				}
			}
		}
	}
	cout << token_num << " tokens read from \"" << filename << "\"" << endl;
}

int Dict::findWord(const string&str) {
	for (size_t i = 0; i < dict.size(); i++) {
		if (this->dict[i].getWord() == strtoupper(str)) {
			return i;
		}
	}
	return -1;
}

string Dict::findPropInWord(const string&word, const string&prop) {
	int wordPos = 0;
	wordPos = findWord(strtoupper(word));
	return this->dict[wordPos].getPropValByName(strtoupper(prop));
}

bool Dict::findListInWord(const string&word, const string&list) {
	int wordPos = 0;
	wordPos = findWord(strtoupper(word));
	if (!this->dict[wordPos].getListByName(strtoupper(list)).empty()) return true;
	else return false;
}

DictToken* Dict::getTokenById(size_t id) {
	if (id < this->dict.size())
		return &dict[id];
	else
		return NULL;
}

int Dict::insert(DictToken&token) {
	if (!token.getWord().empty() && !token.getMorph().empty() && token.getId() > -1) {
		this->dict.push_back(token);
		return 0;
	}
	else {
		return 1;
	}
}

size_t Dict::size() {
	return this->dict.size();
}

bool Dict::empty() {
	return this->dict.empty();
}

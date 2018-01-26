#include "dict.h"

void read_dict_from_file(const string&filename, Dict*dict) {
	string str = "";
	string str2 = "";
	const string token = "<token";
	const string word = "<word>";
	const string word_end = "</word>";
	const string morph = "<morph>";
	const string morph_end = "</morph>";
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
	fin.open(filename);
	if (!fin) return;
	DictToken tmp;
	while (str != dict_end) {
		fin >> str;
		str2 = "";
		if (str == token) {	// token id
			fin >> str;
			for (int i = 4; i < str.length(); i++) {
				if (str[i] < '0' || str[i] > '9') {
					break;
				}
				str2.push_back(str[i]);
			}
			tmp.id = stoi(str2);
			cout << "reading token #" << str2 << endl;
		}
		else if (strstr(str.c_str(), word.c_str())) { // word
			for (int i = word.length(); i < str.length() - word_end.length(); i++) {
				str2.push_back(str[i]);
			}
			tmp.word = str2;
		}
		else if (strstr(str.c_str(), morph.c_str())) { // morph
			for (int i = morph.length(); i < str.length() - morph_end.length(); i++) {
				str2.push_back(str[i]);
			}
			tmp.morph = str2;
		}
		else if (strstr(str.c_str(), parent.c_str())) { // parent
			for (int i = parent.length(); i < str.length() - parent_end.length(); i++) {
				str2.push_back(str[i]);
			}
			tmp.parent = stoi(str2);
		}
		else if (str == prop) { // prop
			fin >> str2;
			while (str2 != prop_end) {
				str = "";
				for (int i = 0; i < str2.length(); i++) {
					if (str2[i] == '=') {
						tmp.prop.push_back(str);
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
				tmp.propval.push_back(str);
				fin >> str2;
			}
		}
		else if (str == list) { // list
			fin >> str2;
			while (str2 != list_end) {
				tmp.list.push_back(str2);
				fin >> str2;
			}
		}
		else if (str == func) { // func
			fin >> str2;
			while (str2 != func_end) {
				if (strstr(str2.c_str(), ";")) {
					for (int i = 0; i < str2.length() - 1; i++) {
						str[i] = str2[i];
					}

				}
				tmp.func.push_back(str2);
				fin >> str2;
			}
		}
		else if (str == token_end) { // dict.pushback
			dict->dict.push_back(tmp);
			tmp.morph.clear();
			tmp.prop.clear();
			tmp.propval.clear();
			tmp.list.clear();
			tmp.func.clear();
		}
	}
}

Dict::Dict() {

}

Dict::Dict(const string&filename) {
	read_dict_from_file(filename, this);
}

int Dict::findWord(const string&str) {
	for (int i = 0; i < dict.size(); i++) {
		if (dict[i].word == str) {
			return i;
		}
	}
	return -1;
}

string Dict::findPropInWord(const string&word, const string&prop) {
	int wordPos = 0;
	wordPos = findWord(word);
	for (int i = 0; i < dict[wordPos].prop.size(); i++) {
		if (dict[wordPos].prop[i] == prop) {
			return dict[wordPos].propval[i];
		}
	}
	return 0;
}

bool Dict::findListInWord(const string&word, const string&list) {
	int wordPos = 0;
	wordPos = findWord(word);
	for (int i = 0; i < dict[wordPos].list.size(); i++) {
		if (dict[wordPos].list[i] == list) {
			return true;
		}
	}
	return false;
}

Dict::~Dict() {
	dict.clear();
}
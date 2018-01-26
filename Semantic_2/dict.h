#pragma once
#include "semantica.h"

class DictToken {
public:
	int id;
	string word;
	string morph;
	int parent;
	vector<string>prop;
	vector<string>propval;
	vector<string>list;
	vector<string>func;
	DictToken() {
		id = -1;
		word = "";
		morph = "";
		parent = -1;
	}
	~DictToken() {
		if (prop.size()) prop.clear();
		if (propval.size()) propval.clear();
		if (list.size()) list.clear();
		if (func.size()) func.clear();
	}
	string get_propval(string&str) {
		if (prop.size()) {
			for (int i = 0; i < prop.size(); i++) {
				if (strtolower2(str) == strtolower2(prop[i])) {
					return propval[i];
				}
			}
		}
		return "";
	}
};

class Dict {
public:
	vector<DictToken>dict;
	Dict();
	Dict(const string&filename);
	int findWord(const string&str); // ���������� id ������ � ������ ������ str � ���� word, ���� ��� ���� � �������, ����� -1
	string findPropInWord(const string&word, const string&prop); // ���� ������� ���������� �������� � ���� propval, ���������������� prop, ����� 0
	bool findListInWord(const string&word, const string&list); // ���������� true, ���� ������ str ������� � ���� list, ����� false
	~Dict();
};
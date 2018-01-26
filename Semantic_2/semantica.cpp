#include"semantica.h"

string strtolower2(string&str) {
	string result = "";
	for (int i = 0; i < str.size(); i++) {
		if (str[i] > 64 && str[i] < 91) {
			result.push_back(str[i] + 32);
		}
		else if (str[i] >= -64 && str[i] <= -33) {
			result.push_back(str[i] + 32);
		}
		else {
			result.push_back(str[i]);
		}
	}
	return result;
}

string strtoupper2(string&str) {
	string result = "";
	for (int i = 0; i < str.size(); i++) {
		if (str[i] > 64 + 32 && str[i] < 91 + 32) {
			result.push_back(str[i] - 32);
		}
		else if (str[i] >= -64 + 32 && str[i] <= -33 + 32) {
			result.push_back(str[i] - 32);
		}
		else {
			result.push_back(str[i]);
		}
	}
	return result;
}

void strtolower(string&str) {
	for (int i = 0; i < str.size(); i++) {
		if (str[i] > 64 && str[i] < 91) {
			str[i] += 32;
		}
		else if (str[i] >= -64 && str[i] <= -33) {
			str[i] += 32;
		}
	}
}

void read_from_file(ifstream&fin, Tree&tree) {
	size_t size;
	int number_of_parent;
	string  relation;
	string buf1, buf2;
	char b;
	vector<string>morfology;
	string name;
	string Initial_form;
	int number;
	for (unsigned int i = 0; i < tree.sent_amt(); i++)
	{
		for (unsigned int j = 0; j < tree.word_amt(i); j++)
		{
			fin >> number;

			fin.get(b);
			fin >> name;
			fin >> Initial_form;
			for (unsigned int k = 0; k < Initial_form.size(); k++)
			{
				if ('à' <= Initial_form[k] && Initial_form[k] <= 'ÿ')
				{
					Initial_form[k] += 'À' - 'à';
				}
			}
			fin.get(b);

			while (b != '\n')
			{

				while (b == '\t' || b == ' ' || b == ',')
					fin.get(b);
				if (b != '\n')
					morfology.resize(morfology.size() + 1);
				else
					break;

				while (b != '\t' && b != ' ' && b != ',' && b != '\n')
				{
					morfology.back() = morfology.back() + b;
					fin.get(b);
				}
				for (int q = 0; q < morfology.size() - 1; q++)
				{
					if (morfology[q] == morfology.back())
					{
						morfology.pop_back();
						break;
					}

				}

			}

			fin >> number_of_parent;
			fin >> relation;

			tree.insert(name, Initial_form, morfology, i, number_of_parent, relation, j);
			morfology.clear();
			Initial_form.clear();
			name.clear();
			relation.clear();
			fin >> buf1;
			fin >> buf1;
			buf1.clear();
		}
	}
	fin >> size;
}

bool isInt(string&str) {
	if (str.empty() || (!isdigit(str[0]) && str[0] != '-' && str[0] != '+')) return false;
	if ((str[0] == '-' || str[0] == '+') && str.size() == 1) return false;
	for (int i = 1; i < str.size(); i++) {
		if (!isdigit(str[i])) return false;
	}
	return true;
}

void read_from_file2(string&filename, Tree*tree) {
	string str1 = "";
	ifstream fin;
	int n = 0;
	int sentnum = 0;
	int wordnum = 0;

	int sent = 1;

	int id = 0;
	string word = "";
	string init = "";
	vector<string>morph;
	int parent_id = 0;
	string rel = "";
	
	if (filename.empty()) return;

	fin.open(filename);

	fin >> wordnum;
	if (wordnum > 0) {
		tree->tree.resize(sent);
		tree->tree[sent - 1].resize(wordnum);
		for (int i = 0; i < wordnum; i++) {
			fin >> id;
			fin >> word;
			fin >> init;
			fin >> str1;
			while (!isInt(str1)) {
				if (str1 != "-" && str1 != "_") morph.push_back(str1);
				fin >> str1;
			}
			parent_id = stoi(str1);
			fin >> str1;
			rel = str1;
			fin >> str1;
			fin >> str1;
			tree->insert(word, init, morph, sent - 1, parent_id, rel, id - 1);
			morph.clear();
		}
	}
}

Tree::Tree() {
	this->sz = 0;
}

Tree::Tree(int sent, vector<int>&word) {
	tree.resize(sent);
	sz = 0;
	for (int i = 0; i < sent; i++) {
		tree[i].resize(word[i]);
		sz += word[i];
	}
}

int Tree::insert(string&word, string&init, vector<string>&morph, int sent, int parent, string&rel, int id) {
	tree[sent][id].id = id;
	tree[sent][id].word = word;
	tree[sent][id].init = init;
	tree[sent][id].morph = morph;
	tree[sent][id].rel.push_back(rel);
	if (parent == 0) {
		tree[sent][id].isroot = true;
		tree[sent][id].parent.push_back(NULL);
		return 0;
	}
	tree[sent][id].parent.push_back(&tree[sent][parent - 1]);
	tree[sent][parent - 1].child.push_back(&tree[sent][id]);
	sz++;
	return 0;
}

int Tree::remove(int sent, int word) { return 0; }
size_t Tree::size() { return 1; }
size_t Tree::sent_amt() { return tree.size(); }
size_t Tree::word_amt(int sent) { return tree[sent].size(); }

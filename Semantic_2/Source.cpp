#include "sem.h"
#include <stdio.h>
#include <Windows.h>

int test() {
	ifstream fin("in.txt");
	Tree tree;
	string str = "dict3s.xml";
	Dict dicts(str);
	int tmp = 0;
	fin >> tmp;
	tree.tree.resize(tmp);
	for (int i = 0; i < tree.sent_amt(); i++) {
		fin >> tmp;
		tree.tree[i].resize(tmp);
	}
	read_from_file(fin, tree);

	return 0;
}

void test2() {
	string str1 = "ЯпабвлгЪ";
	ofstream fout("test.txt");
	strtolower(str1);
	fout << str1 << endl;
	system("pause");
}

void test3() {
	//ifstream fin("in.txt");
	string filename = "Oleg_semantic_in.txt";
	SemTree*tree = new SemTree();
	string str = "dict3s.xml";
	ofstream fout("Oleg_semantic_out.txt");
	//Dict dicts(str);

	read_from_file2(filename, (Tree*)tree);
	tree->analysis();
	str = tree->build_result();
	fout << str;
	return;
}

int main() {
	setlocale(LC_ALL, "rus_rus.1251");
	test3();
	return 0;
}
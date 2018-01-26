#pragma once
#pragma warning(disable:4996)
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>

using namespace std;

struct Conll_line {
	string id;// ���������� ����� �����
	string word;// ���� �����
	string original_form; // ��������� ����� �����
	string parts_of_speech; // ����� ����
	string morph1; // ���������� ������ �����
	string morph2; // ���������� ��������� ����� �����
	string parent; // �������� �����
	string type_of_link; // ��� �����
	string line_1; // - ������������ ���������� ��� ������� � ������� �� ��������
};

/*____________ ������� _____________*/

struct dictionary {
	string words;// ���������� ����� �����
	string parts_of_speach;
	string amount_of_words;
};
struct predlogipadeji {
	string predlog;
	string padej1;
	string padej2;
};
struct fromlettonum {
	string letters;
	string num;
	string mark;
};
struct mathword {
	string word;
	string type;
	string rod;
	string atrib1;
	string atrib2;
	string atrib3;
};

/*__________ ����� _________________*/
struct options {
	string name_of_option;
	string value;
};

bool is_latin(const string& s, const locale& l);
bool is_number(string str);


int read_conll(vector<Conll_line>&conll, int &LINES_COUNT);
void change_id_and_number(vector<Conll_line>&conll, int where_is_delete);
void preparation(vector<Conll_line>&conll,string etap);
void write_conll(vector<Conll_line>&conll, vector<Conll_line>&punct, vector<Conll_line>&punct2, int mode);
int findsomebrack(vector<Conll_line>&conll, size_t start);

void hide(vector<Conll_line>&conll);
void put_in_the_end(vector<Conll_line>&conll, vector<Conll_line>&punct, vector<Conll_line>&punct2,int LINES_COUNT);
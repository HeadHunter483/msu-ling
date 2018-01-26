#pragma once
#pragma warning(disable:4996)
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>

using namespace std;

struct Conll_line {
	string id;// пор€дковый номер слова
	string word;// само слово
	string original_form; // начальна€ форма слова
	string parts_of_speech; // часть речи
	string morph1; // морфологи€ самого слова
	string morph2; // морфологи€ начальной формы слова
	string parent; // родитель слова
	string type_of_link; // тип св€зи
	string line_1; // - об€зательное требование дл€ формата с которым мы работаем
};

/*____________ —ловари _____________*/

struct dictionary {
	string words;// пор€дковый номер слова
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

/*__________ ќпции _________________*/
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
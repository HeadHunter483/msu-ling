#pragma once
#include<fstream>
#include<vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <iostream>
 
using namespace std;
struct parent_pred
{
	int number_sentence;
	int number_word;
	int number_pred;
	parent_pred()
	{
		number_pred = -1;
	}
};
struct predicate
{
	bool is_used;
	int number_of_pred;
	int number_of_standart_pack;
	int number_of_standart_pred;
	int number_of_phrase;
	vector<string> main_word;
	vector<string> dep_words;
	vector<string> atribut;
	vector<parent_pred> pred_atribut;
	vector<parent_pred> children;
	predicate()
	{
		is_used = false;
		number_of_standart_pack = -1;
	}
	void clear()
	{
		main_word.clear();
		dep_words.clear();
		atribut.clear();
		pred_atribut.clear();
		children.clear();
	}
};
struct standart_predicate
{
	vector<string> main_word;
	vector<vector<string>> dep_words;
	vector <string> atribut;
	int amt_of_used_pred;
	standart_predicate()
	{
		amt_of_used_pred = 0;
	}
	
};
struct rules
{
	vector<vector<string>> premise;
	string consequence;
};
struct sentence
{
	string punc;
};
struct node
{
	node* ssilka;
	vector<string>Rule;
	bool root;                       //корень
	bool is_pred;                    // флаг является ли данная вершина предикатной
	bool is_delete;                  // флаг удаления, чтобы не рассматривать вершины, которые были задействованы в создании предиката, так как их значения уже хранятся внутри предиката
	size_t number; 
	vector<node*> parent;            // на всякий случай пока предполагается, что родителей несколько
	vector<node*> children;
	string name;	
	vector<string> relation;         // отношения между рассматриваемым словом его родителями 
	vector<string>morfology;
	string Initial_form;             // лемма
	vector<predicate> predicate;
	bool is_number;
	double name_number;
	bool is_phrase;
	int number_of_phrase;
	int number_of_sentences;
	void clear()
	{
		parent.clear();
		children.clear();
		relation.clear();
		morfology.clear();
	}
};
struct phrase
{
	string main;
	bool is_used;
	vector<phrase> atribut_phrase;
	vector<parent_pred>atribut_predicate;
	vector<string>Rule;
	phrase()
	{
		is_used = false;
	}

};
struct operation
{
	string main;
	string dep;
	string rule;
};
class TREE
{
	size_t amount_of_sentences;
	vector<size_t>amount_of_words_in_sentence;
	vector<vector<node>> My_tree;							// дерево с которым я работаю
	vector<phrase>My_phrase;								// фразы
	vector<vector <standart_predicate>>standart_predicat;	// предикаты из списка, двумерный так как стандартные предикаты поделены на 2 типа
	vector<node*>data;										// пока планирую хранить то что оказалось известно
	vector<sentence> sent_data;								// данные предложения, пока толко знак
	vector<node*>unknown_data;								//вроде как хотела хранить здесь данные которые в конце оказались неизвестными
	vector<vector<operation>>operation;						// список операций
	vector<vector<rules>>Rules;								// список правил
public:
	TREE(size_t amount_of_sentences, vector<size_t>amount_of_words_in_sentence);
	~TREE();
	
	void delete_node(size_t number_of_sentence, size_t number_of_word); // рекурсивно ставит флаги удаления
	int insert(size_t number_of_sentence,  size_t parent_number, string relations, size_t number_of_word, string Initial_form, string name, vector<string>morfology);						// считывает дерево
	void standart_predikat_1();
	void standart_predikat_2();
	void standart_predikat_3();
	void list(ofstream& outfile);
	void start();
	void print_sentences(ofstream& outfile);
	void change_out_words(int number_of_sentence, int number_of_word, int type);								// первичное отождествление
	int find_pred(vector<vector<string>> word, size_t number_of_sentence, size_t number_of_word);	// ищет предикато по имени
	void print_predicate(size_t number_of_sentence, size_t number_of_word, ofstream& outfile);							// распечатывание предиката
	node* find_close_pred(vector<vector<string>>words,size_t number_of_sentece, size_t number_of_word);								// ищет ближайший предикат, если нет непосредственно связанных
	int find_close_child(string words, size_t number_of_sentece, size_t number_of_word);								// ищет ближайший предикат, если нет непосредственно связанных
	void check();																							//	
	void measure();					//скрепляет число и меру
	void number_converter();		//конвертер чисел
	void bonding();					//определение со словом
	void bonding_2();				//скрепляет такие какие может вообще любые (выкл)
	void bonding_4();				// соединает прилагательное существительное
	void bonding_5();				//глагол с детьми
	void standart_predikat_4();	
	void rule();					//считывает и закрепляет правила
	void rule7();					// правила 2 типа
	void rule_1();					//меняет простые правила следствием
	void rule_3();					//правила существования другого элемента такого же
	void rule_5();					//правила фразы "если", "то"
	void rule_6();					//правила про "Доказать" 
	void delete_replace(size_t number_of_sentence, size_t number_of_word);
	int standart_predicate_5();
	void rule_4();					// правила про меньще больше по названию
	void operations();				//обработка операций
	void clear();
	void dash();
};
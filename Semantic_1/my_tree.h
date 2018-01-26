#pragma once
#include <fstream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <iostream>
 
using namespace std;

struct predicate
{
	vector<string> main;
	vector <string> atribut;
	int amt_of_used_pred;
};

struct sentence
{
	string punc;
};
struct node
{
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
	string name_of_pred;             // имя предиката
	vector<vector<string>> arg_p;    // аргементы предиката, двумерный так как предикатов может быть несколько у одной вершины
	node* parent_predicate;          // если предикато составной то нужно запомнить родителя
	bool is_number;
	double name_number;
	void clear()
	{
		parent.clear();
		children.clear();
		relation.clear();
		morfology.clear();
	}
};


class TREE
{
	size_t amount_of_sentences;
	vector<size_t>amount_of_words_in_sentence;
	vector<vector<node>> My_tree;										// дерево с которым я работаю
	vector <vector<predicate>> standart_predicat;	// предикаты из списка, двумерный так как стандартные предикаты поделены на 2 типа
	vector<node*>data;
	vector<sentence> sent_data;
	vector<node*>unknown_data;
public:
	TREE(size_t amount_of_sentences, vector<size_t>amount_of_words_in_sentence);
	~TREE();
	void delete_node(size_t number_of_sentence, size_t number_of_word);									// рекурсивно ставит флаги удаления
	int insert(size_t number_of_sentence,  size_t parent_number, string relations, size_t number_of_word, string Initial_form, string name, vector<string>morfology);						// считывает дерево
	void standart_predikat_1();
	void standart_predikat_2();
	void list();
	void start();
	void change_out_words(size_t number_of_sentence, size_t number_of_word);								// первичное отождествление
	int find_pred(string word, size_t number_of_sentence, size_t number_of_word);							// ищет предикато по имени
	void print_predicate(size_t number_of_sentence, size_t number_of_word);								// распечатывание предиката
	int find_close_pred(size_t number_of_sentece, size_t number_of_word);								// ищет ближайший предикат, если нет непосредственно связанных
	void measure();
	void number_converter();
};
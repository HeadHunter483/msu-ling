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
	bool root;                       //������
	bool is_pred;                    // ���� �������� �� ������ ������� �����������
	bool is_delete;                  // ���� ��������, ����� �� ������������� �������, ������� ���� ������������� � �������� ���������, ��� ��� �� �������� ��� �������� ������ ���������
	size_t number; 
	vector<node*> parent;            // �� ������ ������ ���� ��������������, ��� ��������� ���������
	vector<node*> children;
	string name;	
	vector<string> relation;         // ��������� ����� ��������������� ������ ��� ���������� 
	vector<string>morfology;
	string Initial_form;             // �����
	string name_of_pred;             // ��� ���������
	vector<vector<string>> arg_p;    // ��������� ���������, ��������� ��� ��� ���������� ����� ���� ��������� � ����� �������
	node* parent_predicate;          // ���� ��������� ��������� �� ����� ��������� ��������
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
	vector<vector<node>> My_tree;										// ������ � ������� � �������
	vector <vector<predicate>> standart_predicat;	// ��������� �� ������, ��������� ��� ��� ����������� ��������� �������� �� 2 ����
	vector<node*>data;
	vector<sentence> sent_data;
	vector<node*>unknown_data;
public:
	TREE(size_t amount_of_sentences, vector<size_t>amount_of_words_in_sentence);
	~TREE();
	void delete_node(size_t number_of_sentence, size_t number_of_word);									// ���������� ������ ����� ��������
	int insert(size_t number_of_sentence,  size_t parent_number, string relations, size_t number_of_word, string Initial_form, string name, vector<string>morfology);						// ��������� ������
	void standart_predikat_1();
	void standart_predikat_2();
	void list();
	void start();
	void change_out_words(size_t number_of_sentence, size_t number_of_word);								// ��������� ��������������
	int find_pred(string word, size_t number_of_sentence, size_t number_of_word);							// ���� ��������� �� �����
	void print_predicate(size_t number_of_sentence, size_t number_of_word);								// �������������� ���������
	int find_close_pred(size_t number_of_sentece, size_t number_of_word);								// ���� ��������� ��������, ���� ��� ��������������� ���������
	void measure();
	void number_converter();
};
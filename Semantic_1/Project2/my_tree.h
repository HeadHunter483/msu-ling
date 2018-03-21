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
	vector<vector<node>> My_tree;							// ������ � ������� � �������
	vector<phrase>My_phrase;								// �����
	vector<vector <standart_predicate>>standart_predicat;	// ��������� �� ������, ��������� ��� ��� ����������� ��������� �������� �� 2 ����
	vector<node*>data;										// ���� �������� ������� �� ��� ��������� ��������
	vector<sentence> sent_data;								// ������ �����������, ���� ����� ����
	vector<node*>unknown_data;								//����� ��� ������ ������� ����� ������ ������� � ����� ��������� ������������
	vector<vector<operation>>operation;						// ������ ��������
	vector<vector<rules>>Rules;								// ������ ������
public:
	TREE(size_t amount_of_sentences, vector<size_t>amount_of_words_in_sentence);
	~TREE();
	
	void delete_node(size_t number_of_sentence, size_t number_of_word); // ���������� ������ ����� ��������
	int insert(size_t number_of_sentence,  size_t parent_number, string relations, size_t number_of_word, string Initial_form, string name, vector<string>morfology);						// ��������� ������
	void standart_predikat_1();
	void standart_predikat_2();
	void standart_predikat_3();
	void list(ofstream& outfile);
	void start();
	void print_sentences(ofstream& outfile);
	void change_out_words(int number_of_sentence, int number_of_word, int type);								// ��������� ��������������
	int find_pred(vector<vector<string>> word, size_t number_of_sentence, size_t number_of_word);	// ���� ��������� �� �����
	void print_predicate(size_t number_of_sentence, size_t number_of_word, ofstream& outfile);							// �������������� ���������
	node* find_close_pred(vector<vector<string>>words,size_t number_of_sentece, size_t number_of_word);								// ���� ��������� ��������, ���� ��� ��������������� ���������
	int find_close_child(string words, size_t number_of_sentece, size_t number_of_word);								// ���� ��������� ��������, ���� ��� ��������������� ���������
	void check();																							//	
	void measure();					//��������� ����� � ����
	void number_converter();		//��������� �����
	void bonding();					//����������� �� ������
	void bonding_2();				//��������� ����� ����� ����� ������ ����� (����)
	void bonding_4();				// ��������� �������������� ���������������
	void bonding_5();				//������ � ������
	void standart_predikat_4();	
	void rule();					//��������� � ���������� �������
	void rule7();					// ������� 2 ����
	void rule_1();					//������ ������� ������� ����������
	void rule_3();					//������� ������������� ������� �������� ������ ��
	void rule_5();					//������� ����� "����", "��"
	void rule_6();					//������� ��� "��������" 
	void delete_replace(size_t number_of_sentence, size_t number_of_word);
	int standart_predicate_5();
	void rule_4();					// ������� ��� ������ ������ �� ��������
	void operations();				//��������� ��������
	void clear();
	void dash();
};
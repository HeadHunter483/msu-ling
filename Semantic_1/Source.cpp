#pragma once
#include<iostream>
#include<vector>
#include <stdio.h>
#include <stdlib.h>
#include "my_tree.h"
#include <string>
#include<fstream>
#include <clocale>

using namespace std;

ifstream infile;
ofstream outfile;



void scan_data(TREE* mytree, size_t amount_of_sentences, vector<size_t> amount_of_words_in_sentence)
{
	
	size_t size;
	
	int number_of_parent;
	string  relation;
	string buf1, buf2;
	char b;
	vector<string>morfology;
	string name;
	string Initial_form;
	size_t number;

	for (unsigned int i = 0; i < amount_of_sentences; i++)
	{
		for (unsigned int j = 0; j < amount_of_words_in_sentence[i]; j++)

		{
			infile >>number;
			number--;
			infile.get(b);
			infile >> name;
			infile >> Initial_form;
			for (unsigned int k = 0; k < Initial_form.size(); k++)
			{
				if ('à' <= Initial_form[k] && Initial_form[k] <= 'ÿ')
				{
					Initial_form[k] += 'À' - 'à';
				}
			}
			infile.get(b);

			while (b != '\n')
			{

				while (b == '\t' || b == ' ' || b == ',')
					infile.get(b);
				if (b != '\n')
					morfology.resize(morfology.size() + 1);
				else
					break;
				
				while (b != '\t' && b != ' ' && b != ',' && b != '\n')
				{
					morfology.back() = morfology.back() + b;
					infile.get(b);
				}


			}

			infile >> number_of_parent;
			infile >> relation;
			number_of_parent--;
			mytree->insert(i,number_of_parent, relation, number, Initial_form, name, morfology);
			morfology.clear();
			Initial_form.clear();
			name.clear();
			relation.clear();
			infile >> buf1;
			infile >> buf1;
			buf1.clear();
		}
	}
	infile >> size;
	
}


int main()
{
	
	setlocale(LC_CTYPE, "rus");
	size_t amount_of_sentences;
	vector<size_t> amount_of_words_in_sentence;
	
	infile.open("in.txt");
	outfile.open("out.txt");
	
	if (!infile) {
		cout << "Cannot open file.";
	}
	
	infile >> amount_of_sentences;

	for (unsigned int i = 0; i < amount_of_sentences; i++)
	{
		amount_of_words_in_sentence.resize(amount_of_words_in_sentence.size() + 1);
		infile >> amount_of_words_in_sentence.back();
	}


	TREE * mytree = new TREE(amount_of_sentences, amount_of_words_in_sentence);
	scan_data(mytree, amount_of_sentences, amount_of_words_in_sentence);
	mytree->start();
	mytree->standart_predikat_1();
	mytree->standart_predikat_2();
	mytree->list();
 	infile.close();
	outfile.close();
	getchar();
	return 0;
}
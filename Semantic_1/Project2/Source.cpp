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
	int num_buf=0;
	bool is_num = false;
	for (unsigned int i = 0; i < amount_of_sentences; i++)
	{
		for (unsigned int j = 0; j < amount_of_words_in_sentence[i]; j++)

		{
			
			infile >>number;
			number--;
			
			infile >> name;
			for (unsigned int k = 0; k < name.size(); k++)
			{
				if ('а' <= name[k] && name[k] <= 'я' )
				{
					name[k] += 'А' - 'а';
				}
				if (name[k] <= 'ё')
				{
					name[k] += 'Ё' - 'ё';
				}
			}
			infile >> Initial_form;
			for (unsigned int k = 0; k < Initial_form.size(); k++)
			{
				if ('а' <= Initial_form[k] && Initial_form[k] <= 'я')
				{
					Initial_form[k] += 'А' - 'а';
				}
				if (Initial_form[k] <= 'ё')
				{
					Initial_form[k] += 'Ё' - 'ё';
				}
			}
			infile.get(b);

			while (b != '\n')
			{

				while (b == '\t' || b == ' ' || b == ',' )
					infile.get(b);
				if (b != '\n')
					morfology.resize(morfology.size() + 1);
				else
					break;
				
				while (b != '\t' && b != ' ' && b != ',' && b != '\n' )
				{
					if ('a' <= b && b <= 'z')
					{
						b += 'A' - 'a';
					}
					morfology.back() = morfology.back() + b;
					infile.get(b);
				}
				for (int q = 0; q < morfology.size() - 1; q++)
				{
					if (morfology[q] == morfology.back())
					{
						morfology.pop_back();
						break;
					}

				}
				num_buf = 0;
				is_num = true;
				for (int q = 0; q < morfology.back().size(); q++)
				{
					if ('0' <= morfology.back()[q] && morfology.back()[q] <= '9')
					{

						num_buf = num_buf * 10 + morfology.back()[q] - '0';
					}
					else
					{
						is_num = false;
					}
					
				}
				if (is_num)
				{
					number_of_parent = num_buf;
					morfology.pop_back();
					break;
				}

			}

			infile >> relation;
			number_of_parent--;
			mytree->insert(i,number_of_parent, relation, number, Initial_form, name, morfology);
			morfology.clear();
			Initial_form.clear();
			name.clear();
			relation.clear();
		
			buf1.clear();
		}
	}
	
	
}


int main(int argc, char **argv)
{
	
	setlocale(LC_CTYPE, "rus");
	size_t amount_of_sentences;
	vector<size_t> amount_of_words_in_sentence;
	int amount_w;
	string in = "in.txt";        // задаем название файла, который будет использоваться в случаи не определения входного файла пользователем
	string out = "out.txt";      // задаем название файла, который будет использоваться в случаи не определения выходного файла пользователем
	ofstream outfile;
	for (int i = 1; i < argc; i++)
	{
		if (i==1)
			in = argv[i];
		if (i==2)
			out = argv[i];
	}
	infile.open(in);
	outfile.open(out);
	if (!infile) {
		cout << "Cannot open file.";
	}
	int amount_of_ploblems;
	infile >> amount_of_ploblems;
	for (int j = 0; j < amount_of_ploblems; j++)
	{
		outfile << endl << "Предложение: " << j+1;
		infile >> amount_of_sentences;
		amount_of_words_in_sentence.resize(amount_of_sentences);
		for (unsigned int i = 0; i < amount_of_sentences; i++)
		{
			
			infile >> amount_w;
			amount_of_words_in_sentence[i]=amount_w;
		}
		TREE * mytree = new TREE(amount_of_sentences, amount_of_words_in_sentence);
		scan_data(mytree, amount_of_sentences, amount_of_words_in_sentence);
		mytree->print_sentences(outfile);
		mytree->start();
		mytree->rule();
		mytree->standart_predikat_1();
		mytree->rule_1();
		mytree->standart_predikat_3();
		mytree->standart_predikat_2();
		mytree->standart_predikat_2();
		mytree->standart_predikat_4();
		mytree->rule7();
		mytree->bonding();
		mytree->rule_3();
		mytree->operations();
		mytree->dash();
		mytree->bonding_4();
		mytree->bonding_5();
		mytree->rule_4();
		mytree->rule_5();
		mytree->rule_6();
		mytree->list(outfile);
		mytree->clear();
		amount_of_words_in_sentence.clear();
	}
	infile.close();
	
	return 0;
}
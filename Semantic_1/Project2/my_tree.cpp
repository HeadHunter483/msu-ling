#include"my_tree.h"

TREE::TREE(size_t amount_of_sentences, vector<size_t>amount_of_words_in_sentence)
{ 

	this->amount_of_sentences = amount_of_sentences;
	this->amount_of_words_in_sentence = amount_of_words_in_sentence;
	My_tree.resize(amount_of_sentences);
	for (unsigned int i = 0; i < amount_of_sentences; i++)
	{
		My_tree[i].resize(amount_of_words_in_sentence[i] - 1);
	}
	sent_data.resize(amount_of_sentences);
}
TREE::~TREE()
{
	My_tree.clear();
}
int TREE::insert(size_t number_of_sentence, size_t parent_number, string relations, size_t number_of_word, string Initial_form, string name, vector<string>morfology)
{
	if (My_tree[number_of_sentence].size() == number_of_word)
		sent_data[number_of_sentence].punc = name;
	else
	{

		My_tree[number_of_sentence][number_of_word].morfology = morfology;
		My_tree[number_of_sentence][number_of_word].name = name;
		My_tree[number_of_sentence][number_of_word].number_of_sentences = number_of_sentence;
		My_tree[number_of_sentence][number_of_word].number = number_of_word;
		My_tree[number_of_sentence][number_of_word].Initial_form = Initial_form;
		My_tree[number_of_sentence][number_of_word].is_number = false;
		My_tree[number_of_sentence][number_of_word].relation.push_back(relations);
		My_tree[number_of_sentence][number_of_word].root = false;
		My_tree[number_of_sentence][number_of_word].is_pred = false;
		My_tree[number_of_sentence][number_of_word].is_delete = false;
		My_tree[number_of_sentence][number_of_word].is_phrase = false;
		My_tree[number_of_sentence][number_of_word].ssilka = NULL;
		if (parent_number == -1)
		{
			My_tree[number_of_sentence][number_of_word].root = true;
			My_tree[number_of_sentence][number_of_word].is_pred = false;
			return 1;
		}
		My_tree[number_of_sentence][number_of_word].parent.push_back(&My_tree[number_of_sentence][parent_number]);
		My_tree[number_of_sentence][parent_number].children.push_back(&My_tree[number_of_sentence][number_of_word]);

	}
	return 1;
}
void TREE::change_out_words(int number_of_sentence, int number_of_word, int type)
{

	string buf;
	string exception = "≈√Œ";
	bool is_exception = false;
	bool flag = false;
	string dep_buf;
	if (type == 0)
	{
		if (exception == My_tree[number_of_sentence][number_of_word].Initial_form)
			is_exception = true;
		for (unsigned int i = 0; i < My_tree[number_of_sentence][number_of_word].morfology.size(); i++)
		{
			if (is_exception || My_tree[number_of_sentence][number_of_word].morfology[i] == "M" || My_tree[number_of_sentence][number_of_word].morfology[i] == "SG" || My_tree[number_of_sentence][number_of_word].morfology[i] == "N" || My_tree[number_of_sentence][number_of_word].morfology[i] == "PL")
			{
				if (!is_exception)
					buf = My_tree[number_of_sentence][number_of_word].morfology[i];
				else
					buf = "M";
				while (number_of_sentence >= 0)
				{
					for (int j = number_of_word - 1; j >= 0; j--)
					{
						if (My_tree[number_of_sentence][j].relation[0] == "opred")
						{
							flag = true;
							for (int iter = 0; iter < My_tree[number_of_sentence][j].morfology.size(); iter++)
							{
								if (My_tree[number_of_sentence][j].morfology[iter] == "SG" || My_tree[number_of_sentence][j].morfology[iter] == "M" || My_tree[number_of_sentence][j].morfology[iter] == "N" || My_tree[number_of_sentence][j].morfology[iter] == "PL")
								{
									dep_buf = My_tree[number_of_sentence][j].morfology[iter];
									break;
								}
							}
						}
						if (!My_tree[number_of_sentence][j].morfology.empty() && My_tree[number_of_sentence][j].morfology[0] == "S")
						{
							
							
								if (flag)
								{
									for (unsigned int k = 0; k < My_tree[number_of_sentence][j].morfology.size(); k++)
									{
										if (My_tree[number_of_sentence][j].morfology[k] == dep_buf)
										{
											flag = false;
											break;
										}
									}
									if (!flag)
										continue;
								}
								else
								{
									for (unsigned int k = 0; k < My_tree[number_of_sentence][j].morfology.size(); k++)
									{
										if (My_tree[number_of_sentence][j].morfology[k] == buf)
										{
											My_tree[number_of_sentence][j].ssilka = &My_tree[number_of_sentence][number_of_word];
											My_tree[number_of_sentence][number_of_word].morfology[0] = "S";
											return;
										}
									}
								}

							
						}
					}
					number_of_sentence--;
				}
				if (number_of_word < 0 || number_of_sentence < 0)
					break;
			}
		}
	}
	if (type == 1)
	{
		int num;
		for (size_t parent = 0; parent < My_tree[number_of_sentence][number_of_word].parent.size(); parent++)
		{
			buf = My_tree[number_of_sentence][number_of_word].parent[parent]->Initial_form;
			for (int p = number_of_sentence; p >= 0; p--)
			{
				if (p == number_of_sentence)
					num = number_of_word;
				else
					num = My_tree[p].size() - 1;
				for (int g = num; g >= 0; g--)
				{

					if (My_tree[p][g].Initial_form== buf)
					{
						My_tree[p][g].ssilka = My_tree[number_of_sentence][number_of_word].parent[parent];
						//My_tree[number_of_sentence][number_of_word].parent[parent]->ssilka = &My_tree[p][g];
					}
				}
			}
		}
		delete_node(number_of_sentence, number_of_word);

	}
}
void TREE::measure()
{
	ifstream file;
	vector <string> measure;
	vector<int> number_of_children_with_the_necessary_morfology;
	node mynode;
	file.open("measure.txt");
	if (!file)
		cout << "cannot open file: measure.txt";
	
	while (!file.eof())
	{
		measure.resize(measure.size() + 1);
		file >> measure.back();
	}

	for (unsigned int i = 0; i < My_tree.size(); i++)
	{
		for (unsigned int j = 0; j < My_tree[i].size();j++)
		{
			for (unsigned int k = 0; k < measure.size(); k++)
			{
				if (measure[k] == My_tree[i][j].Initial_form)
				{
					for (unsigned int n = 0; n < My_tree[i][j].children.size(); n++)
					{
						if (!My_tree[i][j].children[n]->morfology.empty() && My_tree[i][j].children[n]->morfology[0] == "NUM")
						{
							number_of_children_with_the_necessary_morfology.push_back(My_tree[i][j].children[n]->number);
						}
					}
					for (unsigned int n = 0; n < number_of_children_with_the_necessary_morfology.size(); n++)
					{
						//My_tree[i].resize(My_tree[i].size() + 1);
						
						My_tree[i][number_of_children_with_the_necessary_morfology[n]].parent = My_tree[i][j].parent;
						My_tree[i][number_of_children_with_the_necessary_morfology[n]].Initial_form = My_tree[i][number_of_children_with_the_necessary_morfology[n]].Initial_form + "_" + My_tree[i][j].Initial_form;
						My_tree[i][number_of_children_with_the_necessary_morfology[n]].name = My_tree[i][number_of_children_with_the_necessary_morfology[n]].Initial_form;	
					
						for (unsigned int l = 0; l < My_tree[i][j].parent.size(); l++)
						{
							My_tree[i][j].parent[l]->children.push_back(&My_tree[i][number_of_children_with_the_necessary_morfology[n]]);
						}
					}

					number_of_children_with_the_necessary_morfology.clear();
					delete_node(i, j);
				}
				
			}
		}
	}

}
void TREE::start()
{
	ifstream file, file1,file2;
	int buf_int;
	string buf;
	bool flag = false, last_flag = false;
	vector<vector<string>> check_words;
	vector<vector<string>>exception;

	bool is__an_exception = false;
	file.open("check_words.txt");
	if (!file)
		cout << "cannot open file: check_words.txt";

	file2.open("operations.txt");
	if (!file2)
		cout << "cannot open file: standart_predicate.txt";

	while (!file2.eof())
	{

		file2 >> buf;
		if (buf == "operations")
		{
			file2 >> buf_int;
			operation.resize(buf_int);
		}
		else
		{
			operation.back().resize(operation.back().size()+1);
			operation.back().back().main = buf;
			file2 >> buf;
			if (buf == "(")
			{

				file2 >> buf;
				while (buf != ")")
				{
					operation.back().back().dep += buf;
					file2 >> buf;
				}
				file2 >> buf;
			}
			
			operation.back().back().rule = buf;
		}

	}
	file1.open("standart_predicate.txt");
	if (!file1)
		cout << "cannot open file: standart_predicate.txt";

	while (!file.eof())
	{

		file >> buf;
		if (buf == "“»œ")
		{
			file >> buf_int;
			check_words.resize(buf_int);
			file >> buf;
		
		}
		check_words.back().push_back(buf);
		
	}

	buf.clear();
	while (!file1.eof())
	{

		file1 >> buf;
		if (buf == "standart_predicate")
		{
			file1 >> buf_int;
			this->standart_predicat.resize(buf_int);
		}
		else
		{
			if (buf != ")" && buf != "(" && !flag && !last_flag)
			{

				standart_predicat.back().resize(standart_predicat.back().size() + 1);
				while (buf != "(")
				{
					standart_predicat.back().back().main_word.push_back(buf);
					file1 >> buf;
				}
				file1 >> buf;

				while (buf != ")")
				{

					standart_predicat.back().back().atribut.push_back(buf);
					standart_predicat.back().back().amt_of_used_pred = 0;
					file1 >> buf;
				}
			}
			file1 >> buf;
			if (buf == ":")
			{
				flag = true;
				
				while (buf != ";")
				{
					standart_predicat.back().back().dep_words.resize(standart_predicat.back().back().dep_words.size() + 1);
					file1 >> buf;
					while (buf != "," && buf != ";")
					{
						standart_predicat.back().back().dep_words.back().push_back(buf);
						file1 >> buf;
					}
					
				}
				
				
			}
			flag = false;
		}

	}

	for (unsigned int i = 0; i < My_tree.size(); i++)
	{
		for (unsigned int j = 0; j < check_words.size(); j++)
		{
			for (unsigned int k = 0; k < My_tree[i].size(); k++)
			{
				for (unsigned int check = 0; check < check_words.size(); check++)
				{
					if (My_tree[i][k].Initial_form == check_words[j][check])
					{
						change_out_words(i, k,j);
					}
				}
			}
		}
	}
	for (unsigned int i = 0; i < My_tree.size(); i++)
	{
		
			for (unsigned int k = 0; k < My_tree[i].size(); k++)
			{
				if ((My_tree[i][k].Initial_form == "," || My_tree[i][k].Initial_form == "»")&& My_tree[i][k].children.size()==0)
				{
					delete_node(i, k);
				}
			}
		
	}
	number_converter();
	measure();
	/*
cout << endl;
	cout << "ÂÁÛÎ¸Ú‡Ú ÔÂ‚Ó„Ó ¯‡„‡:";
	cout << endl;
	for (unsigned int i = 0; i < My_tree.size(); i++)
	{
		for (unsigned int k = 0; k < My_tree[i].size(); k++)
		{
			if (!My_tree[i][k].is_delete)
			{
				cout << My_tree[i][k].name;
				if (!My_tree[i][k].parent.empty())
					cout << "     " << My_tree[i][k].parent[0]->name << "     " << My_tree[i][k].relation[0];

				cout << endl << "                   ÃÓÙÓÎÓ„Ëˇ: ";

				for (unsigned int j = 0; j < My_tree[i][k].morfology.size(); j++)
					cout << My_tree[i][k].morfology[j] << ",";
				cout << endl;
			}
			
		}
		cout << endl;
	}*/
	file.close();
	file1.close();
	
}
void TREE::delete_node(size_t number_of_sentence, size_t number_of_word)
{
	
	My_tree[number_of_sentence][number_of_word].is_delete = true;
	My_tree[number_of_sentence][number_of_word].Initial_form.clear();

	for (unsigned int k = 0; k < My_tree[number_of_sentence][number_of_word].parent.size(); k++)
	{
		if (!My_tree[number_of_sentence][number_of_word].parent[k]->is_delete)
		{
			for (unsigned int j = 0; j < My_tree[number_of_sentence][number_of_word].parent[k]->children.size() - 1; j++)
			if (My_tree[number_of_sentence][number_of_word].parent[k]->children[j]->number == number_of_word)
			for (unsigned int i = j + 1; i < My_tree[number_of_sentence][number_of_word].parent[k]->children.size(); i++)
				My_tree[number_of_sentence][number_of_word].parent[k]->children[i - 1] = My_tree[number_of_sentence][number_of_word].parent[k]->children[i];
			My_tree[number_of_sentence][number_of_word].parent[k]->children.pop_back();
		}
	}

	My_tree[number_of_sentence][number_of_word].clear();

		for (unsigned int k = 0; k < My_tree[number_of_sentence].size(); k++)
		{
			for (unsigned int j = 0; j < My_tree[number_of_sentence][k].parent.size(); j++)
			if (number_of_word == My_tree[number_of_sentence][k].parent[j]->number)
				delete_node(number_of_sentence, k);
		}
	
		
	
}
void TREE::standart_predikat_1()
{
	vector<int> arg;
	string buf;
	string standart_arg;
	int dep_word = -1;
	bool is_pl = true;
	int number,buf_number;
	int count = 0;
	for (unsigned int i = 0; i < My_tree.size(); i++)
	{
		for (unsigned int j = 0; j < My_tree[i].size(); j++)
		{
			if (!My_tree[i][j].is_pred)
			{
				for (unsigned int count_p = 0; count_p < standart_predicat[0].size(); count_p++)
				{

					if (My_tree[i][j].Initial_form == standart_predicat[0][count_p].main_word[0])
					{
						
						for (unsigned int count_child = 0; count_child < My_tree[i][j].children.size(); count_child++)
						{
							for (unsigned int count_dep = 0; count_dep < standart_predicat[0][count_p].dep_words.size(); count_dep++)
							{

								if (standart_predicat[0][count_p].dep_words[count_dep][0] == My_tree[i][j].children[count_child]->Initial_form)
								{
									dep_word = My_tree[i][j].children[count_child]->number;
								}
							}

							if (!My_tree[i][j].children[count_child]->morfology.empty() && (My_tree[i][j].children[count_child]->morfology[0] == "NONLEX" || My_tree[i][j].children[count_child]->morfology[0] == "ABR" || My_tree[i][j].children[count_child]->morfology[0] == "ATR"))
							{
								for (unsigned int count_parent = 0; count_parent < My_tree[i][j].children[count_child]->parent.size(); count_parent++)
								{
									if (My_tree[i][j].children[count_child]->parent[count_parent]->number == j &&My_tree[i][j].children[count_child]->relation[count_parent] == "atrib")
									{
										arg.push_back(My_tree[i][j].children[count_child]->number);
									}
								}
							}
							if (!My_tree[i][j].children[count_child]->morfology.empty() && My_tree[i][j].children[count_child]->is_number )
							{
								for (unsigned int count_parent = 0; count_parent < My_tree[i][j].children[count_child]->parent.size(); count_parent++)
								{
									if (My_tree[i][j].children[count_child]->parent[count_parent]->number == j &&My_tree[i][j].children[count_child]->relation[count_parent] == "kolichest")
									{
										count = My_tree[i][j].children[count_child]->name_number;
										delete_node(i, My_tree[i][j].children[count_child]->number);
										break;
									}
								}
							}
						}

						if (!arg.empty())
						{
							if (count = arg.size())
							{
								for (unsigned int k = 0; k < arg.size(); k++)
								{
									My_tree[i][j].is_pred = true;
									My_tree[i][j].predicate.resize(My_tree[i][j].predicate.size() + 1);
									My_tree[i][j].predicate.back().main_word.push_back(My_tree[i][j].Initial_form);
									My_tree[i][j].predicate.back().number_of_standart_pack = 0;
									My_tree[i][j].predicate.back().number_of_standart_pred = count_p;
									if (dep_word != -1)
										My_tree[i][j].predicate.back().dep_words.push_back(My_tree[i][dep_word].Initial_form);
									My_tree[i][j].predicate.back().number_of_pred = 0;
									My_tree[i][j].predicate.back().atribut.push_back(My_tree[i][arg[k]].Initial_form);
								}
							}

							if (count < arg.size())
							{
								for (unsigned int k = 0; k < count; k++)
								{
									My_tree[i][j].is_pred = true;
									My_tree[i][j].predicate.resize(My_tree[i][j].predicate.size() + 1);
									My_tree[i][j].predicate.back().main_word.push_back(My_tree[i][j].Initial_form);
									if (dep_word != -1)
										My_tree[i][j].predicate.back().dep_words.push_back(My_tree[i][dep_word].Initial_form);
									My_tree[i][j].predicate.back().number_of_pred = 0;
									My_tree[i][j].predicate.back().number_of_standart_pack = 0;
									My_tree[i][j].predicate.back().number_of_standart_pred = count_p;

									My_tree[i][j].predicate.back().atribut.push_back(My_tree[i][arg[k]].Initial_form);
								}
							}

						}
						else
						{
							for (unsigned int k = 0; k < My_tree[i][j].morfology.size(); k++)
							{
								if (My_tree[i][j].morfology[k] == "SG")
								{
									is_pl = false;
									break;
								}
							}
							if (count == 0)
							{
								My_tree[i][j].is_pred = true;
								
								My_tree[i][j].predicate.resize(My_tree[i][j].predicate.size() + 1);
								My_tree[i][j].predicate.back().main_word.push_back(My_tree[i][j].Initial_form);
								if (dep_word != -1)
									My_tree[i][j].predicate.back().dep_words.push_back(My_tree[i][dep_word].Initial_form);
								My_tree[i][j].predicate.back().number_of_pred = 0;
								standart_predicat[0][count_p].amt_of_used_pred++;
								number = standart_predicat[0][count_p].amt_of_used_pred;
								My_tree[i][j].predicate.back().number_of_standart_pack = 0;
								My_tree[i][j].predicate.back().number_of_standart_pred = count_p;

								buf.clear();
								while (number > 0)
								{

									buf.resize(buf.size() + 1);
									buf.back() = number % 10 + '0';
									number = (number - number % 10) / 10;
								}
								if (is_pl)
								{
									My_tree[i][j].predicate.back().atribut.push_back("ÏÌ: " + standart_predicat[0][count_p].atribut[0] + "_" + buf);
								}
								else
								{
									My_tree[i][j].predicate.back().atribut.push_back(standart_predicat[0][count_p].atribut[0] + "_" + buf);
								}
							}
							else
							{
								for (unsigned int k = 0; k < count; k++)
								{
									My_tree[i][j].is_pred = true;
									My_tree[i][j].predicate.resize(My_tree[i][j].predicate.size() + 1);
									My_tree[i][j].predicate.back().main_word.push_back(My_tree[i][j].Initial_form);
									if (dep_word != -1)
										My_tree[i][j].predicate.back().dep_words.push_back(My_tree[i][dep_word].Initial_form);
									My_tree[i][j].predicate.back().number_of_pred = 0;
									My_tree[i][j].predicate.back().number_of_pred = 0;
									standart_predicat[0][count_p].amt_of_used_pred++;
									number = standart_predicat[0][count_p].amt_of_used_pred;
									My_tree[i][j].predicate.back().number_of_standart_pack = 0;
									My_tree[i][j].predicate.back().number_of_standart_pred = count_p;

									buf.clear();
									while (number > 0)
									{

										buf.resize(buf.size() + 1);
										buf.back() = number % 10 + '0';
										number = (number - number % 10) / 10;
									}
									My_tree[i][j].predicate.back().atribut.push_back(standart_predicat[0][count_p].atribut[0] + "_" + buf);

								}
							}

						}
						if (My_tree[i][j].ssilka != NULL)
						{
							My_tree[i][j].ssilka->is_pred = My_tree[i][j].is_pred;
							My_tree[i][j].ssilka->predicate = My_tree[i][j].predicate;

						}
						for (int del = 0; del < arg.size(); del++)
						{
							delete_node(i, arg[del]);
						}
						if (dep_word != -1)
							delete_node(i, dep_word);

					}
					count = 0;

					dep_word = -1;
					arg.clear();
					buf.clear();

				}
			}
		}
	}

}
void TREE::check()
{
	parent_pred parent;
	for (int number_of_sentence = 0; number_of_sentence < My_tree.size(); number_of_sentence++)
	{
		for (int number_of_word = 0; number_of_word < My_tree[number_of_sentence].size();number_of_word++)
		{
			for (int i = 0; i < My_tree[number_of_sentence][number_of_word].predicate.size();i++)
			{
				for (int k = 0; k < My_tree[number_of_sentence][number_of_word].predicate[i].pred_atribut.size(); k++)
				{
					if (My_tree[number_of_sentence][number_of_word].predicate[i].number_of_pred == 1)
					{
						int count1 = My_tree[number_of_sentence][number_of_word].predicate[i].pred_atribut[0].number_sentence;
						int count2 = My_tree[number_of_sentence][number_of_word].predicate[i].pred_atribut[0].number_word;
						int count3 = My_tree[number_of_sentence][number_of_word].predicate[i].pred_atribut[0].number_pred;
						if (count3 == -1 && My_tree[count1][count2].predicate.size() != 1)
						{
							for (int par = 0; par < My_tree[count1][count2].predicate.size(); par++)
							{
								parent.number_sentence = count1;
								parent.number_word = count1;
								parent.number_pred = par;
								My_tree[number_of_sentence][number_of_word].predicate[i].pred_atribut.push_back(parent);
							}
						}
					}
				}
			}
		}
	}
}
void TREE::print_predicate(size_t number_of_sentence, size_t number_of_word)
{
	parent_pred parent;
	unsigned int i = 0;
	for (i = 0; i < My_tree[number_of_sentence][number_of_word].predicate.size(); i++)
	{
		if (My_tree[number_of_sentence][number_of_word].predicate[i].number_of_pred >=0)
		{
			for (int k = 0; k < My_tree[number_of_sentence][number_of_word].predicate[i].main_word.size(); k++)
				cout << My_tree[number_of_sentence][number_of_word].predicate[i].main_word[k] + "_";
			for (int k = 0; k < My_tree[number_of_sentence][number_of_word].predicate[i].dep_words.size(); k++)
				cout << My_tree[number_of_sentence][number_of_word].predicate[i].dep_words[k] + "_";

			cout << "(";
			for (int k = 0; k < My_tree[number_of_sentence][number_of_word].predicate[i].pred_atribut.size(); k++)
			{

				int count1 = My_tree[number_of_sentence][number_of_word].predicate[i].pred_atribut[k].number_sentence;
				int count2 = My_tree[number_of_sentence][number_of_word].predicate[i].pred_atribut[k].number_word;
				int count3 = My_tree[number_of_sentence][number_of_word].predicate[i].pred_atribut[k].number_pred;
				if (count3 == -1)
					cout << My_tree[count1][count2].predicate[0].atribut[0];
				else
					cout << My_tree[count1][count2].predicate[count3].atribut[0];
				if (k != My_tree[number_of_sentence][number_of_word].predicate[i].pred_atribut.size() - 1)
					cout << ",";
			}
			if (!My_tree[number_of_sentence][number_of_word].predicate[i].atribut.empty())
			{
				if (!My_tree[number_of_sentence][number_of_word].predicate[i].pred_atribut.empty())
					cout << ",";
				for (int k = 0; k < My_tree[number_of_sentence][number_of_word].predicate[i].atribut.size(); k++)
				{
					cout << My_tree[number_of_sentence][number_of_word].predicate[i].atribut[k];
					if (k != My_tree[number_of_sentence][number_of_word].predicate[i].atribut.size() - 1)
						cout << ",";
				}
			}
			/*if (My_tree[number_of_sentence][number_of_word].predicate[i].number_of_pred == 0)
			{

			for (unsigned int j = 0; j < My_tree[number_of_sentence][number_of_word].predicate[i].atribut.size(); j++)
			{
			if (j != 0)
			cout << ",";
			cout << My_tree[number_of_sentence][number_of_word].predicate[i].atribut[j];
			}
			}
			else
			{
			if (My_tree[number_of_sentence][number_of_word].predicate[i].number_of_pred == 1)
			{
			for (unsigned int j = 0; j < My_tree[number_of_sentence][number_of_word].predicate[i].atribut.size(); j++)
			{
			if (j != 0)
			cout << ",";
			cout << My_tree[number_of_sentence][number_of_word].predicate[i].atribut[j];
			}
			cout << ",";
			for (int k = 0; k < My_tree[number_of_sentence][number_of_word].predicate[i].pred_atribut.size(); k++)
			{
			int count1 = My_tree[number_of_sentence][number_of_word].predicate[i].pred_atribut[0].number_sentence;
			int count2 = My_tree[number_of_sentence][number_of_word].predicate[i].pred_atribut[0].number_word;
			int count3 = My_tree[number_of_sentence][number_of_word].predicate[i].pred_atribut[0].number_pred;
			if (count3==-1)
			cout << My_tree[count1][count2].predicate[0].atribut[0];
			else
			cout << My_tree[count1][count2].predicate[count3].atribut[0];
			}

			}
			else
			{
			if (My_tree[number_of_sentence][number_of_word].predicate[i].number_of_pred == 2)
			{
			for (int k = 0; k < My_tree[number_of_sentence][number_of_word].predicate[i].pred_atribut.size(); k++)
			{
			int count1 = My_tree[number_of_sentence][number_of_word].predicate[i].pred_atribut[k].number_sentence;
			int count2 = My_tree[number_of_sentence][number_of_word].predicate[i].pred_atribut[k].number_word;
			int count3 = My_tree[number_of_sentence][number_of_word].predicate[i].pred_atribut[k].number_pred;
			if (count3 == -1)
			cout << My_tree[count1][count2].predicate[0].atribut[0];
			else
			cout << My_tree[count1][count2].predicate[count3].atribut[0];
			if (k != My_tree[number_of_sentence][number_of_word].predicate[i].pred_atribut.size()-1)
			cout << ",";
			}
			}
			else
			{
			if (My_tree[number_of_sentence][number_of_word].predicate[i].number_of_pred == -1)
			{
			for (int k = 0; k < My_tree[number_of_sentence][number_of_word].predicate[i].pred_atribut.size(); k++)
			{
			int count1 = My_tree[number_of_sentence][number_of_word].predicate[i].pred_atribut[k].number_sentence;
			int count2 = My_tree[number_of_sentence][number_of_word].predicate[i].pred_atribut[k].number_word;
			int count3 = My_tree[number_of_sentence][number_of_word].predicate[i].pred_atribut[k].number_pred;
			if (count3 == -1)
			cout << My_tree[count1][count2].predicate[0].atribut[0];
			else
			cout << My_tree[count1][count2].predicate[count3].atribut[0];
			if (k != My_tree[number_of_sentence][number_of_word].predicate[i].pred_atribut.size() - 1)
			cout << ",";
			}
			if (!My_tree[number_of_sentence][number_of_word].predicate[i].atribut.empty())
			{
			if (!My_tree[number_of_sentence][number_of_word].predicate[i].pred_atribut.empty())
			cout << ",";
			for (int k = 0; k < My_tree[number_of_sentence][number_of_word].predicate[i].atribut.size(); k++)
			{
			cout << My_tree[number_of_sentence][number_of_word].predicate[i].atribut[k];
			if (k != My_tree[number_of_sentence][number_of_word].predicate[i].atribut.size()-1)
			cout << ",";
			}
			}
			}
			}
			}
			}*/
			cout << ");";
		}
		else
		{
			if (My_tree[number_of_sentence][number_of_word].predicate[i].number_of_pred == -1)
			{
				for (int k = 0; k < My_tree[number_of_sentence][number_of_word].predicate[i].main_word.size(); k++)
					cout << My_tree[number_of_sentence][number_of_word].predicate[i].main_word[k] + "_";
				for (int k = 0; k < My_tree[number_of_sentence][number_of_word].predicate[i].dep_words.size(); k++)
					cout << My_tree[number_of_sentence][number_of_word].predicate[i].dep_words[k] + "_";

				cout << "(";
				for (int k = 0; k < My_tree[number_of_sentence][number_of_word].predicate[i].pred_atribut.size(); k++)
				{

					int count1 = My_tree[number_of_sentence][number_of_word].predicate[i].pred_atribut[k].number_sentence;
					int count2 = My_tree[number_of_sentence][number_of_word].predicate[i].pred_atribut[k].number_word;
					int count3 = My_tree[number_of_sentence][number_of_word].predicate[i].pred_atribut[k].number_pred;
					if (count3 == -1)
					{

						cout << My_tree[count1][count2].predicate[0].atribut[1];
					}
					else
					{
						cout << My_tree[count1][count2].predicate[count3].atribut[1];
					}
					if (k != My_tree[number_of_sentence][number_of_word].predicate[i].pred_atribut.size() - 1)
						cout << ",";
				}
				if (!My_tree[number_of_sentence][number_of_word].predicate[i].atribut.empty())
				{
					if (!My_tree[number_of_sentence][number_of_word].predicate[i].pred_atribut.empty())
						cout << ",";
					for (int k = 0; k < My_tree[number_of_sentence][number_of_word].predicate[i].atribut.size(); k++)
					{
						cout << My_tree[number_of_sentence][number_of_word].predicate[i].atribut[k];
						if (k != My_tree[number_of_sentence][number_of_word].predicate[i].atribut.size() - 1)
							cout << ",";
					}
				}
			
				cout << ");";
			}
			else
			{
				if (My_tree[number_of_sentence][number_of_word].predicate[i].number_of_pred == -2)
				{
					for (int k = 0; k < My_tree[number_of_sentence][number_of_word].predicate[i].pred_atribut.size(); k++)
					{

						int count1 = My_tree[number_of_sentence][number_of_word].predicate[i].pred_atribut[k].number_sentence;
						int count2 = My_tree[number_of_sentence][number_of_word].predicate[i].pred_atribut[k].number_word;
						int count3 = My_tree[number_of_sentence][number_of_word].predicate[i].pred_atribut[k].number_pred;
						if (count3 == -1)
						{
							for (int count = 0; count < My_tree[count1][count2].predicate[0].pred_atribut.size(); count++)
							{
								int count4 = My_tree[count1][count2].predicate[0].pred_atribut[count].number_sentence;
								int count5 = My_tree[count1][count2].predicate[0].pred_atribut[count].number_word;
								int count6 = My_tree[count1][count2].predicate[0].pred_atribut[count].number_pred;
								if (count6 == -1)
									cout << My_tree[count4][count5].predicate[0].atribut[1];
								else
									cout << My_tree[count4][count5].predicate[count6].atribut[1];
								if (count + 1 != My_tree[count1][count2].predicate[count2].pred_atribut.size())
									cout << My_tree[count1][count2].Rule[0];

							}
						}
						else
						{
							if (!My_tree[count1][count2].predicate[count3].pred_atribut.empty() && My_tree[count1][count2].predicate[count3].number_of_pred<0)
							{
								for (int count = 0; count < My_tree[count1][count2].predicate[count3].pred_atribut.size(); count++)
								{
									int count4 = My_tree[count1][count2].predicate[count3].pred_atribut[count].number_sentence;
									int count5 = My_tree[count1][count2].predicate[count3].pred_atribut[count].number_word;
									int count6 = My_tree[count1][count2].predicate[count3].pred_atribut[count].number_pred;
									if (count6 == -1)
										cout << My_tree[count4][count5].predicate[0].atribut[1];
									else
										cout << My_tree[count4][count5].predicate[count6].atribut[1];
									if (count + 1 != My_tree[count1][count2].predicate[count3].pred_atribut.size())
										cout << My_tree[count1][count2].Rule[0];

								}
							}
							else
							{
								cout << My_tree[count1][count2].predicate[count3].atribut[0];
							}
						}
						if (k != My_tree[number_of_sentence][number_of_word].predicate[i].pred_atribut.size() - 1)
							cout << My_tree[number_of_sentence][number_of_word].Rule[0];
					}
					if (!My_tree[number_of_sentence][number_of_word].predicate[i].atribut.empty())
					{
						if (!My_tree[number_of_sentence][number_of_word].predicate[i].pred_atribut.empty())
							cout << My_tree[number_of_sentence][number_of_word].Rule[0];
						for (int k = 0; k < My_tree[number_of_sentence][number_of_word].predicate[i].atribut.size(); k++)
						{
							cout << My_tree[number_of_sentence][number_of_word].predicate[i].atribut[k];
							if (k != My_tree[number_of_sentence][number_of_word].predicate[i].atribut.size() - 1)
								cout << My_tree[number_of_sentence][number_of_word].Rule[0];
						}
					}

					cout << ";";
				}
				else
				{
					if (My_tree[number_of_sentence][number_of_word].predicate[i].number_of_pred == -3)
					{
						for (int k = 0; k < My_tree[number_of_sentence][number_of_word].predicate[i].main_word.size(); k++)
							cout << My_tree[number_of_sentence][number_of_word].predicate[i].main_word[k] + "_";
						for (int k = 0; k < My_tree[number_of_sentence][number_of_word].predicate[i].dep_words.size(); k++)
							cout << My_tree[number_of_sentence][number_of_word].predicate[i].dep_words[k] + "_";

						cout << "(";
						for (int k = 0; k < My_tree[number_of_sentence][number_of_word].predicate[i].pred_atribut.size(); k++)
						{

							int count1 = My_tree[number_of_sentence][number_of_word].predicate[i].pred_atribut[k].number_sentence;
							int count2 = My_tree[number_of_sentence][number_of_word].predicate[i].pred_atribut[k].number_word;
							int count3 = My_tree[number_of_sentence][number_of_word].predicate[i].pred_atribut[k].number_pred;
							if (count3 == -1)
							{
								for (int count = 0; count < My_tree[count1][count2].predicate[0].pred_atribut.size(); count++)
								{
									int count4 = My_tree[count1][count2].predicate[0].pred_atribut[count].number_sentence;
									int count5 = My_tree[count1][count2].predicate[0].pred_atribut[count].number_word;
									int count6 = My_tree[count1][count2].predicate[0].pred_atribut[count].number_pred;
									if (count6 == -1)
										cout << My_tree[count4][count5].predicate[0].atribut[1];
									else
										cout << My_tree[count4][count5].predicate[count6].atribut[1];
									if (count + 1 != My_tree[count1][count2].predicate[count2].pred_atribut.size())
										cout << My_tree[count1][count2].Rule[0];

								}
							}
							else
							{
								for (int count = 0; count < My_tree[count1][count2].predicate[count3].pred_atribut.size(); count++)
								{
									int count4 = My_tree[count1][count2].predicate[count3].pred_atribut[count].number_sentence;
									int count5 = My_tree[count1][count2].predicate[count3].pred_atribut[count].number_word;
									int count6 = My_tree[count1][count2].predicate[count3].pred_atribut[count].number_pred;
									if (count6 == -1)
										cout << My_tree[count4][count5].predicate[0].atribut[1];
									else
										cout << My_tree[count4][count5].predicate[count6].atribut[1];
									if (count + 1 != My_tree[count1][count2].predicate[count3].pred_atribut.size())
										cout << My_tree[count1][count2].Rule[0];

								}
							}
							if (k != My_tree[number_of_sentence][number_of_word].predicate[i].pred_atribut.size() - 1)
								cout << ",";
						}
						if (!My_tree[number_of_sentence][number_of_word].predicate[i].atribut.empty())
						{
							if (!My_tree[number_of_sentence][number_of_word].predicate[i].pred_atribut.empty())
								cout << ",";
							for (int k = 0; k < My_tree[number_of_sentence][number_of_word].predicate[i].atribut.size(); k++)
							{
								cout << My_tree[number_of_sentence][number_of_word].predicate[i].atribut[k];
								if (k != My_tree[number_of_sentence][number_of_word].predicate[i].atribut.size() - 1)
									cout << ",";
							}
						}

						cout << ");";
					}
				}
			}
		}
	}
	cout << endl;

}
void TREE::number_converter()
{
	ifstream file;
	file.open("Numerals.txt");
	vector<string> number_in_words;
	vector<double> number;
	int result = 0;
	vector<int> must_delete;
	int first_word_of_number = -1;
	int buf;
	node mynode;
	while (!file.eof())
	{
		number.resize(number.size() + 1);
		number_in_words.resize(number.size());
		file >> number_in_words.back();
		file >> number.back();
	}

	for (unsigned int n = 0; n < My_tree.size(); n++)
	{
		for (unsigned int i = 0; i < My_tree[n].size(); i++)
		{
			if (!My_tree[n][i].morfology.empty() && My_tree[n][i].morfology[0] == "NUM")
			{

				if (My_tree[n][i].morfology.size() == 1)
				{
					buf = My_tree[n][i].name.size();
					for (int j = 0; j < buf; j++)
					{
						result = result + (My_tree[n][i].name[j] - '0')*pow(10.0, buf) / pow(10.0, j);
					}
					My_tree[n][i].is_number = true;
					My_tree[n][i].name_number = result;
					result = 0;
					
				}

				else
				{
					////////////ÔÂ‰ÔÓÎ‡„‡ÂÏ, ˜ÚÓ ˆÂÎÓÂ//////////////////////


					while (My_tree[n][i].relation[0] == "ÍÓÎË˜-vspom")
					{
						for (unsigned int k = 0; k < number.size(); k++)
						{
							if (My_tree[n][i].Initial_form == number_in_words[k])
							{
								result = result + number[k];
							}
						}
						i++;


					}
					if (My_tree[n][i].relation[0] == "kolich" || My_tree[n][i].relation[0] == "kolichest")
						{
							for (unsigned int k = 0; k < number.size(); k++)
							{
								if (My_tree[n][i].Initial_form == number_in_words[k])
								{
									result = result + number[k];
								}
							}
						}
					My_tree[n][i].name_number = result;
					char buf1;
					if (result != 0)
					{
						My_tree[n][i].name.clear();
						while (result != 0)
						{

							buf1 = result % 10 + '0';
							result = (result - result % 10) / 10;
							My_tree[n][i].name.resize(My_tree[n][i].name.size() + 1);
							for (unsigned int k = My_tree[n][i].name.size() - 1; k > 0; k--)
							{
								My_tree[n][i].name[k] = My_tree[n][i].name[k - 1];
							}
							My_tree[n][i].name[0] = buf1;
						}
					}
					    My_tree[n][i].Initial_form = My_tree[n][i].name;
						My_tree[n][i].is_number = true;
						
						result = 0;
						for (unsigned int k = 0; k < My_tree[n][i].children.size(); k++)
						{
							delete_node(n, My_tree[n][i].children[k]->number);
						}
						
					/////////////////////////////////////////////////////////
				}
			}
		}
	}

}
int TREE::find_pred(vector<vector<string>> word,size_t number_of_sentence, size_t number_of_word)
{

	int arg = number_of_word;
	int arg_1 = number_of_word;
	int buf = number_of_word;
	int buf_1 = number_of_word;
	bool flag = 1;
	bool flag_1 = 1;
	queue <int> used;
	queue <int> used1;
	unsigned int count = 0;
	used.push(number_of_word);
	used1.push(number_of_word);
	while (!used.empty())
	{
		arg = used.front();
		used.pop();
		for (unsigned int i = 0; i < My_tree[number_of_sentence][arg].parent.size(); i++)
		{
			used.push(My_tree[number_of_sentence][arg].parent[i]->number);
			for (int j = 0; j < word.size(); j++)
			{
				if (!My_tree[number_of_sentence][arg].parent[i]->is_delete && My_tree[number_of_sentence][arg].parent[i]->is_pred && My_tree[number_of_sentence][arg].parent[i]->predicate[0].main_word.size() <= word[j].size())
				{
					flag = 1;
					for (count = 0; count < My_tree[number_of_sentence][arg].parent[i]->predicate[0].main_word.size(); count++)
					{
						if (My_tree[number_of_sentence][arg].parent[i]->predicate[0].main_word[count] != word[j][count])
						{
							flag = 0;
							break;
						}
					}
					if (flag && word[j].size() <= (My_tree[number_of_sentence][arg].parent[i]->predicate[0].dep_words.size()))
					{
						for (int k = 0; k < My_tree[number_of_sentence][arg].parent[i]->predicate[0].dep_words.size(); k++)
						{
							if (My_tree[number_of_sentence][arg].parent[i]->predicate[0].dep_words[k] != word[j][k])
								flag = 0;
								
						}
						
					}
					if (flag)return My_tree[number_of_sentence][arg].parent[i]->number;
				}
			}
		}

	}
	while (!used1.empty())
	{
		arg = used1.front();
		used1.pop();
		for (unsigned int i = 0; i < My_tree[number_of_sentence][arg].children.size(); i++)
		{
			used1.push(My_tree[number_of_sentence][arg].children[i]->number);
			for (int j = 0; j < word.size(); j++)
			{
				if (!My_tree[number_of_sentence][arg].children[i]->is_delete && My_tree[number_of_sentence][arg].children[i]->is_pred && My_tree[number_of_sentence][arg].children[i]->predicate[0].main_word.size() <= word[j].size())
				{
					flag = 1;
					for (count = 0; count < My_tree[number_of_sentence][arg].children[i]->predicate[0].main_word.size(); count++)
					{
						if (My_tree[number_of_sentence][arg].children[i]->predicate[0].main_word[count] != word[j][count])
						{
							flag = 0;
							break;
						}
					}
					if (flag && word[j].size() <= (My_tree[number_of_sentence][arg].children[i]->predicate[0].dep_words.size()))
					{
						for (int k = 0; k < My_tree[number_of_sentence][arg].children[i]->predicate[0].dep_words.size(); k++)
						{
							if (My_tree[number_of_sentence][arg].children[i]->predicate[0].dep_words[k] != word[j][k])
								flag = 0;

						}

					}
					if (flag)return My_tree[number_of_sentence][arg].children[i]->number;
				}
			}
		}

	}
	return -1;

}
void TREE::rule()
{

	ifstream file;
	string buf;
	int buf_int;
	bool flag = false;
	file.open("rules.txt");
	if (!file)
		cout << "cannot open file: standart_predicate.txt";


	while (!file.eof())
	{

		file >> buf;
		if (buf == "rule")
		{
			file >> buf_int;
			this->Rules.resize(buf_int);
		}
		else
		{
			if (buf != "->" && buf != ";")
			{

				Rules.back().resize(Rules.back().size() + 1);
				Rules.back().back().premise.resize(Rules.back().back().premise.size() + 1);
				Rules.back().back().premise.back().resize(Rules.back().back().premise.back().size() + 1);
				while (buf != "->")
				{
					if (buf == "&&")
					{
						Rules.back().back().premise.back().resize(Rules.back().back().premise.back().size() + 1);
						file >> buf;
					}
					if (buf == "||")
					{
						Rules.back().back().premise.resize(Rules.back().back().premise.size() + 1);
						file >> buf;
					}
					Rules.back().back().premise.back().back() = Rules.back().back().premise.back().back() + buf;
					file >> buf;
					if (buf == ";")
						break;
				}
				file >> buf;
			}
			while (buf != ";")
			{

				Rules.back().back().consequence = Rules.back().back().consequence + buf;

				file >> buf;
			}
		}

	}

}
void TREE::standart_predikat_2()
{

	int arg_num = -1;
	string main_word;
	vector<node> dep_predict;
	vector<string> dep_word ;
	dep_word.push_back("–¿¬Õ€…");
	dep_word.push_back("—Œ—“¿¬Àﬂ“‹");


	vector<int> arg1;
	vector<int> arg2;
	vector<int> must_delete;
	vector<string> standart_arg;
	bool is_pl = false;
	string buf;
	bool is_dep = true;
	int count_p;
	bool flag = true;
	int count_of_pred = 0;
	for (unsigned int n = 0; n < My_tree.size(); n++)
	{
		for (unsigned int count = 0; count < My_tree[n].size(); count++)
		{
			if (!My_tree[n][count].is_pred)
			{
				for (unsigned int count_p = 0; count_p < standart_predicat[1].size(); count_p++)
				{
					if (My_tree[n][count].Initial_form == standart_predicat[1][count_p].main_word[0])
					{
						for (unsigned int count_child = 0; count_child < My_tree[n][count].children.size(); count_child++)
						{
							if (!My_tree[n][count].children[count_child]->morfology.empty() && My_tree[n][count].children[count_child]->is_number)
							{
								for (unsigned int count_parent = 0; count_parent < My_tree[n][count].children[count_child]->parent.size(); count_parent++)
								{
									if (My_tree[n][count].children[count_child]->number < count && My_tree[n][count].children[count_child]->parent[count_parent]->number == count && My_tree[n][count].children[count_child]->relation[count_parent] == "kolichest")
									{
										count_of_pred = My_tree[n][count].children[count_child]->name_number;
										delete_node(n, My_tree[n][count].children[count_child]->number);
										break;
									}
								}
							}
						}
						for (unsigned int count_child = 0; count_child < My_tree[n][count].children.size(); count_child++)
						{
							
							for (unsigned int count_dep = 0; count_dep < standart_predicat[1][count_p].dep_words.size(); count_dep++)
							{
								if (My_tree[n][count].children[count_child]->is_pred)
								{
									if (My_tree[n][count].children[count_child]->predicate[0].main_word[0].size() >= standart_predicat[1][count_p].dep_words[count_dep][0].size())
									{
										for (unsigned int dep_w = 0; dep_w < standart_predicat[1][count_p].dep_words[count_dep][0].size(); dep_w++)
										{
											is_dep = true;
											int key = My_tree[n][count].children[count_child]->predicate[0].main_word[0].size() - dep_w - 1;
											int key2 = standart_predicat[1][count_p].dep_words[count_dep][0].size() - dep_w - 1;
											if (My_tree[n][count].children[count_child]->predicate[0].main_word[0][key] != standart_predicat[1][count_p].dep_words[count_dep][0][key2])
											{
												is_dep = false;
												continue;
											}
										}
									}
								}
								else
									is_dep = false;
								if (is_dep)
								{
									flag = true;
									for (int dep_pred = 0; dep_pred < dep_predict.size(); dep_pred++)
									{
										if (dep_predict[dep_pred].number == My_tree[n][count].children[count_child]->number)
										{
											flag = false;
											break;
										}
									}
									if (flag)
										dep_predict.push_back(*My_tree[n][count].children[count_child]);

								}
							}
							if (!My_tree[n][count].children[count_child]->morfology.empty() && (My_tree[n][count].children[count_child]->morfology[0] == "NONLEX" || My_tree[n][count].children[count_child]->morfology[0] == "ATR" || My_tree[n][count].children[count_child]->morfology[0] == "ABR"))
							{
								bool is_atrib = false;
								for (unsigned int count_parent = 0; count_parent < My_tree[n][count].children[count_child]->parent.size(); count_parent++)
								{

									if (My_tree[n][count].children[count_child]->parent[count_parent]->number == count && My_tree[n][count].children[count_child]->relation[count_parent] == "atrib")
									{
										is_atrib = true;
										break;
									}

								}
								if (is_atrib)
									arg1.push_back(My_tree[n][count].children[count_child]->number);
							}


						}
						if (dep_predict.empty())
						{
							int kill = find_pred(standart_predicat[1][count_p].dep_words, n, count);
							if (kill != -1)
								dep_predict.push_back(My_tree[n][kill]);
						}
						if (dep_predict.empty())
						{
							node* kill = find_close_pred(standart_predicat[1][count_p].dep_words, n, count);
							if (kill != NULL)
								dep_predict.push_back(*kill);
						}
						if (dep_predict.empty())
							break;
						for (int count_parent = 0; count_parent < My_tree[n][count].parent.size(); count_parent++)
						{
							for (int dep = 0; dep < dep_word.size(); dep++)
							{
								if (My_tree[n][count].parent[count_parent]->Initial_form == dep_word[dep])
								{
									bool kill = false;
									int num_kill;
									for (int count_child = 0; count_child < My_tree[n][count].parent[count_parent]->children.size(); count_child++)
									{
										if (!My_tree[n][count].parent[count_parent]->children[count_child]->morfology.empty())
										{
											if ( (My_tree[n][count].parent[count_parent]->children[count_child]->morfology[0] == "NUM" || My_tree[n][count].parent[count_parent]->children[count_child]->morfology[0] == "NONLEX" || My_tree[n][count].parent[count_parent]->children[count_child]->morfology[0] == "ATR" || My_tree[n][count].parent[count_parent]->children[count_child]->morfology[0] == "ABR"))
											{
												arg2.push_back(My_tree[n][count].parent[count_parent]->children[count_child]->number);
												num_kill = My_tree[n][count].parent[count_parent]->number;
												kill = true;
											}
										}
									}
									if (kill)
									{
										delete_replace(n, num_kill);
										break;
									}
								}
								if (My_tree[n][count].parent[count_parent]->Initial_form == standart_predicat[1][count_p].atribut[2])
								{
									for (int count_par = 0; count_par < My_tree[n][count].parent[count_parent]->parent.size(); count_par++)
									{
										if (My_tree[n][count].parent[count_parent]->parent[count_par]->Initial_form == dep_word[dep])
										{
											bool kill = 0;
											for (int count_child = 0; count_child < My_tree[n][count].parent[count_parent]->parent[count_par]->children.size(); count_child++)
											{
												if (!My_tree[n][count].parent[count_parent]->parent[count_par]->children[count_child]->morfology.empty())
												{
													if (!My_tree[n][count].parent[count_parent]->parent[count_par]->children[count_child]->morfology.empty() && (My_tree[n][count].parent[count_parent]->parent[count_par]->children[count_child]->morfology[0] == "NUM" || My_tree[n][count].parent[count_parent]->parent[count_par]->children[count_child]->morfology[0] == "NONLEX" || My_tree[n][count].parent[count_parent]->parent[count_par]->children[count_child]->morfology[0] == "ATR" || My_tree[n][count].parent[count_parent]->parent[count_par]->children[count_child]->morfology[0] == "ABR"))
													{
														arg2.push_back(My_tree[n][count].parent[count_parent]->parent[count_par]->children[count_child]->number);
														kill = true;
													}
												}
											}
											if (kill)
											{
												delete_replace(n, My_tree[n][count].parent[count_parent]->parent[count_par]->number);
												break;
											}
										}
									}
								}
							}
						}
						if (arg2.empty())
						{
							for (int dep = 0; dep < dep_word.size(); dep++)
							{
								int kill = find_close_child(dep_word[dep], n, count);
								if (kill != -1)
								{
									for (int count_child = 0; count_child < My_tree[n][kill].children.size(); count_child++)
									{
										if (!My_tree[n][kill].children[count_child]->morfology.empty() && (My_tree[n][kill].children[count_child]->morfology[0] == "NUM" || My_tree[n][kill].children[count_child]->morfology[0] == "NONLEX" || My_tree[n][kill].children[count_child]->morfology[0] == "ATR" || My_tree[n][kill].children[count_child]->morfology[0] == "ABR"))
										{
											arg2.push_back(My_tree[n][kill].children[count_child]->number);
										}
									}
									delete_replace(n, kill);
									break;
									
								}
							}
						}

						if (arg2.empty())
						{
							for (int count_child = 0; count_child < My_tree[n][count].children.size(); count_child++)
							{
								if (My_tree[n][count].children[count_child]->morfology[0] == "NUM" || My_tree[n][count].children[count_child]->morfology[0] == "NONLEX")
								{
									arg2.push_back(My_tree[n][count].children[count_child]->number);
								}
							}
						}
						if (arg2.empty())
						{
							int kill = find_close_child(standart_predicat[1][count_p].atribut[2], n, count);
							if (kill != -1)
							{
								for (int count_child = 0; count_child < My_tree[n][kill].children.size(); count_child++)
								{
									if (My_tree[n][kill].children[count_child]->morfology[0] == "NUM" || My_tree[n][kill].children[count_child]->morfology[0] == "NONLEX")
									{
										arg2.push_back(My_tree[n][kill].children[count_child]->number);
									}
								}
							}
						}
						My_tree[n][count].is_pred = true;
						unsigned int i;
						if (count_of_pred == 0)
						{
							
							for (i = 0; i < arg2.size(); i++)
							{
								if (dep_predict.size() == 1 && dep_predict[0].predicate.size() == 1)
								{
									My_tree[n][count].predicate.resize(My_tree[n][count].predicate.size() + 1);
									My_tree[n][count].predicate.back().main_word.push_back(My_tree[n][count].Initial_form);
									My_tree[n][count].predicate.back().pred_atribut.resize(My_tree[n][count].predicate.back().pred_atribut.size() + 1);
									My_tree[n][count].predicate.back().pred_atribut.back().number_sentence = dep_predict[0].number_of_sentences;
									My_tree[n][count].predicate.back().pred_atribut.back().number_word = dep_predict[0].number;
									My_tree[n][count].predicate.back().number_of_standart_pack = 1;
									My_tree[n][count].predicate.back().number_of_standart_pred = count_p;
									My_tree[n][count].predicate.back().number_of_pred = 1;
									if (i < arg1.size())
									{
										My_tree[n][count].predicate.back().atribut.push_back(My_tree[n][arg1[i]].Initial_form);
									}
									else
									{
										standart_predicat[1][count_p].amt_of_used_pred++;
										int number = standart_predicat[1][count_p].amt_of_used_pred;
										buf.clear();
										while (number > 0)
										{

											buf.resize(buf.size() + 1);
											buf.back() = number % 10 + '0';
											number = (number - number % 10) / 10;
										}

										My_tree[n][count].predicate.back().atribut.push_back(standart_predicat[1][count_p].atribut[0] + "_" + buf);


									}

									My_tree[n][count].predicate.back().atribut.push_back(My_tree[n][arg2[i]].Initial_form);

								}
							}
							if (!arg1.empty())
							{
								if (dep_predict.size() == 1 && dep_predict[0].predicate.size() == 1)
								{
									for (unsigned int j = i; j < arg1.size(); j++)
									{
										My_tree[n][count].predicate.resize(My_tree[n][count].predicate.size() + 1);
										My_tree[n][count].predicate.back().main_word.push_back(My_tree[n][count].Initial_form);
										My_tree[n][count].predicate.back().number_of_standart_pred = count_p;
										My_tree[n][count].predicate.back().number_of_pred = 1;
										My_tree[n][count].predicate.back().number_of_standart_pack = 1;

										My_tree[n][count].predicate.back().atribut.push_back(My_tree[n][arg1[j]].Initial_form);
										My_tree[n][count].predicate.back().pred_atribut.resize(My_tree[n][count].predicate.back().pred_atribut.size() + 1);
										My_tree[n][count].predicate.back().pred_atribut.back().number_sentence = (dep_predict[0].number_of_sentences);
										My_tree[n][count].predicate.back().pred_atribut.back().number_word = dep_predict[0].number;
										standart_predicat[1][count_p].amt_of_used_pred++;
										int number = standart_predicat[1][count_p].amt_of_used_pred;
										buf.clear();
										while (number > 0)
										{

											buf.resize(buf.size() + 1);
											buf.back() = number % 10 + '0';
											number = (number - number % 10) / 10;
										}
										My_tree[n][count].predicate.back().atribut.push_back(standart_predicat[1][count_p].atribut[2] + "_" + buf);


									}
								}
								else
								{
									for (unsigned int j = i; j < arg1.size(); j++)
									{
										for (int dep = 0; dep < dep_predict.size(); dep++)
										{
											for (int dep_p = 0; dep_p < dep_predict[dep].predicate.size(); dep_p++)
											{
												My_tree[n][count].predicate.resize(My_tree[n][count].predicate.size() + 1);
												My_tree[n][count].predicate.back().main_word.push_back(My_tree[n][count].Initial_form);
												My_tree[n][count].predicate.back().number_of_standart_pred = count_p;
												My_tree[n][count].predicate.back().number_of_pred = 1;
												My_tree[n][count].predicate.back().number_of_standart_pack = 1;
												My_tree[n][count].predicate.back().atribut.push_back(My_tree[n][arg1[j]].Initial_form);
												My_tree[n][count].predicate.back().pred_atribut.resize(My_tree[n][count].predicate.back().pred_atribut.size() + 1);
												My_tree[n][count].predicate.back().pred_atribut.back().number_sentence = (dep_predict[dep].number_of_sentences);
												My_tree[n][count].predicate.back().pred_atribut.back().number_word = dep_predict[dep].number;
												My_tree[n][count].predicate.back().pred_atribut.back().number_pred = dep_p;
												standart_predicat[1][count_p].amt_of_used_pred++;
												int number = standart_predicat[1][count_p].amt_of_used_pred;
												buf.clear();
												while (number > 0)
												{
													buf.resize(buf.size() + 1);
													buf.back() = number % 10 + '0';
													number = (number - number % 10) / 10;
												}
												My_tree[n][count].predicate.back().atribut.push_back(standart_predicat[1][count_p].atribut[2] + "_" + buf);


											}
										}
										
									}
								}
							}
							else
							{
								
							}
							if (arg1.empty() && arg2.empty())
							{
								for (int g = 0; g < My_tree[n][count].morfology.size(); g++)
								{
									if (My_tree[n][count].morfology[g] == "PL")
									{
										is_pl = true;
										break;
									}
								}

								for (int d = 0; d < dep_predict.size(); d++)
								{
									if (is_pl)
									{
										if (dep_predict.size() == 1 && dep_predict[0].predicate.size() == 1)
										{
											My_tree[n][count].predicate.resize(My_tree[n][count].predicate.size() + 1);
											My_tree[n][count].predicate.back().main_word.push_back(My_tree[n][count].Initial_form);
											My_tree[n][count].predicate.back().pred_atribut.resize(My_tree[n][count].predicate.back().pred_atribut.size() + 1);
											My_tree[n][count].predicate.back().pred_atribut.back().number_sentence = dep_predict[0].number_of_sentences;
											My_tree[n][count].predicate.back().pred_atribut.back().number_word = dep_predict[0].number;
											My_tree[n][count].predicate.back().number_of_pred = 1;
											My_tree[n][count].predicate.back().number_of_standart_pred = count_p;
											My_tree[n][count].predicate.back().number_of_pred = 1;
											My_tree[n][count].predicate.back().number_of_standart_pack = 1;
											standart_predicat[1][count_p].amt_of_used_pred++;
											int number = standart_predicat[1][count_p].amt_of_used_pred;
											buf.clear();
											while (number > 0)
											{

												buf.resize(buf.size() + 1);
												buf.back() = number % 10 + '0';
												number = (number - number % 10) / 10;
											}
											My_tree[n][count].predicate.back().atribut.push_back("ÏÌ: " + standart_predicat[1][count_p].atribut[0] + "_" + buf);
											My_tree[n][count].predicate.back().atribut.push_back("ÏÌ: " + standart_predicat[1][count_p].atribut[2] + "_" + buf);

											break;
										}
										for (int pr = 0; pr < dep_predict[d].predicate.size(); pr++)
										{
											My_tree[n][count].predicate.resize(My_tree[n][count].predicate.size() + 1);
											My_tree[n][count].predicate.back().main_word.push_back(My_tree[n][count].Initial_form);
											My_tree[n][count].predicate.back().number_of_pred = 1;
											My_tree[n][count].predicate.back().pred_atribut.resize(My_tree[n][count].predicate.back().pred_atribut.size() + 1);
											My_tree[n][count].predicate.back().pred_atribut.back().number_sentence = dep_predict[d].number_of_sentences;
											My_tree[n][count].predicate.back().pred_atribut.back().number_word = dep_predict[d].number;
											My_tree[n][count].predicate.back().pred_atribut.back().number_pred = pr;
											standart_predicat[1][count_p].amt_of_used_pred++;
											My_tree[n][count].predicate.back().number_of_standart_pred = count_p;
											My_tree[n][count].predicate.back().number_of_standart_pack = 1;

											int number = standart_predicat[1][count_p].amt_of_used_pred;
											buf = "";
											while (number > 0)
											{

												buf.resize(buf.size() + 1);
												buf.back() = number % 10 + '0';
												number = (number - number % 10) / 10;
											}
											My_tree[n][count].predicate.back().atribut.push_back(standart_predicat[1][count_p].atribut[0] + "_" + buf);
											My_tree[n][count].predicate.back().atribut.push_back(standart_predicat[1][count_p].atribut[2] + "_" + buf);

										}


									}

									else
									{

										My_tree[n][count].predicate.resize(My_tree[n][count].predicate.size() + 1);
										My_tree[n][count].predicate.back().main_word.push_back(My_tree[n][count].Initial_form);
										My_tree[n][count].predicate.back().pred_atribut.resize(My_tree[n][count].predicate.back().pred_atribut.size() + 1);
										My_tree[n][count].predicate.back().pred_atribut.back().number_sentence = (dep_predict[d].number_of_sentences);
										My_tree[n][count].predicate.back().pred_atribut.back().number_word = (dep_predict[d].number);
										My_tree[n][count].predicate.back().number_of_pred = 1;
										My_tree[n][count].predicate.back().number_of_standart_pred = count_p;
										My_tree[n][count].predicate.back().number_of_standart_pack = 1;

										standart_predicat[1][count_p].amt_of_used_pred++;
										int number = standart_predicat[1][count_p].amt_of_used_pred;
										buf.clear();
										while (number > 0)
										{
											buf.resize(buf.size() + 1);
											buf.back() = number % 10 + '0';
											number = (number - number % 10) / 10;
										}
										My_tree[n][count].predicate.back().atribut.push_back(standart_predicat[1][count_p].atribut[0] + "_" + buf);



										My_tree[n][count].predicate.back().atribut.push_back(standart_predicat[1][count_p].atribut[2] + "_" + buf);

									}

								}
							}
						}
						else
						{
							for (int pr = 0; pr < count_of_pred; pr++)
							{
								for (int d = 0; d < dep_predict.size(); d++)
								{
									if (dep_predict[d].predicate.size() == 1)
									{
										My_tree[n][count].predicate.resize(My_tree[n][count].predicate.size() + 1);
										My_tree[n][count].predicate.back().main_word.push_back(My_tree[n][count].Initial_form);
										My_tree[n][count].predicate.back().number_of_pred = 1;
										My_tree[n][count].predicate.back().pred_atribut.resize(My_tree[n][count].predicate.back().pred_atribut.size() + 1);
										My_tree[n][count].predicate.back().pred_atribut.back().number_sentence = dep_predict[d].number_of_sentences;
										My_tree[n][count].predicate.back().pred_atribut.back().number_word = dep_predict[d].number;
										My_tree[n][count].predicate.back().pred_atribut.back().number_pred = 0;
										My_tree[n][count].predicate.back().number_of_standart_pred = count_p;
										My_tree[n][count].predicate.back().number_of_standart_pack = 1;

										if (arg1.empty() || arg2.empty())
										{
											standart_predicat[1][count_p].amt_of_used_pred++;
											int number = standart_predicat[1][count_p].amt_of_used_pred;
											buf = "";
											while (number > 0)
											{

												buf.resize(buf.size() + 1);
												buf.back() = number % 10 + '0';
												number = (number - number % 10) / 10;
											}
										}

										if (arg1.empty())
										{

											My_tree[n][count].predicate.back().atribut.push_back(standart_predicat[1][count_p].atribut[0] + "_" + buf);

										}
										else
										{
											if (arg1.size() >= count_of_pred)
												My_tree[n][count].predicate.back().atribut.push_back(My_tree[n][arg1[pr]].Initial_form);

										}
										if (arg2.empty())
										{

											My_tree[n][count].predicate.back().atribut.push_back(standart_predicat[1][count_p].atribut[2] + "_" + buf);

										}
										else
										{
											if (arg2.size() >= count_of_pred)
												My_tree[n][count].predicate.back().atribut.push_back(My_tree[n][arg2[pr]].Initial_form);

										}
									}
									else
									{
										for (int q = 0; q < dep_predict[d].predicate.size();q++)
										{
											My_tree[n][count].predicate.resize(My_tree[n][count].predicate.size() + 1);
											My_tree[n][count].predicate.back().main_word.push_back(My_tree[n][count].Initial_form);
											My_tree[n][count].predicate.back().number_of_pred = 1;
											My_tree[n][count].predicate.back().pred_atribut.resize(My_tree[n][count].predicate.back().pred_atribut.size() + 1);
											My_tree[n][count].predicate.back().pred_atribut.back().number_sentence = dep_predict[d].number_of_sentences;
											My_tree[n][count].predicate.back().pred_atribut.back().number_word = dep_predict[d].number;
											My_tree[n][count].predicate.back().pred_atribut.back().number_pred = q;
											My_tree[n][count].predicate.back().number_of_standart_pred = count_p;
											My_tree[n][count].predicate.back().number_of_standart_pack = 1;

											if (arg1.empty() || arg2.empty())
											{
												standart_predicat[1][count_p].amt_of_used_pred++;
												int number = standart_predicat[1][count_p].amt_of_used_pred;
												buf = "";
												while (number > 0)
												{

													buf.resize(buf.size() + 1);
													buf.back() = number % 10 + '0';
													number = (number - number % 10) / 10;
												}
											}

											if (arg1.empty())
											{

												My_tree[n][count].predicate.back().atribut.push_back(standart_predicat[1][count_p].atribut[0] + "_" + buf);

											}
											else
											{
												if (arg1.size() >= count_of_pred)
													My_tree[n][count].predicate.back().atribut.push_back(My_tree[n][arg1[pr]].Initial_form);

											}
											if (arg2.empty())
											{

												My_tree[n][count].predicate.back().atribut.push_back(standart_predicat[1][count_p].atribut[2] + "_" + buf);

											}
											else
											{
												if (arg2.size() >= count_of_pred)
													My_tree[n][count].predicate.back().atribut.push_back(My_tree[n][arg2[pr]].Initial_form);

											}
										}
									}
								}
							}
						}
						for (int d = count; d < arg1.size();d++)
						{
							arg1.pop_back();
							d = count;
						}
						for (int d = count; d < arg2.size(); d++)
						{
							arg2.pop_back();
							d = count;
						}
						if (My_tree[n][count].ssilka != NULL)
						{
							My_tree[n][count].ssilka->is_pred = My_tree[n][count].is_pred;
							My_tree[n][count].ssilka->predicate = My_tree[n][count].predicate;

						}
						for (int del = 0; del < arg1.size(); del++)
						{
							delete_node(n, arg1[del]);
						}
						for (int del = 0; del < arg2.size(); del++)
						{
							delete_node(n, arg2[del]);
						}
						for (int g = 0; g < n; g++)
						{
							
							for (int del = 0; del < arg2.size(); del++)
							{
								delete_node(n, arg2[del]);
							}
						}

						dep_predict.clear();
						arg1.clear();
						arg2.clear();
						buf.clear();
						count_of_pred = 0;
						is_pl = false;
						break;
					}

					
				}
			}
		}
	}
	/*cout << "ÿ‡„ ‚ÚÓÓÈ:" << endl;
	for (unsigned int i = 0; i < My_tree.size(); i++)
	{
		for (unsigned int j = 0; j < My_tree[i].size(); j++)
		{
			if (My_tree[i][j].is_pred)
				print_predicate(i, j);
		}
	}*/

}
int TREE::find_close_child(string word, size_t number_of_sentence, size_t number_of_word)
{
	int arg = number_of_word;
	int arg_1 = number_of_word;
	int buf = number_of_word;
	int buf_1 = number_of_word;
	bool flag = 1;
	bool flag_1 = 1;
	queue <int> used;
	unsigned int count = 0;
	used.push(number_of_word);
	while (!used.empty())
	{
		arg = used.front();
		used.pop();
		for (unsigned int i = 0; i < My_tree[number_of_sentence][arg].children.size(); i++)
		{
			used.push(My_tree[number_of_sentence][arg].children[i]->number);

			if (!My_tree[number_of_sentence][arg].children[i]->is_delete)
			{
				if (My_tree[number_of_sentence][arg].children[i]->Initial_form == word)
				{
					return My_tree[number_of_sentence][arg].children[i]->number;
				}
			}
		}
	}

	
	return -1;

}
node* TREE::find_close_pred(vector<vector<string>> word, size_t number_of_sentece, size_t number_of_word)
{
	vector<int>result;
	string buf = "";
	vector<int>pred;
	int min = -1;
	int count;
	bool flag = 1;
	bool flag1 = 1;
	int number_of_w;
	for (int i = number_of_sentece; i >= 0; i--)
	{
		if (i == number_of_sentece)
		{
			number_of_w = number_of_word;
		}
		else
		{
			number_of_w = My_tree[i].size() - 1;
		}
			for (int j = number_of_w; j >= 0; j--)
			{
				if (My_tree[i][j].is_pred)
				{
					
					for (int q = 0; q < word.size(); q++)
					{
						flag = 1;
						flag1 = 1;
						if (My_tree[i][j].predicate[0].main_word.size() <= word[q].size())
						{
							for (count = 0; count < My_tree[i][j].predicate[0].main_word.size(); count++)
							{
								if (My_tree[i][j].predicate[0].main_word[count] != word[q][count])
								{
									flag = 0;
									break;
								}
								
							}
							
							if (flag && word[q].size()-1 >= (My_tree[i][j].predicate[0].dep_words.size()))
							{
								for (int k = 0; k < My_tree[i][j].predicate[0].dep_words.size(); k++)
								{
									
									if (My_tree[i][j].predicate[0].dep_words[k] != word[q][k+count])
										flag1 = 0;

								}
								
							}
							if (flag && flag1){

								return &My_tree[i][j];
							}
						}
					}
				}
			}
		
		
	}
	
	return NULL;
}
void TREE::rule_1()
{
	bool flag = false;
	for (int i = 0; i < My_tree.size(); i++)
	{
		for (int j = 0; j < My_tree[i].size(); j++)
		{
			if (My_tree[i][j].is_pred)
			{

				for (int g = 0; g < Rules[0].size(); g++)
				{
					flag = true;
					for (int k = Rules[0][g].premise[0][0].size() - 1; k >= 0; k--)
					{
						for (int num_pred = 0; num_pred < My_tree[i][j].predicate.size(); num_pred++)
						{
							
							int p = My_tree[i][j].predicate[num_pred].main_word[0].size() - Rules[0][g].premise[0][0].size()  + k;
							if (p>=0 && Rules[0][g].premise[0][0][k] != My_tree[i][j].predicate[num_pred].main_word[0][p])
							{
								flag = false;
								break;
							}
						}
						if (!flag)
							break;
					}
					if (flag)
					{
						My_tree[i][j].Rule.push_back(Rules[0][g].consequence);
					}
				}

			}
		}
	}
}
void TREE::clear()
{
	My_tree.clear();

	standart_predicat.clear();
	amount_of_words_in_sentence.clear();										
	data.clear();
	sent_data.clear();
	unknown_data.clear();
	Rules.clear();


}
void TREE::standart_predikat_3()
{
	int arg_num = -1;
	string main_word;
	string dep_predict = "";
	bool flag = false;
	vector<int> must_delete;
	vector<string> standart_arg;
	bool is_pl = false;
	string buf;
	int stan_pred;
	vector<int>arg1;
	vector<int>arg2;
	predicate predic;
	standart_predicate*pred = NULL;
	string exception = "Œ –”∆ÕŒ—“‹";
	vector<vector<string>>except;
	except.resize(except.size() + 1);
	except.back().push_back(exception);
	for (unsigned int n = 0; n < My_tree.size(); n++)
	{
		for (unsigned int count = 0; count < My_tree[n].size(); count++)
		{

			for (unsigned int count_p = 0; count_p < standart_predicat[2].size(); count_p++)
			{
				if (My_tree[n][count].Initial_form == standart_predicat[2][count_p].main_word[0])
				{
					for (int child = 0; child < My_tree[n][count].children.size(); child++)
					{
						/////////////ÍÓÒÚ˚Î¸ ËÒÔ‡‚¸//////////////////////////////////////////////////////////////////////////

						if (My_tree[n][count].children[child]->relation[0] == "2-kompl" || My_tree[n][count].children[child]->relation[0] == "1-kompl" || My_tree[n][count].children[child]->relation[0] == "predik")
						{

							arg1.push_back(My_tree[n][count].children[child]->number);

						}

					}
					for (int parent = 0; parent < My_tree[n][count].parent.size(); parent++)
					{
						/////////////ÍÓÒÚ˚Î¸ ËÒÔ‡‚¸//////////////////////////////////////////////////////////////////////////

						if (My_tree[n][count].relation[parent] == "opred" || My_tree[n][count].relation[parent] == "1-kompl" || My_tree[n][count].relation[parent] == "predik")
						{

							arg1.push_back(My_tree[n][count].parent[parent]->number);

						}

					}
					for (int child = 0; child < My_tree[n][count].children.size(); child++)
					{
						/////////////ÍÓÒÚ˚Î¸ ËÒÔ‡‚¸//////////////////////////////////////////////////////////////////////////
						if (My_tree[n][count].children[child]->morfology[0] == "PR" && (My_tree[n][count].children[child]->relation[0] == "obst" || My_tree[n][count].children[child]->relation[0] == "2-kompl" || My_tree[n][count].children[child]->relation[0] == "1-kompl" || My_tree[n][count].children[child]->relation[0] =="kolich-kopred" ))
						{

							arg2.push_back(My_tree[n][count].children[child]->children[0]->number);
						}
					}

					if (arg2.empty())
					{
						int kill = find_close_child(exception, n, count);
						if (kill != -1)
							arg2.push_back(kill);
					}
					
					if (arg2.empty())
					{
						int kill = find_pred(except, n, count);
						if (kill != -1)
							arg2.push_back(kill);
					}
					
					if (!arg1.empty() && !arg2.empty())
					{
						for (int i = 0; i < arg1.size(); i++)
						{
							buf = '0';
							for (int f = 0; f < My_tree[n][arg1[i]].Rule.size(); f++)
							{
								if (My_tree[n][arg1[i]].is_pred && My_tree[n][arg1[i]].predicate[0].number_of_pred == 1 && My_tree[n][arg1[i]].Rule[f] == "ÃÕŒ√Œ”√ŒÀ‹Õ» ")
								{
									if (strncmp(My_tree[n][arg1[i]].predicate[0].atribut[0].c_str(), "ÏÌ:", 3) == 0)
									{
										predic = My_tree[n][arg1[i]].predicate.back();
										My_tree[n][arg1[i]].predicate.pop_back();

										for (int j = 0; j < arg2.size(); j++)
										{
											for (int q = 0; q < My_tree[n][arg2[j]].predicate.size(); q++)
											{
												My_tree[n][arg1[i]].predicate.resize(My_tree[n][arg1[i]].predicate.size() + 1);
												My_tree[n][arg1[i]].predicate.back() = predic;
												standart_predicat[1][count_p].amt_of_used_pred++;
												int number = standart_predicat[1][count_p].amt_of_used_pred;
												buf.clear();
												while (number > 0)
												{

													buf.resize(buf.size() + 1);
													buf.back() = number % 10 + '0';
													number = (number - number % 10) / 10;
												}
												My_tree[n][arg1[i]].predicate.back().atribut.push_back(predic.main_word[0] + "_" + buf);
											}
										}
										if (My_tree[n][arg1[i]].ssilka != NULL)
										My_tree[n][arg1[i]].ssilka->predicate = My_tree[n][arg1[i]].predicate;
									}

								}
							}
						}

						for (int i = 0; i < arg1.size(); i++)
						{

							if (My_tree[n][arg1[i]].is_pred && strncmp(My_tree[n][arg1[i]].predicate[0].atribut[0].c_str(), "ÏÌ:", 3) == 0)
							{
								predic = My_tree[n][arg1[i]].predicate.back();
								My_tree[n][arg1[i]].predicate.pop_back();
								predic.atribut.clear();
								for (int j = 0; j < arg2.size(); j++)
								{
									for (int f = 0; f < My_tree[n][arg2[j]].Rule.size(); f++)
									{
										if (My_tree[n][arg2[j]].Rule[f] == "ÃÕŒ√Œ”√ŒÀ‹Õ» ")
										{
											for (int q = 0; q < My_tree[n][arg2[j]].predicate.size(); q++)
											{
												My_tree[n][arg1[i]].predicate.resize(My_tree[n][arg1[i]].predicate.size() + 1);
												My_tree[n][arg1[i]].predicate.back() = predic;
												standart_predicat[1][count_p].amt_of_used_pred++;
												int number = standart_predicat[1][count_p].amt_of_used_pred;
												buf.clear();
												while (number > 0)
												{
													buf.resize(buf.size() + 1);
													buf.back() = number % 10 + '0';
													number = (number - number % 10) / 10;
												}
												My_tree[n][arg1[i]].predicate.back().atribut.push_back(predic.main_word[0] + "_" + buf);
											}
										}

									}

								}
								if (My_tree[n][arg1[i]].ssilka != NULL)
								My_tree[n][arg1[i]].ssilka->predicate = My_tree[n][arg1[i]].predicate;
							}
						}

						for (int i = 0; i < arg2.size(); i++)
						{
							buf = '0';
							for (int f = 0; f < My_tree[n][arg2[i]].Rule.size(); f++)
							{
								if (My_tree[n][arg2[i]].Rule[f] == "ÃÕŒ√Œ”√ŒÀ‹Õ» ")
								{
									if (strncmp(My_tree[n][arg2[i]].predicate[0].atribut[0].c_str(), "ÏÌ:", 3) == 0 && strncmp(My_tree[n][arg1[i]].predicate[0].atribut[0].c_str(), "ÏÌ:", 3) != 0)
									{
										predic = My_tree[n][arg2[i]].predicate.back();
										My_tree[n][arg2[i]].predicate.pop_back();
										for (int j = 0; j < arg1.size(); j++)
										{
											for (int q = 0; q < My_tree[n][arg1[j]].predicate.size(); q++)
											{
												My_tree[n][arg2[i]].predicate.resize(My_tree[n][arg2[i]].predicate.size() + 1);
												My_tree[n][arg2[i]].predicate.back() = predic;
												standart_predicat[1][count_p].amt_of_used_pred++;
												int number = standart_predicat[1][count_p].amt_of_used_pred;
												buf.clear();
												while (number > 0)
												{
													buf.resize(buf.size() + 1);
													buf.back() = number % 10 + '0';
													number = (number - number % 10) / 10;
												}
												My_tree[n][arg2[i]].predicate.back().atribut.clear();
												My_tree[n][arg2[i]].predicate.back().atribut.push_back(predic.main_word[0] + "_" + buf);

											}
										}
										if (My_tree[n][arg2[i]].ssilka != NULL)
										My_tree[n][arg2[i]].ssilka->predicate = My_tree[n][arg1[i]].predicate;

									}
								}
							}
						}

						for (int i = 0; i < arg2.size(); i++)
						{
							buf = '0';


							if (My_tree[n][arg2[i]].is_pred && strncmp(My_tree[n][arg2[i]].predicate[0].atribut[0].c_str(), "ÏÌ:", 3) == 0)
							{
								predic = My_tree[n][arg1[i]].predicate.back();
								My_tree[n][arg2[i]].predicate.pop_back();
								for (int j = 0; j < arg1.size(); j++)
								{
									for (int f = 0; f < My_tree[n][arg1[j]].Rule.size(); f++)
									{
										if (My_tree[n][arg1[j]].Rule[f] == "ÃÕŒ√Œ”√ŒÀ‹Õ» ")
										{
											for (int q = 0; q < My_tree[n][arg1[j]].predicate.size(); q++)
											{
												My_tree[n][arg2[i]].predicate.resize(My_tree[n][arg2[i]].predicate.size() + 1);
												My_tree[n][arg2[i]].predicate.back() = predic;
												standart_predicat[1][count_p].amt_of_used_pred++;
												int number = standart_predicat[1][count_p].amt_of_used_pred;
												buf.clear();
												while (number > 0)
												{
													buf.resize(buf.size() + 1);
													buf.back() = number % 10 + '0';
													number = (number - number % 10) / 10;
												}
												My_tree[n][arg2[i]].predicate.back().atribut.clear();
												My_tree[n][arg2[i]].predicate.back().atribut.push_back(predic.main_word[0] + "_" + buf);

											}
										}
									}

								}
								if (My_tree[n][arg2[i]].ssilka!=NULL)
								My_tree[n][arg2[i]].ssilka->predicate = My_tree[n][arg1[i]].predicate;

							}
						}
						for (int i = 0; i < arg1.size(); i++)
						{
							if (My_tree[n][arg1[i]].is_pred)
							{


								for (int g = 0; g < My_tree[n][arg1[i]].predicate.size(); g++)
								{
									if (!arg2.empty())
									{
										for (int j = 0; j < arg2.size(); j++)
										{
											if (My_tree[n][arg2[j]].is_pred)
											{
												for (int q = 0; q < My_tree[n][arg2[j]].predicate.size(); q++)
												{
													My_tree[n][count].is_pred = true;
													My_tree[n][count].predicate.resize(My_tree[n][count].predicate.size() + 1);
													My_tree[n][count].predicate.back().main_word = standart_predicat[2][count_p].main_word;
													My_tree[n][count].predicate.back().number_of_pred = 2;


													My_tree[n][count].predicate.back().pred_atribut.resize(My_tree[n][count].predicate.back().pred_atribut.size() + 1);
													My_tree[n][count].predicate.back().pred_atribut.back().number_sentence = n;
													My_tree[n][count].predicate.back().pred_atribut.back().number_word = arg1[i];
													My_tree[n][count].predicate.back().pred_atribut.back().number_pred = g;

													My_tree[n][count].predicate.back().pred_atribut.resize(My_tree[n][count].predicate.back().pred_atribut.size() + 1);
													My_tree[n][count].predicate.back().pred_atribut.back().number_sentence = n;
													My_tree[n][count].predicate.back().pred_atribut.back().number_word = arg2[j];
													My_tree[n][count].predicate.back().pred_atribut.back().number_pred = q;
													flag = false;
													for (int w = 0; w < My_tree[n][count].predicate.size() - 1; w++)
													{
														for (int bred = 0; bred < 2; bred++)
														{
															if (My_tree[n][count].predicate.back().pred_atribut[bred].number_word == My_tree[n][count].predicate[w].pred_atribut[bred].number_word && My_tree[n][count].predicate.back().pred_atribut[bred].number_sentence == My_tree[n][count].predicate[w].pred_atribut[bred].number_sentence && My_tree[n][count].predicate.back().pred_atribut[bred].number_pred == My_tree[n][count].predicate[w].pred_atribut[bred].number_pred)
															{
																flag = true;
																My_tree[n][count].predicate.pop_back();
																break;
															}
														}
														if (flag)
															break;
													}


												}
											}




										}
									}
									else
									{
									}
								}

							}
						}
						arg1.clear();
					}
					else
					{
						arg1.clear();
						arg2.clear();
					}
				}
			}
		}
	}
	
}
void TREE::bonding()
{
	parent_pred par;
	for (int i = 0; i < My_tree.size(); i++)
	{
		for (int j = 0; j < My_tree[i].size(); j++)
		{
			
			if (!My_tree[i][j].is_pred)
			{
				for (int k = 0; k < My_tree[i][j].relation.size(); k++)
				{
					if (My_tree[i][j].relation[k] == "opred")
					{
						My_tree[i][j].is_pred = true;
						My_tree[i][j].predicate.resize(My_tree[i][j].predicate.size() + 1);
						if (!My_tree[i][j].morfology.empty() && (My_tree[i][j].morfology[0] == "A" || My_tree[i][j].morfology[0] == "¿"))
						My_tree[i][j].predicate.back().main_word.push_back(My_tree[i][j].name);
						else
							My_tree[i][j].predicate.back().main_word.push_back(My_tree[i][j].Initial_form);
						My_tree[i][j].predicate.back().number_of_pred = 4;

						if (My_tree[i][j].parent[k]->is_pred)
						{
							for (int p = 0; p < My_tree[i][j].parent[k]->predicate.size(); p++)
							{
								if (My_tree[i][j].parent[k]->predicate[p].number_of_pred == 0 || My_tree[i][j].parent[k]->predicate[p].number_of_pred == 1)
								{
									par.number_sentence = My_tree[i][j].parent[k]->number_of_sentences;
									par.number_word = My_tree[i][j].parent[k]->number;
									par.number_pred = p;
									My_tree[i][j].predicate.back().pred_atribut.push_back(par);
								}
							}
						}
						else
						{
							My_tree[i][j].predicate.back().atribut.push_back(My_tree[i][j].parent[k]->Initial_form);
						}
						for (int child = 0; child < My_tree[i][j].children.size(); child++)
						{
							if (My_tree[i][j].children[child]->is_pred)
							{
								for (int p = 0; p < My_tree[i][j].children[child]->predicate.size(); p++)
								{
									if (My_tree[i][j].children[child]->predicate[p].number_of_pred == 0 || My_tree[i][j].children[child]->predicate[p].number_of_pred == 1)
									{
										par.number_sentence = My_tree[i][j].children[child]->number_of_sentences;
										par.number_word = My_tree[i][j].children[child]->number;
										par.number_pred = p;
										My_tree[i][j].predicate.back().pred_atribut.push_back(par);
									}
								}
							}
							else
							{
								My_tree[i][j].predicate.back().atribut.push_back(My_tree[i][j].children[child]->Initial_form);
							}
						}
					}
				}
				
			}
		}
	}
	

}
void TREE::bonding_4()
{
	parent_pred par;
	for (int i = 0; i < My_tree.size(); i++)
	{
		for (int j = 0; j < My_tree[i].size(); j++)
		{

			if (!My_tree[i][j].is_pred && !My_tree[i][j].morfology.empty() &&( My_tree[i][j].morfology[0] == "A" ))
			{
				My_tree[i][j].is_pred = true;
				My_tree[i][j].predicate.resize(My_tree[i][j].predicate.size() + 1);
				My_tree[i][j].predicate.back().main_word.push_back(My_tree[i][j].name);
				My_tree[i][j].predicate.back().number_of_pred = 4;
				for (int k = 0; k < My_tree[i][j].children.size(); k++)
				{
					if (My_tree[i][j].children[k]->is_pred)
					{
						for (int p = 0; p < My_tree[i][j].children[k]->predicate.size(); p++)
						{
							if (My_tree[i][j].children[k]->predicate[p].number_of_pred == 0 || My_tree[i][j].children[k]->predicate[p].number_of_pred == 1)
							{
								par.number_sentence = My_tree[i][j].children[k]->number_of_sentences;
								par.number_word = My_tree[i][j].children[k]->number;
								par.number_pred = p;
								My_tree[i][j].predicate.back().pred_atribut.push_back(par);
							}
						}
					}
					else
					{
						My_tree[i][j].predicate.back().atribut.push_back(My_tree[i][j].children[k]->Initial_form);
					}
				}
			}
		}
	}


}
void TREE::bonding_5()
{
	parent_pred par;
	bool flag;
	for (int i = 0; i < My_tree.size(); i++)
	{
		for (int j = 0; j < My_tree[i].size(); j++)
		{

			if (!My_tree[i][j].is_pred && !My_tree[i][j].morfology.empty() && (My_tree[i][j].morfology[0] == "V"))
			{
				for (int k = 0; k < My_tree[i][j].children.size(); k++)
				{
					if (My_tree[i][j].children[k]->is_pred)
					{
						My_tree[i][j].is_pred = true;
						My_tree[i][j].predicate.resize(My_tree[i][j].predicate.size() + 1);
						My_tree[i][j].predicate.back().main_word.push_back(My_tree[i][j].Initial_form);
						My_tree[i][j].predicate.back().number_of_pred = -1;
						

						for (int p = 0; p < My_tree[i][j].children[k]->predicate.size(); p++)
						{
							if (My_tree[i][j].children[k]->predicate[p].number_of_pred == -1)
							{

								My_tree[i][j].predicate.back().number_of_pred = -3;

							}
							
								par.number_sentence = My_tree[i][j].children[k]->number_of_sentences;
								par.number_word = My_tree[i][j].children[k]->number;
								par.number_pred = p;
								My_tree[i][j].predicate.back().pred_atribut.push_back(par);
							
						}
					}
					
				}
			}
		}
	}


}
void TREE::rule_3()
{
	int num;
	string	main = "¡Œ Œ¬¿ﬂ";
	string	dep = "—“Œ–ŒÕ¿";
	for (int i = 0; i < My_tree.size(); i++)
	{
		for (int j = 0; j < My_tree[i].size(); j++)
		{
			if (My_tree[i][j].is_pred  )
			{
				if (My_tree[i][j].predicate[0].pred_atribut.size()==1 && My_tree[i][j].predicate[0].main_word[0] == main)
				{
					int number_sentence = My_tree[i][j].predicate[0].pred_atribut[0].number_sentence;
					int number_word = My_tree[i][j].predicate[0].pred_atribut[0].number_word;
					if (My_tree[number_sentence][number_word].Initial_form == dep)
					{
						if (My_tree[number_sentence][number_word].predicate.size() == 1)
						{
							My_tree[number_sentence][number_word].predicate.push_back(My_tree[number_sentence][number_word].predicate[0]);
							for (int stand = 0; stand < standart_predicat[My_tree[number_sentence][number_word].predicate[0].number_of_pred].size(); stand++)
							{
								if (standart_predicat[My_tree[number_sentence][number_word].predicate[0].number_of_pred][stand].main_word == My_tree[number_sentence][number_word].predicate[0].main_word)
								{
									standart_predicat[My_tree[number_sentence][number_word].predicate[0].number_of_pred][stand].amt_of_used_pred++;
									int number = standart_predicat[My_tree[number_sentence][number_word].predicate[0].number_of_pred][stand].amt_of_used_pred;
									string buf = "";
									while (number > 0)
									{

										buf.resize(buf.size() + 1);
										buf.back() = number % 10 + '0';
										number = (number - number % 10) / 10;
									}
									int number_sentence1 = My_tree[number_sentence][number_word].predicate[0].pred_atribut[0].number_sentence;
									int number_word1 = My_tree[number_sentence][number_word].predicate[0].pred_atribut[0].number_word;

									if (!My_tree[number_sentence1][number_word1].predicate[0].dep_words.empty() && My_tree[number_sentence1][number_word1].predicate[0].dep_words[0] == "–¿¬ÕŒ¡≈ƒ–≈ÕÕ€…")
									{
										My_tree[number_sentence][number_word].predicate[1].atribut[0].clear();
										My_tree[number_sentence][number_word].predicate[1].atribut[0] = standart_predicat[My_tree[number_sentence][number_word].predicate[0].number_of_pred][stand].atribut[0] + "_" + buf;
									}
									else
									{
										My_tree[number_sentence][number_word].predicate[1].atribut[0].clear();
										My_tree[number_sentence][number_word].predicate[1].atribut[0] = standart_predicat[My_tree[number_sentence][number_word].predicate[0].number_of_pred][stand].atribut[0] + "_" + buf;
										My_tree[number_sentence][number_word].predicate[1].atribut[1].clear();
										My_tree[number_sentence][number_word].predicate[1].atribut[1] = standart_predicat[My_tree[number_sentence][number_word].predicate[0].number_of_pred][stand].atribut[2] + "_" + buf;


									}
									My_tree[i][j].predicate[0].pred_atribut.push_back(My_tree[i][j].predicate[0].pred_atribut[0]);
									My_tree[i][j].predicate[0].pred_atribut[0].number_pred = 0;
									My_tree[i][j].predicate[0].pred_atribut[1].number_pred = 1;
								}
							}
						}
					}
				}
			}

		}
	}
	
}
void TREE::rule_4()
{
	int num;
	vector<string>	main;
	main.push_back("Œ—“–€…");
	main.push_back("“”œŒ…");
	main.push_back("œ–ﬂÃŒ…");
	vector<string>	pred;
	pred.push_back("Ã≈Õ‹ÿ≈");
	pred.push_back("¡ŒÀ‹ÿ≈");
	pred.push_back("–¿¬Õ€…");

	string	dep = "”√ŒÀ";
	for (int i = 0; i < My_tree.size(); i++)
	{
		for (int j = 0; j < My_tree[i].size(); j++)
		{
			if (My_tree[i][j].is_pred && My_tree[i][j].predicate[0].number_of_pred == 4)
			{
				for (int m = 0; m < main.size(); m++)
				{
					if (My_tree[i][j].predicate[0].pred_atribut.size() == 1 && My_tree[i][j].predicate[0].main_word[0] == main[m])
					{
						int number_sentence = My_tree[i][j].predicate[0].pred_atribut[0].number_sentence;
						int number_word = My_tree[i][j].predicate[0].pred_atribut[0].number_word;
						if (My_tree[number_sentence][number_word].Initial_form == dep)
						{
							My_tree[i][j].predicate[0].main_word[0] = pred[m];
							My_tree[i][j].predicate[0].atribut.push_back("90_√–¿ƒ”—");
						}
					}
				}
			}

		}
	}
}
void TREE::standart_predikat_4()
{

	int arg_num = -1;
	string main_word;
	vector<node> dep_predict;


	vector<int> arg1;

	vector<int> must_delete;
	vector<string> standart_arg;
	bool is_pl = false;
	string buf;
	bool is_dep = true;
	int count_p;
	bool flag = true;
	for (unsigned int n = 0; n < My_tree.size(); n++)
	{
		for (unsigned int count = 0; count < My_tree[n].size(); count++)
		{
			if (!My_tree[n][count].is_pred)
			{
				for (unsigned int count_p = 0; count_p < standart_predicat[3].size(); count_p++)
				{
					if (My_tree[n][count].Initial_form == standart_predicat[3][count_p].main_word[0])
					{

						for (unsigned int count_child = 0; count_child < My_tree[n][count].children.size(); count_child++)
						{
							if (!standart_predicat[3][count_p].dep_words.empty())
							{
								for (unsigned int count_dep = 0; count_dep < standart_predicat[3][count_p].dep_words.size(); count_dep++)
								{
									if (My_tree[n][count].children[count_child]->is_pred)
									{
										if (My_tree[n][count].children[count_child]->predicate[0].main_word[0].size() >= standart_predicat[3][count_p].dep_words[count_dep][0].size())
										{
											for (unsigned int dep_w = 0; dep_w < standart_predicat[3][count_p].dep_words[count_dep][0].size(); dep_w++)
											{
												is_dep = true;
												int key = My_tree[n][count].children[count_child]->predicate[0].main_word[0].size() - dep_w - 1;
												int key2 = standart_predicat[3][count_p].dep_words[count_dep][0].size() - dep_w - 1;
												if (My_tree[n][count].children[count_child]->predicate[0].main_word[0][key] == standart_predicat[3][count_p].dep_words[count_dep][0][key2])
												{
													is_dep = false;
													continue;
												}
											}
										}
									}
									else
										is_dep = false;
									if (is_dep)
									{
										flag = true;
										for (int dep_pred = 0; dep_pred < dep_predict.size(); dep_pred++)
										{
											if (dep_predict[dep_pred].number == My_tree[n][count].children[count_child]->number)
											{
												flag = false;
												break;
											}
										}
										if (flag)
											dep_predict.push_back(*My_tree[n][count].children[count_child]);

									}
								}
							}

							if (!My_tree[n][count].children[count_child]->morfology.empty() && (My_tree[n][count].children[count_child]->morfology[0] == "NONLEX" || My_tree[n][count].children[count_child]->morfology[0] == "ATR" || My_tree[n][count].children[count_child]->morfology[0] == "ABR"))
							{
								bool is_atrib = false;
								for (unsigned int count_parent = 0; count_parent < My_tree[n][count].children[count_child]->parent.size(); count_parent++)
								{

									if (My_tree[n][count].children[count_child]->parent[count_parent]->number == count && My_tree[n][count].children[count_child]->relation[count_parent] == "atrib")
									{
										is_atrib = true;
										break;
									}

								}
								if (is_atrib)
									arg1.push_back(My_tree[n][count].children[count_child]->number);
							}


						}
						if (dep_predict.empty())
						{
							int kill = find_pred(standart_predicat[3][count_p].dep_words, n, count);
							if (kill != -1)
								dep_predict.push_back(My_tree[n][kill]);
						}
						if (dep_predict.empty())
						{
							node* kill = find_close_pred(standart_predicat[3][count_p].dep_words, n, count);
							if (kill != NULL)
								dep_predict.push_back(*kill);
						}
						if (dep_predict.empty())
							break;



						My_tree[n][count].is_pred = true;
						unsigned int i;

						for (i = 0; i < arg1.size(); i++)
						{
							if (dep_predict.size() == 1 && dep_predict[0].predicate.size() == 1)
							{
								My_tree[n][count].predicate.resize(My_tree[n][count].predicate.size() + 1);
								My_tree[n][count].predicate.back().main_word.push_back(My_tree[n][count].Initial_form);
								My_tree[n][count].predicate.back().pred_atribut.resize(My_tree[n][count].predicate.back().pred_atribut.size() + 1);
								My_tree[n][count].predicate.back().pred_atribut.back().number_sentence = dep_predict[0].number_of_sentences;
								My_tree[n][count].predicate.back().pred_atribut.back().number_word = dep_predict[0].number;

								My_tree[n][count].predicate.back().number_of_pred = 1;
								if (i < arg1.size())
								{
									My_tree[n][count].predicate.back().atribut.push_back(My_tree[n][arg1[i]].Initial_form);
								}
								else
								{
									standart_predicat[3][count_p].amt_of_used_pred++;
									int number = standart_predicat[3][count_p].amt_of_used_pred;
									buf.clear();
									while (number > 0)
									{

										buf.resize(buf.size() + 1);
										buf.back() = number % 10 + '0';
										number = (number - number % 10) / 10;
									}

									My_tree[n][count].predicate.back().atribut.push_back(standart_predicat[3][count_p].atribut[0] + "_" + buf);


								}

							}
						}

						if (arg1.empty())
						{
							for (int g = 0; g < My_tree[n][count].morfology.size(); g++)
							{
								if (My_tree[n][count].morfology[g] == "PL")
								{
									is_pl = true;
									break;
								}
							}

							for (int d = 0; d < dep_predict.size(); d++)
							{
								if (is_pl)
								{
									if (dep_predict.size() == 1 && dep_predict[0].predicate.size() == 1)
									{
										My_tree[n][count].predicate.resize(My_tree[n][count].predicate.size() + 1);
										My_tree[n][count].predicate.back().main_word.push_back(My_tree[n][count].Initial_form);
										My_tree[n][count].predicate.back().pred_atribut.resize(My_tree[n][count].predicate.back().pred_atribut.size() + 1);
										My_tree[n][count].predicate.back().pred_atribut.back().number_sentence = dep_predict[0].number_of_sentences;
										My_tree[n][count].predicate.back().pred_atribut.back().number_word = dep_predict[0].number;
										My_tree[n][count].predicate.back().number_of_pred = 1;

										standart_predicat[3][count_p].amt_of_used_pred++;
										int number = standart_predicat[3][count_p].amt_of_used_pred;
										buf.clear();
										while (number > 0)
										{

											buf.resize(buf.size() + 1);
											buf.back() = number % 10 + '0';
											number = (number - number % 10) / 10;
										}
										My_tree[n][count].predicate.back().atribut.push_back("ÏÌ: " + standart_predicat[3][count_p].atribut[0] + "_" + buf);

										break;
									}
									for (int pr = 0; pr < dep_predict[d].predicate.size(); pr++)
									{
										My_tree[n][count].predicate.resize(My_tree[n][count].predicate.size() + 1);
										My_tree[n][count].predicate.back().main_word.push_back(My_tree[n][count].Initial_form);
										My_tree[n][count].predicate.back().number_of_pred = 1;
										My_tree[n][count].predicate.back().pred_atribut.resize(My_tree[n][count].predicate.back().pred_atribut.size() + 1);
										My_tree[n][count].predicate.back().pred_atribut.back().number_sentence = dep_predict[d].number_of_sentences;
										My_tree[n][count].predicate.back().pred_atribut.back().number_word = dep_predict[d].number;
										My_tree[n][count].predicate.back().pred_atribut.back().number_pred = pr;


										standart_predicat[3][count_p].amt_of_used_pred++;
										int number = standart_predicat[3][count_p].amt_of_used_pred;
										buf = "";
										while (number > 0)
										{

											buf.resize(buf.size() + 1);
											buf.back() = number % 10 + '0';
											number = (number - number % 10) / 10;
										}
										My_tree[n][count].predicate.back().atribut.push_back(standart_predicat[3][count_p].atribut[0] + "_" + buf);


									}


								}

								else
								{

									My_tree[n][count].predicate.resize(My_tree[n][count].predicate.size() + 1);
									My_tree[n][count].predicate.back().main_word.push_back(My_tree[n][count].Initial_form);
									My_tree[n][count].predicate.back().pred_atribut.resize(My_tree[n][count].predicate.back().pred_atribut.size() + 1);
									My_tree[n][count].predicate.back().pred_atribut.back().number_sentence = (dep_predict[d].number_of_sentences);
									My_tree[n][count].predicate.back().pred_atribut.back().number_word = (dep_predict[d].number);
									My_tree[n][count].predicate.back().number_of_pred = 1;
									standart_predicat[3][count_p].amt_of_used_pred++;
									int number = standart_predicat[3][count_p].amt_of_used_pred;
									buf.clear();
									while (number > 0)
									{

										buf.resize(buf.size() + 1);
										buf.back() = number % 10 + '0';
										number = (number - number % 10) / 10;
									}
									My_tree[n][count].predicate.back().atribut.push_back(standart_predicat[3][count_p].atribut[0] + "_" + buf);
								}

							}
						}
						if (My_tree[n][count].ssilka != NULL)
						{
							My_tree[n][count].ssilka->is_pred = My_tree[n][count].is_pred;
							My_tree[n][count].ssilka->predicate = My_tree[n][count].predicate;

						}
						for (int del = 0; del < arg1.size(); del++)
						{
							delete_node(n, arg1[del]);
						}
						dep_predict.clear();
						for (int q = 0; q < arg1.size(); q++)
						{
							delete_node(n, arg1[q]);
						}
						arg1.clear();

						buf.clear();
						is_pl = false;
						break;
					}


				}
			}
		}
	}


}
void TREE::bonding_2()
{
	parent_pred par;
	for (int i = 0; i < My_tree.size(); i++)
	{
		for (int j = 0; j < My_tree[i].size(); j++)
		{

			if (My_tree[i][j].is_pred && My_tree[i][j].predicate[0].number_of_pred == 0 || My_tree[i][j].is_pred && My_tree[i][j].predicate[0].number_of_pred == 1)
			{
				for (int k = 0; k < My_tree[i][j].children.size(); k++)
				{

					if (!My_tree[i][j].children[k]->is_pred)
					{
						My_tree[i][j].is_pred = true;
						My_tree[i][j].predicate.resize(My_tree[i][j].predicate.size() + 1);
						My_tree[i][j].predicate.back().main_word.push_back(My_tree[i][j].Initial_form + "_" + My_tree[i][j].children[k]->Initial_form);
						My_tree[i][j].predicate.back().number_of_pred = 4;
						par.number_sentence =i;
						par.number_word =j;
						par.number_pred = 0;
						My_tree[i][j].predicate.back().pred_atribut.push_back(par);
						for (int l = 0; l < My_tree[i][j].children[k]->children.size(); l++)
						{
							if (My_tree[i][j].children[k]->children[l]->is_pred)
							{
								for (int p = 0; p < My_tree[i][j].children[k]->children[l]->predicate.size(); p++)
								{
									if (My_tree[i][j].children[k]->children[l]->predicate[p].number_of_pred == 0 || My_tree[i][j].children[k]->children[l]->predicate[p].number_of_pred == 1)
									{
										par.number_sentence = My_tree[i][j].children[k]->children[l]->number_of_sentences;
										par.number_word = My_tree[i][j].children[k]->children[l]->number;
										par.number_pred = p;
										My_tree[i][j].predicate.back().pred_atribut.push_back(par);
									}
								}
							}
							else
							{
								My_tree[i][j].predicate.back().atribut.push_back(My_tree[i][j].children[k]->children[l]->Initial_form);
							}


						}
					}
				}
			}
		}
	}


}
void TREE::delete_replace(size_t number_of_sentence, size_t number_of_word)
{
	for (int count_parent = 0; count_parent < My_tree[number_of_sentence][number_of_word].parent.size(); count_parent++)
	{
		for (int parent_child = 0; parent_child < My_tree[number_of_sentence][number_of_word].parent[count_parent]->children.size(); parent_child++)
		{
			if (My_tree[number_of_sentence][number_of_word].parent[count_parent]->children[parent_child]->number == number_of_word)
			{
				for (int child = parent_child; child < My_tree[number_of_sentence][number_of_word].parent[count_parent]->children.size() - 1; child++)
				{
					My_tree[number_of_sentence][number_of_word].parent[count_parent]->children[child] = My_tree[number_of_sentence][number_of_word].parent[count_parent]->children[child + 1];
				}
				My_tree[number_of_sentence][number_of_word].parent[count_parent]->children.pop_back();
				for (int child = 0; child < My_tree[number_of_sentence][number_of_word].children.size(); child++)
				{
					for (int child_parent = 0; child_parent < My_tree[number_of_sentence][number_of_word].children[child]->parent.size(); child_parent++)
					{
						if (My_tree[number_of_sentence][number_of_word].children[child]->parent[child_parent]->number == number_of_word)
						{
							for (int par = child_parent; par < My_tree[number_of_sentence][number_of_word].children[child]->parent.size() - 1; par++)
							{
								My_tree[number_of_sentence][number_of_word].children[child]->parent[par] = My_tree[number_of_sentence][number_of_word].children[child]->parent[par + 1];
							}
							My_tree[number_of_sentence][number_of_word].children[child]->parent.pop_back();
							break;
						}
					}
					My_tree[number_of_sentence][number_of_word].children[child]->parent.push_back(My_tree[number_of_sentence][number_of_word].parent[count_parent]);
					My_tree[number_of_sentence][number_of_word].parent[count_parent]->children.push_back(My_tree[number_of_sentence][number_of_word].children[child]);
				}
				
				break;
			}
		}
	}
	if (My_tree[number_of_sentence][number_of_word].root)
	{
		for (int child = 0; child < My_tree[number_of_sentence][number_of_word].children.size(); child++)
		{
			for (int child_parent = 0; child_parent < My_tree[number_of_sentence][number_of_word].children[child]->parent.size(); child_parent++)
			{
				if (My_tree[number_of_sentence][number_of_word].children[child]->parent[child_parent]->number == number_of_word)
				{
					for (int par = child_parent; par < My_tree[number_of_sentence][number_of_word].children[child]->parent.size() - 1; par++)
					{
						My_tree[number_of_sentence][number_of_word].children[child]->parent[par] = My_tree[number_of_sentence][number_of_word].children[child]->parent[par + 1];
					}
					My_tree[number_of_sentence][number_of_word].children[child]->parent.pop_back();
					break;
				}
			}
			My_tree[number_of_sentence][number_of_word].children[child]->root = true;
			
		}
	}
	My_tree[number_of_sentence][number_of_word].children.clear();
	My_tree[number_of_sentence][number_of_word].parent.clear();
	delete_node(number_of_sentence, number_of_word);
}
void TREE::operations()
{
	
	int buf_int;
	string buf;
	bool flag = false, last_flag = false;
	buf.clear();
	bool is_pl;
	parent_pred par;
	
	for (int i = 0; i < My_tree.size(); i++) 
	{
		for (int j = 0; j < My_tree[i].size(); j++)
		{
			for (int k = 0; k < operation[0].size(); k++)
			{
				if (operation[0][k].main == My_tree[i][j].Initial_form)
				{
					My_tree[i][j].is_pred = true;
					My_tree[i][j].predicate.resize(My_tree[i][j].predicate.size() + 1);
					My_tree[i][j].predicate.back().main_word.push_back(My_tree[i][j].Initial_form);
					My_tree[i][j].predicate.back().number_of_pred = -1;
					My_tree[i][j].Rule.push_back(operation[0][k].rule);
					for (int child = 0; child < My_tree[i][j].children.size(); child++)
					{

						if (My_tree[i][j].children[child]->is_pred)
						{
							for (int p = 0; p < My_tree[i][j].children[child]->predicate.size(); p++)
							{
								if (My_tree[i][j].children[child]->predicate[p].number_of_pred == 0 || My_tree[i][j].children[child]->predicate[p].number_of_pred == 1)
								{
									par.number_sentence = My_tree[i][j].children[child]->number_of_sentences;
									par.number_word = My_tree[i][j].children[child]->number;
									par.number_pred = p;
									My_tree[i][j].predicate.back().pred_atribut.push_back(par);
								}
							}
						}
						else
						{
							if (My_tree[i][j].children[child]->Initial_form == "ƒÀ»Õ¿"  || My_tree[i][j].children[child]->Initial_form == operation[0][k].dep)
							{
								for (int child_1 = 0; child_1 < My_tree[i][j].children[child]->children.size(); child_1++)
								{

									if (My_tree[i][j].children[child]->children[child_1]->is_pred)
									{
										for (int p = 0; p < My_tree[i][j].children[child]->children[child_1]->predicate.size(); p++)
										{
											if (My_tree[i][j].children[child]->children[child_1]->predicate[p].number_of_pred == 0 || My_tree[i][j].children[child]->children[child_1]->predicate[p].number_of_pred == 1)
											{
												par.number_sentence = My_tree[i][j].children[child]->children[child_1]->number_of_sentences;
												par.number_word = My_tree[i][j].children[child]->children[child_1]->number;
												par.number_pred = p;
												My_tree[i][j].predicate.back().pred_atribut.push_back(par);
											}
										}
									}
									else
									{
										if (!My_tree[i][j].children[child]->children[child_1]->morfology.empty() && (My_tree[i][j].children[child]->children[child_1]->morfology[0] == "S" || My_tree[i][j].children[child]->children[child_1]->morfology[0] == "NUM" || My_tree[i][j].children[child]->children[child_1]->morfology[0] == "abr"))
											My_tree[i][j].predicate.back().atribut.push_back(My_tree[i][j].children[child]->children[child_1]->Initial_form);
									}
								}
							}
							else
							{
								if (!My_tree[i][j].children[child]->morfology.empty() && (My_tree[i][j].children[child]->morfology[0] == "S" || My_tree[i][j].children[child]->morfology[0] == "NUM" || My_tree[i][j].children[child]->morfology[0] == "abr"))
									My_tree[i][j].predicate.back().atribut.push_back(My_tree[i][j].children[child]->Initial_form);
							}
						}
					}
				}
			}
	
			for (int k = 0; k < operation[1].size(); k++)
			{
				if (operation[1][k].main == My_tree[i][j].Initial_form)
				{
					My_tree[i][j].is_pred = true;
					My_tree[i][j].predicate.resize(My_tree[i][j].predicate.size() + 1);
					My_tree[i][j].predicate.back().main_word.push_back(My_tree[i][j].Initial_form);
					My_tree[i][j].predicate.back().number_of_pred = 2;
					My_tree[i][j].Rule.push_back(operation[1][k].rule);
					flag = 0;
					for (int child = 0; child < My_tree[i][j].children.size(); child++)
					{


						if (My_tree[i][j].children[child]->is_pred)
						{
							for (int p = 0; p < My_tree[i][j].children[child]->predicate.size(); p++)
							{
								if (My_tree[i][j].children[child]->predicate[p].number_of_pred == -1)
								{
									for (int count = 0; count < operation[0].size(); count++)
									{
										if (My_tree[i][j].children[child]->Initial_form == operation[0][count].main)
										{
											flag = 1;
											break;
										}
									}

									if (flag)
									{
										My_tree[i][j].predicate.back().number_of_pred = -2;


									}

								}

								par.number_sentence = My_tree[i][j].children[child]->number_of_sentences;
								par.number_word = My_tree[i][j].children[child]->number;
								par.number_pred = p;
								My_tree[i][j].predicate.back().pred_atribut.push_back(par);
							}




						}
						else
						{
							if (!My_tree[i][j].children[child]->morfology.empty() && (My_tree[i][j].children[child]->morfology[0] == "NONLEX" || My_tree[i][j].children[child]->morfology[0] == "S" || My_tree[i][j].children[child]->morfology[0] == "NUM" || My_tree[i][j].children[child]->morfology[0] == "abr"))
								My_tree[i][j].predicate.back().atribut.push_back(My_tree[i][j].children[child]->Initial_form);
						}

					}
				}

				
			}
	
		}
	}
	for (int i = 0; i < My_tree.size(); i++)
	{
		for (int j = 0; j < My_tree[i].size(); j++)
		{
			for (int k = 0; k < operation[2].size(); k++)
			{
				if (operation[2][k].main == My_tree[i][j].Initial_form)
				{
					for (int parent = 0; parent < My_tree[i][j].parent.size(); parent++)
					{
						if (My_tree[i][j].parent[parent]->is_pred && My_tree[i][j].parent[parent]->Initial_form == operation[2][k].dep)
						{
							My_tree[i][j].is_pred = true;
							My_tree[i][j].predicate.resize(My_tree[i][j].predicate.size() + 1);
							My_tree[i][j].predicate.back().main_word.push_back("–¿¬Õ€…");
							My_tree[i][j].predicate.back().number_of_pred = -2;
							My_tree[i][j].Rule.push_back(operation[2][k].rule);
							for (int p = 0; p < My_tree[i][j].parent[parent]->predicate.size(); p++)
							{
								par.number_sentence = My_tree[i][j].parent[parent]->number_of_sentences;
								par.number_word = My_tree[i][j].parent[parent]->number;
								par.number_pred = p;
								if (My_tree[par.number_sentence][par.number_word].predicate[par.number_pred].pred_atribut.size() == 1)
								{
									is_pl = false;
									parent_pred par2 = My_tree[par.number_sentence][par.number_word].predicate[par.number_pred].pred_atribut[0];
									if (strncmp(My_tree[par2.number_sentence][par2.number_word].predicate[0].atribut[0].c_str(), "ÏÌ:", 3) == 0)
									{
										is_pl = true;
									}
								}
								My_tree[i][j].predicate.back().pred_atribut.push_back(par);
								
							}
							for (int child = 0; child < My_tree[i][j].children.size(); child++)
							{
								if (!My_tree[i][j].morfology.empty() && My_tree[i][j].morfology[0] != "PR")
								{
									if (My_tree[i][j].children[child]->is_pred)
									{
										for (int p = 0; p < My_tree[i][j].children[child]->predicate.size(); p++)
										{
											if (My_tree[i][j].children[child]->predicate[p].number_of_pred == 0 || My_tree[i][j].children[child]->predicate[p].number_of_pred == 1)
											{
												par.number_sentence = My_tree[i][j].children[child]->number_of_sentences;
												par.number_word = My_tree[i][j].children[child]->number;
												par.number_pred = p;
												My_tree[i][j].predicate.back().pred_atribut.push_back(par);
											}
										}
									}
									else
									{
										if (My_tree[i][j].children[child]->Initial_form == "ƒÀ»Õ¿" || My_tree[i][j].children[child]->Initial_form == operation[0][k].dep)
										{
											for (int child_1 = 0; child_1 < My_tree[i][j].children[child]->children.size(); child_1++)
											{

												if (My_tree[i][j].children[child]->children[child_1]->is_pred)
												{
													for (int p = 0; p < My_tree[i][j].children[child]->children[child_1]->predicate.size(); p++)
													{
														if (My_tree[i][j].children[child]->children[child_1]->predicate[p].number_of_pred == 0 || My_tree[i][j].children[child]->children[child_1]->predicate[p].number_of_pred == 1)
														{
															par.number_sentence = My_tree[i][j].children[child]->children[child_1]->number_of_sentences;
															par.number_word = My_tree[i][j].children[child]->children[child_1]->number;
															par.number_pred = p;
															My_tree[i][j].predicate.back().pred_atribut.push_back(par);
															
														}
													}
												}
												else
												{
													if (!My_tree[i][j].children[child]->children[child_1]->morfology.empty() && (My_tree[i][j].children[child]->children[child_1]->morfology[0] == "S" || My_tree[i][j].children[child]->children[child_1]->morfology[0] == "NUM" || My_tree[i][j].children[child]->children[child_1]->morfology[0] == "abr"))
													{
														if (is_pl && My_tree[i][j].children[child]->children[child_1]->morfology[0] == "NUM")
														{
															int amount = 0;
															for (int length = 0; length < My_tree[i][j].children[child]->children[child_1]->Initial_form.size(); length++)
															{
																if (My_tree[i][j].children[child]->children[child_1]->Initial_form[length] == ':')
																{
																	amount++;
																}
															}
															if (amount != 0)
															{
																parent_pred par1 = My_tree[par.number_sentence][par.number_word].predicate[par.number_pred].pred_atribut[0];
																predicate predic = My_tree[par1.number_sentence][par.number_word].predicate[0];
																for (int f = 0; f < predic.atribut.size(); f++)
																{
																	predic.atribut[f].substr(3);
																}
																My_tree[par1.number_sentence][par.number_word].predicate.clear();
																for (int g = 0; g < amount; g++)
																{

																}
															}
														}
														My_tree[i][j].predicate.back().atribut.push_back(My_tree[i][j].children[child]->children[child_1]->Initial_form);
													}
												}
											}
										}
										else
										{
											if (!My_tree[i][j].children[child]->morfology.empty() && (My_tree[i][j].children[child]->morfology[0] == "NONLEX" || My_tree[i][j].children[child]->morfology[0] == "S" || My_tree[i][j].children[child]->morfology[0] == "NUM" || My_tree[i][j].children[child]->morfology[0] == "abr"))
											{
												if (is_pl && My_tree[i][j].children[child]->morfology[0] == "NUM")
												{
													int amount = 0;
													for (int length = 0; length < My_tree[i][j].children[child]->Initial_form.size(); length++)
													{
														if (My_tree[i][j].children[child]->Initial_form[length] == ':')
														{
															amount++;
														}
													}
													if (amount != 0)
													{
														parent_pred par1 = My_tree[par.number_sentence][par.number_word].predicate[par.number_pred].pred_atribut[0];
														standart_predicate* st_pr = &standart_predicat[My_tree[par1.number_sentence][par1.number_word].predicate[0].number_of_standart_pack][My_tree[par1.number_sentence][par1.number_word].predicate[0].number_of_standart_pred];

														predicate predic = My_tree[par1.number_sentence][par1.number_word].predicate[0];
														My_tree[par1.number_sentence][par1.number_word].predicate.clear();
														char buf = st_pr->amt_of_used_pred+'0';
														My_tree[par1.number_sentence][par1.number_word].predicate.resize(My_tree[par1.number_sentence][par1.number_word].predicate.size() + 1);
														My_tree[par1.number_sentence][par1.number_word].predicate.back().main_word = st_pr->main_word;
														for (int atr = 0; atr < predic.atribut.size(); atr++)
														{
															predic.atribut[atr].erase(0, 3);
														}
														My_tree[par1.number_sentence][par1.number_word].predicate.back().pred_atribut = predic.pred_atribut;
														My_tree[par1.number_sentence][par1.number_word].predicate.back().atribut = predic.atribut;
														My_tree[par1.number_sentence][par1.number_word].predicate.back().number_of_pred = predic.number_of_pred;
														My_tree[par.number_sentence][par.number_word].predicate[par.number_pred].pred_atribut.clear();
														My_tree[par.number_sentence][par.number_word].predicate[par.number_pred].pred_atribut.resize(My_tree[par.number_sentence][par.number_word].predicate[par.number_pred].pred_atribut.size() + 1);
														My_tree[par.number_sentence][par.number_word].predicate[par.number_pred].pred_atribut.back().number_sentence = par1.number_sentence;
														My_tree[par.number_sentence][par.number_word].predicate[par.number_pred].pred_atribut.back().number_word = par1.number_word;
														My_tree[par.number_sentence][par.number_word].predicate[par.number_pred].pred_atribut.back().number_pred = 0;
														st_pr->amt_of_used_pred++;
														for (int atr = 0; atr < predic.atribut.size(); atr++)
														{
															predic.atribut[atr].pop_back();
														}
														for (int g = 0; g < amount; g++)
														{
															buf++;
															
															My_tree[par1.number_sentence][par1.number_word].predicate.resize(My_tree[par1.number_sentence][par1.number_word].predicate.size()+1);
															My_tree[par1.number_sentence][par1.number_word].predicate.back().main_word = predic.main_word;
															My_tree[par1.number_sentence][par1.number_word].predicate.back().pred_atribut = predic.pred_atribut;
															My_tree[par1.number_sentence][par1.number_word].predicate.back().number_of_pred = predic.number_of_pred;
															st_pr->amt_of_used_pred++; 
															for (int f = 0; f < predic.atribut.size(); f++)
															{
																My_tree[par1.number_sentence][par1.number_word].predicate.back().atribut.push_back(predic.atribut[f]+buf);
															}
															My_tree[par.number_sentence][par.number_word].predicate[par.number_pred].pred_atribut.resize(My_tree[par.number_sentence][par.number_word].predicate[par.number_pred].pred_atribut.size() + 1);
															My_tree[par.number_sentence][par.number_word].predicate[par.number_pred].pred_atribut.back().number_sentence = par1.number_sentence;
															My_tree[par.number_sentence][par.number_word].predicate[par.number_pred].pred_atribut.back().number_word = par1.number_word;
															My_tree[par.number_sentence][par.number_word].predicate[par.number_pred].pred_atribut.back().number_pred = g+1;

														}
													}
												}
												My_tree[i][j].predicate.back().atribut.push_back(My_tree[i][j].children[child]->Initial_form);
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}
void TREE::dash()
{
	parent_pred par;
	for (int i = 0; i < My_tree.size(); i++)
	{
		for (int j = 0; j < My_tree[i].size(); j++)
		{

			if (!My_tree[i][j].is_pred && My_tree[i][j].Initial_form == "-")
			{
				My_tree[i][j].is_pred = true;
				My_tree[i][j].predicate.resize(My_tree[i][j].predicate.size() + 1);
				My_tree[i][j].predicate.back().main_word.push_back(My_tree[i][j].Initial_form);
				My_tree[i][j].predicate.back().number_of_pred = -2;
				My_tree[i][j].Rule.push_back("=");
				for (int child = 0; child < My_tree[i][j].children.size(); child++)
				{

					if (My_tree[i][j].children[child]->is_pred)
					{
						for (int p = 0; p < My_tree[i][j].children[child]->predicate.size(); p++)
						{
							if(My_tree[i][j].children[child]->predicate[p].number_of_pred == 0 || My_tree[i][j].children[child]->predicate[p].number_of_pred == 1)
							{
								par.number_sentence = My_tree[i][j].children[child]->number_of_sentences;
								par.number_word = My_tree[i][j].children[child]->number;
								par.number_pred = p;
								My_tree[i][j].predicate.back().pred_atribut.push_back(par);
							}
						}
					}
					else
					{
						My_tree[i][j].predicate.back().atribut.push_back(My_tree[i][j].children[child]->Initial_form);
					}
					for (int parent = 0; parent < My_tree[i][j].parent.size(); parent++)
					{

						if (My_tree[i][j].parent[parent]->is_pred)
						{
							for (int p = 0; p < My_tree[i][j].parent[parent]->predicate.size(); p++)
							{
								if (My_tree[i][j].parent[parent]->predicate[p].number_of_pred == 0 || My_tree[i][j].parent[parent]->predicate[p].number_of_pred == 1)
								{
									par.number_sentence = My_tree[i][j].parent[parent]->number_of_sentences;
									par.number_word = My_tree[i][j].parent[parent]->number;
									par.number_pred = p;
									My_tree[i][j].predicate.back().pred_atribut.push_back(par);
								}
							}
						}
					}
				}
			}
		}
	}


}
void TREE::rule_5()
{
	string main_word = "≈—À»";
	vector<string>dep_word(2);
	dep_word[0] = "“Œ";
	dep_word[1] = "“Œ√ƒ¿";
	int flag = 0;
	for (int i = 0; i < My_tree.size(); i++)
	{
		for (int j = 0; j < My_tree[i].size(); j++)
		{
			if (main_word == My_tree[i][j].Initial_form)
			{
				flag = 1;
				My_phrase.resize(My_phrase.size() + 1);
				My_phrase.back().main = My_tree[i][j].Initial_form;
				My_tree[i][j].is_phrase = true;
				My_tree[i][j].number_of_phrase = My_phrase.size() - 1;
				continue;
			}
			for (int dep = 0; dep < dep_word.size(); dep++)
			{
				if (dep_word[dep] == My_tree[i][j].Initial_form)
				{
					flag = 2;
					My_phrase.resize(My_phrase.size() + 1);
					My_phrase.back().main = My_tree[i][j].Initial_form;
					My_phrase[My_phrase.size() - 2].is_used = true;
					My_phrase.back().atribut_phrase.push_back(My_phrase[My_phrase.size() - 2]);
					My_phrase.back().Rule.push_back("=>");
					My_tree[i][j].is_phrase = true;
					My_tree[i][j].number_of_phrase = My_phrase.size() - 1;
					continue;
				}
			}
			if (flag != 0)
			{
				if (My_tree[i][j].is_pred)
				for (int pred = 0; pred < My_tree[i][j].predicate.size(); pred++)
				{

					My_tree[i][j].predicate[pred].is_used = true;
					My_tree[i][j].predicate[pred].number_of_phrase = My_phrase.size() - 1;
					My_phrase.back().atribut_predicate.resize(My_phrase.back().atribut_predicate.size() + 1);
					My_phrase.back().atribut_predicate.back().number_sentence = i;
					My_phrase.back().atribut_predicate.back().number_word = j;
					My_phrase.back().atribut_predicate.back().number_pred = pred;

				}
				
			}
		}
	}
	if (flag == 1)
	{
		for (int i = 0; i < My_phrase.back().atribut_predicate.size(); i++)
		{
			int count = My_phrase.back().atribut_predicate[i].number_sentence;
			int count1 = My_phrase.back().atribut_predicate[i].number_word;
			int count2 = My_phrase.back().atribut_predicate[i].number_pred;
			My_tree[count][count1].predicate[count2].is_used = false;
		}
		My_phrase.pop_back();

	}
}
void TREE::rule_6()
{
	string main_word = "ƒŒ ¿«¿“‹";
	
	int flag = 0;
	for (int i = 0; i < My_tree.size(); i++)
	{
		for (int j = 0; j < My_tree[i].size(); j++)
		{
			if (main_word == My_tree[i][j].Initial_form)
			{
				flag = 1;
				My_phrase.resize(My_phrase.size() + 1);
				My_phrase.back().main = My_tree[i][j].Initial_form;
				if (My_phrase.size() != 1)
				{
					My_phrase[My_phrase.size() - 2].is_used = true;
					My_phrase.back().atribut_phrase.push_back(My_phrase[My_phrase.size() - 2]);
				}
				else
				for (int k = 0; k <My_tree[i][j].children.size(); k++)
				{
					if (My_tree[i][j].children[k]->Initial_form == "◊“Œ")
					{
						for (int child = 0; child < My_tree[i][j].children[k]->children.size(); child++)
						{
							if (My_tree[i][j].children[k]->children[child]->is_pred)
							{
								My_phrase.back().atribut_predicate.resize(My_phrase.back().atribut_predicate.size() + 1);
								My_phrase.back().atribut_predicate.back().number_sentence = My_tree[i][j].children[k]->children[child]->number_of_sentences;
								My_phrase.back().atribut_predicate.back().number_word= My_tree[i][j].children[k]->children[child]->number;
								My_phrase.back().atribut_predicate.back().number_pred = -1;

							}
						}
					}

				}
				continue;
			}
			
		}
	}
	
}
void TREE::list()
{
	int start = 0;
	vector<string>exeption;
	cout << endl;
	for (int i = 0; i < My_tree.size(); i++)
	{
		for (int j = 0; j < My_tree[i].size(); j++)
		{
			if (My_tree[i][j].is_pred && My_tree[i][j].predicate[0].number_of_standart_pack != -1)
			{
				for (int k = i; k < My_tree.size(); k++)
				{
					if (k == i)
						start = j + 1;
					else
						start = 0;
					for (int q = start; q < My_tree[k].size(); q++)
					{
						if (My_tree[k][q].is_pred && My_tree[k][q].predicate[0].number_of_standart_pack != -1 && strncmp(My_tree[i][j].predicate[0].atribut[0].c_str(), standart_predicat[My_tree[i][j].predicate[0].number_of_standart_pack][My_tree[i][j].predicate[0].number_of_standart_pred].atribut[0].c_str(), standart_predicat[My_tree[i][j].predicate[0].number_of_standart_pack][My_tree[i][j].predicate[0].number_of_standart_pred].atribut[0].size()- 2) == 0)
						{
							if (strncmp(My_tree[i][j].predicate[0].atribut[0].c_str(), My_tree[k][q].predicate[0].atribut[0].c_str(), My_tree[i][j].predicate[0].atribut[0].size() - 2) == 0)
							{
								My_tree[k][q].predicate = My_tree[i][j].predicate;
							}
						}
					}
				}
			}
		}
	}
	for (int i = 0; i < My_tree.size(); i++)
	{
		for (int j = 0; j < My_tree[i].size(); j++)
		{
			if (My_tree[i][j].is_pred && !My_tree[i][j].predicate[0].is_used)
				print_predicate(i,j);
		}
	}
	cout << endl;
	int j = 0;
	for (int i = 0; i < My_phrase.size(); i++)
	{
		if (!My_phrase[i].is_used)
		{
			if (My_phrase[i].Rule.empty())
			{
				cout << My_phrase[i].main << "{" << endl;
				for (j = 0; j < My_phrase[i].atribut_phrase.size(); j++)
				{
					if (My_phrase[i].atribut_phrase[j].Rule.empty())
					{
						cout << My_phrase[i].atribut_phrase[j].main << "{" << endl;
						for (int k = 0; k < My_phrase[i].atribut_phrase[j].atribut_predicate.size(); k++)
						{
							print_predicate(My_phrase[i].atribut_phrase[j].atribut_predicate[k].number_sentence, My_phrase[i].atribut_phrase[j].atribut_predicate[k].number_word);
						}
					}
					else
					{
						for (int count = 0; count < My_phrase[i].atribut_phrase[j].atribut_phrase.size(); count++)
						{
							int count1 = 0;
							cout << My_phrase[i].atribut_phrase[j].atribut_phrase[count].main << "{" << endl;
							for (count1 = 0; count1 < My_phrase[i].atribut_phrase[j].atribut_phrase[count].atribut_predicate.size(); count1++)
							{
								print_predicate(My_phrase[i].atribut_phrase[j].atribut_phrase[count].atribut_predicate[count1].number_sentence, My_phrase[i].atribut_phrase[j].atribut_phrase[count].atribut_predicate[count1].number_word);
							}
							if (count1 != 0)
								cout << "}" << My_phrase[i].atribut_phrase[j].Rule[0];
							cout << "{" << endl;
							for (int k = 0; k < My_phrase[i].atribut_phrase[j].atribut_predicate.size(); k++)
							{
								print_predicate(My_phrase[i].atribut_phrase[j].atribut_predicate[k].number_sentence, My_phrase[i].atribut_phrase[j].atribut_predicate[k].number_word);
							}
							cout << "}" << endl;
						}
					}
				}
				if (j != 0)
				{
					cout << "}" << endl;
				}
				for (int k = 0; k < My_phrase[i].atribut_predicate.size(); k++)
				{
					print_predicate(My_phrase[i].atribut_predicate[k].number_sentence, My_phrase[i].atribut_predicate[k].number_word);
				}
				cout << "}" << endl;
			}
			else
			{
				for (j = 0; j < My_phrase[i].atribut_phrase.size(); j++)
				{
					cout << My_phrase[i].atribut_phrase[j].main << "{" << endl;
					for (int k = 0; k < My_phrase[i].atribut_phrase[j].atribut_predicate.size(); k++)
					{
						print_predicate(My_phrase[i].atribut_phrase[j].atribut_predicate[k].number_sentence, My_phrase[i].atribut_phrase[j].atribut_predicate[k].number_word);
					}
				}
				if (j != 0)
				{
					cout << "}" << My_phrase[i].Rule[0];
				}
				cout << "{";
				for (int k = 0; k < My_phrase[i].atribut_predicate.size(); k++)
				{
					print_predicate(My_phrase[i].atribut_predicate[k].number_sentence, My_phrase[i].atribut_predicate[k].number_word);
				}
				cout << "}" << endl;
			}
		}
	}
}
void TREE::print_sentences()
{
	cout << endl;
	for (int i = 0; i < My_tree.size(); i++)
	{
		for (int j = 0; j < My_tree[i].size(); j++)
		{
			
				cout << My_tree[i][j].name << " ";
			

		}
		cout << sent_data[i].punc << " ";
	}
	cout << endl<<endl;
}
void TREE::rule7()
{
	bool flag = false;
	for (int i = 0; i < My_tree.size(); i++)
	{
		for (int j = 0; j < My_tree[i].size(); j++)
		{
			if (My_tree[i][j].is_pred)
			{
				
				for (int k = 0; k < My_tree[i][j].predicate.size(); k++)
				{
					for (int h = 0; h < Rules[1].size(); h++)
					{
						if (My_tree[i][j].predicate[k].main_word == Rules[1][h].premise[0])
						{
							if (strncmp(My_tree[i][j].predicate[k].atribut[0].c_str(), "ÏÌ:", 3) == 0)
							{
								predicate predic;
								predic = My_tree[i][j].predicate[k];

								for (int atr = 0; atr < My_tree[i][j].predicate[k].atribut.size(); atr++)
								{
									My_tree[i][j].predicate[k].atribut[atr].erase(0, 3);
									predic.atribut[atr].pop_back();
									predic.atribut[atr].erase(0, 3);
									
								}
								int amt = Rules[1][h].consequence[0] - '0';
								for (int amount = 1; amount < amt; amount++)
								{
									int num;
									char buf;
									
									My_tree[i][j].predicate.resize(My_tree[i][j].predicate.size() + 1);
									My_tree[i][j].predicate.back() = My_tree[i][j].predicate[k];
									My_tree[i][j].predicate.back().atribut.clear();
									num = ++standart_predicat[My_tree[i][j].predicate[k].number_of_standart_pack][My_tree[i][j].predicate[k].number_of_standart_pred].amt_of_used_pred;
									buf = num + '0';
									for (int atribut = 0; atribut < My_tree[i][j].predicate[k].atribut.size(); atribut++)
									{
										My_tree[i][j].predicate.back().atribut.push_back(predic.atribut[atribut] + buf);
									}
								}
							}
						}
					}
				}
			}
		}
	}
}
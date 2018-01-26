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

TREE::~TREE(){ My_tree.clear(); }

int TREE::insert(size_t number_of_sentence, size_t parent_number, string relations, size_t number_of_word, string Initial_form, string name, vector<string>morfology)
{
	if (My_tree[number_of_sentence].size() == number_of_word)
		sent_data[number_of_sentence].punc = name;
	else
	{

		My_tree[number_of_sentence][number_of_word].morfology = morfology;
		My_tree[number_of_sentence][number_of_word].name = name;
		My_tree[number_of_sentence][number_of_word].number = number_of_word;
		My_tree[number_of_sentence][number_of_word].Initial_form = Initial_form;
		My_tree[number_of_sentence][number_of_word].is_number = false;
		My_tree[number_of_sentence][number_of_word].relation.push_back(relations);
		My_tree[number_of_sentence][number_of_word].root = false;
		My_tree[number_of_sentence][number_of_word].is_pred = false;
		My_tree[number_of_sentence][number_of_word].is_delete = false;
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

void TREE::change_out_words(size_t number_of_sentence, size_t number_of_word)
{
	
	string buf;
	for (unsigned int i = 0; i < My_tree[number_of_sentence][number_of_word].morfology.size(); i++)
	{
		if (My_tree[number_of_sentence][number_of_word].morfology[i] == "m" || My_tree[number_of_sentence][number_of_word].morfology[i] == "f" || My_tree[number_of_sentence][number_of_word].morfology[i] == "n")
		{
			buf = My_tree[number_of_sentence][number_of_word].morfology[i];
			while (number_of_sentence != 0)
			{
				for ( unsigned int j = number_of_word - 1; j >= 0; j--)
				{
					if (My_tree[number_of_sentence][j].morfology[0] == "S")
					{
						for (unsigned int k = 0; k < My_tree[number_of_sentence][j].morfology.size(); k++)
						if (My_tree[number_of_sentence][j].morfology[k] == buf)
						{
							My_tree[number_of_sentence][number_of_word].name = My_tree[number_of_sentence][j].Initial_form;
							My_tree[number_of_sentence][number_of_word].name = My_tree[number_of_sentence][j].Initial_form;
							My_tree[number_of_sentence][number_of_word].morfology[0] = "S";
							return;
						}
					}
				}
				number_of_sentence--;
			}
		}
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
						if (My_tree[i][j].children[n]->morfology[0] == "NUM")
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
	ifstream file, file1;
	int buf_int;
	string buf;
	vector<string> check_words;

	file.open("check_words.txt");
	if (!file)
		cout << "cannot open file: check_words.txt";


	file1.open("standart_predicate.txt");
	if (!file1)
		cout << "cannot open file: standart_predicate.txt";

	while (!file.eof())
	{
		check_words.resize(check_words.size() + 1);
		file >> check_words.back();
	}


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
			if (buf != ")" && buf != "(")
			{

				standart_predicat.back().resize(standart_predicat.back().size() + 1);
				while (buf != "(")
				{
					standart_predicat.back().back().main.push_back(buf);
					file1 >> buf;
				}
			}
			while (buf != ")")
			{
				file1 >> buf;
				standart_predicat.back().back().atribut.push_back(buf);
				standart_predicat.back().back().amt_of_used_pred = 0;
			}
		}

	}

	for (unsigned int i = 0; i < My_tree.size(); i++)
	{
		for (unsigned int j = 0; j < check_words.size(); j++)
		{
			for (unsigned int k = 0; k < My_tree[i].size(); k++)
			{
				if (My_tree[i][k].Initial_form == check_words[j])
				{
					change_out_words(i, k);
				}
			}
		}
	}
	number_converter();
	measure();

	cout << endl;
	cout << "результат первого шага:";
	cout << endl;
	for (unsigned int i = 0; i < My_tree.size(); i++)
	{
		for (unsigned int k = 0; k < My_tree[i].size(); k++)
		{
			if (!My_tree[i][k].is_delete)
			{
				cout << My_tree[i][k].name;


				cout << endl << "                   Морфология: ";

				for (unsigned int j = 0; j < My_tree[i][k].morfology.size(); j++)
					cout << My_tree[i][k].morfology[j] << ",";
				cout << endl;
			}
			
		}
		cout << endl;
	}
	file.close();
	file1.close();
	
}

void TREE::delete_node(size_t number_of_sentence, size_t number_of_word)
{
	
	My_tree[number_of_sentence][number_of_word].is_delete = true;


	for (unsigned int k = 0; k < My_tree[number_of_sentence][number_of_word].parent.size(); k++)
	{
		for (unsigned int j = 0; j < My_tree[number_of_sentence][number_of_word].parent[k]->children.size()-1;j++)
		if (My_tree[number_of_sentence][number_of_word].parent[k]->children[j]->number == number_of_word)
		for (unsigned int i = j + 1; i < My_tree[number_of_sentence][number_of_word].parent[k]->children.size();i++)
			My_tree[number_of_sentence][number_of_word].parent[k]->children[i-1] = My_tree[number_of_sentence][number_of_word].parent[k]->children[i];
		My_tree[number_of_sentence][number_of_word].parent[k]->children.pop_back();
	}

	My_tree[number_of_sentence][number_of_word].parent.clear();

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
	char buf;
	string standart_arg;
	vector<int> dep_word;


	for (unsigned int n = 0; n < My_tree.size(); n++)
	{
		for (unsigned int count = 0; count < My_tree[n].size(); count++)
		{
			for (unsigned int count_p = 0; count_p < standart_predicat[0].size(); count_p++)
			{
				if (My_tree[n][count].Initial_form == standart_predicat[0][count_p].main[0])
				{
					dep_word.push_back(count);

					//если самое главное слово совпало нужно найти остальные составляющие предиката 
					for (unsigned int main_word = 1; main_word < standart_predicat[0][count_p].main.size(); main_word++)
					{

						for (unsigned int child = 0; child < My_tree[n][count].children.size(); child++)
						{
							if (standart_predicat[0][count_p].main[main_word] == My_tree[n][count].children[child]->Initial_form)
							{
								dep_word.push_back(My_tree[n][count].children[child]->number);
							}
						}
					}

					if (dep_word.size() != standart_predicat[0][count_p].main.size())
					{
						dep_word.clear();
					}
					else
					{
						standart_arg = standart_predicat[0][count_p].atribut[0];
						standart_predicat[0][count_p].amt_of_used_pred++;
						buf = '0' + standart_predicat[0][count_p].amt_of_used_pred;
						break;
					}

					//////////////////////////////////////////////////////////////////////////////////
				}

			}



			//множественное число определение
			if (!dep_word.empty())
			{

				for (unsigned int i = 0; i < My_tree[n][count].morfology.size(); i++)
				{
					if (My_tree[n][count].morfology[i] == "Pl")
					{


						// если во множествоенном числе то нужно понять выделять 2 предиката или нет по тому сколько у нас аргументов известно , так как не знаем сколько у нас фигур
						for (unsigned int j = 0; j < My_tree[n][count].children.size(); j++)
						{
							for (unsigned int k = 0; k < My_tree[n][count].children[j]->parent.size(); k++)
							{
								int child = My_tree[n][count].children[j]->number;


								if (My_tree[n][child].relation[k] == "kolichest" && My_tree[n][child].parent[k]->number == count)
								{
									for (unsigned int p = 0; p < My_tree[n][child].children.size(); p++)
									{
										
										
										
										
										
										//////////////////////////////////////
										if ( My_tree[n][child].children[p]->morfology[0] == "NONLEX")
										{
											arg.push_back(My_tree[n][child].children[p]->number);
										}
										///////////////////////////////////





									}
								}
								else
								{
									arg.push_back(-1);// хочу чтобы если не известно сколько фигур чтобы определял множество фигур
								}
							}
						}
						///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					}
					else
					{
						if (My_tree[n][count].morfology[i] == "Sg")
						{
							for (unsigned int i = 0; i < My_tree[n][count].children.size(); i++)
							{
								int child = My_tree[n][count].children[i]->number;




								if (My_tree[n][child].morfology[0] == "NONLEX" )////////////////////////////////////////////////
								{
									arg.push_back(child);
								}





							}
						}
					}
					///////////////////////////////////////////////

					//здесь нужно прописать когда у нас задано число , например :"2 фигуры"
					//else
					//{
					//if
					//}

				}

				My_tree[n][count].is_pred = true;
				My_tree[n][count].name_of_pred = "";
				for (unsigned int j = 0; j < dep_word.size(); j++)
				{
					My_tree[n][count].name_of_pred = My_tree[n][count].name_of_pred + My_tree[n][dep_word[j]].Initial_form + "_";
				}

				if (!arg.empty() && arg[0]!=-1)
				{
					for (unsigned int i = 0; i < arg.size(); i++)
					{
						My_tree[n][count].arg_p.resize(My_tree[n][count].arg_p.size() + 1);
						My_tree[n][count].arg_p.back().push_back(My_tree[n][arg[i]].Initial_form);
					}

				}
				else
				{
					My_tree[n][count].arg_p.resize(My_tree[n][count].arg_p.size() + 1);
					My_tree[n][count].arg_p.back().push_back(standart_arg + "_" + buf);
				}
				My_tree[n][count].parent_predicate = NULL;

				for (unsigned int i = dep_word.size() - 1; i > 0; i--)
				{
					delete_node(n,dep_word[i]);
				}

				for (unsigned int i = 0; i < arg.size(); i++)
				{
					if (arg[0] != -1)
						delete_node(n,arg[i]);
				}
			}
			dep_word.clear();
			arg.clear();
			standart_arg = "";
		}
	}
	cout << endl;
	cout << "результат второго шага:" << endl;
	cout << endl;
	for (unsigned int i = 0; i < My_tree.size(); i++)
	{
		for (unsigned int j = 0; j < My_tree[i].size(); j++)
		{
			if (!My_tree[i][j].is_delete)
			{
				if (My_tree[i][j].is_pred)
					print_predicate(i,j);
				else
					cout << My_tree[i][j].name;
			}
			cout << endl;
		}
	}
	
}

int TREE::find_pred(string word, size_t number_of_sentence,size_t number_of_word)
{
	
	int arg = number_of_word;
	int arg_1 = number_of_word;
	int buf = number_of_word;
	int buf_1 = number_of_word;
	int flag = 1;
	int flag_1 = 0;
	queue <int> used;
	used.push(number_of_word);
	while (!used.empty())
	{
		arg = used.front();
		used.pop();
		for (unsigned int i = 0; i < My_tree[number_of_sentence][arg].children.size(); i++)
		{
			used.push(My_tree[number_of_sentence][arg].children[i]->number);
			if (!My_tree[number_of_sentence][arg].children[i]->is_delete && My_tree[number_of_sentence][arg].children[i]->is_pred == true && My_tree[number_of_sentence][arg].children[i]->name_of_pred == word)
			{
				return (My_tree[number_of_sentence][arg].children[i]->number);
			}
		}

	}
	used.push(number_of_word);
	while (!used.empty())
	{
		arg = used.front();
		used.pop();
		for (unsigned int i = 0; i < My_tree[number_of_sentence][arg].parent.size(); i++)
		{
			used.push(My_tree[number_of_sentence][arg].parent[i]->number);
			if (!My_tree[number_of_sentence][arg].parent[i]->is_delete && My_tree[number_of_sentence][arg].parent[i]->is_pred == true && My_tree[number_of_sentence][arg].parent[i]->name_of_pred == word)
			{
				return My_tree[number_of_sentence][arg].parent[i]->number;
			}
		}

	}
	return -1;
	
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
			if (My_tree[n][i].morfology[0] == "NUM")
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
					////////////предполагаем, что целое//////////////////////


					while (My_tree[n][i].relation[0] == "колич-vspom")
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
					char buf1;
					My_tree[n][i].name.clear();
					while (result != 0)
					{
						
						buf1 = result % 10 + '0';
						result = (result - result % 10) / 10;
						My_tree[n][i].name.resize(My_tree[n][i].name.size() + 1);
						for (unsigned int k = My_tree[n][i].name.size()-1; k >0; k--)
						{
							My_tree[n][i].name[k] = My_tree[n][i].name[k - 1];
						}
						My_tree[n][i].name[0] = buf1;
					}
					    My_tree[n][i].Initial_form = My_tree[n][i].name;
						My_tree[n][i].is_number = true;
						My_tree[n][i].name_number = result;
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

int TREE::find_close_pred(size_t number_of_sentece, size_t number_of_word)
{
	
	string buf = "";
	vector<int>pred;
	int min = -1;
	for (unsigned int i = 0; i < My_tree.size(); i++)
	{

		if (My_tree[number_of_sentece][i].is_pred)
		{
			for (unsigned int j = 0; j < standart_predicat[0].size(); j++)
			{
				for (unsigned int k = 0; k < standart_predicat[0][j].main.size(); k++)
				{
					buf = buf + standart_predicat[0][j].main[k] + "_";
				}
				if (My_tree[number_of_sentece][i].name_of_pred == buf)
				{
					pred.push_back(i);
				}
				buf = "";
			}
		}


	}
	if (!pred.empty())
		min = pred[0];
	for (unsigned int i = 0; i < pred.size(); i++)
	{
		if (abs(min - (int)number_of_word)>abs(pred[i] - (int)number_of_word) && pred[i] != number_of_word)
			min = pred[i];
	}
	pred.clear();
	return min;
	
}

void TREE::standart_predikat_2()
{

	int arg_num = -1;
	string main_word;
	string dep_predict = "";
	string dep_word = "РАВНЫЙ";
	vector<string> arg1;
	vector<string> arg2;
	vector<int> must_delete;
	vector<string> standart_arg;
	char buf;
	int stan_pred;
	for (unsigned int n = 0; n < My_tree.size(); n++)
	{
		for (unsigned int count = 0; count < My_tree.size(); count++)
		{

			for (unsigned int count_p = 0; count_p < standart_predicat[1].size(); count_p++)
			{
				if (My_tree[n][count].Initial_form == standart_predicat[1][count_p].main[0])
				{
					for (unsigned int main_p = 1; main_p < standart_predicat[1][count_p].main.size(); main_p++)
						dep_predict = dep_predict + standart_predicat[1][count_p].main[main_p] + "_";

					if (!dep_predict.empty())
						arg_num = find_pred(dep_predict, n, count);
					else
						arg_num = find_close_pred(n,count);

					if (arg_num != -1)
					{
						main_word = standart_predicat[1][count_p].main[0];
						for (unsigned int i = 0; i < standart_predicat[1][count_p].atribut.size(); i++)
						{
							standart_arg.push_back(standart_predicat[1][count_p].atribut[i]);
						}
						stan_pred = count_p;
						buf = '0' + standart_predicat[1][count_p].amt_of_used_pred;
						break;
					}

					dep_predict = "";
				}

			}

			////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			if (arg_num != -1)
			{


				for (unsigned int i = 0; i < My_tree[n][count].morfology.size(); i++)
				{
					if (My_tree[n][count].morfology[i] == "Pl")
					{


						// если во множествоенном числе то нужно понять выделять 2 предиката или нет по тому сколько у нас аргументов известно , так как не знаем сколько у нас фигур
						for (unsigned int j = 0; j < My_tree[n][count].children.size(); j++)
						{
							for (unsigned int k = 0; k < My_tree[n][count].children[j]->parent.size(); k++)
							{
								int child = My_tree[n][count].children[j]->number;


								if (My_tree[n][child].relation[k] == "kolichest" )
								{
									for (unsigned int p = 0; p < My_tree[n][child].children.size(); p++)
									{
										if (My_tree[n][child].children[p]->morfology[0] == "NONLEX")
										{
											must_delete.push_back(child);
											arg1.push_back(My_tree[n][child].children[p]->Initial_form);
										}

									}
								}
								if (My_tree[n][child].Initial_form == dep_word)
								{
									for (unsigned int p = 0; p < My_tree[n][child].children.size(); p++)
									{
										if (My_tree[n][child].children[p]->relation[k] == "kolichest" && My_tree[n][child].children[p]->parent[k]->number == child)
										{
											for (unsigned int amt = 0; amt < My_tree[n][child].children[p]->children.size(); amt++)
											{
												if ((My_tree[n][child].children[p]->children[amt]->morfology[0] == "NUM" || My_tree[n][child].children[p]->children[amt]->morfology[0] == "NONLEX") )
													arg2.push_back(My_tree[n][child].children[p]->children[amt]->Initial_form);
											}
											must_delete.push_back(child);
										}
										if (arg2.empty() && (My_tree[n][child].children[p]->morfology[0] == "NUM" || My_tree[n][child].children[p]->morfology[0] == "NONLEX") )
											arg2.push_back(My_tree[n][child].children[p]->Initial_form);
									}
								}



							}
						}
						for (unsigned int j = 0; j < My_tree[n][count].parent.size(); j++)
						{
							int parent = My_tree[n][count].parent[j]->number;
							if (My_tree[n][parent].morfology[0] == "NUM" )
							{

								arg2.push_back(My_tree[n][parent].Initial_form);
							}
							if (arg2.empty() && My_tree[n][parent].Initial_form == dep_word)
							{
								for (unsigned int k = 0; k < My_tree[n][parent].children.size(); k++)
								{
									if ((My_tree[n][parent].children[k]->morfology[0] == "NUM" || My_tree[n][parent].children[k]->morfology[0] == "NONLEX"))
									{
										arg2.push_back(My_tree[n][parent].children[k]->Initial_form);

									}
								}
							}

						}
						///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					}
					else
					{
						if (My_tree[n][count].morfology[i] == "Sg")

						{
							for (unsigned int i = 0; i < My_tree[n][count].children.size(); i++)
							{
								int child = My_tree[n][count].children[i]->number;
								if (My_tree[n][child].morfology[0] == "NONLEX")
								{
									must_delete.push_back(child);
									arg1.push_back(My_tree[n][child].Initial_form);
								}
								if (My_tree[n][child].morfology[0] == "NUM" )
								{
									must_delete.push_back(child);
									arg2.push_back(My_tree[n][child].Initial_form);
								}
								if (arg2.empty() && My_tree[n][child].Initial_form == dep_word)
								{
									must_delete.push_back(child);
									for (unsigned int j = 0; j < My_tree[n][child].children.size(); j++)
									{
										

										if ((My_tree[n][child].children[j]->morfology[0] == "NUM" || My_tree[n][child].children[j]->morfology[0] == "NONLEX") )
										{
											arg2.push_back(My_tree[n][child].children[j]->Initial_form);
										}
									}
								}


							}
							for (unsigned int i = 0; i < My_tree[n][count].parent.size(); i++)
							{
								int parent = My_tree[n][count].parent[i]->number;
								if (My_tree[n][parent].morfology[0] == "NUM" )
								{

									arg2.push_back(My_tree[n][parent].Initial_form);
								}
								if (arg2.empty() && My_tree[n][parent].Initial_form == dep_word)
								{

									for (unsigned int j = 0; j < My_tree[n][parent].children.size(); j++)
									{
										if ((My_tree[n][parent].children[j]->morfology[0] == "NUM" || My_tree[n][parent].children[j]->morfology[0] == "NONLEX"))
										{
											arg2.push_back(My_tree[n][parent].children[j]->Initial_form);

										}
									}
								}

							}
						}
					}
					/*

					здесь нужно прописать когда у нас задано число , например :"2 фигуры"
					else
					{
					if
					}*/

				}

				My_tree[n][count].is_pred = true;
				if (arg_num != -1)
				{



					unsigned int i;
					My_tree[n][count].name_of_pred = My_tree[n][count].name_of_pred + main_word + "_" + My_tree[n][arg_num].name_of_pred;

					for (i = 0; i < arg2.size(); i++)
					{

						My_tree[n][count].arg_p.resize(My_tree[n][count].arg_p.size() + 1);

						if (i < arg1.size())
						{
							My_tree[n][count].arg_p.back().push_back(arg1[i]);
						}
						else
						{
							My_tree[n][count].arg_p.back().push_back(standart_arg[0] + "_" + buf);
							buf++;
							standart_predicat[1][stan_pred].amt_of_used_pred++;
						}
						My_tree[n][count].arg_p.back().push_back(My_tree[n][arg_num].arg_p[0][0]);
						My_tree[n][count].arg_p.back().push_back(arg2[i]);


					}
					if (!arg1.empty())
					{
						for (unsigned int j = i; j < arg1.size(); j++)
						{
							My_tree[n][count].arg_p.resize(My_tree[n][count].arg_p.size() + 1);
							My_tree[n][count].arg_p.back().push_back(arg1[j]);
							My_tree[n][count].arg_p.back().push_back(My_tree[n][arg_num].arg_p[0][0]);
							My_tree[n][count].arg_p.back().push_back(standart_arg[2] + "_" + buf);
							buf++;
							standart_predicat[1][stan_pred].amt_of_used_pred++;
						}
					}

					if (arg1.empty() && arg2.empty())
					{
						My_tree[n][count].arg_p.resize(My_tree[n][count].arg_p.size() + 1);
						My_tree[n][count].arg_p.back().push_back(standart_arg[0] + "_" + buf);
						My_tree[n][count].arg_p.back().push_back(My_tree[n][arg_num].arg_p[0][0]);
						My_tree[n][count].arg_p.back().push_back(standart_arg[2] + "_" + buf);
						buf++;
						standart_predicat[1][stan_pred].amt_of_used_pred++;
					}





				}
				for ( int i = must_delete.size() - 1; i >= 0; i--)
				{
					delete_node(n,must_delete[i]);
				}


			}

			arg1.clear();
			standart_arg.clear();
			arg2.clear();
			arg_num = -1;
			main_word.clear();
			must_delete.clear();
			dep_predict.clear();
		}
	}

}

void TREE::print_predicate(size_t number_of_sentence, size_t number_of_word)
{
	
	unsigned int i = 0;
	for (i = 0; i < My_tree[number_of_sentence][number_of_word].arg_p.size(); i++)
	{
		cout << My_tree[number_of_sentence][number_of_word].name_of_pred + "( ";
		for (unsigned int j = 0; j < My_tree[number_of_sentence][number_of_word].arg_p[i].size(); j++)
		{
			if (j != 0)
				cout << ",";
			cout << My_tree[number_of_sentence][number_of_word].arg_p[i][j];
		}
		cout << ");";
	}
	if (i == 0)
		cout << My_tree[number_of_sentence][number_of_word].name_of_pred;
	cout << endl;
	
}

void TREE::list()
{
	
	ifstream file, file1;
	
	string buf;
	vector<string> data_words;
	vector<string> unknown_words;
	file.open("data.txt");
	if (!file)
		cout << "cannot open file: check_words.txt";


	file1.open("unknown.txt");
	if (!file1)
		cout << "cannot open file: standart_predicate.txt";

	while (!file.eof())
	{
		data_words.resize(data_words.size() + 1);
		file >> data_words.back();
	}


	while (!file1.eof())
	{
		unknown_words.resize(unknown_words.size() + 1);
		file1 >> unknown_words.back();
	}
	for (unsigned int k = 0; k < My_tree.size(); k++)
	{
		for (unsigned int i = 0; i < My_tree[k].size(); i++)
		{
			for (unsigned int j = 0; j < data_words.size(); j++)
			{
				if (data_words[j] == My_tree[k][i].Initial_form)
				{
					My_tree[k][i].is_pred = true;
					My_tree[k][i].name_of_pred = "";
					My_tree[k][i].name_of_pred = My_tree[k][i].name_of_pred + "ДАНО" + ":";
					My_tree[k][i].parent_predicate = NULL;
				}
			}
		}
	}
	for (unsigned int n = 0; n < My_tree.size(); n++)
	{
		for (unsigned int i = 0; i < My_tree[n].size(); i++)
		{
			for (unsigned int j = i; j < My_tree[n].size(); j++)
			{
				if (My_tree[n][i].is_pred && My_tree[n][j].is_pred)
				{
					if (My_tree[n][i].arg_p[0][0] == My_tree[n][j].arg_p[0][0] && My_tree[n][i].name_of_pred == My_tree[n][j].name_of_pred)
					{
						for (unsigned int k = 0; k < My_tree[n][j].arg_p.size(); k++)
						{
							for (unsigned int p = 0; p < My_tree[n][j].arg_p[k].size(); p++)
							{
								My_tree[n][j].arg_p[k][p] = My_tree[n][i].arg_p[k][p];
							}
						}
					}
				}
			}
		}
	}
	for (unsigned int k = 0; k < My_tree.size(); k++)
	{
		for (unsigned int i = 0; i < My_tree[k].size(); i++)
		{
			for (unsigned int j = 0; j < unknown_words.size(); j++)
			{
				if (unknown_words[j] == My_tree[k][i].Initial_form)
				{
					My_tree[k][i].is_pred = true;
					My_tree[k][i].name_of_pred = "";
					My_tree[k][i].name_of_pred = My_tree[k][i].name_of_pred + "НАЙТИ" + ":";
					My_tree[k][i].parent_predicate = NULL;
				}
			}
		}
	}
	cout << endl;
	cout << "результат третьего шага:" << endl;
	cout << endl;
	
	for (unsigned int i = 0; i < My_tree.size(); i++)
	{
		for (unsigned j = 0; j < My_tree[i].size();j++)
		
		if (My_tree[i][j].is_pred)
			print_predicate(i,j);
		

	}
	file.close();
	file1.close();
	
}


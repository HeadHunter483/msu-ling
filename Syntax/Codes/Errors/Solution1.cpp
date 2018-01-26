#pragma warning(disable:4996)
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <stdio.h>
#include "Transliter.h"
#include "utilities.h"
#include "Tester.h"
#include "order.h"

using namespace std;

int global_counter = 0;
int predicate_link = 0; //  номер сказуемого(корня)
int pr_link = 0; // связь с предлогом 
int words_until_comma = 0;//// считаем количество элементов до запятой
int LINES_COUNT = 0;// число токенов в предложении
int subj = 0;// хранит номер подлежащего для случаев,когда сказуемое не может быть корнем,потому что его нет

string str5;// строковое представление номера корня
string pr;

void numbers(vector<Conll_line>&conll){
	size_t t = 0;
	size_t begin = 0;
	size_t end = 0;
	string line3;
	string line4;

	size_t i2 = 0;
	string line2;
	string math_oper[10] = { "'","+", "=", "*", "/", "-","<",">","<=",">=" };

	int mode = 0;

	for (size_t v = 1; v < conll.size() - 1; v++) {
		if (conll[v + 1].parts_of_speech == "NUM" && conll[v - 1].parts_of_speech == "NUM") {
			mode = 1;
		}
		else {
			if (conll[v + 1].parts_of_speech == "NUM" && conll[v - 1].parts_of_speech != "NUM") {
				mode = 2;
			}

			if (conll[v - 1].parts_of_speech == "NUM" && conll[v + 1].parts_of_speech != "NUM") {
				mode = 3;
			}
		}

		for (size_t v2 = 0; v2 < 10; v2++) {
			if (conll[v].word.find(math_oper[v2]) != string::npos) {
				if (mode == 1) {
					conll[v - 1].word = conll[v - 1].word + math_oper[v2] + conll[v + 1].word;
					conll[v - 1].parts_of_speech = "NUM";
					change_id_and_number(conll, v + 1);
					conll.erase(conll.begin() + v + 1);
					change_id_and_number(conll, v);
					conll.erase(conll.begin() + v);
					v2 = 0;
				}

				if (mode == 2) {
					conll[v - 1].word = conll[v - 1].word + math_oper[v2] + conll[v + 1].word;
					conll[v - 1].parts_of_speech = "NUM";
					change_id_and_number(conll,v+1);
					conll.erase(conll.begin() + v + 1);
					change_id_and_number(conll, v);
					conll.erase(conll.begin() + v);
					v2 = 0;
				}

				if (mode == 3) {
					conll[v - 1].word = math_oper[v2] + conll[v - 1].word;
					conll[v - 1].parts_of_speech = "NUM";
					change_id_and_number(conll, v + 1);
					conll.erase(conll.begin() + v + 1);
					change_id_and_number(conll, v);
					conll.erase(conll.begin() + v);
					v2 = 0;
				}
			}
		}
	}
}
void brackets_(vector<Conll_line>&conll){
	int it_was_already = 0;
	int amount_of_words_until_bracket = 0;

	int a = 0;
	int b = 0;
	int beg = 0;
	int mod = 0;

	int was_already = 0;
	int open = 0;
	int close = 0;

	int end_start = 0;


	int counter = 0;

	int now = 0;
	int now2 = 0;

	int check = 0;
	int error_mode = 0;
	size_t begin= 0;
	size_t end = 0;
	string line3;

	for (size_t w = 0; w < conll.size() - 1; w++) {
		if (conll[w].word == "(") {
			check++;
		}

		if (conll[w].word == ")") {
			check--;
		}

		if (check > 0) {
			error_mode = 1;
		}

		if (check < 0) {
			error_mode = 1;
		}

		if (check == 0) {
			error_mode = 0;
		}
	}



	if (error_mode != 1) {
		for (size_t i = 0; i < conll.size(); i++) {
			if (conll[i].word == "(") {
				for (size_t r = i + 1; r < conll.size(); r++) {
					if (conll[r].word.find("(") != string::npos) {
						now = r;
					}
					now2 = i;
				}


				begin = now2;
				while (begin < conll.size()) {
					if (conll[begin].word == ")") {
						end = begin;
						counter = counter + 1;
						if (counter == findsomebrack(conll, now2)) {
							break;
						}
					}
					begin = begin + 1;
				}

				while ((now2 + amount_of_words_until_bracket) != end + 1) {
					line3 = line3 + conll[now2 + amount_of_words_until_bracket].word;
					++amount_of_words_until_bracket;
					conll[now2].word = line3;
				}

				for (int i = 0; i < amount_of_words_until_bracket - 1; i++){
					change_id_and_number(conll, now2 + 1);
					conll.erase(conll.begin() + now2 + 1);
				}

				now2 = 0;
				now = 0;
				end = 0;
				begin = 0;
				amount_of_words_until_bracket = 0;
				line3 = "";
				counter = 0;
			}
		}

	}
}
void combine_(vector<Conll_line>&conll, int LINES_COUNT, string line) {
	int  find_bracket = 0, i2 = 0, amount_of_words_until_bracket = 0, n, where_noun_after_num = 0, flag = 0, amount_of_delets = 0, skolko_udalit = 0, counter_of_parts = 0;
	string what_is_part_of_speach = "";
	string line_of_dict;
	string newline1;
	string separ = "\t";
	size_t i1 = 0;
	ifstream dict("dict.txt");
	ifstream prepad("predlogipadeji.txt");
	ifstream mathworddic("mathword.txt");
	vector <dictionary> diction;
	vector <predlogipadeji> prpad;
	vector <mathword> mthword;
	string math_oper[11] = { "+", "=", "*", "/", "-","<",">","<=",">=",")","(" };

	/* Cчитываем данные из словаря 1 */
	dict >> n;
	dict.ignore(255, '\n');
	diction.resize(n);
	while (getline(dict, line_of_dict)) {
		for (size_t i3 = 0; i3 < line_of_dict.size(); i3++) {
			if (line_of_dict[i3] == ' ') {
				counter_of_parts++; // считаем число пробелов между словами - получаем число слов в строке словаря.
			}
		}
		size_t pos = line_of_dict.find(separ); // разбиваем строку словаря на сам оборот и часть речи
		diction[i1].words = line_of_dict.substr(0, pos);
		diction[i1].parts_of_speach = line_of_dict.substr(pos + separ.length());
		diction[i1].amount_of_words = to_string(counter_of_parts + 1);
		counter_of_parts = 0;
		++i1;
	}
	dict.close();

	/* Считываем из словаря 2*/
	n = 0;
	i1 = 0;
	prepad >> n;
	prepad.ignore(255, '\n');
	prpad.resize(n);

	while (getline(prepad, line_of_dict)) {
		size_t pos = line_of_dict.find(separ);
		prpad[i1].predlog = line_of_dict.substr(0, pos);
		newline1 = line_of_dict.substr(pos + separ.length());
		size_t pos2 = newline1.find(separ);
		prpad[i1].padej1 = newline1.substr(0, pos2);
		prpad[i1].padej2 = newline1.substr(pos2 + separ.length());
		i1++;
	}
	prepad.close();

	string line20;
	string line21;
	string line22;
	string line23;
	string line24;
	string line26;

	n = 0;
	i1 = 0;
	line_of_dict = "";
	mathworddic >> n;
	mathworddic.ignore(255, '\n');
	mthword.resize(n);
	while (getline(mathworddic, line_of_dict)) {
		size_t pos34 = line_of_dict.find("\t"); // разбиваем строку словаря на сам оборот и часть речи
		mthword[i1].word = line_of_dict.substr(0, pos34);
		line20 = line_of_dict.substr(pos34 + separ.length());

		size_t pos20 = line20.find(separ);
		mthword[i1].type = line20.substr(0, pos20);
		line21 = line20.substr(pos20 + separ.length());

		size_t pos30 = line21.find(separ);
		mthword[i1].rod = line21.substr(0, pos30);
		line22 = line21.substr(pos30 + separ.length());
		
		size_t pos40 = line22.find(separ);
		mthword[i1].atrib1 = line22.substr(0, pos40);
		line23 = line22.substr(pos40 + separ.length());
		
		size_t pos50 = line23.find(separ);
		mthword[i1].atrib2 = line23.substr(0, pos50);
		mthword[i1].atrib3 = line23.substr(pos50 + separ.length());

		i1++;
	}
	mathworddic.close();

	//for (size_t i = 0; i < mthword.size(); i++){
		//cout << mthword[i].word << "|" << mthword[i].type << "|" << mthword[i].rod << "|" << mthword[i].atrib1 << "|" << mthword[i].atrib2 << "|" << mthword[i].atrib3 << endl;
	//}

/* Сравниваем слова с выражениями из словаря */
int schetchik = 0;
for (size_t i = 0; i < diction.size(); i++) { // если в словаре есть две (вроде должно хватить) соседние строки из
	// слов таблицы то объединяем эти слова в одну строку таблицы.
	for (size_t v = 0; v < conll.size() - schetchik - 1; ++v) {
		if (conll[v].word.length() > 1) { // ставим ограничение на длину слова для однобуквенных слов
			if (diction[i].amount_of_words == to_string(1) && (diction[i].words.find(conll[v].word) != string::npos)) {
				conll[v].word = diction[i].words;
				conll[v].original_form = diction[i].words;
				conll[v].parts_of_speech = diction[i].parts_of_speach;
				conll[v].morph1 = "_";
				conll[v].morph2 = "_";
				conll[v].type_of_link = "atrib";
			}
			else {
				if ((diction[i].words.find(conll[v].word + " " + conll[v + 1].word) != string::npos)) {
					conll[v].word = diction[i].words;
					conll[v].original_form = diction[i].words;
					conll[v].parts_of_speech = diction[i].parts_of_speach;
					conll[v].morph1 = "_";
					conll[v].morph2 = "_";
					skolko_udalit = atoi(diction[i].amount_of_words.c_str());
					where_noun_after_num = v;
					flag = 1;
					schetchik = schetchik + 1;
				}
			}
		}
	}
	if (flag == 1) {
		for (int i = 0; i < skolko_udalit - 1; i++) {
			if (skolko_udalit == 2) {
				change_id_and_number(conll, where_noun_after_num + 1);
				conll.erase(conll.begin() + where_noun_after_num + 1);
			}
			if (skolko_udalit == 3) {
				change_id_and_number(conll, where_noun_after_num);
				conll.erase(conll.begin() + where_noun_after_num);
			}
		}
		where_noun_after_num = 0;
		flag = 0;
	}
}
















/* Соединяем сложное сказуемое с начальной формой быть ..... */
for (size_t i = 0; i < conll.size(); i++) {
	if (conll[i].parts_of_speech == "V" && conll[i].original_form.find("byitmz") != string::npos) {
		//cout << "here" << endl;
		for (size_t i2 = i; i2 < conll.size(); i2++) {
			if (conll[i2].parts_of_speech == "PARTCP") {
				//conll[i].parent = conll[i2].parent;
				//conll[i].type_of_link = conll[i2].type_of_link;
				conll[i].morph1 = conll[i2].morph1;
				conll[i].morph2 = transliter(2,conll[i].word + " " + conll[i2].word);
				break;
			}
		}
	}
}


/*Объединяем числительное и существительное(местоимение)в родительном падеже*/
for (size_t i = 0; i < conll.size(); i++) {
	if (conll[i].parts_of_speech == "NUM") {
		for (size_t i2 = i; i2 < conll.size(); i2++) {
			if ((conll[i2].parts_of_speech == "S" || conll[i2].parts_of_speech == "SPRO") &&
				conll[i2].morph1.find("gen") != string::npos || conll[i2].morph1.find("nonflex") != string::npos) {
				where_noun_after_num = i2 + 1;
				flag = 1;
				amount_of_delets = amount_of_delets + 1;
				if (conll[i2].parts_of_speech == "s") {
					what_is_part_of_speach = "s";
				}
				else {
					what_is_part_of_speach = "s";
				}
				conll[i].morph2 = conll[i].word + " " + conll[i2].word;
				break;
			}
		}
	}
}

for (size_t i = 1; i < conll.size(); i++){
	if (conll[i].parts_of_speech == "abr") {
		conll[i-1].morph2=transliter(2,conll[i-1].word) + " "+ conll[i].word;
	}
}

/*________________________________________________________________________________*/

string measure[7] = { "'", "sm", "mm","dm","m","km","gradus"};
int where = 0;
	for (size_t i = 0; i < conll.size(); i++){
		for (size_t j = 0; j < 7; j++){
			if (conll[i].original_form == measure[j]) {
				flag = 1;
				where = i;
				conll[i - 1].word = conll[i - 1].word + " " + conll[i].word;
			}
		}
	}

	if (flag == 1){
		change_id_and_number(conll, where);
		conll.erase(conll.begin() + where);
		flag = 0;
	}
	
	/*_______________________________________________________________*/

	for (size_t i = 0; i < conll.size(); i++) {
		for (size_t j = 0; j < 6; j++) {
			if (conll[i].morph2.find(measure[j])!=string::npos) {
				//conll[atoi(line.c_str())].morph2 == conll[atoi(line.c_str())].word + " " + conll[i].morph2;
			}
		}
	}

	



	/*_______________________________________________________________*/

	for (size_t i = 0; i < conll.size(); i++){
		if (conll[i].parts_of_speech == "PR"){ // если встретили предлог
			for (size_t i1 = i; i1 < conll.size(); i1++){
				for (size_t i2 = 0; i2 < prpad.size(); i2++){
					//// ищем в словаре этот предлог.затем ищем сущ или местоимение в падеже соотв. данному предлогу
					if (prpad[i2].predlog.find(conll[i].word) != string::npos &&
						(conll[i1].parts_of_speech == "S" || conll[i1].parts_of_speech == "SPRO") &&
						(conll[i1].morph1.find(prpad[i2].padej1) != string::npos
						|| conll[i1].morph1.find(prpad[i2].padej2) != string::npos)){

						if (conll[i1 + 1].parts_of_speech == "CONJ" || conll[i1].word==","){ //если след.союз
							for (size_t i3 = i1+1; i3 < conll.size(); i3++) { // ищем сущ с соот падежом без предлога
								if ((conll[i3].parts_of_speech == "S" || conll[i3].parts_of_speech == "SPRO") &&
									(conll[i3].morph1.find(prpad[i2].padej1) != string::npos
										|| conll[i3].morph1.find(prpad[i2].padej2) != string::npos)) {
									conll[i3].morph2 = "("+conll[i].word + ")"+" "+conll[i3].word;

								}
							}

						}
						

						conll[i].morph2 = transliter(2,conll[i].word + " " + conll[i1].word);
						break;
					}
				}
			}
		}
			
	}

	/*for (size_t v = 0; v < conll.size(); v++) {
	if (!is_number(conll[v].word) && conll[v].parts_of_speech == "NUM") {
	if (it_was_already == 0) { // если
	where_noun_after_num = v;
	it_was_already = 1;
	}
	v5 = v;
	while (conll[v5].parts_of_speech == "NUM") {
	newline2 = newline2 + conll[v5].word + " ";
	newline3 = newline3 + conll[v5].original_form + " ";
	flag = 1;
	conll[v].word = newline2;
	conll[v].original_form = newline3;
	v5 = v5 + 1;
	}


	schetchik2 = schetchik2 + 1;
	}
	}

	if (flag == 1) {
	for (int i = 0; i < schetchik2 - 1; i++) {
	change_id_and_number(conll, where_noun_after_num + 1);
	conll.erase(conll.begin() + where_noun_after_num + 1);
	}
	where_noun_after_num = 0;
	flag = 0;
	}
	*/
}

string find_the_predicate(vector<Conll_line>&conll){

	/// Разбиваем предложение на части.Части отделяются запятыми.

	for (size_t i = 0; i < conll.size(); i++){
		if (conll[i].word != ","){
			words_until_comma++;
		}

		if (conll[i].word == ","){ // проверяем падеж
			if (conll[i - 1].morph1 == conll[i + 1].morph1){ // если запятая отделяет однородные слова.
				words_until_comma = words_until_comma + 1;
			}
			else break;

		}
	}


	///////////        Ищем корень       //////////

	for (size_t i = 0; i < words_until_comma; i++) {

		if (conll[i].parts_of_speech == "V") {
			predicate_link = i + 1;
			conll[i].type_of_link = "root";
			conll[i].parent = "0";
			str5 = to_string(predicate_link);

		}
		else {
			string str3 = "brev"; // краткая форма прилагательного или причастия.встречается в "равны,эквивалентны" и тд
			if (conll[i].parts_of_speech == "A" && conll[i].morph1.find(str3) != string::npos) {
				predicate_link = i + 1;
				conll[i].type_of_link = "root";
				conll[i].parent = "0";
				str5 = to_string(predicate_link);
			}

			if (conll[i].parts_of_speech == "PARTCP" && conll[i].morph1.find(str3) != string::npos) {
				predicate_link = i + 1;
				conll[i].type_of_link = "root";
				conll[i].parent = "0";
				str5 = to_string(predicate_link);
			}
		}
	}
	return str5;
}

void special_words_and_idioms(vector<Conll_line>&conll){

	string words[8] = { "pryamaya", "pryamie", "kasatelnaya", "kasatelnie", "sekushaya", "sekushie", "kriviye", "krivaya" };

	//// mystem в большинстве случаев слова типа  прямая,касательная и секущая определяет как прилагательные

	for (size_t i = 0; i < conll.size(); i++){

		string letters[4] = { "alpha", "beta", "gamma", "delta" }; // обозначаем буквы как атрибуты предыдущего слова
		//  угол альфа.Так есть проблемы с кодировкой,
		//   то заменяем на латинские эквиваленты
		for (int h = 0; h < 4; h++){

			if (conll[i].word.find(letters[h]) != string::npos) //начальная форма 
			{
				conll[i].parent = to_string(i);
				conll[i].type_of_link = "atrib";
				conll[i].parts_of_speech = "NONLEX";
			}
		}

		////// Слово прямой нужно оставить прилагательным если после него идет слово угол

		for (size_t i = 0; i < conll.size(); i++){
			for (int g = 0; g <= 8; g++){
				if (conll[i].word.find(words[g]) != string::npos)
				{
					conll[i].parts_of_speech = "S";
				}

				if (conll[i].word == "pryamoy" && conll[i + 1].word == "ugol"){ //если первое слово справа или первое слово до запятой слева - сущ, то "прямой" - прилагательное. возможно можно применить для всех прилагательных
					conll[i].parts_of_speech = "A";
				}

				if (conll[i].word == "pryamie" && conll[i + 1].word == "ugli"){// не нужно, необходимо проверять начальную форму
					conll[i].parts_of_speech = "A";
				}
			}
		}
	}
}

void find_the_subject(vector<Conll_line>&conll, string &predicate_link){

	string str9 = "Acc";// винительный падеж
	string str10 = "gEN";// родительный падеж

	/// У треугольника три стороны. Mystem определяет стороны как существительное в родительном падеже.


	for (size_t i = 0; i < words_until_comma; i++) {
		string str4 = "NOM";// именительный падеж.Если существительное или местоимение в им.падеже - то это подлежащее.

		if ((conll[i].parts_of_speech == "S" || conll[i].parts_of_speech == "SPRO") && (conll[i].morph1.find(str4) != string::npos))  {
			conll[i].type_of_link = "Subj-Predic";// тип связи подлежащее.
			conll[i].parent = str5;
			subj = i + 1;

		}

		else {

			/// В некоторых случаях опираемся на исходную связь для определения подлежащего

			if ((conll[i].type_of_link == "predic" && (conll[i].parts_of_speech == "S"
				|| conll[i].parts_of_speech == "SPRO"))){
				conll[i].type_of_link = "Subj-Predic";
				conll[i].parent = str5;
				subj = i + 1;
			}

		}
	}


	///// Если нет глагола причастия или краткого прилагательного.То сущ или местоимение в им.падеже.- корень



	int count = 0;/// считаем,что если прошли все слова и среди них нет тех кто может быть корнем.

	for (size_t i = 0; i < words_until_comma; i++){
		if (conll[i].parts_of_speech != "V" && conll[i].parts_of_speech != "A"
			&& conll[i].parts_of_speech != "PARTCP" && conll[i].morph1.find("BrEV") == string::npos){
			count++;
		}
	}

	if (count == words_until_comma){
		for (size_t t = 0; t < words_until_comma; t++){
			if ((conll[t].parts_of_speech == "S" || conll[t].parts_of_speech == "SPRO")
				&& conll[t].morph1.find("NOM") != string::npos){

				conll[t].parent = "0";
				conll[t].type_of_link = "root";

				predicate_link = t + 1;
				subj = t + 1;
			}
		}
	}
}
void addit_subj(vector<Conll_line>&conll){

	int noun_ins = 0;

	string str9 = "iNS";
	string str10 = "gEN";


	//Найти периметр треугольника ABC c длиной(Тв.падеж) стороны 5 см.

	// Ищем сушествительное или местоимение в тв.падеж.

	for (size_t i = words_until_comma - 1; i >0; i--) {
		if ((conll[i].parts_of_speech == "S" || conll[i].parts_of_speech == "SPRO") && conll[i].morph1.find(str9) != string::npos){
			noun_ins = i + 1;
		}
	}

	/// У треугольника есть три стороны.

	for (size_t i = 0; i < words_until_comma; i++) {
		if (conll[i].parts_of_speech == "NUM" && conll[i + 1].parts_of_speech == "S"){
			if (conll[i + 1].type_of_link.find("predic") != string::npos
				|| conll[i + 1].morph1.find(str10) != string::npos){
				conll[i + 1].type_of_link = "Subj-Predic";
				conll[i + 1].parent = to_string(predicate_link);
				subj = i + 2;
			}

			else {
				if (noun_ins != 0){
					conll[i + 1].type_of_link = "kompl-appoz";
					conll[i + 1].parent = to_string(noun_ins);
				}
				if (noun_ins == 0){
					conll[i + 1].type_of_link = "kompl-appoz";
					conll[i + 1].parent = to_string(predicate_link);
				}
			}
		}

	}


	int noun_pr;

	for (size_t i = 0; i < words_until_comma; i++) {
		int verb_noun = 0;
		noun_pr = 0;
		string str4 = "Acc";
		string str5 = "NOM";

		//// В некоторых случаях mystem путает винительный и именительный падежи.У винительного падежа может быть предлог

		if (conll[i].parts_of_speech == "S" && conll[i].morph1.find(str4) != string::npos){
			int g = i;
			noun_pr = 0;

			for (g = i; g > 0; g--){
				if (conll[g - 1].parts_of_speech == "PR"){
					pr = to_string(g);
					pr_link = g;
					noun_pr++;
				}
			}
		}




		if (noun_pr == 0){
			if (conll[i].parts_of_speech == "S" && conll[i].morph1.find(str4) != string::npos
				&& conll[i].morph1.find(str5) == string::npos){
				conll[i].type_of_link = "Subj-Predic";
				conll[i].parent = to_string(predicate_link);
				subj = i + 1;

			}


		}

		if (noun_pr == 1){
			if ((conll[i].parts_of_speech == "S" || conll[i].parts_of_speech == "SPRO") && conll[i].morph1.find(str4) != string::npos
				&& conll[i].morph1.find(str5) == string::npos){
				conll[i].type_of_link = "obst";
				conll[pr_link - 1].parent = to_string(predicate_link);
				conll[pr_link - 1].type_of_link = "obst";
			}


		}


	}

}
void join_to_neighbour(vector<Conll_line>&conll){

	string str8 = "NONflEx";
	string par1, par2;
	string numbers[2] = { "Pl", "Sg" };

	for (size_t i = 0; i < words_until_comma; i++){

		///  Сторона равна 9 см.

		if (conll[i].parts_of_speech == "NUM" && (conll[i + 1].parts_of_speech == "S"
			&& conll[i + 1].morph1.find(str8) != string::npos))  {
			conll[i + 1].type_of_link = "1-kompl";
			conll[i + 1].parent = to_string(i);
			conll[i].parent = to_string(i + 2);
		}

	}

	// Латинский набор букв привязываем к предыдущему слову

	for (size_t i = 0; i < words_until_comma; i++) {
		if ((conll[i].parts_of_speech == "NONLEX")){
			conll[i].type_of_link = "atrib";
			conll[i].parent = to_string(i);
		}
	}




	//// Запятую соединяем с предыдущим словом.

	for (size_t i = 0; i < words_until_comma; i++) {
		if (conll[i].parts_of_speech == ","){
			conll[i].parent = to_string(i);
		}

	}


	// Если после запятой идет сущ,местоимение или латинские буквы.На данный момент предполагаем что это однородные

	// Рисовал круги,квадраты


	for (size_t i = 0; i < words_until_comma; i++) {
		if (conll[i].parts_of_speech == "," && (conll[i + 1].parts_of_speech == "S"
			|| conll[i + 1].parts_of_speech == "SPRO" || conll[i + 1].parts_of_speech == "NONLEX")){
			conll[i + 1].parent = conll[i - 1].parent;
		}
	}


	for (size_t i = 0; i < words_until_comma; i++) {
		if (conll[i].parts_of_speech == "CONJ"){

			conll[i].parent = to_string(i);// соединяем союз с предыдущим словом

			/// если соседи союза латинские буквы то определить их как однородные.Отрезки A и B

			if (conll[i - 1].parts_of_speech == "NONLEX" && conll[i + 1].parts_of_speech == "NONLEX"){
				conll[i + 1].type_of_link = conll[i - 1].type_of_link;
				conll[i + 1].parent = conll[i - 1].parent;

			}

			/*for (int j = i; j < words_until_comma; j++){
			if (conll[j].parts_of_speech == conll[i - 1].parts_of_speech){
			conll[j].parent = conll[i - 1].parent;
			conll[j].type_of_link = conll[i-1].type_of_link;

			}
			}*/



			//// Если союз соединяет две части речи с одинаковым число,то вероятнее всего это однородные члены.

			for (int j = 0; j < 2; j++){

				if (conll[i - 1].morph1.find(numbers[j]) != string::npos){
					par1 = numbers[j];
				}

				if (conll[i + 1].morph1.find(numbers[j]) != string::npos){
					par2 = numbers[j];
				}

				if (par1 == par2){
					conll[i + 1].type_of_link = conll[i - 1].type_of_link;
					conll[i + 1].parent = conll[i - 1].parent;

				}

			}

		}


	}


	for (size_t i = 0; i < words_until_comma; i++){
		if ((conll[i].parts_of_speech == "S" || conll[i].parts_of_speech == "SPRO")
			&& conll[i + 1].parts_of_speech == "NONLEX"){
			conll[i + 1].parent = to_string(i + 1);
		}
	}

	/// Вокруг треугольника описана окружность

	for (size_t i = 0; i < words_until_comma; i++) {
		if ((conll[i].parts_of_speech == "S" && conll[i].morph1.find("gEN") != string::npos)){
			for (int f = i - 1; f >= 0; f--){

				if (conll[f].parts_of_speech == "PR"){
					conll[i].parent = to_string(f + 2);
					break;
				}


				if (conll[f].parts_of_speech == "S" || conll[f].parts_of_speech == "SPRO"){
					conll[i].parent = to_string(f + 1);
					conll[i].type_of_link = "kvaziagent";
					break;
				}

			}
		}
	}

	int pr_count = 0;

	for (size_t i = 0; i < words_until_comma; i++){
		if (conll[i].parts_of_speech == "PR"){
			pr_count = i + 1;
			break;
		}
	}

	for (size_t i = 0; i < words_until_comma; i++) {
		if (conll[i].parts_of_speech == "S" && conll[i].morph1.find("lOc") != string::npos){
			for (int g = i; g >= pr_count; g--){
				{
					conll[i].parent = to_string(g);
					conll[i].type_of_link = "predl";
					break;
				}
			}
		}
	}
}

void partcp_parsing(vector<Conll_line>&conll){

	/// связываем главное слово и глагол в причастном обороте

	if (words_until_comma > 1){
		for (size_t j = words_until_comma; j < conll.size(); j++){
			if (conll[j].parts_of_speech == "A" || conll[j].parts_of_speech == "PARTCP"){
				for (size_t h = j; h < conll.size(); h++){

					if (conll[h].parts_of_speech == "V" || (conll[h].parts_of_speech == "A"
						&& conll[h].morph1.find("BrEV") != string::npos)) {

						conll[h].parent = to_string(j);
						conll[h].type_of_link = "predic";

					}

				}


			}

		}


	}

}
void unknown_word(vector<Conll_line>&conll){

	//// слова,которые не были отнесены ни в  одну из категорий.Смотрим по части речи соседа слева.

	for (size_t i = 1; i < conll.size(); i++) {

		if (conll[i].type_of_link == "rOOt") {
			conll[i].parent = to_string(i);

			if (conll[i - 1].parts_of_speech == "PR"){
				conll[i].type_of_link = "predl";
			}

			if (conll[i - 1].parts_of_speech == "NUM"){

				conll[i].type_of_link = "kolich";
			}

			if (conll[i - 1].parts_of_speech == "A"){
				conll[i].type_of_link = "opred";
			}

			if (conll[i - 1].parts_of_speech == "S" || conll[i - 1].parts_of_speech == "SPRO"){
				conll[i].type_of_link = "1-kompl";

			}

		}

	}

}
void comma(vector<Conll_line>&conll){

	int noun_zap = 0;
	string str11 = "f";
	string str12 = "M";
	string par1;
	string par2;
	string padeji[4] = { "NOM", "iNS", "gEN" };



	// у ТРЕУГОЛЬНИКА ТРИ СТОРОНЫ. НЕЯСНО К ЧЕМУ ПРИСОЕДИНЯТЬ ПРЕДЛОГ
	for (size_t i = 0; i < words_until_comma; i++) {
		if (conll[i].parts_of_speech == "PR" && conll[i].type_of_link == "rOOt" && subj > 1){
			conll[i].parent = to_string(subj);
			conll[i].type_of_link = "atrib";
		}


		else {

			if (conll[i].parts_of_speech == "PR" && conll[i].parent == "0" && predicate_link > 1){
				conll[i].parent = to_string(predicate_link);
				conll[i].type_of_link = "atrib";

			}


		}
	}


	/// ОТНЕСТИ СУЩ В ПРЕДЛ ИЛИ РОДИТ ПАДЕЖАХ С ПРЕДЛОГАМ

	for (size_t i = 0; i < words_until_comma; i++) {
		if (conll[i].parts_of_speech == "PR"){
			for (int f = i; f < words_until_comma; f++){
				if ((conll[f].parts_of_speech == "S" || conll[f].parts_of_speech == "SPRO")
					&& ((conll[f].morph1.find("lOc") || conll[f].morph1.find("gEN")))){
					conll[f].parent = to_string(i + 1);
					conll[f].type_of_link = "predl";
					break;

				}
			}



		}
	}



	//}



	///// Если глагол в повелительном наклонении,то подлежащего не будет.

	for (size_t i = 0; i < words_until_comma; i++) {
		if ((conll[i].parts_of_speech == "V" && conll[i].morph1.find("iMPEr") != string::npos)) {
			for (int f = i; f < words_until_comma; f++){
				if ((conll[f].parts_of_speech == "S" || conll[f].parts_of_speech == "SPRO") &&
					conll[f].morph1.find("NOM") != string::npos){
					conll[f].type_of_link = "kvaziagent";
					conll[f].parent = to_string(i + 1);
				}
			}

		}

	}



	for (size_t i = 0; i < words_until_comma; i++) {
		if (conll[i].parts_of_speech == "V") {
			for (int f = i; f < words_until_comma; f++){
				if ((conll[f].parts_of_speech == "S" || conll[f].parts_of_speech == "SPRO") &&
					conll[f].morph1.find("Acc") != string::npos){
					conll[f].type_of_link = "1-kompl";
					conll[f].parent = to_string(i + 1);
				}
			}

		}

	}


	for (size_t i = 0; i < words_until_comma; i++) {
		if (conll[i].parts_of_speech == "S") {
			for (int f = i; f < predicate_link; f++){
				if ((conll[f].parts_of_speech == "S") &&
					conll[i].morph1 == conll[f].morph1){
					conll[f].type_of_link = conll[i].type_of_link;
					conll[f].parent = conll[i].parent;
				}
			}

		}

		if (conll[i].parts_of_speech == "NUM" && conll[i + 1].parts_of_speech == "S"){
			conll[i].parent = to_string(i + 2);
			conll[i].type_of_link = "kolichest";
		}

	}

	///// Точка A,являющаяся серединой. Соединяем точку и являющаяся

	for (size_t i = 0; i < conll.size(); i++) {

		if ((conll[i].parts_of_speech == "A" || conll[i].parts_of_speech == "PARTCP")
			&& conll[i].morph1.find("BrEV") == string::npos){

			for (int j = i; j>0; j--){
				if (conll[j].parts_of_speech != "S"){
					noun_zap++;

					if (conll[j].parts_of_speech == "NONLEX"){
						noun_zap = noun_zap - 1;
					}

					if (conll[j].parts_of_speech == "PARTCP"){
						noun_zap = noun_zap - 1;
					}

				}

				if (conll[j].parts_of_speech == "S"){

					for (int k = 0; k < 7; k++){
						if (conll[i].morph1.find(padeji[k]) != string::npos){
							par1 = padeji[k];
						}

						if (conll[j].morph1.find(padeji[k]) != string::npos){
							par2 = padeji[k];
						}

						if (par1 == par2){
							conll[i].parent = to_string(noun_zap + 2);
							break;

						}

					}


				}
			}

			for (size_t i = 0; i < words_until_comma; i++){
				int noun_zap2 = 0;
				if ((conll[i].parts_of_speech == "A" && conll[i].morph1.find("BrEV") == string::npos)
					|| (conll[i].parts_of_speech == "PARTCP" && conll[i].morph1.find("BrEV") == string::npos)){

					for (int j = i; j < words_until_comma; j++){

						if (conll[j].parts_of_speech == "S"){

							for (int k = 0; k < 7; k++){
								if (conll[i].morph1.find(padeji[k]) != string::npos){
									par1 = padeji[k];
								}

								if (conll[j].morph1.find(padeji[k]) != string::npos){
									par2 = padeji[k];
								}

if (par1 == par2) {
conll[i].parent = to_string(j + 1);
}

							}

							break;

						}

					}

				}

			}
		}
	}
}


void order_func(vector<Conll_line>&conll, vector <fromlettonum> num, int first,int end, int flag3,size_t pos) {
	int counter = 0;
	int hundred = 0; // разряд сотни
	int tens = 0; // разряд десятки
	int ones = 0; // разряд единицы
	int flag = 0;
	int flag2 = 0; // если встретили тринадцать,двенадцать и т.д. то установить и единицы и десятки
	int flag4 = 0;
	int flag5 = 0;
	int counting = 0;// В зависимости от того есть ли десятки,будет зависеть откуда пойдет отчет единиц
					 // будет включен если единиц нет и встретится число с меткой order. встретит сто двадцать четвертых.Неопределенность
	int warning = 0;
	int hund = 0;// численное представление сотен
	int ten = 0; // численное представление десятков
	int one = 0; // численное представление единиц
	int number = 0; // число целиком
	size_t pos2 = 0;
	size_t b3 = 0;
	string line410; // текстовое представление сотен
	string line420; // текстовое представление десятков
	string line430; // текстовое представление единиц
	string line3;

	int amount_of_delete_word = 0;
	int count_hund = 0;
	int count_ten = 0;
	int count_on = 0;
	int hund_start = 0;
	int ten_start = 0;
	int on_start = 0;

	size_t t = 0;

	for (size_t i = first; i < end; i++) {
		if (conll[i].parts_of_speech == "numbers") {
			if (conll[i].word.length() == 3 && conll[i].word.find("/")==string::npos) {
				count_hund = 1;
			}

			if (conll[i].word.length() == 2) {
				count_ten = 1;
			}

			if (conll[i].word.length() == 1) {
				count_on = 1;
			}

			if (conll[i].word.find(transliter(1, "надцать")) != string::npos) {
				count_ten = 1;
				count_on = 0;
				flag2 = 1;
				if (count_hund == 1) {
					counter = 2;
				}
				if (count_hund == 0) {
					counter = 1;
				}
			}
		}
	}

	

	for (size_t i = first; i < end; i++) {
		if (conll[i].parts_of_speech == "numbers") {
			for (size_t y = i; y < end; y++) {
				if (conll[y].word.length() == 2) {
					if (conll[y + 1].morph1.find("order") != string::npos) {
						warning = 1;
					}
				}
			}

			if (flag3 == 0 && warning != 1) {
				if (conll[t].parts_of_speech == "numbers" && conll[t].original_form.length() == 3) {
					hundred = 1;
					line410 = conll[i].word;
					counter = 1;
					hund_start = i;
				}
				if (count_hund == 0) { // если нет сотен
					t = first;
				}
				else {
					t = hund_start;
				}
				for (; t < end; t++) {
					if (conll[t].parts_of_speech == "numbers" && conll[t].original_form.length() == 2) {
						tens = 1;
						line420 = conll[t].word;
						counter = 2;
						break;
					}
					if (flag2 == 0) {
						if (hund_start == 1 && count_ten == 1) { // единицы ищем оттуда где кончились десятки
							b3 = t;
							counter = 3;
						}
						if (hund_start == 1 && count_ten == 0) { // единицы считаем оттуда где кончились сотни
							b3 = i;
							line420 = to_string(0);
							counter = 2;
						}

						if (hund_start == 0 && count_ten == 0) {
							b3 = first;
							counter = 1;
						}

						for (; b3 < end; b3++) {
							if (conll[b3].parts_of_speech == "numbers" && conll[b3].original_form.length() == 1) {
								ones = 1;
								line430 = conll[b3].original_form;
								flag3 = 1;
								break;
							}
						}
					}
				}
			}
		}
	}

	hund = atoi(line410.c_str());
	ten = atoi(line420.c_str());
	one = atoi(line430.c_str());

	number = hund + ten + one;
	int a = 0, b = 0; // по слэшу делим дробь на числитель и знаменатель
	string separator = "/";
	line3 = conll[pos].original_form;
	
	size_t pos5 = line3.find(separator);
	a = atoi(line3.substr(0, pos5).c_str());
	b = atoi(line3.substr(pos5 + separator.length()).c_str());
	
	conll[first].word = to_string(number) + "/" + to_string(b);
	conll[first].morph1 = "nonlex";
	
		for (int i = 0; i < counter-1; i++) {
			change_id_and_number(conll, first + 1);
			conll.erase(conll.begin() + first + 1);
		}

		first = 0;
		counter = 0;
		a = 0;
		b = 0;
	
}
void float_func(vector<Conll_line>&conll, vector <fromlettonum> num, int first,int end) {
	int mnojitel = 0;
	double mnojitel2 = 0.0;
	string line35;
	string line34;
	int c = 0;
	double j = 0;
	float a = 0;
	float b = 0;
	int plus = 0;
	int counter = 0;
	int pos = 0;
	int flag3 = 0;
	for (size_t g = first; g < end; g++) {
		for (size_t j = 0; j < num.size(); j++) {
			if (conll[g].original_form == num[j].num && num[j].mark == "float") {
				pos = g;
				line35 = conll[g].original_form;
				
				a = atoi(string(1, line35[0]).c_str());
				b = atoi(string(1, line35[2]).c_str());
				flag3 = 1;
				++counter;
			}
		}
	}

	for (size_t g = first; g < end; g++) {
		if (conll[g].parts_of_speech == "numbers") {
			line34 = conll[g].original_form;
			mnojitel = atoi(line34.c_str());
			++counter;
		}
		c += mnojitel;
		mnojitel = 0;
	}

	if (flag3 == 1) {
		conll[first].word = to_string(c*a) + "/" + to_string(b);
		conll[first].original_form = to_string(c*a) + "/" + to_string(b);
	}

	for (int i = 0; i < counter-1; i++) {
		change_id_and_number(conll, first + 1);
		conll.erase(conll.begin() + first + 1);
	}

	counter = 0;
	
}
void intfloat_func(vector<Conll_line>&conll, vector <fromlettonum> num, int first,int end) {
	int intfloat = 0; // сто двадцать девятых; нуль целых двадцать шесть сотых
	int floa = 0; // три с половиной
	int order_ = 0; // сто девятнадцать четырнадцатых
	int a = 0, b = 0; // по слэшу делим дробь на числитель и знаменатель
	int counter = 0; // сколько знаков для объединения встретили
	int float_part = 0;
	int int_part = 0;
	int mode = 0;
	int intpart = 0;
	int floatpart = 0;
	int flag3 = 0;
	string line34;
	string line35;
	int pos = 0;
	float number = 0;
	float number2 = 0;
	double c = 0.0;
	float d = 0;
	string line36;

	for (size_t g = first; g < end; g++) {
		if (conll[g].original_form == transliter(1, "целый")) {
			pos = g;
		}
	}

	if (pos == 0) {
		mode = 1;
	}

	for (size_t g = first; g < pos; g++) {
		if (conll[g].original_form == ",") {
			break;
		}
		if (conll[g].parts_of_speech == "numbers") {
			line34 = conll[g].original_form;
			int_part = atoi(line34.c_str());
			++counter;
		}
		intpart += int_part;
		int_part = 0;
	}

	for (size_t l = first; l < end; l++) {
		if (conll[l].original_form == ",") {
			break;
		}
		
		if (conll[l].parts_of_speech == "numbers" && conll[l].original_form.find("/") == string::npos) {
			line35 = conll[l].original_form;
			float_part = atoi(line35.c_str());
			++counter;
		}
		floatpart += float_part;
		float_part = 0;
	}

	for (size_t g = first; g < end; g++) {
		if (conll[g].original_form == transliter(1, "тысячный") || conll[g].original_form == transliter(1, "тысячная") || conll[g].original_form == "1/1000") {
			c = intpart + floatpart*0.001;
			counter++;
			break;
		}

		if (conll[g].original_form == transliter(1, "сотый") || conll[g].original_form == transliter(1, "сотая") || conll[g].original_form == "1/100") {
			c = intpart + floatpart*0.01;
			counter++;
			break;
		}

		if (conll[g].original_form == transliter(1, "десятый") || conll[g].original_form == transliter(1, "десятая") || conll[g].original_form == "1/10") {
			c = intpart + floatpart*0.1;
			counter++;
			break;
		}

		if (mode==1 && conll[g].morph1.find("pluspart") != string::npos) {
			string separator = "/";
			string line3 = conll[g].original_form;
			size_t pos5 = line3.find(separator);
			a = atoi(line3.substr(0, pos5).c_str());
			d = atoi(line3.substr(pos5 + separator.length()).c_str());	
			number2 = a/d;
			mode = 2;
		}


		if (mode==1 && conll[g].morph1.find("order") != string::npos){
			mode = 3;
			
			if (conll[g - 1].original_form.length() == 2 && conll[g - 2].original_form.length() == 1) {
				floatpart = floatpart - atoi(conll[g - 1].original_form.c_str());
				number2 = atoi(conll[g - 1].original_form.c_str());
			}
			string separator = "/";
			string line3 = conll[g].original_form;

			size_t pos5 = line3.find(separator);
			a = atoi(line3.substr(0, pos5).c_str());
			b = atoi(line3.substr(pos5 + separator.length()).c_str());
			b = number2 + b;
		}

	}

	if (mode == 3) {
		counter = counter + 1;
		conll[first].word = to_string(floatpart) + "/" + to_string(b);
		conll[first].original_form = to_string(floatpart) + "/" + to_string(b);
	}
	

	if (mode == 2) {
		counter = counter + 2;
		conll[first].word = to_string(floatpart + number2);
		conll[first].original_form = to_string(floatpart + number2);
		intfloat = 0;
		pos = 0;
	}

	if (mode == 1) {
		conll[first].word = to_string(floatpart);
		conll[first].original_form = to_string(floatpart);
		intfloat = 0;
		pos = 0;
	}
	
	if(mode==0){
		counter = counter + 1;
		conll[first].word = to_string(c);
		conll[first].original_form = to_string(c);
		intfloat = 0;
		pos = 0;
	}


	for (int i = 0; i < counter-1; i++) {
		//change_id_and_number(conll, first + 1);
		//conll.erase(conll.begin() + first +1);
	}
		
	counter = 0;
	flag3 = 0;
	intfloat = 0;
	floatpart = 0;
	pos = 0;

}
void fromlettonum2(vector<Conll_line>&conll, vector <fromlettonum> &num) {
		
/*__ Метки __ */
int intfloat = 0; // сто двадцать девятых; нуль целых двадцать шесть сотых
int float_ = 0; // три с половиной
int order_ = 0; // сто девятнадцать четырнадцатых

int first = 0; // откуда начинаем собирать число
int counter = 0; // сколько знаков для объединения встретили
int flag = 0;// выставляем флаг,что после работы нужно будет удалять строки
int float_part = 0;
int int_part = 0;
int mode = 0;
int intpart = 0;
int floatpart = 0;
int flag3 = 0;

string line34;
string line35;
size_t pos = 0;

int position = 0;
int end = 0;

int abc = 0;
int abc2 = 0;

/*____ Ищем слово,которое есть в таблице чисел.В зависимости от метки расставляем флаги __*/
for (size_t i = 0; i < conll.size(); i++) {
	for (size_t j = 0; j < num.size(); j++) {
		if (conll[i].original_form == num[j].letters) {
			if (num[j].mark == "intfloat") {
				abc = 1;
			}

			if (num[j].mark == "float") {
				conll[i].word = num[j].num;
				conll[i].parts_of_speech = "numbers";
				conll[i].original_form = num[j].num;
				flag = 1;
				abc2 = 2;
			}

			if (num[j].mark == "pluspart") {
				conll[i].morph1 = conll[i].morph1 + " pluspart";
				abc = 1;
			}

			if (num[j].mark == "order") {
				first = i;
				for (size_t z = 0; z < i; z++) {
					if (conll[z].original_form == transliter(1, "целый")) {
						flag3 = 1;
					}
				}
				if (flag3 == 0) {
					order_ = 1;
					pos = i;
				}
				conll[i].morph1 = conll[i].morph1 + " order";
			}


			conll[i].word = num[j].num;
			conll[i].parts_of_speech = "numbers";
			conll[i].original_form = num[j].num;
			flag = 1;

			if (first == 0) {
				first = i;
			}
		}
	}
}

end = 0;
first = 0;

for (size_t b = 0; b < conll.size(); b++) {
	if (conll[b].parts_of_speech == "numbers") {
		if (first == 0) {
			first = b;
		}
		
		for (size_t y = first; y < conll.size(); y++) {
			if (conll[y].word == "." || conll[y].word == "i" || conll[y].word == ",") {
				end = y;
				break;
			}
		}

			if (abc == 1){
				intfloat_func(conll, num, first, end);
				first = end + 1;
			}
			if (abc2 == 2) {
				float_func(conll, num, first,end);
				first = end + 1;
			}
				first = 0;
			}
		}
	}


void work_with_punc(vector<Conll_line>&punct) {
	int parent = 0;
	for (size_t u = 0; u < punct.size(); u++) {
		if (punct[u].original_form == transliter(1, "который")) {
			parent = u;
		}
	}

	for (size_t u = 0; u < punct.size(); u++) {
		if (punct[u].parts_of_speech == "V" || punct[u].parts_of_speech == "PARTCP"){
			punct[u].parent = to_string(parent+1);
			punct[u].type_of_link = "prediK";
		}
	}

	for (size_t u = 0; u < punct.size(); u++) {
		if (punct[u].parts_of_speech == "PR") {
			for (size_t t = u; t > 0; t--) {
				if (punct[t].parts_of_speech == "V" || punct[t].parts_of_speech == "PARTCP") {
					punct[u].parent = to_string(t+1);
					punct[u].type_of_link = "predL";
				}
			}
		}
	}

}


int main(int argc, char *argv[]) {
		int hide_ = 0, numb = 0, number_of_lines = 0, mode = 0, i1 = 0, LINES_COUNT = 0, k = 0;
		int  brackets = 0;
		int numfromlettertonum = 0;
		string line, option_line, option_line2;
		string separ = "\t";
		vector<Conll_line>conll;
		vector<Conll_line>punct;
		vector<Conll_line>punct2;
		vector <options> optionlist;
		ifstream myfile, optionfile;

		if (argc < 2){
			return -1;
		}

		myfile.open("out.txt");
		while (getline(myfile, line)) {
			if (line != "") {
				++LINES_COUNT; // посчитали число строк
			}
		}
		myfile.close();
		//cout << LINES_COUNT << endl;


		/*___________ Чтение данных и заполнение вектора структур __________ */

		read_conll(conll, LINES_COUNT);
	
		/*___________________ Чтение файла опций  _________________________*/

		optionfile.open("options.ini");
		optionfile.ignore(255, '\n');
		optionfile >> k;
		optionfile.ignore(255, '\n');
		optionlist.resize(k);

		while (getline(optionfile, option_line)) {
			size_t pos = option_line.find(separ);
			optionlist[i1].name_of_option = option_line.substr(0, pos);
			optionlist[i1].value = option_line.substr(pos + separ.length());
			i1++;
		}
		optionfile.close();

		/*_____________ Расставляем флаги в соответствии с опциями ____________*/

		for (size_t i = 0; i < optionlist.size(); i++) {
			if (optionlist[i].name_of_option == "brackets" && optionlist[i].value == "1") {
				brackets = 1;
			}

			if (optionlist[i].name_of_option == "numfromlettertonum" && optionlist[i].value == "1") {
				numfromlettertonum = 1;
			}

			if (optionlist[i].name_of_option == "numbers" && optionlist[i].value == "1") {
				numb = 1;
			}

			if (optionlist[i].name_of_option == "hide" && optionlist[i].value == "1") {
				hide_ = 1;
			}
		}

		/* ____________________ Основной блок _________________________ */

		if (strcmp(argv[1],"test")==0){
			mode = 1;
		}
		if (strcmp(argv[1], "auto") == 0){
			mode = 2;
		}

		size_t i4 = 0;
		vector <fromlettonum> num;
		ifstream in;
		int n = 0;
		string line1, line2;
		in.open("num.txt");
		in >> n;
		
		in.ignore(255, '\n');
		num.resize(n);
		while (getline(in, line1)) {
			size_t pos = line1.find(separ);
			num[i4].letters = line1.substr(0, pos);
			line2 = line1.substr(pos + separ.length());
			size_t pos2 = line2.find(separ);
			num[i4].num = line2.substr(0, pos2);
			num[i4].mark = line2.substr(pos2 + separ.length());
			i4++;
		}
		in.close();

		//if (mode == 1) {
		
		//}

		preparation(conll, "until"); // подготовка данных

		/*______________________ Редактирующие правила _______________*/


		//
		//cout << "work_with_punc_is_ready" << endl;


		string b = find_the_predicate(conll);
		//combine_(conll, LINES_COUNT,b);


		//if (brackets == 1) {
			//brackets_(conll);
		//}

		//if (numb == 1) {
			//numbers(conll);
		//}

		//if (numfromlettertonum == 1) {
			//fromlettonum2(conll,num); //Одна из боковых сторон равнобокой трапеции равна семь целых двенадцать сотых,вторая равна пять сто семнадцатых
		//}

		//if (hide_ == 1) {
			//hide(conll);
		//}

		//razbit(conll, punct, punct2, LINES_COUNT);
		//put_in_the_end(conll, punct, punct2, LINES_COUNT);
		//work_with_punc(punct);


		//order(conll);
		//tester2(conll);
		//tester2(punct);
		
		/*int flag = 0;
		int where_ = 0;
		for (size_t i = 0; i < conll.size(); i++){
			if (conll[i].parts_of_speech == "numbers" && conll[i + 1].word == "sm") {
				conll[i].word = conll[i].word + " " + conll[i + 1].word;
				conll[i].original_form = conll[i].word;
				where_ = i;
				flag = 1;
			}
		}
	
		if (flag == 1) {
			conll.erase(conll.begin() +where_ + 1);
			flag = 0;
		}*/
	//special_words_and_idioms(conll);
	//find_the_subject(conll, b);
	//join_to_neighbour(conll);
	//addit_subj(conll);
	//comma(conll);
	//partcp_parsing(conll);
	//unknown_word(conll);

	/*_______________ Выходной файл будет определен в зависимости от того какой передан параметр (режим работы) ___________*/

	//mode = 2;
	
	preparation(conll, "after");
	write_conll(conll,punct,punct2,mode);

	/*_______________________________________________________________________________________________________*/

	//system("pause");
	return 0;
}
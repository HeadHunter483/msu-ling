#pragma warning(disable:4996)
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <stdio.h>
#include "Transliter.h"
#include "utilities.h"

using namespace std;


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


void combine(vector<Conll_line>&conll,int LINES_COUNT){
	int  find_bracket=0,i2=0,amount_of_words_until_bracket=0,n, where_noun_after_num = 0, flag = 0, amount_of_delets = 0,skolko_udalit=0,counter_of_parts=0;
	string what_is_part_of_speach = "";
	string line_of_dict;
	string newline1;
	string separ = "\t";
	size_t i1 = 0;
	ifstream dict("dict.txt");
	ifstream prepad("predlogipadeji.txt");
	vector <dictionary> diction;
	vector <predlogipadeji> prpad;
	string math_oper[11] = { "+", "=", "*", "/", "-","<",">","<=",">=",")","(" };

	/* Cчитываем данные из словаря 1 */
	dict >> n; 
	dict.ignore(255, '\n');
	diction.resize(n);
	while (getline(dict, line_of_dict)){
		for (size_t i3 = 0; i3 < line_of_dict.size(); i3++){
			if (line_of_dict[i3] == ' '){
				counter_of_parts++; // считаем число пробелов между словами - получаем число слов в строке словаря.
			}
		}
		size_t pos = line_of_dict.find(separ); // разбиваем строку словаря на сам оборот и часть речи
		diction[i1].words = line_of_dict.substr(0, pos);
		diction[i1].parts_of_speach = line_of_dict.substr(pos + separ.length());
		diction[i1].amount_of_words = to_string(counter_of_parts+1);
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

	while (getline(prepad, line_of_dict)){
		size_t pos = line_of_dict.find(separ);
		prpad[i1].predlog = line_of_dict.substr(0, pos);
		newline1 = line_of_dict.substr(pos + separ.length());
		size_t pos2 = newline1.find(separ);
		prpad[i1].padej1 = newline1.substr(0, pos2);
		prpad[i1].padej2 = newline1.substr(pos2 + separ.length());
		i1++;
	}
	prepad.close();

	/* Сравниваем слова с выражениями из словаря */
	int schetchik = 0;
	for (size_t i = 0; i < diction.size(); i++){ // если в словаре есть две (вроде должно хватить) соседние строки из
		// слов таблицы то объединяем эти слова в одну строку таблицы.
		for (size_t v = 0; v < conll.size() - schetchik - 1; ++v){
			if (conll[v].word.length()>1){ // ставим ограничение на длину слова для однобуквенных слов
				if (diction[i].amount_of_words == to_string(1) && (diction[i].words.find(conll[v].word) != string::npos)){
					conll[v].word = diction[i].words;
					conll[v].original_form = diction[i].words;
					conll[v].parts_of_speech = diction[i].parts_of_speach;
					conll[v].morph1 = "_";
					conll[v].morph2 = "_";
					conll[v].type_of_link = "atrib";
				}
				else {
					if ((diction[i].words.find(conll[v].word + " " + conll[v + 1].word) != string::npos)){
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
		if (flag == 1){
			for (int i = 0; i < skolko_udalit - 1; i++){
				if (skolko_udalit == 2){
					change_id_and_number(conll, where_noun_after_num + 1);
					conll.erase(conll.begin() + where_noun_after_num + 1);
				}
				if (skolko_udalit == 3){
					change_id_and_number(conll, where_noun_after_num);
					conll.erase(conll.begin() + where_noun_after_num);
				}
			}
			where_noun_after_num = 0;
			flag = 0;
		}
	}
	/* Соединяем сложное сказуемое с начальной формой быть ..... */
	 for (size_t i = 0; i < conll.size(); i++){
		if (conll[i].parts_of_speech == "V" && conll[i].original_form.find("byitmz") != string::npos){
			for (size_t i2 = i; i2 < conll.size(); i2++){
				if (conll[i2].parts_of_speech == "PARTCP"){
					where_noun_after_num = i2 + 1;
					flag = 1;
					amount_of_delets = amount_of_delets + 1;
					conll[i].word = conll[i].word + " " + conll[i2].word;
					conll[i].original_form = conll[i].original_form + "+" + conll[i2].original_form;
					conll[i].parts_of_speech = "Complex+partcp";
					conll[i].parent = conll[i2].parent;
					conll[i].type_of_link = conll[i2].type_of_link;
					conll[i].morph1 = conll[i2].morph1;
					conll[i].morph2 = conll[i2].morph2;
					break;
				}
			}
		}
		if (flag == 1){
			change_id_and_number(conll, where_noun_after_num);
			conll.erase(conll.begin() + where_noun_after_num - 1);
			where_noun_after_num = 0;
			flag = 0;
		}
	}
	
	 
	 /*Объединяем числительное и существительное(местоимение)в родительном падеже*/
	for (size_t i = 0; i < conll.size(); i++){
		if (conll[i].parts_of_speech == "NUM"){
			for (size_t i2 = i; i2 < conll.size(); i2++){
				if ((conll[i2].parts_of_speech == "S" || conll[i2].parts_of_speech == "SPRO") &&
					conll[i2].morph1.find("gen") != string::npos || conll[i2].morph1.find("nonflex") != string::npos){
					where_noun_after_num = i2+1;
					flag = 1;
					amount_of_delets = amount_of_delets + 1;
					if (conll[i2].parts_of_speech == "s"){
						what_is_part_of_speach = "s";
					} 
					else {
						what_is_part_of_speach = "s";
					}
					conll[i].word = conll[i].word + " " + conll[i2].word;
					conll[i].original_form = conll[i].original_form + " " + conll[i2].original_form;
					conll[i].parts_of_speech = "num+" + what_is_part_of_speach;
					conll[i].parent = conll[i2].parent;
					conll[i].type_of_link = conll[i2].type_of_link;
					conll[i].morph1 = conll[i2].morph1;
					conll[i].morph2 = conll[i2].morph2;
					break;
					}
				}
			}

		if (flag == 1){
			change_id_and_number(conll, where_noun_after_num);
			conll.erase(conll.begin() + where_noun_after_num - 1);
			where_noun_after_num = 0;
			flag = 0;
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


						where_noun_after_num = i1 + 1;
						flag = 1;
						if (conll[i1].parts_of_speech == "S"){
							what_is_part_of_speach = "S";
						}
						else {
							what_is_part_of_speach = "SPRO";
						}

						if (conll[i1 + 1].parts_of_speech == "CONJ" || conll[i1].word==","){ //если след.союз
							for (size_t i3 = i1+1; i3 < conll.size(); i3++) { // ищем сущ с соот падежом без предлога
								if ((conll[i3].parts_of_speech == "S" || conll[i3].parts_of_speech == "SPRO") &&
									(conll[i3].morph1.find(prpad[i2].padej1) != string::npos
										|| conll[i3].morph1.find(prpad[i2].padej2) != string::npos)) {
									conll[i3].word = "("+conll[i].word + ")"+" "+conll[i3].word;

								}
							}

						}
						

						conll[i].word = conll[i].word + " " + conll[i1].word;
						conll[i].original_form = conll[i].word;
						conll[i].parts_of_speech = "PR+" + what_is_part_of_speach;
						conll[i].parent = conll[i1].parent;
						conll[i].type_of_link = conll[i1].type_of_link;
						conll[i].morph1 = conll[i1].morph1;
						conll[i].morph2 = conll[i1].morph2;

						break;
					}
				}
			}
		}
			
		if (flag == 1){
			change_id_and_number(conll, where_noun_after_num);
			conll.erase(conll.begin() + where_noun_after_num - 1);
			where_noun_after_num = 0;
			flag = 0;
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

			//break;

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

								if (par1 == par2){
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

void fromlettonum2(vector<Conll_line>&conll) {
	vector <fromlettonum> num;
	ifstream in;

	int flag = 0;
	int i1 = 0;
	int n = 0;
	string line12,line13;
	string separ = "\t";
	in.open("num.txt");
	in >> n;
	in.ignore(255, '\n');
	num.resize(n);
	while (getline(in, line12)) {
		size_t pos = line12.find(separ);
		num[i1].letters = line12.substr(0, pos);
		line13 = line12.substr(pos + separ.length());
		size_t pos2 = line13.find(separ);
		num[i1].num = line13.substr(0, pos2);
		num[i1].mark = line13.substr(pos2 + separ.length());
		++i1;
	}
	in.close();

int first = 0;
int drobnaya = 0;
int now = 0;
int d = 0;
int p = 0;
int celaya = 0;
int intfloat = 0;
int float_ = 0;
int  counter = 0;
string line34;


for (size_t i = 0; i < conll.size(); i++){
	for (size_t j = 0; j < num.size(); j++){
		if (conll[i].original_form == num[j].letters){
			if (num[j].mark == "intfloat") {
				intfloat == 1;
			}
			if (num[j].mark == "float") {
				float_ = 1;
			}

			if (first == 0) {
				first = i;
			}
			conll[i].word = num[j].num;
			conll[i].parts_of_speech = "numbers";
			conll[i].original_form = num[j].num;
			flag = 1;
		}
	}
}

if (intfloat == 1 || float_ == 0) {
	for (size_t g = 0; g < conll.size(); g++) {
		if (conll[g].original_form == transliter(1, "целый")) {
			now = g;
		}
	}

	for (size_t g = 0; g < now; g++) {
		if (conll[g].parts_of_speech == "numbers") {
			line34 = conll[g].original_form;
			celaya = atoi(line34.c_str());
			++counter;
		}
		p += celaya;
		celaya = 0;
	}

	cout << p << "celaya" << endl;

	string line35;

	for (size_t l = now; l < conll.size(); l++) {
		if (conll[l].parts_of_speech == "numbers") {
			line35 = conll[l].original_form;
			drobnaya = atoi(line35.c_str());
			++counter;
		}
		d += drobnaya;
		drobnaya = 0;
	}

	cout << d << "drobnaya" << endl;

	double fg = 0.0;

	string line36;
	
	for (size_t g = 0; g < conll.size(); g++) {
		if (conll[g].original_form == transliter(1, "тысячный")) {
			fg = p + d*0.001;
			break;
		}

		if (conll[g].original_form == transliter(1, "сотый")) {
			fg = p + d*0.01;
			break;
		}

		if (conll[g].original_form == transliter(1, "десятый")) {
			fg = p + d*0.1;
			break;
		}
	}

	conll[first].word = to_string(fg);
	intfloat = 0;
}

if (intfloat == 0 && float_ == 1) {
	int mnojitel = 0;
	double mnojitel2 = 0.0;
	string line35;
	double j = 0;
	float a = 0;
	float b = 0;
	for (size_t g = 0; g < conll.size(); g++){
		for (size_t j = 0; j < num.size(); j++) {
			if (conll[g].original_form == num[j].num && num[j].mark == "float") {
				now = g;
				line35 = conll[g].original_form;
				
				a = atoi(string(1,line35[0]).c_str());
				b= atoi(string(1, line35[2]).c_str());
				++counter;
				}
			}
		}
		
		for (size_t g = 0; g < now; g++) {
			if (conll[g].parts_of_speech == "numbers") {
				line34 = conll[g].original_form;
				mnojitel = atoi(line34.c_str());
				++counter;
			}
			p += mnojitel;
			mnojitel = 0;
		}
	
		conll[first].word = to_string(p*(a/b));
		float_ = 0;
	}
	
if (flag == 1){
	for (int i = 0; i < counter; i++) {
		change_id_and_number(conll, first+1);
		conll.erase(conll.begin() + first+1);
	}
	first = 0;
	flag = 0;
	counter = 0;
	}

}

int main(int argc, char *argv[]){
	int numb = 0, number_of_lines = 0, mode = 0, i1 = 0, LINES_COUNT = 0, k = 0;
	int combine = 0;
	int  brackets = 0;
	int numfromlettertonum = 0;
	string line,option_line, option_line2;
	string separ = "\t";
	vector<Conll_line>conll;
	vector <options> optionlist;
	ifstream myfile,optionfile;
	
	if (argc < 2) {
		return -1;
	}

	myfile.open("output.txt");
	while (getline(myfile, line)){
		++LINES_COUNT; // посчитали число строк
	}
	myfile.close();

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
		optionlist[i1].value = option_line.substr(pos+separ.length());
		i1++;
	}
	optionfile.close();

	/*_____________ Расставляем флаги в соответствии с опциями ____________*/

	for (size_t i = 0; i < optionlist.size(); i++) {
		if (optionlist[i].name_of_option == "combine" && optionlist[i].value == "1") {
			combine = 1;
		}

		if (optionlist[i].name_of_option == "brackets" && optionlist[i].value == "1") {
			brackets = 1;
		}

		if (optionlist[i].name_of_option == "numfromlettertonum" && optionlist[i].value == "1") {
			numfromlettertonum = 1;
		}

		if (optionlist[i].name_of_option == "numbers" && optionlist[i].value == "1") {
			numb = 1;
		}
	}

	/* ____________________ Основной блок _________________________ */

	preparation(conll,"until"); // подготовка данных

	/*______________________ Редактирующие правила _______________*/

	string b = find_the_predicate(conll);

	if (brackets == 1) {
		brackets_(conll);
	}

	if (numb == 1) {
		numbers(conll);
	}

	if (numfromlettertonum == 1) {
		//fromlettonum2(conll);
	}
	if (combine == 1) {
		//combine(conll,LINES_COUNT);
	}
	
	
	//special_words_and_idioms(conll);
	//find_the_subject(conll, b);
	//join_to_neighbour(conll);
	//addit_subj(conll);
	//comma(conll);
	//partcp_parsing(conll);
	//unknown_word(conll);

	/*_______________ Выходной файл будет определен в зависимости от того какой передан параметр (режим работы) ___________*/

	//mode == 2;
	
	if (strcmp(argv[1],"test")==0){
		mode = 1;
	}
	if (strcmp(argv[1], "auto") == 0){
		mode = 2;
	}
	preparation(conll, "after");
	write_conll(conll, mode);

	/*_______________________________________________________________________________________________________*/

	system("pause");
	return 0;
}
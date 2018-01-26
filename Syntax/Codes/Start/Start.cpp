#pragma warning(disable:4996)
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include <clocale>
#include <Windows.h>

using namespace std;

int skolko_tochek = 0;// содержит сколько знаков конца предложения найдено

/*_______________ Структуры данных для работы со словарями_____________________*/
struct dictionary {
	string words;
	string parts_of_speach;
	string amount_of_words;
};
struct predlogipadeji {
	string predlog;
	string padej1;
	string padej2;
};
struct fromletterstonum {
	string letters;
	string num;
	string mark;
};

/*___________________ Вспомогательные функции  ________________________________-*/
/*	-проверка является ли данная строка числом */
bool isnum(string str){ // проверка является ли данный символ числом.
	                   // Нужен для случая когда  целая и вещественная части 
					   // в вещественном числе отделяются точкой
	bool numeric = false;
	if (str.find_first_not_of("0123456789.") == string::npos)
		numeric = true;
	return numeric;
}
/*	-увеличить длину строки */
char *substr(char *str, int number){ 
	return str + number;
}

/*____________________ Замена символов ____________________ */
/*-с русских на латинские */
void Transliteraterustolat(char* str, char* newStr)
{
	for (; *str != 0; str++)
	{
		switch (str[0])
		{
		case 'а': strcat(&newStr[0], "a"); break;
		case 'б': strcat(&newStr[0], "b"); break;
		case 'в': strcat(&newStr[0], "v"); break;
		case 'г': strcat(&newStr[0], "g"); break;
		case 'д': strcat(&newStr[0], "d"); break;
		case 'е': strcat(&newStr[0], "e"); break;
		case 'ё': strcat(&newStr[0], "yo"); break;
		case 'ж': strcat(&newStr[0], "zh"); break;
		case 'з': strcat(&newStr[0], "z"); break;
		case 'и': strcat(&newStr[0], "i"); break;
		case 'й': strcat(&newStr[0], "y"); break;
		case 'к': strcat(&newStr[0], "k"); break;
		case 'л': strcat(&newStr[0], "l"); break;
		case 'м': strcat(&newStr[0], "m"); break;
		case 'н': strcat(&newStr[0], "n"); break;
		case 'о': strcat(&newStr[0], "o"); break;
		case 'п': strcat(&newStr[0], "p"); break;
		case 'р': strcat(&newStr[0], "r"); break;
		case 'с': strcat(&newStr[0], "s"); break;
		case 'т': strcat(&newStr[0], "t"); break;
		case 'у': strcat(&newStr[0], "u"); break;
		case 'ф': strcat(&newStr[0], "f"); break;
		case 'х': strcat(&newStr[0], "x"); break;
		case 'ц': strcat(&newStr[0], "tc"); break;
		case 'ч': strcat(&newStr[0], "ch"); break;
		case 'ш': strcat(&newStr[0], "sh"); break;
		case 'щ': strcat(&newStr[0], "shch"); break;
		case 'ъ': strcat(&newStr[0], "tz"); break;
		case 'ы': strcat(&newStr[0], "yi"); break;
		case 'ь': strcat(&newStr[0], "mz"); break;
		case 'э': strcat(&newStr[0], "ye"); break;
		case 'ю': strcat(&newStr[0], "yu"); break;
		case 'я': strcat(&newStr[0], "ya"); break;
		case 'А': strcat(&newStr[0], "A"); break;
		case 'Б': strcat(&newStr[0], "B"); break;
		case 'В': strcat(&newStr[0], "V"); break;
		case 'Г': strcat(&newStr[0], "G"); break;
		case 'Д': strcat(&newStr[0], "D"); break;
		case 'Е': strcat(&newStr[0], "E"); break;
		case 'Ё': strcat(&newStr[0], "Yo"); break;
		case 'Ж': strcat(&newStr[0], "Zh"); break;
		case 'З': strcat(&newStr[0], "Z"); break;
		case 'И': strcat(&newStr[0], "I"); break;
		case 'Й': strcat(&newStr[0], "Y"); break;
		case 'К': strcat(&newStr[0], "K"); break;
		case 'Л': strcat(&newStr[0], "L"); break;
		case 'М': strcat(&newStr[0], "M"); break;
		case 'Н': strcat(&newStr[0], "N"); break;
		case 'О': strcat(&newStr[0], "O"); break;
		case 'П': strcat(&newStr[0], "P"); break;
		case 'Р': strcat(&newStr[0], "R"); break;
		case 'С': strcat(&newStr[0], "S"); break;
		case 'Т': strcat(&newStr[0], "T"); break;
		case 'У': strcat(&newStr[0], "U"); break;
		case 'Ф': strcat(&newStr[0], "F"); break;
		case 'Х': strcat(&newStr[0], "X"); break;
		case 'Ц': strcat(&newStr[0], "Tc"); break;
		case 'Ч': strcat(&newStr[0], "Ch"); break;
		case 'Ш': strcat(&newStr[0], "Sh"); break;
		case 'Щ': strcat(&newStr[0], "Shch"); break;
		case 'Ъ': strcat(&newStr[0], "Tz"); break;
		case 'Ы': strcat(&newStr[0], "Yi"); break;
		case 'Ь': strcat(&newStr[0], "Mz"); break;
		case 'Э': strcat(&newStr[0], "Ye"); break;
		case 'Ю': strcat(&newStr[0], "Yu"); break;
		case 'Я': strcat(&newStr[0], "Ya"); break;
		default: { char Temp[2] = { str[0], 0 }; strcat(&newStr[0], &Temp[0]); }
		}
	}
}
/*-c латинских на русские */
void Transliteratelattorus(char* str, char* newStr) {
	for (; *str != 0; str++) {
		switch (str[0])
		{
		case 'a': strcat(&newStr[0], "а"); break;
		case 'b': strcat(&newStr[0], "б"); break;
		case 'v': strcat(&newStr[0], "в"); break;
		case 'g': strcat(&newStr[0], "г"); break;
		case 'd': strcat(&newStr[0], "д"); break;
		case 'e': strcat(&newStr[0], "е"); break;
		case 'yo': strcat(&newStr[0], "ё"); break;
		case 'zh': strcat(&newStr[0], "ж"); break;
		case 'z': strcat(&newStr[0], "з"); break;
		case 'i': strcat(&newStr[0], "и"); break;
		case 'yu': strcat(&newStr[0], "ю"); break;
		case 'y': strcat(&newStr[0], "й"); break;
		case 'k': strcat(&newStr[0], "к"); break;
		case 'l': strcat(&newStr[0], "л"); break;
		case 'm': strcat(&newStr[0], "м"); break;
		case 'n': strcat(&newStr[0], "н"); break;
		case 'o': strcat(&newStr[0], "о"); break;
		case 'p': strcat(&newStr[0], "п"); break;
		case 'r': strcat(&newStr[0], "р"); break;
		case 's': strcat(&newStr[0], "с"); break;
		case 't': strcat(&newStr[0], "т"); break;
		case 'u': strcat(&newStr[0], "у"); break;
		case 'f': strcat(&newStr[0], "ф"); break;
		case 'x': strcat(&newStr[0], "х"); break;
		case 'tc': strcat(&newStr[0], "ц"); break;
		case 'ch': strcat(&newStr[0], "ч"); break;
		case 'sh': strcat(&newStr[0], "ш"); break;
		case 'shch': strcat(&newStr[0], "щ"); break;
		case 'tz': strcat(&newStr[0], "ъ"); break;
		case 'yi': strcat(&newStr[0], "ы"); break;
		case 'mz': strcat(&newStr[0], "ь"); break;
		case 'ye': strcat(&newStr[0], "э"); break;
		case 'ya': strcat(&newStr[0], "я"); break;
		case 'A': strcat(&newStr[0], "А"); break;
		case 'B': strcat(&newStr[0], "Б"); break;
		case 'V': strcat(&newStr[0], "В"); break;
		case 'G': strcat(&newStr[0], "Г"); break;
		case 'D': strcat(&newStr[0], "Д"); break;
		case 'E': strcat(&newStr[0], "Е"); break;
		case 'Yo': strcat(&newStr[0], "Ё"); break;
		case 'Zh': strcat(&newStr[0], "Ж"); break;
		case 'Z': strcat(&newStr[0], "З"); break;
		case 'I': strcat(&newStr[0], "И"); break;
		case 'Y': strcat(&newStr[0], "Й"); break;
		case 'K': strcat(&newStr[0], "К"); break;
		case 'L': strcat(&newStr[0], "Л"); break;
		case 'M': strcat(&newStr[0], "М"); break;
		case 'N': strcat(&newStr[0], "Н"); break;
		case 'O': strcat(&newStr[0], "О"); break;
		case 'P': strcat(&newStr[0], "П"); break;
		case 'R': strcat(&newStr[0], "Р"); break;
		case 'S': strcat(&newStr[0], "С"); break;
		case 'T': strcat(&newStr[0], "Т"); break;
		case 'U': strcat(&newStr[0], "У"); break;
		case 'F': strcat(&newStr[0], "Ф"); break;
		case 'X': strcat(&newStr[0], "Х"); break;
		case 'Tc': strcat(&newStr[0], "Ц"); break;
		case 'Ch': strcat(&newStr[0], "Ч"); break;
		case 'Sh': strcat(&newStr[0], "Ш"); break;
		case 'Shch': strcat(&newStr[0], "Щ"); break;
		case 'Tz': strcat(&newStr[0], "Ы"); break;
		case 'Yi': strcat(&newStr[0], "Ы"); break;
		case 'Mz': strcat(&newStr[0], "Ь"); break;
		case 'Ye': strcat(&newStr[0], "Э"); break;
		case 'Yu': strcat(&newStr[0], "Ю"); break;
		case 'Ya': strcat(&newStr[0], "Я"); break;
		default: { char Temp[2] = { str[0], 0 }; strcat(&newStr[0], &Temp[0]); }
		}
	}
}
/*- в зависимости от значения первого аогумента заменяет символы */
string transliter(int mode, string line) {
	string line2;
	if (mode == 1) {
		char y[100] = { 0 };

		char * writable = new char[line.size() + 1];
		copy(line.begin(), line.end(), writable);
		writable[line.size()] = '\0';
		Transliteraterustolat(writable, &y[0]);
		line2 = string(y);
	}

	if (mode == 2) {
		int i = 0;
		char dy[100] = { 0 };
		while (i <= line.size()) {
			if (line[i] == 'y' || line[i] == 'Y') {
				if (line[i + 1] == 'e') {
					line[i] = 'э';
					line.erase(line.begin() + i + 1);
				}
				if (line[i + 1] == 'o') {
					line[i] = 'ё';
					line.erase(line.begin() + i + 1);
				}
				if (line[i + 1] == 'u') {
					line[i] = 'ю';
					line.erase(line.begin() + i + 1);
				}
				if (line[i + 1] == 'a') {
					line[i] = 'я';
					line.erase(line.begin() + i + 1);
				}
				if (line[i + 1] == 'i') {
					line[i] = 'ы';
					line.erase(line.begin() + i + 1);
				}
			}
			if ((line[i] == 't' && line[i + 1] == 'c') || (line[i] == 'T' && line[i + 1] == 'c')) {
				line[i] = 'ц';
				line.erase(line.begin() + i + 1);
			}

			if (line[i] == 'm' && line[i + 1] == 'z') {
				line[i] = 'ь';
				line.erase(line.begin() + i + 1);
			}

			if (line[i] == 't' && line[i + 1] == 'z') {
				line[i] = 'ъ';
				line.erase(line.begin() + i + 1);
			}
			if ((line[i] == 'z' && line[i + 1] == 'h') || (line[i] == 'Z' && line[i + 1] == 's')) {
				line[i] = 'ж';
				line.erase(line.begin() + i + 1);
			}
			if ((line[i] == 's' && line[i + 1] == 'h' && line[i + 2] == 'c' && line[i + 3] == 'h') ||
				(line[i] == 'S' && line[i + 1] == 'h' && line[i + 2] == 'c' && line[i + 3] == 'h')) {
				line[i] = 'щ';
				line.erase(line.begin() + i + 1);
				line.erase(line.begin() + i + 2);
				line.erase(line.begin() + i + 1);
			}

			if ((line[i] == 's' && line[i + 1] == 'h') || (line[i] == 'S' && line[i + 1] == 'h')) {
				line[i] = 'ш';
				line.erase(line.begin() + i + 1);
			}

			if ((line[i] == 'c' && line[i + 1] == 'h') || (line[i] == 'C' && line[i + 1] == 'h')) {
				line[i] = 'ч';
				line.erase(line.begin() + i + 1);
			}
			i++;
		}

		char * writable = new char[line.size() + 1];
		copy(line.begin(), line.end(), writable);
		writable[line.size()] = '\0';
		Transliteratelattorus(writable, &dy[0]);
		line2 = string(dy);
	}

	return line2;
}

/*______________    Заполнение файла конфигурации____________ */
void config_write(char *path) { // заполняет файл конфигурации в соотв. с путем.
	ofstream file1;
	file1.open("config.ini");

	file1 << "[DEFAULT]" << endl;
	file1 << "APP_ROOT" << " " << "=" << " " << string(path) << endl;
	file1 << "BIN_PATH" << " " << "=" << " " << "%(APP_ROOT)s\\bin" << endl;
	file1 << "TMP_PATH" << " " << "=" << " " << "%(APP_ROOT)s\\tmp" << endl;

	file1 << endl;
	file1 << "[mystem]" << endl;
	file1 << "MYSTEM_PATH" << " " << "=" << " " << "%(BIN_PATH)s\\mystem.exe" << endl;

	file1 << endl;
	file1 << "[treetagger]" << endl;
	file1 << "treetagger_bin" << " " << "=" << " " << "%(BIN_PATH)s\\" << "\\Treetagger" << "\\" << "\\bin" << "\\" << "\\tree-tagger.exe" << endl;
	file1 << "treetagger_par" << " " << "=" << " " << "%(APP_ROOT)s\\" << "\\tree_alltags_model.par";

	file1.close();
}

/*____________________	Синтаксический анализ с ручным вводом данных   _________________ */
/*- определяем сколько предложений */
void find_skolko_tochek(string str, char splitBy1, char splitBy2, char splitBy3) { // По точкам считаем число предложений.
																				   // Если точка часть вещественного числа ее пропускаем
	size_t i = 0;
	size_t j = 0;
	while (i <= str.length()) {
		if (str[i] == splitBy1 || str[i] == splitBy2 || str[i] == splitBy3) {
			if ((isnum(string(1, str[i - 1])) == 0 && isnum(string(1, str[i + 1])) == 0) ||
				(isnum(string(1, str[i - 1])) != 0 && isnum(string(1, str[i + 1])) == 0)) {
				while (j <= i) {
					if (j == i) {
						skolko_tochek = skolko_tochek + 1;
					}
					j++;
				}
			}
		}
		i++;
	}
}
/*-	делим текст на отдельные предложения */
void split_write(string str, char splitBy1, char splitBy2, char splitBy3, ofstream&file2) {  // разделяем текст на предложения.
																							 //Теперь пишем разделенные предложения 
																							 //каждое на своей строчке 
																							 //Если после точки есть пробел,затираем его.
	size_t i = 0;
	size_t j = 0;
	int gde = 0;
	while (i <= str.length()) {
		if (str[i] == splitBy1 || str[i] == splitBy2 || str[i] == splitBy3) {
			if ((isnum(string(1, str[i - 1])) == 0 && isnum(string(1, str[i + 1])) == 0) ||
				(isnum(string(1, str[i - 1])) != 0 && isnum(string(1, str[i + 1])) == 0)) {
				if (str[i + 1] == ' ') { // удаляем пробел после конца предложения
					gde = i + 1;
					str.erase(gde, 1);
				}
				while (j <= i) {
					file2 << str[j];
					if (j == i) {
						file2 << endl;
					}
					j++;
				}
			}
		}
		i++;
	}
}
/*- проверка входного файла.Разделение текста.	*/
void splitter() {
	fstream file1;
	ofstream file2;
	string line;

	file1.open("in.txt", ios::in);
	long file_size;
	file1.seekg(0, ios::end);
	file_size = file1.tellg();
	if (file_size == 0) { // проверяем входной файл пуст ли он.
		cout << "Входной файл пуст " << endl;
		system("pause");
		exit(0);
	}
	file1.close();

	file1.open("in.txt");
	file2.open("buffer1.txt");
	while (!file1.eof()) {
		getline(file1, line);
	}
	find_skolko_tochek(line, '.', '?', '!');
	file2 << skolko_tochek << endl;
	split_write(line, '.', '?', '!', file2);
	file1.close();
	file2.close();
}
/*- предложения из буффера по одному пишутся во входной файл */
int readfrombuffer(int n, const char *str1) {
	string str;
	ifstream in;
	in.open(str1);
	ofstream fout("in.txt");

	for (int i = 1; i < n + 1; i++) { //пропускаем n+1 строку
		in.ignore(255, '\n');
	}
	if (!in.eof()) {
		getline(in, str);
		fout << str;
	}
	in.close();
	return 0;
}
/*-  запись предложений во входной файл.Запуск ru-syntax-master*/
int programm(char *path){ // для  анализа,когда данные вводятся вручную.
	//Текст разбивается на отдельные предложения.Предложения по одному пишутся во входной файл.
	ifstream file1,file3;
	ofstream file2;
	int n;
	string line2;
	splitter();
	file1.open("buffer1.txt");
	file1 >> n;// считали количество предложений
	file1.close();

	cout << string(path) << endl;

	for (int i = 0; i < n; i++){
		string line5 = "python ru-syntax-master.py " + string(path)+" auto "+to_string(i+1)+" "+to_string(n);
		const char *line_5 = line5.c_str();
		readfrombuffer(i + 1,"buffer1.txt");
		system(line_5);
	}
	return 0;
}
void autoanalyzer(char *writable){
	FILE *in, *out;
	char c, d;
	in = fopen("in.txt", "r");
	out = fopen("buffer2.txt", "w");
	while ((c = fgetc(in)) != EOF) {
		fputc(c, out); //записываем содержимое in.txt в buffer2.txt
	}
	fclose(in);
	fclose(out);
	cout << "4.Подготовка входного файла:" << endl;
	system("python converter.py in.txt utf8");// переводим входной файл в кодировку utf-8

	cout << "5.Запуск программы:" << endl;
	programm(writable); // основной блок работы режима с ручным вводом текстов

	in = fopen("buffer2.txt", "r");
	out = fopen("in.txt", "w");
	while ((d = fgetc(in)) != EOF) {
		fputc(d, out); //записываем содержимое buffer2.txt в in.txt
	}
	fclose(in);
	fclose(out);
}

/*_______________ Режим редактирования и добавления для словарей _______________*/
void add(char *writable, string name_of_dict, int mode) {
	int n = 0;
	int count_of_new_idioms = 0;
	FILE *in, *out;
	ifstream in2;
	ofstream out2;
	char c;

	string name_for_new_word = string(writable) + +"\\" + "bin" + "\\Malt\\" + "\\" + name_of_dict;
	const char *name_for_word_add = name_for_new_word.c_str();

	string name_for_new_word2 = string(writable) + +"\\" + "bin" + "\\Malt\\" + "\\temp5.txt";
	const char *name_for_word2_add = name_for_new_word2.c_str();

	string name_for_new_word3 = string(writable) + +"\\" + "bin" + "\\Malt\\" + "\\temp6.txt";
	const char *name_for_word3_add = name_for_new_word3.c_str();

	in = fopen(name_for_word_add, "r");
	out = fopen(name_for_word2_add, "w");
	while ((c = fgetc(in)) != EOF) { /*по-символьно все содержимое словаря отправляем во временный файл*/
		fputc(c, out); //чтобы по окончании работы востановить содержимое словаря.
	}
	fclose(in);
	fclose(out);

	in2.open(name_for_word_add);
	in2 >> n;  // считали количество уже существующих в словаре.
	in2.close();

	out2.open(name_for_word3_add, ios::app); //написали инструкцию
	if (mode == 1) {
		out2 << "Напишите вместо пунктирной линии новые идиомы и сохраните файл.Каждая идиома на новой строке.Формат: идиома табуляция часть речи (большими латинскими буквами)" << endl;
	}

	if (mode == 2) {
		out2 << "Напишите вместо пунктирной линии новые предлоги и сохраните файл.Каждый предлог на новой строке.Формат:предлог табуляция падеж1 табуляция падеж2 (или No если его нет)" << endl;
	}

	if (mode == 3) {
		out2 << "Напишите вместо пунктирной линии новые числа и сохраните файл.Каждое число на новой строке.Формат: число прописью в именит.падеже табуляция число цифрами табуляция intfloat/float" << endl;
	}
	out2 << "_ _ _";
	out2.close();

	system(name_for_word3_add);// открыли для записи

	in2.open(name_for_word3_add);
	while (!in2.eof()) { // по числу переходов на новую строку определяем число новых идиом
		char s = in2.get();
		if (s == '\n') {
			count_of_new_idioms++;
		}
	}
	in2.close();

	out2.open(name_for_word_add, ios::trunc);//очищаем словарь
	out2.close();

	in2.open(name_for_word2_add);
	out2.open(name_for_word_add, ios::app);

	string line8, line9;

	out2 << n + count_of_new_idioms << endl; // написали новое число идиом

	in2.ignore(255, '\n'); // пропустили число-количество,которое было ранее
	while (getline(in2, line8)) {
		out2 << line8 << endl;
	}
	in2.close();

	in2.open(name_for_word3_add);
	in2.ignore(255, '\n'); // пропустили инструкцию
	while (getline(in2, line9)) {
		out2 << transliter(1, line9) << endl;
	}

	in2.close();
	out2.close();

	cout << "Добавление завершено." << endl;
}
void edit(char *writable, string name_of_dict, int mode) {
	ifstream in2;
	ofstream out2;
	string line8, line9,line10;
	string separ = "\t";
	vector <dictionary> diction;
	vector <predlogipadeji> prepad;
	vector <fromletterstonum> num;
	int n = 0;
	int counter_of_parts = 0;
	int i1 = 0;

	string name_for_new_word = string(writable) + +"\\" + "bin" + "\\Malt\\" + "\\" + name_of_dict;
	const char *name_for_word_add = name_for_new_word.c_str();

	string name_for_new_word2 = string(writable) + +"\\" + "bin" + "\\Malt\\" + "\\temp5.txt";
	const char *name_for_word2_add = name_for_new_word2.c_str();

	if (mode == 1) {
		in2.open(name_for_word_add);
		out2.open(name_for_word2_add);

		in2 >> n;
		in2.ignore(255, '\n');
		diction.resize(n);

		while (getline(in2, line8)) {
			for (size_t i3 = 0; i3 < line8.size(); i3++){
				if (line8[i3] == ' ') {
					counter_of_parts++; // считаем число пробелов между словами - получаем число слов в строке словаря.
				}
			}
			size_t pos = line8.find(separ); // разбиваем строку словаря на сам оборот и часть речи
			diction[i1].words = line8.substr(0, pos);
			diction[i1].parts_of_speach = line8.substr(pos + separ.length());
			diction[i1].amount_of_words = to_string(counter_of_parts + 1);
			counter_of_parts = 0;
			++i1;
		}
		in2.close();

		out2 << n << endl;

		for (size_t i = 0; i < diction.size(); i++) {
			out2 << transliter(2, diction[i].words) << "	" << diction[i].parts_of_speach << endl;
		}

		out2.close();
	}

	if (mode == 2) {
		in2.open(name_for_word_add);
		out2.open(name_for_word2_add);

		in2 >> n;
		in2.ignore(255, '\n');
		prepad.resize(n);

		while (getline(in2, line8)) {
			size_t pos = line8.find("\t");
			prepad[i1].predlog = line8.substr(0, pos);
			line9 = line8.substr(pos + separ.length());
			size_t pos2 = line9.find(separ);
			prepad[i1].padej1 = line9.substr(0, pos2);
			prepad[i1].padej2 = line9.substr(pos2 + separ.length());
			i1++;
		}
		in2.close();

		out2 << n << endl;
		
		for (size_t i = 0; i < prepad.size(); i++){
			out2 << transliter(2, prepad[i].predlog) << "	" << prepad[i].padej1 << "	" << prepad[i].padej2<<endl;
		}

		out2.close();
	}

	if (mode == 3) {
		in2.open(name_for_word_add);
		out2.open(name_for_word2_add);

		in2 >> n;
		in2.ignore(255, '\n');
		num.resize(n);

		while (getline(in2, line8)){
			size_t pos = line8.find(separ); 
			num[i1].letters = line8.substr(0, pos);
			line9 = line8.substr(pos + separ.length());
			size_t pos2 = line9.find(separ);
			num[i1].num = line9.substr(0,pos2);
			num[i1].mark=line9.substr(pos2 + separ.length());
			++i1;
		}
		in2.close();

		out2 << n << endl;

		for (size_t i = 0; i < num.size(); i++) {
			out2 << transliter(2, num[i].letters) << "	" << num[i].num << "	" << num[i].mark << endl;
		}

		out2.close();
	}

	system(name_for_word2_add);

	in2.open(name_for_word2_add);
	out2.open(name_for_word_add);

	while (getline(in2, line9)) {
		out2 << transliter(1, line9) << endl;
	}

	cout << "Редактирование завершено." << endl;
}
void addmode(char *writable) {
	ifstream file1;
	string line41, line42, text2, text3;
	int kolichestvo_perehodov_na_novuyu_stroku = 0;

	string line19 = "python clean.py " + string(writable) + " addmode " + to_string(1);//запускаем скрипт, который удаляет временные файлы
	const char *line_19 = line19.c_str();
	system(line_19); /* выполняем clean.py */

	cout << "Введите,что нужно добавить: " << endl;
	cout << "	test-добавление тестов" << endl;
	cout << "	idiom-добавление или редактирование базы идиом" << endl;
	cout << "	pr-добавление или редактирование базы предлогов" << endl;
	cout << "	num-добавление или редактирование базы числительных" << endl;
	cin >> text2;

	if (text2 != "test" && text2 != "idiom" && text2 != "pr" && text2 !="num") {
		cout << "	 Неверно введена команда.Работа программы завершена." << endl;
		system("pause");/*выполняем проверку на неверно введенные команды.Сообщаем и выходим. */
		exit(0);
	}

	if (text2 == "test") {
		file1.open("test.txt");
		while (!file1.eof()) { // считаем сколько уже тестов есть в test.exe
			char s = file1.get();
			if (s == '\n') {
				kolichestvo_perehodov_na_novuyu_stroku++;
			}
		}
		file1.close();

		ofstream file_1;
		cout << "4.Введите новое предложение:" << endl; // добавляем свое предложение.
		ofstream out("buffer1.txt", ios::app);
		out << "Напишите вместо пунктирной линии свой тест и сохраните файл." << endl;
		out << "_ _ _"; // внутри файла,куда нужно вести предложение будет инструкция
		out.close();
		file1.open("buffer1.txt");
		file_1.open("test.txt", ios::app);
		system("buffer1.txt");// открываем файл куда пользователь будет осуществлять ввод.
		cout << "------------------------------------------------------------------------" << endl;

		cout << "5.Подготовка файлов:" << endl;
		system("python converter.py buffer1.txt utf8"); // меняем кодировку,чтобы не было лишних символов

		file1.ignore(255, '\n');//пропускаем строку с инструкцией
		while (getline(file1, line41)) {
			string line2 = line41 + '\n';
			file_1 << line2; // дописываем новое предложение в конец test.txt
		}
		file1.close();
		file_1.close();
		system("python converter.py test.txt utf8"); // меняем кодировку,чтобы не было лишних символов

		file1.open("buffer1.txt");
		file_1.open("in.txt");
		file1.ignore(255, '\n');
		while (getline(file1, line41)) {
			file_1 << line41; // новое предложение пишем во входной файл.
		}
		file1.close();
		file_1.close();

		system("python converter.py in.txt utf8"); // меняем кодировку,чтобы не было лишних символов

		cout << "6.Запуск программы и исправление ошибок:" << endl;
		string path11 = "python ru-syntax-master.py " + string(writable) + " addmode " + to_string(1) + " " + to_string(1); // запускаем в режиме тест для 
																												//одного предложения								
		const char *path_11 = path11.c_str();
		system(path_11);
		string path12 = string(writable) + +"\\" + "bin" + "\\Malt\\" + "\\final.txt";
		const char *path_12 = path12.c_str();
		file1.open(path_12);

		string name_for_new_test = string(writable) + +"\\" + "bin" + "\\Malt\\" + "\\test_"
			+ to_string(kolichestvo_perehodov_na_novuyu_stroku + 1) + ".txt";
		const char *name_for_test_add = name_for_new_test.c_str();
		file_1.open(name_for_test_add);

		string line30;
		while (!file1.eof()) {
			getline(file1, line30); // копируем данные из файла,где хранится результат разбора 
									// в тестовый файл с соотв.индексом
			file_1 << line30 << endl;
		}
		file1.close();
		file_1.close();

		cout << "7.Внесите необходимые правки:" << endl;
		system(name_for_test_add);// откроет файл для редактирования
		string line431 = "cd " + string(writable) + "\\" + "bin" + "\\Malt\\" + "&& python converter.py test_"
			+ to_string(kolichestvo_perehodov_na_novuyu_stroku + 1) + ".txt" + " utf8";
		const char *line131 = line431.c_str();
		system(line131);// меняем кодировку,чтобы не было лишних символов
		cout << "Тест_" + to_string(kolichestvo_perehodov_na_novuyu_stroku + 1) + " успешно добавлен." << endl;
		cout << "------------------------------------------------------------------------" << endl;
	}

	if (text2 == "idiom" || text2 == "pr" || text2=="num") {
		cout << "	Редактирование или добавление edit/add ?" << endl;
		cin >> text3;

		if (text3 == "add" && text2 == "idiom") { // режим добавления.
			add(writable, "dict.txt", 1);
		}

		if (text3 == "add" && text2 == "pr") { // режим добавления.
			add(writable, "predlogipadeji.txt", 2);
		}

		if (text3 == "add" && text2 == "num") {
			edit(writable, "num.txt", 3);
		}

		if (text3 == "edit" && text2 == "idiom") {
			edit(writable, "dict.txt", 1);
		}

		if (text3 == "edit" && text2 == "pr") { // режим добавления.
			edit(writable, "predlogipadeji.txt", 2);
		}

		if (text3 == "edit" && text2 == "num") { // режим добавления.
			edit(writable, "num.txt", 3);
		}
	}
}

/*_________________		Режим тестирования _________________________*/
void test_analyzer(char *writable) {
	ifstream file1;
	ofstream fout;
	int kolvo_testov = 0, kolichestvo_perehodov_na_novuyu_stroku = 0;

	//system("python converter.py test.txt utf8");
	system("python converter.py test_corpus.txt utf8");
	system("python converter.py in.txt utf8");

	//file1.open("test.txt");
	file1.open("test_corpus.txt");
	while (!file1.eof()) { // по числу переходов на новую строку определяем число тестов в test.txt
		char s = file1.get();
		if (s == '\n') {
			kolichestvo_perehodov_na_novuyu_stroku++;
		}
	}
	file1.close();

	cout << "	Введите количество тестов.Максимальное число тестов " + to_string(kolichestvo_perehodov_na_novuyu_stroku) << endl;
	cin >> kolvo_testov;
	// если введенное число тестов больше максимального то закончить работу программы			
	if (kolvo_testov > kolichestvo_perehodov_na_novuyu_stroku) {
		cout << "	Превышено максимальное число тестов.";
		system("pause");
		exit(0);
	}

	string line18 = "python clean.py " + string(writable) + " test " + to_string(kolvo_testov);//запускаем скрипт, который удаляет временные файлы
	const char *line_18 = line18.c_str();
	system(line_18);

	cout << "4.Подготовка к тестированию:" << endl;
	cout << "------------------------------------------------------------------------" << endl;

	for (int i = 0; i < kolvo_testov; i++) {
		string line5 = "python ru-syntax-master.py " + string(writable) + " test " + to_string(i + 1) + " " + to_string(kolvo_testov);
		const char *line_5 = line5.c_str();
		readfrombuffer(i, "test.txt"); // добавляем в in.txt тестовые предложения по одному из test.txt
		system(line_5); //запускаем 
		line_5 = "";
	}
}

/*________________________ Основной блок ________________*/
int main(){
	FILE *in, *out;
	string text, answer;
	char c, d;
	char *temp;
	char name[MAX_PATH];
	int i = 0, v = 0;
	setlocale(LC_ALL, "");

	cout << " " << endl;
	cout << "	Здравствуйте.Это синтаксический анализатор текстов математических задач." << endl;
	cout << "	------------------------------------------------------------------------" << endl;
	cout << "1.1. Путь до директории ru-syntax-master:" << endl;
	
	GetCurrentDirectory(MAX_PATH, name);
	cout << name << endl;
	
	temp = new char[strlen(name) + 10];
	int razm = strlen(name);
	while (i <= razm){ // если встретили просто одинарный слеш.То добавляем к нему еще один
		if (name[i] == '\\'){
			if (name[i + 1] != '\\'){
				strcpy(temp, substr(name, i + 1));
				name[i + 1] = '\0';
				strcat(name, "\\");
				strcat(name, temp);
			}
			if (name[i + 1] == '\\'){ // если слеш двойной, то пропустить.
				i = i + 1;
			}
		}
		i++;
	}

	cout << "1.2.Использовать текущий (Y) или установленный по умолчанию файл-конфигурации (N) ? Y/N" << endl;
	cin >> answer;
	if (answer == "N"){ // выбран файл по умолчанию.
		cout << "Выбран файл-конфигурации по умолчанию." << endl;
		config_write(name);// заполняем config.ini с учетом введенного пути
	}
	else{
		cout << "	Выбран уже существующий в директории файл-конфигурации." << endl;
		cout << "	Не забудьте внести необходимые правки в файл ru-syntax-master и config.ini."<<endl;
	}
	cout << "------------------------------------------------------------------------";

	cout << " " << endl;
	cout << "2.Введите:" << endl;
	cout << "	test для тестирования программы." << endl;
	cout << "	auto для анализа с заполнением входного файла вручную." << endl;
	cout << "	add для добавления данных." << endl;
	cout << "	install для установки необходимых компонентов.Рекомендуется запускать при первом запуске." << endl;
	cout << "	options для внесения изменений в настройки исправляющей программы." << endl;
	cout << "	exit для выхода из программы." << endl;
	cin >> text;
	cout << "------------------------------------------------------------------------" << endl;

	if (text != "test" && text != "auto" && text != "add" && text != "install" && text!="options" && text != "exit"){
		cout << "	 Неверно введена команда.Работа программы завершена." << endl;
		system("pause");/*выполняем проверку на неверно введенные команды.Сообщаем и выходим. */
		exit(0);
	}

	string line16 = "python install.py " + string(name);//инструкция,какие программы нужно установить,есть возможность поставить необходимую библиотеку 
	const char *line_16 = line16.c_str();

	if (text == "install"){
		system(line_16); /* выполнили install.py */
		cout << "Завершение работы" << endl; /* завершили работу */
		system("pause");
		exit(0);
	}

	if (text == "auto"){ //выбран режим с ручным вводом текстов
		string line6 = "python clean.py " + string(name) + " auto " + to_string(1);//запускаем скрипт, который удаляет временные файлы
		const char *line_6 = line6.c_str();
		system(line_6);
		autoanalyzer(name);
	}

	if (text == "test"){ // если выбран режим тестирования
		test_analyzer(name);
	}

	if (text == "add"){ /*режим добавления для тестов (полуавтоматический) и данных в словари */
		addmode(name);
	}

	if (text == "options"){
		string line19 = "python clean.py " + string(name) + " addmode " + to_string(1);//запускаем скрипт, который удаляет временные файлы
		const char *line_19 = line19.c_str();
		system(line_19); /* выполняем clean.py */
		string option = string(name) + +"\\" + "bin" + "\\Malt\\" + "\\options.ini";
		const char *options = option.c_str();

		system(options);
	}

	if (text == "exit"){
		exit(0);
	}
	
	cout << "Завершение работы:" << endl;
	system("pause");
	return 0;
}




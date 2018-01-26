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

int skolko_tochek = 0;// �������� ������� ������ ����� ����������� �������

/*_______________ ��������� ������ ��� ������ �� ���������_____________________*/
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

/*___________________ ��������������� �������  ________________________________-*/
/*	-�������� �������� �� ������ ������ ������ */
bool isnum(string str){ // �������� �������� �� ������ ������ ������.
	                   // ����� ��� ������ �����  ����� � ������������ ����� 
					   // � ������������ ����� ���������� ������
	bool numeric = false;
	if (str.find_first_not_of("0123456789.") == string::npos)
		numeric = true;
	return numeric;
}
/*	-��������� ����� ������ */
char *substr(char *str, int number){ 
	return str + number;
}

/*____________________ ������ �������� ____________________ */
/*-� ������� �� ��������� */
void Transliteraterustolat(char* str, char* newStr)
{
	for (; *str != 0; str++)
	{
		switch (str[0])
		{
		case '�': strcat(&newStr[0], "a"); break;
		case '�': strcat(&newStr[0], "b"); break;
		case '�': strcat(&newStr[0], "v"); break;
		case '�': strcat(&newStr[0], "g"); break;
		case '�': strcat(&newStr[0], "d"); break;
		case '�': strcat(&newStr[0], "e"); break;
		case '�': strcat(&newStr[0], "yo"); break;
		case '�': strcat(&newStr[0], "zh"); break;
		case '�': strcat(&newStr[0], "z"); break;
		case '�': strcat(&newStr[0], "i"); break;
		case '�': strcat(&newStr[0], "y"); break;
		case '�': strcat(&newStr[0], "k"); break;
		case '�': strcat(&newStr[0], "l"); break;
		case '�': strcat(&newStr[0], "m"); break;
		case '�': strcat(&newStr[0], "n"); break;
		case '�': strcat(&newStr[0], "o"); break;
		case '�': strcat(&newStr[0], "p"); break;
		case '�': strcat(&newStr[0], "r"); break;
		case '�': strcat(&newStr[0], "s"); break;
		case '�': strcat(&newStr[0], "t"); break;
		case '�': strcat(&newStr[0], "u"); break;
		case '�': strcat(&newStr[0], "f"); break;
		case '�': strcat(&newStr[0], "x"); break;
		case '�': strcat(&newStr[0], "tc"); break;
		case '�': strcat(&newStr[0], "ch"); break;
		case '�': strcat(&newStr[0], "sh"); break;
		case '�': strcat(&newStr[0], "shch"); break;
		case '�': strcat(&newStr[0], "tz"); break;
		case '�': strcat(&newStr[0], "yi"); break;
		case '�': strcat(&newStr[0], "mz"); break;
		case '�': strcat(&newStr[0], "ye"); break;
		case '�': strcat(&newStr[0], "yu"); break;
		case '�': strcat(&newStr[0], "ya"); break;
		case '�': strcat(&newStr[0], "A"); break;
		case '�': strcat(&newStr[0], "B"); break;
		case '�': strcat(&newStr[0], "V"); break;
		case '�': strcat(&newStr[0], "G"); break;
		case '�': strcat(&newStr[0], "D"); break;
		case '�': strcat(&newStr[0], "E"); break;
		case '�': strcat(&newStr[0], "Yo"); break;
		case '�': strcat(&newStr[0], "Zh"); break;
		case '�': strcat(&newStr[0], "Z"); break;
		case '�': strcat(&newStr[0], "I"); break;
		case '�': strcat(&newStr[0], "Y"); break;
		case '�': strcat(&newStr[0], "K"); break;
		case '�': strcat(&newStr[0], "L"); break;
		case '�': strcat(&newStr[0], "M"); break;
		case '�': strcat(&newStr[0], "N"); break;
		case '�': strcat(&newStr[0], "O"); break;
		case '�': strcat(&newStr[0], "P"); break;
		case '�': strcat(&newStr[0], "R"); break;
		case '�': strcat(&newStr[0], "S"); break;
		case '�': strcat(&newStr[0], "T"); break;
		case '�': strcat(&newStr[0], "U"); break;
		case '�': strcat(&newStr[0], "F"); break;
		case '�': strcat(&newStr[0], "X"); break;
		case '�': strcat(&newStr[0], "Tc"); break;
		case '�': strcat(&newStr[0], "Ch"); break;
		case '�': strcat(&newStr[0], "Sh"); break;
		case '�': strcat(&newStr[0], "Shch"); break;
		case '�': strcat(&newStr[0], "Tz"); break;
		case '�': strcat(&newStr[0], "Yi"); break;
		case '�': strcat(&newStr[0], "Mz"); break;
		case '�': strcat(&newStr[0], "Ye"); break;
		case '�': strcat(&newStr[0], "Yu"); break;
		case '�': strcat(&newStr[0], "Ya"); break;
		default: { char Temp[2] = { str[0], 0 }; strcat(&newStr[0], &Temp[0]); }
		}
	}
}
/*-c ��������� �� ������� */
void Transliteratelattorus(char* str, char* newStr) {
	for (; *str != 0; str++) {
		switch (str[0])
		{
		case 'a': strcat(&newStr[0], "�"); break;
		case 'b': strcat(&newStr[0], "�"); break;
		case 'v': strcat(&newStr[0], "�"); break;
		case 'g': strcat(&newStr[0], "�"); break;
		case 'd': strcat(&newStr[0], "�"); break;
		case 'e': strcat(&newStr[0], "�"); break;
		case 'yo': strcat(&newStr[0], "�"); break;
		case 'zh': strcat(&newStr[0], "�"); break;
		case 'z': strcat(&newStr[0], "�"); break;
		case 'i': strcat(&newStr[0], "�"); break;
		case 'yu': strcat(&newStr[0], "�"); break;
		case 'y': strcat(&newStr[0], "�"); break;
		case 'k': strcat(&newStr[0], "�"); break;
		case 'l': strcat(&newStr[0], "�"); break;
		case 'm': strcat(&newStr[0], "�"); break;
		case 'n': strcat(&newStr[0], "�"); break;
		case 'o': strcat(&newStr[0], "�"); break;
		case 'p': strcat(&newStr[0], "�"); break;
		case 'r': strcat(&newStr[0], "�"); break;
		case 's': strcat(&newStr[0], "�"); break;
		case 't': strcat(&newStr[0], "�"); break;
		case 'u': strcat(&newStr[0], "�"); break;
		case 'f': strcat(&newStr[0], "�"); break;
		case 'x': strcat(&newStr[0], "�"); break;
		case 'tc': strcat(&newStr[0], "�"); break;
		case 'ch': strcat(&newStr[0], "�"); break;
		case 'sh': strcat(&newStr[0], "�"); break;
		case 'shch': strcat(&newStr[0], "�"); break;
		case 'tz': strcat(&newStr[0], "�"); break;
		case 'yi': strcat(&newStr[0], "�"); break;
		case 'mz': strcat(&newStr[0], "�"); break;
		case 'ye': strcat(&newStr[0], "�"); break;
		case 'ya': strcat(&newStr[0], "�"); break;
		case 'A': strcat(&newStr[0], "�"); break;
		case 'B': strcat(&newStr[0], "�"); break;
		case 'V': strcat(&newStr[0], "�"); break;
		case 'G': strcat(&newStr[0], "�"); break;
		case 'D': strcat(&newStr[0], "�"); break;
		case 'E': strcat(&newStr[0], "�"); break;
		case 'Yo': strcat(&newStr[0], "�"); break;
		case 'Zh': strcat(&newStr[0], "�"); break;
		case 'Z': strcat(&newStr[0], "�"); break;
		case 'I': strcat(&newStr[0], "�"); break;
		case 'Y': strcat(&newStr[0], "�"); break;
		case 'K': strcat(&newStr[0], "�"); break;
		case 'L': strcat(&newStr[0], "�"); break;
		case 'M': strcat(&newStr[0], "�"); break;
		case 'N': strcat(&newStr[0], "�"); break;
		case 'O': strcat(&newStr[0], "�"); break;
		case 'P': strcat(&newStr[0], "�"); break;
		case 'R': strcat(&newStr[0], "�"); break;
		case 'S': strcat(&newStr[0], "�"); break;
		case 'T': strcat(&newStr[0], "�"); break;
		case 'U': strcat(&newStr[0], "�"); break;
		case 'F': strcat(&newStr[0], "�"); break;
		case 'X': strcat(&newStr[0], "�"); break;
		case 'Tc': strcat(&newStr[0], "�"); break;
		case 'Ch': strcat(&newStr[0], "�"); break;
		case 'Sh': strcat(&newStr[0], "�"); break;
		case 'Shch': strcat(&newStr[0], "�"); break;
		case 'Tz': strcat(&newStr[0], "�"); break;
		case 'Yi': strcat(&newStr[0], "�"); break;
		case 'Mz': strcat(&newStr[0], "�"); break;
		case 'Ye': strcat(&newStr[0], "�"); break;
		case 'Yu': strcat(&newStr[0], "�"); break;
		case 'Ya': strcat(&newStr[0], "�"); break;
		default: { char Temp[2] = { str[0], 0 }; strcat(&newStr[0], &Temp[0]); }
		}
	}
}
/*- � ����������� �� �������� ������� ��������� �������� ������� */
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
					line[i] = '�';
					line.erase(line.begin() + i + 1);
				}
				if (line[i + 1] == 'o') {
					line[i] = '�';
					line.erase(line.begin() + i + 1);
				}
				if (line[i + 1] == 'u') {
					line[i] = '�';
					line.erase(line.begin() + i + 1);
				}
				if (line[i + 1] == 'a') {
					line[i] = '�';
					line.erase(line.begin() + i + 1);
				}
				if (line[i + 1] == 'i') {
					line[i] = '�';
					line.erase(line.begin() + i + 1);
				}
			}
			if ((line[i] == 't' && line[i + 1] == 'c') || (line[i] == 'T' && line[i + 1] == 'c')) {
				line[i] = '�';
				line.erase(line.begin() + i + 1);
			}

			if (line[i] == 'm' && line[i + 1] == 'z') {
				line[i] = '�';
				line.erase(line.begin() + i + 1);
			}

			if (line[i] == 't' && line[i + 1] == 'z') {
				line[i] = '�';
				line.erase(line.begin() + i + 1);
			}
			if ((line[i] == 'z' && line[i + 1] == 'h') || (line[i] == 'Z' && line[i + 1] == 's')) {
				line[i] = '�';
				line.erase(line.begin() + i + 1);
			}
			if ((line[i] == 's' && line[i + 1] == 'h' && line[i + 2] == 'c' && line[i + 3] == 'h') ||
				(line[i] == 'S' && line[i + 1] == 'h' && line[i + 2] == 'c' && line[i + 3] == 'h')) {
				line[i] = '�';
				line.erase(line.begin() + i + 1);
				line.erase(line.begin() + i + 2);
				line.erase(line.begin() + i + 1);
			}

			if ((line[i] == 's' && line[i + 1] == 'h') || (line[i] == 'S' && line[i + 1] == 'h')) {
				line[i] = '�';
				line.erase(line.begin() + i + 1);
			}

			if ((line[i] == 'c' && line[i + 1] == 'h') || (line[i] == 'C' && line[i + 1] == 'h')) {
				line[i] = '�';
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

/*______________    ���������� ����� ������������____________ */
void config_write(char *path) { // ��������� ���� ������������ � �����. � �����.
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

/*____________________	�������������� ������ � ������ ������ ������   _________________ */
/*- ���������� ������� ����������� */
void find_skolko_tochek(string str, char splitBy1, char splitBy2, char splitBy3) { // �� ������ ������� ����� �����������.
																				   // ���� ����� ����� ������������� ����� �� ����������
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
/*-	����� ����� �� ��������� ����������� */
void split_write(string str, char splitBy1, char splitBy2, char splitBy3, ofstream&file2) {  // ��������� ����� �� �����������.
																							 //������ ����� ����������� ����������� 
																							 //������ �� ����� ������� 
																							 //���� ����� ����� ���� ������,�������� ���.
	size_t i = 0;
	size_t j = 0;
	int gde = 0;
	while (i <= str.length()) {
		if (str[i] == splitBy1 || str[i] == splitBy2 || str[i] == splitBy3) {
			if ((isnum(string(1, str[i - 1])) == 0 && isnum(string(1, str[i + 1])) == 0) ||
				(isnum(string(1, str[i - 1])) != 0 && isnum(string(1, str[i + 1])) == 0)) {
				if (str[i + 1] == ' ') { // ������� ������ ����� ����� �����������
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
/*- �������� �������� �����.���������� ������.	*/
void splitter() {
	fstream file1;
	ofstream file2;
	string line;

	file1.open("in.txt", ios::in);
	long file_size;
	file1.seekg(0, ios::end);
	file_size = file1.tellg();
	if (file_size == 0) { // ��������� ������� ���� ���� �� ��.
		cout << "������� ���� ���� " << endl;
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
/*- ����������� �� ������� �� ������ ������� �� ������� ���� */
int readfrombuffer(int n, const char *str1) {
	string str;
	ifstream in;
	in.open(str1);
	ofstream fout("in.txt");

	for (int i = 1; i < n + 1; i++) { //���������� n+1 ������
		in.ignore(255, '\n');
	}
	if (!in.eof()) {
		getline(in, str);
		fout << str;
	}
	in.close();
	return 0;
}
/*-  ������ ����������� �� ������� ����.������ ru-syntax-master*/
int programm(char *path){ // ���  �������,����� ������ �������� �������.
	//����� ����������� �� ��������� �����������.����������� �� ������ ������� �� ������� ����.
	ifstream file1,file3;
	ofstream file2;
	int n;
	string line2;
	splitter();
	file1.open("buffer1.txt");
	file1 >> n;// ������� ���������� �����������
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
		fputc(c, out); //���������� ���������� in.txt � buffer2.txt
	}
	fclose(in);
	fclose(out);
	cout << "4.���������� �������� �����:" << endl;
	system("python converter.py in.txt utf8");// ��������� ������� ���� � ��������� utf-8

	cout << "5.������ ���������:" << endl;
	programm(writable); // �������� ���� ������ ������ � ������ ������ �������

	in = fopen("buffer2.txt", "r");
	out = fopen("in.txt", "w");
	while ((d = fgetc(in)) != EOF) {
		fputc(d, out); //���������� ���������� buffer2.txt � in.txt
	}
	fclose(in);
	fclose(out);
}

/*_______________ ����� �������������� � ���������� ��� �������� _______________*/
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
	while ((c = fgetc(in)) != EOF) { /*��-��������� ��� ���������� ������� ���������� �� ��������� ����*/
		fputc(c, out); //����� �� ��������� ������ ����������� ���������� �������.
	}
	fclose(in);
	fclose(out);

	in2.open(name_for_word_add);
	in2 >> n;  // ������� ���������� ��� ������������ � �������.
	in2.close();

	out2.open(name_for_word3_add, ios::app); //�������� ����������
	if (mode == 1) {
		out2 << "�������� ������ ���������� ����� ����� ������ � ��������� ����.������ ������ �� ����� ������.������: ������ ��������� ����� ���� (�������� ���������� �������)" << endl;
	}

	if (mode == 2) {
		out2 << "�������� ������ ���������� ����� ����� �������� � ��������� ����.������ ������� �� ����� ������.������:������� ��������� �����1 ��������� �����2 (��� No ���� ��� ���)" << endl;
	}

	if (mode == 3) {
		out2 << "�������� ������ ���������� ����� ����� ����� � ��������� ����.������ ����� �� ����� ������.������: ����� �������� � ������.������ ��������� ����� ������� ��������� intfloat/float" << endl;
	}
	out2 << "_ _ _";
	out2.close();

	system(name_for_word3_add);// ������� ��� ������

	in2.open(name_for_word3_add);
	while (!in2.eof()) { // �� ����� ��������� �� ����� ������ ���������� ����� ����� �����
		char s = in2.get();
		if (s == '\n') {
			count_of_new_idioms++;
		}
	}
	in2.close();

	out2.open(name_for_word_add, ios::trunc);//������� �������
	out2.close();

	in2.open(name_for_word2_add);
	out2.open(name_for_word_add, ios::app);

	string line8, line9;

	out2 << n + count_of_new_idioms << endl; // �������� ����� ����� �����

	in2.ignore(255, '\n'); // ���������� �����-����������,������� ���� �����
	while (getline(in2, line8)) {
		out2 << line8 << endl;
	}
	in2.close();

	in2.open(name_for_word3_add);
	in2.ignore(255, '\n'); // ���������� ����������
	while (getline(in2, line9)) {
		out2 << transliter(1, line9) << endl;
	}

	in2.close();
	out2.close();

	cout << "���������� ���������." << endl;
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
					counter_of_parts++; // ������� ����� �������� ����� ������� - �������� ����� ���� � ������ �������.
				}
			}
			size_t pos = line8.find(separ); // ��������� ������ ������� �� ��� ������ � ����� ����
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

	cout << "�������������� ���������." << endl;
}
void addmode(char *writable) {
	ifstream file1;
	string line41, line42, text2, text3;
	int kolichestvo_perehodov_na_novuyu_stroku = 0;

	string line19 = "python clean.py " + string(writable) + " addmode " + to_string(1);//��������� ������, ������� ������� ��������� �����
	const char *line_19 = line19.c_str();
	system(line_19); /* ��������� clean.py */

	cout << "�������,��� ����� ��������: " << endl;
	cout << "	test-���������� ������" << endl;
	cout << "	idiom-���������� ��� �������������� ���� �����" << endl;
	cout << "	pr-���������� ��� �������������� ���� ���������" << endl;
	cout << "	num-���������� ��� �������������� ���� ������������" << endl;
	cin >> text2;

	if (text2 != "test" && text2 != "idiom" && text2 != "pr" && text2 !="num") {
		cout << "	 ������� ������� �������.������ ��������� ���������." << endl;
		system("pause");/*��������� �������� �� ������� ��������� �������.�������� � �������. */
		exit(0);
	}

	if (text2 == "test") {
		file1.open("test.txt");
		while (!file1.eof()) { // ������� ������� ��� ������ ���� � test.exe
			char s = file1.get();
			if (s == '\n') {
				kolichestvo_perehodov_na_novuyu_stroku++;
			}
		}
		file1.close();

		ofstream file_1;
		cout << "4.������� ����� �����������:" << endl; // ��������� ���� �����������.
		ofstream out("buffer1.txt", ios::app);
		out << "�������� ������ ���������� ����� ���� ���� � ��������� ����." << endl;
		out << "_ _ _"; // ������ �����,���� ����� ����� ����������� ����� ����������
		out.close();
		file1.open("buffer1.txt");
		file_1.open("test.txt", ios::app);
		system("buffer1.txt");// ��������� ���� ���� ������������ ����� ������������ ����.
		cout << "------------------------------------------------------------------------" << endl;

		cout << "5.���������� ������:" << endl;
		system("python converter.py buffer1.txt utf8"); // ������ ���������,����� �� ���� ������ ��������

		file1.ignore(255, '\n');//���������� ������ � �����������
		while (getline(file1, line41)) {
			string line2 = line41 + '\n';
			file_1 << line2; // ���������� ����� ����������� � ����� test.txt
		}
		file1.close();
		file_1.close();
		system("python converter.py test.txt utf8"); // ������ ���������,����� �� ���� ������ ��������

		file1.open("buffer1.txt");
		file_1.open("in.txt");
		file1.ignore(255, '\n');
		while (getline(file1, line41)) {
			file_1 << line41; // ����� ����������� ����� �� ������� ����.
		}
		file1.close();
		file_1.close();

		system("python converter.py in.txt utf8"); // ������ ���������,����� �� ���� ������ ��������

		cout << "6.������ ��������� � ����������� ������:" << endl;
		string path11 = "python ru-syntax-master.py " + string(writable) + " addmode " + to_string(1) + " " + to_string(1); // ��������� � ������ ���� ��� 
																												//������ �����������								
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
			getline(file1, line30); // �������� ������ �� �����,��� �������� ��������� ������� 
									// � �������� ���� � �����.��������
			file_1 << line30 << endl;
		}
		file1.close();
		file_1.close();

		cout << "7.������� ����������� ������:" << endl;
		system(name_for_test_add);// ������� ���� ��� ��������������
		string line431 = "cd " + string(writable) + "\\" + "bin" + "\\Malt\\" + "&& python converter.py test_"
			+ to_string(kolichestvo_perehodov_na_novuyu_stroku + 1) + ".txt" + " utf8";
		const char *line131 = line431.c_str();
		system(line131);// ������ ���������,����� �� ���� ������ ��������
		cout << "����_" + to_string(kolichestvo_perehodov_na_novuyu_stroku + 1) + " ������� ��������." << endl;
		cout << "------------------------------------------------------------------------" << endl;
	}

	if (text2 == "idiom" || text2 == "pr" || text2=="num") {
		cout << "	�������������� ��� ���������� edit/add ?" << endl;
		cin >> text3;

		if (text3 == "add" && text2 == "idiom") { // ����� ����������.
			add(writable, "dict.txt", 1);
		}

		if (text3 == "add" && text2 == "pr") { // ����� ����������.
			add(writable, "predlogipadeji.txt", 2);
		}

		if (text3 == "add" && text2 == "num") {
			edit(writable, "num.txt", 3);
		}

		if (text3 == "edit" && text2 == "idiom") {
			edit(writable, "dict.txt", 1);
		}

		if (text3 == "edit" && text2 == "pr") { // ����� ����������.
			edit(writable, "predlogipadeji.txt", 2);
		}

		if (text3 == "edit" && text2 == "num") { // ����� ����������.
			edit(writable, "num.txt", 3);
		}
	}
}

/*_________________		����� ������������ _________________________*/
void test_analyzer(char *writable) {
	ifstream file1;
	ofstream fout;
	int kolvo_testov = 0, kolichestvo_perehodov_na_novuyu_stroku = 0;

	//system("python converter.py test.txt utf8");
	system("python converter.py test_corpus.txt utf8");
	system("python converter.py in.txt utf8");

	//file1.open("test.txt");
	file1.open("test_corpus.txt");
	while (!file1.eof()) { // �� ����� ��������� �� ����� ������ ���������� ����� ������ � test.txt
		char s = file1.get();
		if (s == '\n') {
			kolichestvo_perehodov_na_novuyu_stroku++;
		}
	}
	file1.close();

	cout << "	������� ���������� ������.������������ ����� ������ " + to_string(kolichestvo_perehodov_na_novuyu_stroku) << endl;
	cin >> kolvo_testov;
	// ���� ��������� ����� ������ ������ ������������� �� ��������� ������ ���������			
	if (kolvo_testov > kolichestvo_perehodov_na_novuyu_stroku) {
		cout << "	��������� ������������ ����� ������.";
		system("pause");
		exit(0);
	}

	string line18 = "python clean.py " + string(writable) + " test " + to_string(kolvo_testov);//��������� ������, ������� ������� ��������� �����
	const char *line_18 = line18.c_str();
	system(line_18);

	cout << "4.���������� � ������������:" << endl;
	cout << "------------------------------------------------------------------------" << endl;

	for (int i = 0; i < kolvo_testov; i++) {
		string line5 = "python ru-syntax-master.py " + string(writable) + " test " + to_string(i + 1) + " " + to_string(kolvo_testov);
		const char *line_5 = line5.c_str();
		readfrombuffer(i, "test.txt"); // ��������� � in.txt �������� ����������� �� ������ �� test.txt
		system(line_5); //��������� 
		line_5 = "";
	}
}

/*________________________ �������� ���� ________________*/
int main(){
	FILE *in, *out;
	string text, answer;
	char c, d;
	char *temp;
	char name[MAX_PATH];
	int i = 0, v = 0;
	setlocale(LC_ALL, "");

	cout << " " << endl;
	cout << "	������������.��� �������������� ���������� ������� �������������� �����." << endl;
	cout << "	------------------------------------------------------------------------" << endl;
	cout << "1.1. ���� �� ���������� ru-syntax-master:" << endl;
	
	GetCurrentDirectory(MAX_PATH, name);
	cout << name << endl;
	
	temp = new char[strlen(name) + 10];
	int razm = strlen(name);
	while (i <= razm){ // ���� ��������� ������ ��������� ����.�� ��������� � ���� ��� ����
		if (name[i] == '\\'){
			if (name[i + 1] != '\\'){
				strcpy(temp, substr(name, i + 1));
				name[i + 1] = '\0';
				strcat(name, "\\");
				strcat(name, temp);
			}
			if (name[i + 1] == '\\'){ // ���� ���� �������, �� ����������.
				i = i + 1;
			}
		}
		i++;
	}

	cout << "1.2.������������ ������� (Y) ��� ������������� �� ��������� ����-������������ (N) ? Y/N" << endl;
	cin >> answer;
	if (answer == "N"){ // ������ ���� �� ���������.
		cout << "������ ����-������������ �� ���������." << endl;
		config_write(name);// ��������� config.ini � ������ ���������� ����
	}
	else{
		cout << "	������ ��� ������������ � ���������� ����-������������." << endl;
		cout << "	�� �������� ������ ����������� ������ � ���� ru-syntax-master � config.ini."<<endl;
	}
	cout << "------------------------------------------------------------------------";

	cout << " " << endl;
	cout << "2.�������:" << endl;
	cout << "	test ��� ������������ ���������." << endl;
	cout << "	auto ��� ������� � ����������� �������� ����� �������." << endl;
	cout << "	add ��� ���������� ������." << endl;
	cout << "	install ��� ��������� ����������� �����������.������������� ��������� ��� ������ �������." << endl;
	cout << "	options ��� �������� ��������� � ��������� ������������ ���������." << endl;
	cout << "	exit ��� ������ �� ���������." << endl;
	cin >> text;
	cout << "------------------------------------------------------------------------" << endl;

	if (text != "test" && text != "auto" && text != "add" && text != "install" && text!="options" && text != "exit"){
		cout << "	 ������� ������� �������.������ ��������� ���������." << endl;
		system("pause");/*��������� �������� �� ������� ��������� �������.�������� � �������. */
		exit(0);
	}

	string line16 = "python install.py " + string(name);//����������,����� ��������� ����� ����������,���� ����������� ��������� ����������� ���������� 
	const char *line_16 = line16.c_str();

	if (text == "install"){
		system(line_16); /* ��������� install.py */
		cout << "���������� ������" << endl; /* ��������� ������ */
		system("pause");
		exit(0);
	}

	if (text == "auto"){ //������ ����� � ������ ������ �������
		string line6 = "python clean.py " + string(name) + " auto " + to_string(1);//��������� ������, ������� ������� ��������� �����
		const char *line_6 = line6.c_str();
		system(line_6);
		autoanalyzer(name);
	}

	if (text == "test"){ // ���� ������ ����� ������������
		test_analyzer(name);
	}

	if (text == "add"){ /*����� ���������� ��� ������ (������������������) � ������ � ������� */
		addmode(name);
	}

	if (text == "options"){
		string line19 = "python clean.py " + string(name) + " addmode " + to_string(1);//��������� ������, ������� ������� ��������� �����
		const char *line_19 = line19.c_str();
		system(line_19); /* ��������� clean.py */
		string option = string(name) + +"\\" + "bin" + "\\Malt\\" + "\\options.ini";
		const char *options = option.c_str();

		system(options);
	}

	if (text == "exit"){
		exit(0);
	}
	
	cout << "���������� ������:" << endl;
	system("pause");
	return 0;
}




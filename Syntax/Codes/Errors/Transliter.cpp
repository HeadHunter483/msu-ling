#pragma warning(disable:4996)
#include <fstream>
#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <locale>

using namespace std;

bool is_latin(const string& s, const locale& l){
	for (string::const_iterator it = s.begin(); it != s.end(); ++it){
		const char ch = toupper(*it, l);
		if (!(ch >= 'A' && ch <= 'Z'))
			return false;
	}
	return true;
}
bool is_number(string str){ 
	bool numeric = false;
	if (str.find_first_not_of("0123456789.") == string::npos)
		numeric = true;
	return numeric;
}
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
void Transliteratelattorus(char* str, char* newStr){
	for (; *str != 0; str++){
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
void Transliterlattolat(char* str, char* newStr){
	for (; *str != 0; str++){
		switch (str[0])
		{
		case 'a': strcat(&newStr[0], "A"); break;
		case 'b': strcat(&newStr[0], "B"); break;
		case 'c': strcat(&newStr[0], "C"); break;
		case 'd': strcat(&newStr[0], "D"); break;
		case 'e': strcat(&newStr[0], "E"); break;
		case 'f': strcat(&newStr[0], "F"); break;
		case 'g': strcat(&newStr[0], "G"); break;
		case 'h': strcat(&newStr[0], "H"); break;
		case 'i': strcat(&newStr[0], "I"); break;
		case 'j': strcat(&newStr[0], "J"); break;
		case 'k': strcat(&newStr[0], "K"); break;
		case 'l': strcat(&newStr[0], "L"); break;
		case 'm': strcat(&newStr[0], "M"); break;
		case 'n': strcat(&newStr[0], "N"); break;
		case 'o': strcat(&newStr[0], "O"); break;
		case 'p': strcat(&newStr[0], "P"); break;
		case 'q': strcat(&newStr[0], "Q"); break;
		case 'r': strcat(&newStr[0], "R"); break;
		case 's': strcat(&newStr[0], "S"); break;
		case 't': strcat(&newStr[0], "T"); break;
		case 'u': strcat(&newStr[0], "U"); break;
		case 'v': strcat(&newStr[0], "V"); break;
		case 'w': strcat(&newStr[0], "W"); break;
		case 'x': strcat(&newStr[0], "X"); break;
		case 'y': strcat(&newStr[0], "Y"); break;
		case 'z': strcat(&newStr[0], "Z"); break;
		default: { char Temp[2] = { str[0], 0 }; strcat(&newStr[0], &Temp[0]); }
		}
	}
}
string transliter(int mode,string line){
	string line2;
	if (mode == 1){
		char y[100] = { 0 };

		char * writable = new char[line.size() + 1];
		copy(line.begin(), line.end(), writable);
		writable[line.size()] = '\0';
		Transliteraterustolat(writable, &y[0]);
		line2 = string(y);
	}

	if (mode == 2){
		int i = 0;
		char dy[100] = { 0 };
		while (i <= line.size()){
			if (line[i] == 'y' || line[i] == 'Y'){
				if (line[i + 1] == 'e'){
					line[i] = '�';
					line.erase(line.begin() + i + 1);
				}
				if (line[i + 1] == 'o'){
					line[i] = '�';
					line.erase(line.begin() + i + 1);
				}
				if (line[i + 1] == 'u'){
					line[i] = '�';
					line.erase(line.begin() + i + 1);
				}
				if (line[i + 1] == 'a'){
					line[i] = '�';
					line.erase(line.begin() + i + 1);
				}
				if (line[i + 1] == 'i'){
					line[i] = '�';
					line.erase(line.begin() + i + 1);
				}
			}
			if ((line[i] == 't' && line[i + 1] == 'c') || (line[i] == 'T' && line[i + 1] == 'c')){
				line[i] = '�';
				line.erase(line.begin() + i + 1);
			}

			if ((line[i] == 'm' && line[i + 1] == 'z') || (line[i] == 'M' && line[i + 1] == 'z')){
				line[i] = '�';
				line.erase(line.begin() + i + 1);
			}

			if ((line[i] == 't' && line[i + 1] == 'z') && (line[i] == 'T' && line[i + 1] == 'z')){
				line[i] = '�';
				line.erase(line.begin() + i + 1);
			}
			if ((line[i] == 'z' && line[i + 1] == 'h') || (line[i] == 'Z' && line[i + 1] == 'h')){
				line[i] = '�';
				line.erase(line.begin() + i + 1);
			}
			if ((line[i] == 's' && line[i + 1] == 'h' && line[i + 2] == 'c' && line[i + 3] == 'h') ||
				(line[i] == 'S' && line[i + 1] == 'h' && line[i + 2] == 'c' && line[i + 3] == 'h')){
				line[i] = '�';
				line.erase(line.begin() + i + 1);
				line.erase(line.begin() + i + 2);
				line.erase(line.begin() + i + 1);
			}

			if ((line[i] == 's' && line[i + 1] == 'h') || (line[i] == 'S' && line[i + 1] == 'h')){
				line[i] = '�';
				line.erase(line.begin() + i + 1);
			}

			if ((line[i] == 'c' && line[i + 1] == 'h') || (line[i] == 'C' && line[i + 1] == 'h')){
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

	if (mode == 3){
		char z[100] = { 0 };
		char * writable = new char[line.size() + 1];
		copy(line.begin(), line.end(), writable);
		writable[line.size()] = '\0';
		Transliterlattolat(writable, &z[0]);
		line2 = string(z);
	}



	return line2;
}
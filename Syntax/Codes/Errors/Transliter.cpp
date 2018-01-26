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
		case 'à': strcat(&newStr[0], "a"); break;
		case 'á': strcat(&newStr[0], "b"); break;
		case 'â': strcat(&newStr[0], "v"); break;
		case 'ã': strcat(&newStr[0], "g"); break;
		case 'ä': strcat(&newStr[0], "d"); break;
		case 'å': strcat(&newStr[0], "e"); break;
		case '¸': strcat(&newStr[0], "yo"); break;
		case 'æ': strcat(&newStr[0], "zh"); break;
		case 'ç': strcat(&newStr[0], "z"); break;
		case 'è': strcat(&newStr[0], "i"); break;
		case 'é': strcat(&newStr[0], "y"); break;
		case 'ê': strcat(&newStr[0], "k"); break;
		case 'ë': strcat(&newStr[0], "l"); break;
		case 'ì': strcat(&newStr[0], "m"); break;
		case 'í': strcat(&newStr[0], "n"); break;
		case 'î': strcat(&newStr[0], "o"); break;
		case 'ï': strcat(&newStr[0], "p"); break;
		case 'ð': strcat(&newStr[0], "r"); break;
		case 'ñ': strcat(&newStr[0], "s"); break;
		case 'ò': strcat(&newStr[0], "t"); break;
		case 'ó': strcat(&newStr[0], "u"); break;
		case 'ô': strcat(&newStr[0], "f"); break;
		case 'õ': strcat(&newStr[0], "x"); break;
		case 'ö': strcat(&newStr[0], "tc"); break;
		case '÷': strcat(&newStr[0], "ch"); break;
		case 'ø': strcat(&newStr[0], "sh"); break;
		case 'ù': strcat(&newStr[0], "shch"); break;
		case 'ú': strcat(&newStr[0], "tz"); break;
		case 'û': strcat(&newStr[0], "yi"); break;
		case 'ü': strcat(&newStr[0], "mz"); break;
		case 'ý': strcat(&newStr[0], "ye"); break;
		case 'þ': strcat(&newStr[0], "yu"); break;
		case 'ÿ': strcat(&newStr[0], "ya"); break;
		case 'À': strcat(&newStr[0], "A"); break;
		case 'Á': strcat(&newStr[0], "B"); break;
		case 'Â': strcat(&newStr[0], "V"); break;
		case 'Ã': strcat(&newStr[0], "G"); break;
		case 'Ä': strcat(&newStr[0], "D"); break;
		case 'Å': strcat(&newStr[0], "E"); break;
		case '¨': strcat(&newStr[0], "Yo"); break;
		case 'Æ': strcat(&newStr[0], "Zh"); break;
		case 'Ç': strcat(&newStr[0], "Z"); break;
		case 'È': strcat(&newStr[0], "I"); break;
		case 'É': strcat(&newStr[0], "Y"); break;
		case 'Ê': strcat(&newStr[0], "K"); break;
		case 'Ë': strcat(&newStr[0], "L"); break;
		case 'Ì': strcat(&newStr[0], "M"); break;
		case 'Í': strcat(&newStr[0], "N"); break;
		case 'Î': strcat(&newStr[0], "O"); break;
		case 'Ï': strcat(&newStr[0], "P"); break;
		case 'Ð': strcat(&newStr[0], "R"); break;
		case 'Ñ': strcat(&newStr[0], "S"); break;
		case 'Ò': strcat(&newStr[0], "T"); break;
		case 'Ó': strcat(&newStr[0], "U"); break;
		case 'Ô': strcat(&newStr[0], "F"); break;
		case 'Õ': strcat(&newStr[0], "X"); break;
		case 'Ö': strcat(&newStr[0], "Tc"); break;
		case '×': strcat(&newStr[0], "Ch"); break;
		case 'Ø': strcat(&newStr[0], "Sh"); break;
		case 'Ù': strcat(&newStr[0], "Shch"); break;
		case 'Ú': strcat(&newStr[0], "Tz"); break;
		case 'Û': strcat(&newStr[0], "Yi"); break;
		case 'Ü': strcat(&newStr[0], "Mz"); break;
		case 'Ý': strcat(&newStr[0], "Ye"); break;
		case 'Þ': strcat(&newStr[0], "Yu"); break;
		case 'ß': strcat(&newStr[0], "Ya"); break;
		default: { char Temp[2] = { str[0], 0 }; strcat(&newStr[0], &Temp[0]); }
		}
	}
}
void Transliteratelattorus(char* str, char* newStr){
	for (; *str != 0; str++){
		switch (str[0])
		{
		case 'a': strcat(&newStr[0], "à"); break;
		case 'b': strcat(&newStr[0], "á"); break;
		case 'v': strcat(&newStr[0], "â"); break;
		case 'g': strcat(&newStr[0], "ã"); break;
		case 'd': strcat(&newStr[0], "ä"); break;
		case 'e': strcat(&newStr[0], "å"); break;
		case 'yo': strcat(&newStr[0], "¸"); break;
		case 'zh': strcat(&newStr[0], "æ"); break;
		case 'z': strcat(&newStr[0], "ç"); break;
		case 'i': strcat(&newStr[0], "è"); break;
		case 'yu': strcat(&newStr[0], "þ"); break;
		case 'y': strcat(&newStr[0], "é"); break;
		case 'k': strcat(&newStr[0], "ê"); break;
		case 'l': strcat(&newStr[0], "ë"); break;
		case 'm': strcat(&newStr[0], "ì"); break;
		case 'n': strcat(&newStr[0], "í"); break;
		case 'o': strcat(&newStr[0], "î"); break;
		case 'p': strcat(&newStr[0], "ï"); break;
		case 'r': strcat(&newStr[0], "ð"); break;
		case 's': strcat(&newStr[0], "ñ"); break;
		case 't': strcat(&newStr[0], "ò"); break;
		case 'u': strcat(&newStr[0], "ó"); break;
		case 'f': strcat(&newStr[0], "ô"); break;
		case 'x': strcat(&newStr[0], "õ"); break;
		case 'tc': strcat(&newStr[0], "ö"); break;
		case 'ch': strcat(&newStr[0], "÷"); break;
		case 'sh': strcat(&newStr[0], "ø"); break;
		case 'shch': strcat(&newStr[0], "ù"); break;
		case 'tz': strcat(&newStr[0], "ú"); break;
		case 'yi': strcat(&newStr[0], "û"); break;
		case 'mz': strcat(&newStr[0], "ü"); break;
		case 'ye': strcat(&newStr[0], "ý"); break;
		case 'ya': strcat(&newStr[0], "ÿ"); break;
		case 'A': strcat(&newStr[0], "À"); break;
		case 'B': strcat(&newStr[0], "Á"); break;
		case 'V': strcat(&newStr[0], "Â"); break;
		case 'G': strcat(&newStr[0], "Ã"); break;
		case 'D': strcat(&newStr[0], "Ä"); break;
		case 'E': strcat(&newStr[0], "Å"); break;
		case 'Yo': strcat(&newStr[0], "¨"); break;
		case 'Zh': strcat(&newStr[0], "Æ"); break;
		case 'Z': strcat(&newStr[0], "Ç"); break;
		case 'I': strcat(&newStr[0], "È"); break;
		case 'Y': strcat(&newStr[0], "É"); break;
		case 'K': strcat(&newStr[0], "Ê"); break;
		case 'L': strcat(&newStr[0], "Ë"); break;
		case 'M': strcat(&newStr[0], "Ì"); break;
		case 'N': strcat(&newStr[0], "Í"); break;
		case 'O': strcat(&newStr[0], "Î"); break;
		case 'P': strcat(&newStr[0], "Ï"); break;
		case 'R': strcat(&newStr[0], "Ð"); break;
		case 'S': strcat(&newStr[0], "Ñ"); break;
		case 'T': strcat(&newStr[0], "Ò"); break;
		case 'U': strcat(&newStr[0], "Ó"); break;
		case 'F': strcat(&newStr[0], "Ô"); break;
		case 'X': strcat(&newStr[0], "Õ"); break;
		case 'Tc': strcat(&newStr[0], "Ö"); break;
		case 'Ch': strcat(&newStr[0], "×"); break;
		case 'Sh': strcat(&newStr[0], "Ø"); break;
		case 'Shch': strcat(&newStr[0], "Ù"); break;
		case 'Tz': strcat(&newStr[0], "Ú"); break;
		case 'Yi': strcat(&newStr[0], "Û"); break;
		case 'Mz': strcat(&newStr[0], "Ü"); break;
		case 'Ye': strcat(&newStr[0], "Ý"); break;
		case 'Yu': strcat(&newStr[0], "Þ"); break;
		case 'Ya': strcat(&newStr[0], "ß"); break;
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
					line[i] = 'ý';
					line.erase(line.begin() + i + 1);
				}
				if (line[i + 1] == 'o'){
					line[i] = '¸';
					line.erase(line.begin() + i + 1);
				}
				if (line[i + 1] == 'u'){
					line[i] = 'þ';
					line.erase(line.begin() + i + 1);
				}
				if (line[i + 1] == 'a'){
					line[i] = 'ÿ';
					line.erase(line.begin() + i + 1);
				}
				if (line[i + 1] == 'i'){
					line[i] = 'û';
					line.erase(line.begin() + i + 1);
				}
			}
			if ((line[i] == 't' && line[i + 1] == 'c') || (line[i] == 'T' && line[i + 1] == 'c')){
				line[i] = 'ö';
				line.erase(line.begin() + i + 1);
			}

			if ((line[i] == 'm' && line[i + 1] == 'z') || (line[i] == 'M' && line[i + 1] == 'z')){
				line[i] = 'ü';
				line.erase(line.begin() + i + 1);
			}

			if ((line[i] == 't' && line[i + 1] == 'z') && (line[i] == 'T' && line[i + 1] == 'z')){
				line[i] = 'ú';
				line.erase(line.begin() + i + 1);
			}
			if ((line[i] == 'z' && line[i + 1] == 'h') || (line[i] == 'Z' && line[i + 1] == 'h')){
				line[i] = 'æ';
				line.erase(line.begin() + i + 1);
			}
			if ((line[i] == 's' && line[i + 1] == 'h' && line[i + 2] == 'c' && line[i + 3] == 'h') ||
				(line[i] == 'S' && line[i + 1] == 'h' && line[i + 2] == 'c' && line[i + 3] == 'h')){
				line[i] = 'ù';
				line.erase(line.begin() + i + 1);
				line.erase(line.begin() + i + 2);
				line.erase(line.begin() + i + 1);
			}

			if ((line[i] == 's' && line[i + 1] == 'h') || (line[i] == 'S' && line[i + 1] == 'h')){
				line[i] = 'ø';
				line.erase(line.begin() + i + 1);
			}

			if ((line[i] == 'c' && line[i + 1] == 'h') || (line[i] == 'C' && line[i + 1] == 'h')){
				line[i] = '÷';
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
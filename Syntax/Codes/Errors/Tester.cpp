#include "Tester.h"
#include <string>
#include <string.h>
#include "Transliter.h"

using namespace std;

void tester(vector<Conll_line>&conll, vector <fromlettonum> &num) {
	ofstream fout("test_out.txt", ios::app);
	int root = 0;
	int count_root = 0;
	int error_count = 0;

fout << "S:";
for (size_t i = 0; i < conll.size(); i++) {
fout << conll[i].word << " (" + conll[i].id + " " + conll[i].parent << ") ";
}
fout << endl;

//for (size_t i = 0; i < conll.size(); i++) {
	//if (conll[i].word == ",") {

	//}
//}

for (size_t i = 0; i < conll.size(); i++) {
	if (conll[i].type_of_link == "root") {
		root = 1;
		count_root++;

		if (conll[i].parts_of_speech == "S" || conll[i].parts_of_speech == "SPRO") {
			fout << "Корень существительное	" << conll[i].id << endl;
			error_count++;
		}
	}
}

string line;
string line2;
size_t d = 0;

for (size_t i = 0; i < conll.size(); i++) {
for (size_t j = 0; j < conll.size(); j++) {
	if (atoi(conll[j].id.c_str()) == atoi(conll[i].parent.c_str())) {
		if ((conll[i].parts_of_speech == "S" || conll[i].parts_of_speech == "SPRO") && conll[j].parts_of_speech == "PARTCP") {
			if (conll[i].morph1.find("acc") == string::npos || conll[i].morph1.find("gen") == string::npos) {
				error_count++;
				fout << "Несоответствующий падеж у сущ,родитель которого причастие	" << conll[i].id << " " << conll[j].id << endl;
			}
		}

		if (conll[i].parts_of_speech == "V" && conll[j].parts_of_speech == "PARTCP") {
			error_count++;
			fout << "Причастие и глагол не могут быть связаны	" << conll[i].id + " " + conll[j].id << endl;
		}

		if ((conll[j].parts_of_speech == "V" && conll[j].morph1.find("imper") != string::npos) &&
			((conll[i].parts_of_speech == "S" || conll[i].parts_of_speech == "SPRO") && conll[i].morph1.find("nom") != string::npos)) {
			error_count++;
			fout << "Существительное в им.падеже.Родитель глагол в повелительном наклонении	" << conll[i].id + " " + conll[j].id << endl;
		}

		if (conll[i].type_of_link == "opred" && conll[j].parts_of_speech != "S") {
			error_count++;
			fout << "Определение отнесено нетуда	" << conll[i].id + " " + conll[j].id << endl;
		}

		if ((conll[i].parts_of_speech == "S" || conll[i].parts_of_speech == "SPRO") && (conll[j].parts_of_speech == "A" && conll[j].type_of_link != "root")) {
			error_count++;
			fout << "Прилагательное родитель существительного	" << conll[i].id + " " + conll[j].id << endl;
		}

		if (conll[i].parts_of_speech == "PR" && (conll[j].parts_of_speech != "S" && conll[j].parts_of_speech != "PARTCP" && conll[j].parts_of_speech != "V")) {
			error_count++;
			fout << "Неверная часть речи родителя предлога	" << conll[i].id + " " + conll[j].id << endl;
		}

		if ((conll[i].type_of_link == "predik" && conll[i].morph1.find("nom") == string::npos) && (conll[i].parts_of_speech == "S" || conll[i].parts_of_speech == "SPRO")) {
			error_count++;
			fout << "Неверно определено подлежащее.Предполагаемое существительное не в именительном падеже !" << conll[i].id + " " + conll[j].id << endl;
		}

		for (size_t b = 0; b < num.size(); b++) {
			if ((conll[i].original_form == num[b].letters) && conll[i].parts_of_speech != "NUM") {
				error_count++;
				fout << "Часть речи числа не числительное	" << conll[i].id + " " + conll[j].id << endl;
			}
		}
	}
}
}

fout << "Количество ошибок:	" << error_count << endl;

if (root == 0) {
	fout << "Нет корня	" << endl;
}

if (count_root > 1) {
	fout << "Несколько корней	" << endl;
}

fout << endl;
}


void tester2(vector<Conll_line>&conll) {
	int root = 0;
	int subj = 0;
	int dop = 0;
	int remember = 0;
	int mode = 0;

	// добавить меру

	for (size_t u = 0; u < conll.size(); u++) {
		if (conll[u].original_form == transliter(1, "который")) {
			root = u;
			conll[u].line_1 = "0";
			mode = 2;
		}
	}

	if (mode != 2) {
		for (size_t i = 0; i < conll.size(); i++) {
			if (conll[i].parts_of_speech == "V" || conll[i].parts_of_speech == "PARTCP") {
				conll[i].line_1 = "0";
				root = i;
			}
		}
	}

	if (mode == 2) {
		for (size_t i = 0; i < conll.size(); i++) {
			if (conll[i].parts_of_speech == "V" || conll[i].parts_of_speech == "PARTCP") {
				conll[i].line_1 = to_string(root + 1);
			}
		}
	}

	for (size_t i = 0; i < conll.size(); i++) {
		if (conll[i].parts_of_speech == "PR") {
			for (size_t j = 0; j < conll.size(); j++){
				if (conll[j].parts_of_speech == "V" || conll[j].parts_of_speech == "PARTCP") {
					conll[i].line_1 = to_string(j + 1);
				}
			}
		}
	}




	for (size_t i = 0; i < conll.size(); i++){
		if ((conll[i].parts_of_speech == "S" || conll[i].parts_of_speech == "SPRO") && conll[i].morph1.find("nom")!=string::npos){
			conll[i].line_1 = to_string(root + 1);
			subj = i;
		}
	}

	for (size_t i = 0; i < conll.size(); i++) {
		if ((conll[i].parts_of_speech == "S" || conll[i].parts_of_speech == "SPRO") && conll[i].morph1.find("gen") != string::npos) {
			for (size_t j = i; j > 0; j--) {
				if (conll[j].parts_of_speech == "S" && (conll[j].morph1.find("acc") != string::npos || conll[j].morph1.find("gen") != string::npos)) {
					remember = j;
				}
			}
			conll[i].line_1 = to_string(remember + 1);
			dop = i;
		}

		if ((conll[i].parts_of_speech == "S" || conll[i].parts_of_speech == "SPRO") && conll[i].morph1.find("acc") != string::npos) {
			conll[i].line_1 = to_string(root + 1);
			dop = i;
		}
	}

	for (size_t i = 0; i < conll.size(); i++) {
		cout << conll[i].word << endl;
		if (conll[i].original_form == "vne" && conll[i+1].morph1.find("loc") != string::npos) {
			conll[i + 1].line_1 = to_string(i + 1);
		}

	}






	//for (size_t i = 0; i < conll.size(); i++) {
		//if ((conll[i].parts_of_speech == "A" && (conll[i+1].parts_of_speech == "SPRO" || conll[i + 1].parts_of_speech == "S") && conll[i].morph1.find("gen") != string::npos)) {
			//conll[i].line_1 = to_string(dop + 1);
		//}
	//}

	for (size_t i = 0; i < conll.size(); i++) {
		if (conll[i].line_1 != "_") {
			if (conll[i].parent!=conll[i].line_1){
				cout << "Error:" << endl;
				cout << "Id of word is:" << to_string(i + 1) << endl;
			}
		}
	}
	
}
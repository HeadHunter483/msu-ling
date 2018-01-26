#include "utilities.h"
#include "Transliter.h"


int read_conll(vector<Conll_line>&conll, int &LINES_COUNT) {
	size_t num = 0;
	string str1, str2;
	const locale l("");
	ifstream file_in("out.txt");
	conll.resize(LINES_COUNT);
	while (file_in >> str1) {
		conll[num].id = str1;
		for (size_t i = 1; i < 8; i++) {
			file_in >> str1;
			switch (i) {
			case 1: {
				conll[num].word = str1;
				break;
			}
			case 2: {
				conll[num].original_form = transliter(1,str1);
				break;
			}
			case 3: {
				conll[num].parts_of_speech = transliter(3, str1);
				break;
			}
			case 4: {
				conll[num].morph1 = str1;
				while (true) {
					file_in >> str2;
					if (str1 == str2 || str2 == "punc") {
						conll[num].morph2.append(str2);
						while (true) {
							file_in >> str2;
							if (is_number(str2)) {
								conll[num].parent = str2;
								break;
							}
							conll[num].morph2.append(" ");
							conll[num].morph2.append(str2);
						}
						break;
					}
					conll[num].morph1.append(" ");
					conll[num].morph1.append(str2);
				}

			}
			case 5: {
				conll[num].type_of_link = transliter(1, str1);
				break;
			}
			case 6: conll[num].line_1 = "_"; break;
			case 7: break;
			}
		}
		num++;
	}
	return 0;
}
void change_id_and_number(vector<Conll_line>&conll, int where_is_delete) {
	int b1 = 0;
	int b2 = 0;
	string newid1 = "";
	string newid2 = "";
	for (size_t i = 0; i<conll.size(); i++) {
		newid1 = conll[i].parent;
		newid2 = conll[i].id;
		b1 = atoi(newid1.c_str());
b2 = atoi(newid2.c_str());
if (b1 >= where_is_delete) {
	b1 = atoi(newid1.c_str()) - 1;
	conll[i].parent = to_string(b1);
}
b1 = 0;

if (b2 > where_is_delete) {
	b2 = atoi(newid2.c_str()) - 1;
	conll[i].id = to_string(b2);
}
b2 = 0;
	}
}
void preparation(vector<Conll_line>&conll, string etap) {
	const locale l("");
	if (etap == "until") {
		for (size_t i = 0; i < conll.size(); i++) {
			conll[i].morph2 = "_";
		}
		/* Если слово и начальная форма не состоят полностью из латинских букв то переводим их в таковую */
		for (size_t i = 0; i < conll.size(); i++) {
			if (!is_latin(conll[i].word, l)) {
				conll[i].word = transliter(1, conll[i].word);
				conll[i].original_form = transliter(1, conll[i].original_form);
			}
			else {
				// если слово состоит из латинских букв и его длина больше 1 символа,то меняем регистр на верхний
				if (conll[i].word.length() > 1) {
					conll[i].word = transliter(3, conll[i].word);
					conll[i].original_form = transliter(3, conll[i].original_form);
					conll[i].parts_of_speech = "abr";
					conll[i].morph1 = "_";
					conll[i].morph2 = "_";
					conll[i].type_of_link = "atrib";
					conll[i].parent = to_string(i);
				}
				if (conll[i].word.length() == 1) {
					conll[i].parts_of_speech = "abr";
					conll[i].morph1 = "_";
					conll[i].morph2 = "_";
					conll[i].type_of_link = "atrib";
					conll[i].parent = to_string(i);
				}
			}
		}
	}

	if (etap == "after") {
		for (size_t i = 0; i < conll.size(); i++) {
			if (conll[i].parts_of_speech != "abr") {
				conll[i].word = transliter(2, conll[i].word);
				conll[i].original_form = transliter(2, conll[i].original_form);
			}
		}
	}
}
int findsomebrack(vector<Conll_line>&conll, size_t start) {
	size_t j = start;
	int open = 0;
	while (conll[j].word != ")") {
		if (conll[j].word == "(") {
			open = open + 1;
		}
		j = j + 1;
	}
	return open;
}
void hide(vector<Conll_line>&conll) {
	string line;
	string line2;
	size_t d = 0;
	for (size_t i = 0; i < conll.size(); i++) {
		for (size_t j = 0; j < conll.size(); j++) {
			if (atoi(conll[j].id.c_str()) == atoi(conll[i].parent.c_str())) {
				if (conll[d].type_of_link == "root") {
					conll[d].line_1 = "root";
					d = d + 1;
				}
				conll[d].line_1 = transliter(2, conll[j].word);
				d = d + 1;
				break;
			}
		}
	}
}

void put_in_the_end(vector<Conll_line>&conll, vector<Conll_line>&punct, vector<Conll_line>&punct2, int LINES_COUNT) {
	size_t b = 0;
	size_t h = 0;
	size_t c = 0;
	size_t start = 0;
	size_t end = 0;
	size_t remember = 0;

	punct.resize(10);
	punct2.resize(10);

	int j = 0;

	for (size_t i = 0; i < conll.size(); i++){
		if (conll[i].original_form == "." || conll[i].original_form == "-" || conll[i].original_form == "," || conll[i].original_form == "?" || conll[i].original_form == "!") {
			punct2[h].word = conll[i].word;
			punct2[h].parent = conll[i].parent;
			if (conll[i].original_form == ".") {
				conll.erase(conll.begin() + i);
			}
			h = h + 1;
		}
	}

	for (size_t i = 0; i < conll.size(); i++){
		if (conll[i].original_form == ","){
			start = i;
			for (size_t v = i; v < conll.size(); v++){
				if (conll[v].original_form == ",") {
					end = v;

					for (size_t c = start+1; c < end; c++){
						punct[b].id = conll[c].id;
						punct[b].word = conll[c].word;
						punct[b].original_form = conll[c].original_form;
						punct[b].morph1 = conll[c].morph1;
						punct[b].morph2 = conll[c].morph2;
						punct[b].parent = conll[c].parent;

						if (conll[c].original_form == transliter(1, "который")) {
							for (size_t d = c; d > 0; d--) {
								if (conll[d].parts_of_speech == "S" || conll[d].parts_of_speech == "SPRO") {
									punct[b].parent = conll[d].id;
								}
							}
						}

						punct[b].type_of_link = conll[c].type_of_link;
						punct[b].parts_of_speech = conll[c].parts_of_speech;
						b = b + 1;
					}
						
				}

			}

			
			if (end - start == 0) {
				break;
			}
			
			while (j <= end-start) {
				conll.erase(conll.begin() + start);
				change_id_and_number(conll, start);
				j = j + 1;
			}
		
		}
	}
}


void write_conll(vector<Conll_line>&conll, vector<Conll_line>&punct, vector<Conll_line>&punct2, int mode){
	//mode = 2;
	if (mode == 1) { /* для тестового мода */
		ofstream fout("final.txt");
		for (size_t i = 0; i < conll.size(); i++) {
			fout << conll[i].id << "	" << conll[i].word << "	" << conll[i].parts_of_speech
				<< "	" << conll[i].parent << "	"
				<< conll[i].type_of_link << endl;
		}
		

		for (size_t b = 0; b < punct2.size(); b++) {
		fout << conll.size()+b + 1 << "	" << punct2[b].word << "	" << "SENT	" << punct2[b].parent << "	punc	" << endl;
		}

		fout.close();
	}

	if (mode == 2) { /* дописываем все во временный файл */
		ofstream fout3;
		ofstream fout("final.txt");

		for (size_t i = 0; i < conll.size(); i++) {
			fout << conll[i].id << "	" << conll[i].word << "	" << conll[i].original_form << "	" << conll[i].parts_of_speech << "	" << conll[i].morph1 << "	"
				<< conll[i].morph2 << "	" << conll[i].parent << "	"
				<< conll[i].type_of_link << "	" << conll[i].line_1
				<< "	" << "_" << endl;
		}
		//fout.close();

		//fout.open("punc.txt");
		//for (size_t b = 0; b < punct.size(); b++) {
			//fout << b+1 << "	"<< transliter(2,punct[b].word) << "	" << transliter(2,punct[b].original_form) << "	" << punct[b].parts_of_speech << "	" << punct[b].morph1 << "	" 
				//<< punct[b].morph2 << "	" << punct[b].parent <<	"	" << punct[b].type_of_link << "	" << punct[b].line_1
				//<< "	" << "_" << endl;
		//}

		//for (size_t b = 0; b < punct2.size(); b++) {
			//fout << conll.size()+b + 1 << "	" << punct2[b].word << "	" << punct2[b].word << "	" << "SENT	SENT	SENT	" << punct2[b].parent << "	punc	_	_" << endl;
		//}


		fout.close();

		fout3.open("temp2.txt", ios::app);
		for (size_t i = 0; i < conll.size(); i++) {
			fout3 << conll[i].id << "	" << conll[i].word << "	" << conll[i].original_form << "	" << conll[i].parts_of_speech << "	" << conll[i].morph1 << "	"
				<< conll[i].morph2 << "	" << conll[i].parent << "	"
				<< conll[i].type_of_link << "	" << conll[i].line_1
				<< "	" << "_" << endl;
		}
		fout3 << endl;
		fout3.close();

		fout3.open("semantic.txt", ios::app);
		for (size_t i = 0; i < conll.size(); i++) {
			fout3 << conll[i].id << "	" << conll[i].word << "	" << conll[i].original_form << "	" << conll[i].parts_of_speech << "	" << conll[i].morph1 << "	"
				 << conll[i].parent << "	"
				<< conll[i].type_of_link << "	" << endl;
		}

		fout3 << endl;
		fout3.close();
	}

	
}
#pragma once
#pragma warning(disable:4996)
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include<algorithm>
#include "utilities.h"
#include "Transliter.h"

using namespace std;

void order(vector<Conll_line>&conll) {
	int num = 0;
	int num2 = 0;
	int mode = 0;
	string parent;
	string id;
	int flag = 1;
	string root = "5";

	// если существительное в именительном падеже ставим его на первое место //
	for (size_t i = 0; i < conll.size(); i++){
		if (conll[i].parts_of_speech == "S" && conll[i].morph1.find("nom") != string::npos){
			num = i;
			parent = conll[i].id;
			flag = 1;
			mode = 1;
		}
		if (conll[i].parent == parent){
			conll[i].parent = "100";
		}
	}

	if (flag = 1){
		iter_swap(conll.begin(), conll.begin() + num);
	}

	// Глагол или причастие на второе место
	for (size_t i = 0; i < conll.size(); i++){
		if (conll[i].parts_of_speech == "PARTCP" || conll[i].parts_of_speech == "V"){
			num = i;
			parent = conll[i].id;
			flag = 2;
		}
if (conll[i].parent == parent) {
	conll[i].parent = "200";
}
	}

	if (flag = 2) {
		if (mode == 1) {
			iter_swap(conll.begin() + 1, conll.begin() + num);
		}
		if (mode == 0) {
			iter_swap(conll.begin(), conll.begin() + num);
		}
	}

	for (size_t i = 0; i < conll.size(); i++) {
		if (conll[i].parts_of_speech == "PR" && conll[i].word == "S") {
			num = i;
			parent = conll[i].id;
			flag = 3;
		}
		if (conll[i].parent == parent) {
			conll[i].parent = "300";
		}
	}

	if (flag = 3) {
		if (mode == 1) {
			iter_swap(conll.begin() + 2, conll.begin() + num);
		}
	}


	//for (size_t i = 0; i < conll.size(); i++) {
		//if (conll[i].parts_of_speech == "S" && conll[i].morph1.find("gen") != string::npos) {
			//num = i;
			//parent = conll[i].id;
			//flag = 4;
		//}
		//if (conll[i].parent == parent){
			//conll[i].parent = "400";
		//}
	//}

	//if (flag = 4){
		//iter_swap(conll.begin() + 3, conll.begin() + num);
	//}

//for (size_t i = 0; i < conll.size(); i++) {
	//if (conll[i].parts_of_speech == "A") {
		//num = i;
		//parent = conll[i].id;
		//flag = 5;
	//}
	//if (conll[i].parent == parent) {
		//conll[i].parent = "500";
	//}
//}

//if (flag = 5) {
	//iter_swap(conll.begin() + 3, conll.begin() + num);
//}











//parent.swap(conll[3].id);

}


void razbit(vector<Conll_line>&conll, vector<Conll_line>&punct, vector<Conll_line>&punct2, int LINES_COUNT) {
	size_t b = 0;
	size_t h = 0;
	size_t c = 0;
	size_t start = 0;
	size_t end = 0;
	size_t remember = 0;
	ofstream file_buf("file_buf.txt");
	int mode = 0;
	vector<Conll_line>pun;
	vector<Conll_line>pun3;

	punct.resize(20);
	pun.resize(20);
	pun3.resize(20);

	int j = 0;
	size_t r = 0;
	size_t r1 = 0;
	size_t r2 = 0;

	for (size_t i = 0; i < conll.size(); i++) {
		if (conll[i].original_form == "," && conll[i+1].word=="chto"){// докажите что
			for (size_t u = 0; u < i; u++){
				pun[u].word = conll[u].word;
			}
			for (size_t u = i + 1; u < conll.size(); u++){
				punct[u].word = conll[u].word;
				if (r1 == 0 && conll[u].original_form == ","){
					r1 = u;
				}
				if (r1 > 0 && conll[u].original_form == ","){
					r2 = u;
				}
			}

			for (size_t u = i + 1; u < punct.size(); u++){
				if (u > r1 && u < r2){
					pun3[u].word = punct[u].word;
				}
			}

			break;
		}
	}

	j = 0;
	while (j <= r2 - r1) {
		punct.erase(punct.begin() + r1);
		j = j + 1;
	}


			//for (size_t j = 0; j < i+2; j++) {
				//punct[j].word = conll[j].word;
				//remember = i;
			//}
			//punct[j + 3].word = "\n";
			//mode = 1;
		//}
		//if (mode == 1){
			//r = j + 4;
			//for(size_t j=i+1;conll[j].word!=",";j++){
				//cout << conll[j].word << "b"<<endl;
				//punct[r].word = conll[j].word;
				//r = r + 1;
			//}
		//}
	//}





	
	/*for (size_t i = 0; i < conll.size(); i++) {
		if (conll[i].original_form == ","){
			remember = i;
			for (size_t j = 0; j <= i; j++){
				punct[j].word = conll[j].word;
			}
			if (conll[i + 1].original_form == "chto"){
				punct[j+1].word = conll[i].word;
				punct[j + 2].word = conll[i + 1].word;
			}
			break;
		}
	}

	for (size_t i = 0; i < punct.size(); i++){
		file_buf << punct[i].word << endl;
	}

	punct.resize(20);

	size_t d = 0;

	for (size_t i = remember+2; i < conll.size(); i++) {
		if (conll[i].original_form == ","){
			j = i + 1;
			while (conll[j].original_form != ","){
				cout << conll[j].original_form << endl;
				punct[d].word = conll[j].word;
				d = d + 1;
				j = j + 1;
			}
		}
	}

	file_buf << "part_2" << endl;
	*/

	//cout << r1 << " " << r2 << endl;
	for (size_t i = 0; i < pun.size(); i++) {
		file_buf << pun[i].word << endl;
	}

	for (size_t i = 0; i < punct.size(); i++) {
		file_buf << punct[i].word << endl;
	}


	for (size_t i = 0; i < pun3.size(); i++) {
		file_buf << pun3[i].word << endl;
	}


}
#pragma warning(disable:4996)
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <locale.h>

using namespace std;

bool isnum(string str) {
	bool numeric = false;
	if (str.find_first_not_of("0123456789.") == string::npos)
		numeric = true;
	return numeric;
}

int main(int argc, char* argv[]){
	ifstream file1, file2;
	ofstream file3("results.txt", ios::app);
	char string1[256], string2[256];
	int j = 0;
	int kolichestvo_vseh_slov = 0;
	int parent_link = 0;
	int net_oshibok = 0;
	int procent1 = 0;
	int i = 0;

	if (argc < 4){
		return -1;
	}

	file1.open(argv[1]);
	file2.open(argv[2]);

	while (file1.getline(string1, 256) && file2.getline(string2, 256)) {
		kolichestvo_vseh_slov++;
	}

	file1.close();
	file2.close();

	file1.open(argv[1]);
	file2.open(argv[2]);

	int d = 0;
	int l = 0;

	while (file1.getline(string1, 256) && file2.getline(string2, 256)) {
		if (strcmp(string1, string2) == 0) {
			d = d + 1;
		}
		if (strcmp(string1, string2) != 0) {
			file3 << string1 << endl;
			file3 << string2 << endl;
			j = j + 1;
		}

		l++;
		if (d == kolichestvo_vseh_slov) {
			file3 << "No errors" << endl;
			net_oshibok = 1;
		}
		else {
			if (l == kolichestvo_vseh_slov) {
				procent1 = (100 / (kolichestvo_vseh_slov));
				file3 << "Total amount of lines " << kolichestvo_vseh_slov << endl;
				file3 << "Total amount of lines,where error " << j << endl;
				file3 << "Accurancy of Parent+Type_of_link: " << to_string(100 - j*procent1) << "\%" << endl;
			}
		}
	}

	file1.close();
	file2.close();

	if (net_oshibok != 1) {
		file1.open(argv[1]);
		file2.open(argv[2]);
		
		while (file1.getline(string1, 256) && file2.getline(string2, 256)) {
			string strNum;
			string strNum2;
			for (int i = 0; i < string(string1).length(); i++) {
				while (isnum(string(1, string1[i])) && isnum(string(1, string2[i]))) {
					strNum += string1[i];
					strNum2 += string2[i];
					int value = atoi(strNum.c_str());
					int value2 = atoi(strNum2.c_str());
					if (value != value2) {
						parent_link = parent_link + 1;
					}
					i++;
				}
				strNum = "";
				strNum2 = "";
			}
		}
		file3 << "Parent_links error  " << parent_link << endl;
		file3 << "Accurancy of Parent_links: " << to_string(100 - (parent_link)*procent1) << "\%" << endl;
		file1.close();
		file2.close();
	}

	int schetchik = 0;
	if (net_oshibok != 1) {
		file1.open(argv[1]);
		file2.open(argv[2]);
	
		int h = 0;
		while (file1.getline(string1, 256) && file2.getline(string2, 256)) {
			char * words[30] = { "agent", "predl", "predik", "atrib", "appoz", "opred", "kvaziagent", "1-kompl", "root", "punc","2-kompl","3-kompl",
				"4-kompl", "5-kompl", "Subj-Predic", "relyat", "addit", "dlitel", "kompl-appoz", "obst","electiv",
				"vvodn", "sootnos", "analit", "sochin", "soch-soyuzn", "podch - soyuzn", "sent-soch", "ogranich", "prolept" };
			while (h < 29) {
				if (strstr(string1, words[h]) != NULL && strstr(string2, words[h]) != NULL) {
					break;
				}
				if (strstr(string1, words[h]) != NULL && strstr(string2, words[h]) == NULL) {
					schetchik++;
				}
				if (h == 28) {
					h = 0;
					break;
				}
				h++;
			}
		}

		file3 << "Type_of_links error  " << schetchik << endl;
		file3 << "Accurancy of Type_of_link: " << to_string(100 - (schetchik)*procent1) << "\%" << endl;
		file1.close();
		file2.close();
	}

	file3 << "Test_" + string(argv[3]) + " is finished." << endl;
	file3 << endl;
	file3.close();
	return 0;
}


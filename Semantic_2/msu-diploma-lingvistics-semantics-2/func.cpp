#include "func.h"

const string config_filename = "config.ini";

void parse_ini() {
	int a = 0, b = 0;
	string line = "";
	string setting = "", setting_val = "";
	int after_setting_val = 0;
	ifstream fin(config_filename);

	while (getline(fin, line)) {
		if (line == "\n")
			continue;
		setting = "";
		setting_val = "";
		for (size_t i = 0; i < line.size(); i++) {
			if (line[i] == '=') {	// reading line until symbol '='
				after_setting_val = i + 1;
				break;
			}
			setting.push_back(line[i]);
		}
		for (size_t i = after_setting_val; i < line.size(); i++) {	// reading line until '\n'
			if (line[i] == '\n')
				break;
			setting_val.push_back(line[i]);
		}
		config[setting] = setting_val;	// writing setting value to a setting field in config map
	}
}

string strtolower(const string& str) {
	string result = "";
	for (size_t i = 0; i < str.size(); i++) {
		result.push_back(tolower(str[i]));
	}
	return result;
}

string strtoupper(const string& str) {
	string result = "";
	for (size_t i = 0; i < str.size(); i++) {
		result.push_back(toupper(str[i]));
	}
	return result;
}

bool file_is_empty(std::ifstream& pFile) {
	return pFile.peek() == std::ifstream::traits_type::eof();
}

vector<string> str_split(const string&str, char splitter) {
	vector<string> result;
	string tmp = "";
	size_t elem_start = 0;
	while (elem_start < str.size()) {
		tmp = "";
		for (size_t i = elem_start; i < str.size(); i++) {
			if (str[i] == splitter || str[i] == '\n' || str[i] == '\0') {	// conditions of split
				elem_start = i + 1;
				break;
			}
			tmp.push_back(str[i]);
			if (i == str.size() - 1) {
				elem_start = i + 1;
				break;
			}
		}
		result.push_back(tmp);	// pushing element
	}
	return result;
}

int read_args(const int argc, const char**argv) {
	vector<string>args(argc, "");
	for (int i = 0; i < argc; i++) {
		for (size_t j = 0; j < strlen(argv[i]); j++)
			args[i].push_back(argv[i][j]);			// pushing argv into vector of strings
	}
	return 0;
}

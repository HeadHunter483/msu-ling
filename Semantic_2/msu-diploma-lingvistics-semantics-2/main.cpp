#include "semantics.h"

unordered_map<string, string> config;

int test() {
	int ret_code = 0;
	ifstream fin(config["fin"]);
	ofstream fout(config["fout"]);
	string result = "";
	vector<string>token;
	SemTree tree(fin);

	cout << "starting test sequence...\n";

	tree.analysis();

	result = tree.buildResult();

	cout << "\nresult of semantic analysis:\n\n" << result;

	cout << "test finished\n";

	fout << result;

	// system("pause");
	fin.close();
	fout.close();
	return ret_code;
}

int main(const int argc, const char**argv) {
	setlocale(LC_ALL, "rus_rus.1251");
	parse_ini();
	if (read_args(argc, argv)) {
		return ARGS_ERROR;
	}
	return test();
}
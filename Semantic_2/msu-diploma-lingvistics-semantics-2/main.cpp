#include "semantics.h"

unordered_map<string, string> config;

int test() {
	int ret_code = 0;
	ifstream fin(config["fin"]);
	ofstream fout(config["fout"]);
	vector<string>token;
	SemTree tree(fin);

	cout << "starting test sequence...\n";

	tree.analysis();

	cout << "\nresult of semantic analysis:\n\n" << tree.buildResult();

	cout << "test finished\n";

	system("pause");

	return ret_code;
}

int main(const int argc, const char**argv) {
	setlocale(LC_ALL, "rus_rus.1251");
	parse_ini();
	read_args(argc, argv);
	return test();
}
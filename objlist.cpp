#include "header.h"

int main(int argc, char *argv[])
{
	int ch;
	size_t i;
	int flag = 0; /* mark whether with "-l" */
	opterr = 0;
	uid_t user_id;
	string uname; /* user name */
	string file_name; /* file name */
	string tmp;
	vector<string> user_object_parse;
	vector<string> user_group_parse; /* a user can at most in 10 groups */
	vector<string> usergroup;
	vector<string> userobject;
	vector<string> objlist;
	ifstream file;

	/* input commands */
	while ((ch = getopt(argc, argv, "l")) != -1) {
		switch (ch) {
		case 'l':
			flag = 1;
			break;
		default:
			cerr << "command not found" << endl;
			return 1;
		}
	}
	if ((argc > 2) || ((flag == 0) && (argc == 2))) {
		cerr << "command not found" << endl;
		return 1;
	}
	user_id = getuid();
	uname = to_string(user_id);
	/* check if the user has the privilege to access this file */
	file.open("user_object");
	if (!file) {
		cerr << "file can not open" << endl;
		return 1;
	}
	while (!file.eof()) {
		getline(file, tmp);
		if (tmp.length() != 0)
			userobject.push_back(tmp);
	}
	file.close();
	for (i = 0; i < userobject.size(); i++) {
		char *input_command = new char[userobject[i].length() + 1];
		strcpy(input_command, userobject[i].c_str());
		parse_command(input_command, user_object_parse);
		if (user_object_parse[1] == uname)
			objlist.push_back(user_object_parse[0]);
		delete[] input_command;
	}
	if (objlist.size() == 0) {
		cerr << "user has no file in list" << endl;
		return 1;
	}

	/* if the user has an object list, then print it */
	for (i = 0; i < objlist.size(); i++) {
		if (flag == 1) {
			file_name = uname + "-" + objlist[i];
			file.open(file_name.c_str(), ios::ate | ios::binary);
			if (!file) {
				cerr << "file can not open" << endl;
				return 1;
			}
			cout << uname << " " << file.tellg() << " ";
			file.close();
		}
		cout << objlist[i] << endl;
	}
	return 0;
}


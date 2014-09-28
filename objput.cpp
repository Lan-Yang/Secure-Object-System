#include "header.h"
using namespace std;
int main(int argc, char *argv[])
{
	int ch;
	opterr = 0;
	string uname; //user name
	string gname; //group name
	string object_name; //object name
	string initial_acl;
	string file_name;
	ofstream file;
	char tmp;
	//input commands
	while ((ch = getopt(argc, argv, "u:g:")) != -1) {
		switch (ch) {
		case 'u':
			uname = optarg;
			break;
		case 'g':
			gname = optarg;
			break;
		default:
			cerr << "command not found" << endl;
			return 1;
		}
	}
	//check commands
	if ((gname.empty()) || (uname.empty()) || (argc != 6)) {
		cerr << "command not found" << endl;
		return 1;
	}
	object_name = argv[5];
	//check user name, group name, object name whether valid
	if (!check_name_valid(uname)) {
		cerr << "user name not valid" << endl;
		cerr << "only letters, numbers, underscore are allowed" << endl;
		return 1;
	}
	if (!check_name_valid(gname)) {
		cerr << "group name not valid" << endl;
		cerr << "only letters, numbers, underscore are allowed" << endl;
		return 1;
	}
	if (!check_name_valid(object_name)) {
		cerr << "object name not valid" << endl;
		cerr << "only letters, numbers, underscore are allowed" << endl;
		return 1;
	}
	//check user name, group name whether exist
	if (!check_user_group(uname, gname))
		return 1;
	//read file from stdin, write its content to object
	file_name = uname + "-" + object_name;
	file.open(file_name.c_str(), ios::out | ios::trunc);
	while (cin.peek() != char_traits<char>::eof()) {
		tmp = cin.get();
		file << tmp;
	}
	file.close();
	//initiate corresponding acl object
	initial_acl = uname + "-" + object_name + "-acl";
	file.open(initial_acl.c_str(), ios::out | ios::trunc);
	file << uname << ".* rwxpv";
	file.close();
	//record the information into user_object
	file.open("user_object", ios::ate | ios::app);
	if (!file)
		cerr << "file can not open" << endl;
	else
		file << object_name << " " << uname << " " << gname << '\n';
	file.close();
	return 0;
}


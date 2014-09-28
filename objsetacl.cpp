#include "header.h"

int main(int argc, char *argv[])
{
	int ch;
	opterr = 0;
	string uname; /* user name */
	string uname2;
	string gname; /* group name */
	string object_name; /* object name */
	string acl_file_name;
	vector<string> object_name_parse;
	FILE *fout;
	ofstream file;
	char tmp;

	/* input commands */
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
	/* check commands */
	if ((gname.empty()) || (uname.empty()) || (argc != 6)) {
		cerr << "command not found" << endl;
		return 1;
	}
	object_name = argv[5];
	/* check user name, group name whether valid */
	if (!check_name_valid(uname)) {
		help();
		return 1;
	}
	if (!check_name_valid(gname)) {
		help();
		return 1;
	}
	/* check user name, group name whether exist */
	if (!check_user_group(uname, gname))
		return 1;
	/* check the condition that one references other users' objects */
	if (check_reference(object_name)) {
		parse_command(object_name, object_name_parse);
		uname2 = object_name_parse[0];
		object_name = object_name_parse[1];
		acl_file_name = uname2 + "-" + object_name + "-acl";
	} else {
		acl_file_name = uname + "-" + object_name + "-acl";
	}
	if (uname == uname2) {
		cerr << "command not found" << endl;
		return 1;
	}
	/* check object name whether valid */
	if (!check_name_valid(object_name)) {
		help();
		return 1;
	}

	/* check user whether have "p" permission to acl */
	if (!check_acl(acl_file_name, uname, gname, "p")) {
		cerr << "no permission to change acl" << endl;
		return 1;
	}
	/* read file from stdin, write its content to object */
	fout = fopen(acl_file_name.c_str(), "w");


	//file.open(acl_file_name.c_str(), ios::out | ios::trunc);
	if (fout == NULL) {
		cerr << "file can not open" << endl;
		return 1;
	}

	while ((tmp = getchar()) != EOF)
		fputc(tmp, fout);
	fclose(fout);
	return 0;
}


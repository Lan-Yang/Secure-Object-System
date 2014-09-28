#include "header.h"

int main(int argc, char *argv[])
{
	int ch;
	int flag = 0;
	opterr = 0;
	string uname; /* user name */
	string gname; /* group name */
	string object_name; /* object name */
	string initial_acl;
	string file_name;
	string obj_user_group;
	string tmp_line;
	FILE *fout;
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
	/* check user name, group name, object name whether valid */
	if (!check_name_valid(uname)) {
		help();
		return 1;
	}
	if (!check_name_valid(gname)) {
		help();
		return 1;
	}
	if (!check_name_valid(object_name)) {
		help();
		return 1;
	}
	/* check user name, group name whether exist */
	if (!check_user_group(uname, gname))
		return 1;
	/* read file from stdin, write its content to object */
	file_name = uname + "-" + object_name;
	fout = fopen(file_name.c_str(), "w");
	if (fout == NULL) {
		cerr << "file can not open" << endl;
		return 1;
	}
	while ((tmp = getchar()) != EOF)
		fputc(tmp, fout);
	fclose(fout);

	/* initiate corresponding acl object */
	initial_acl = uname + "-" + object_name + "-acl";
	fout = fopen(initial_acl.c_str(), "w");
	if (fout == NULL) {
		cerr << "file can not open" << endl;
		return 1;
	}
	fprintf(fout, "%s.* rwxpv", uname.c_str());
	fclose(fout);
	obj_user_group = object_name + " " + uname + " " + gname;
	ifstream file2;
	file2.open("user_object");
	while (!file2.eof()) {
		getline(file2, tmp_line);
		if (tmp_line == obj_user_group)
			flag = 1;
	}
	file2.close();
	if (flag == 0) {
		fout = fopen("user_object", "w+");
		if (!fout)
			cerr << "file can not open" << endl;
		else
			fprintf(fout, "%s %s %s \n", object_name.c_str(), uname.c_str(), gname.c_str());
		fclose(fout);
	}
	return 0;
}


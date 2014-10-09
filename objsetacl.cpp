#include "header.h"

int main(int argc, char *argv[])
{
	opterr = 0;
	uid_t user_id;
	string uname; /* user name */
	string uname2;
	gid_t group_id;
	string gname; /* group name */
	string object_name; /* object name */
	string acl_file_name;
	vector<string> object_name_parse;
	FILE *fout;
	ofstream file;
	char tmp;

	user_id = getuid();
	group_id = getgid();
	uname = to_string(user_id);
	gname = to_string(group_id);
	/* check commands */
	if (argc != 2) {
		cerr << "command not found" << endl;
		return 1;
	}
	object_name = argv[1];
	/* check the condition that one references other users' objects */
	if (check_reference(object_name)) {
		parse_command(object_name, object_name_parse);
		uname2 = object_name_parse[0];
		object_name = object_name_parse[1];
		acl_file_name = uname2 + "-" + object_name + "-acl";
		/* check referenced user name whether valid?? */
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


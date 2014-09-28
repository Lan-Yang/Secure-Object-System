#include "header.h"

int main(int argc, char *argv[])
{
	int ch;
	opterr = 0;
	string uname; /* user name */
	string uname2;
	string gname; /* group name */
	string access;
	string object_name; /* object name */
	string acl_name;
	vector<string> object_name_parse;

	/* input commands */
	while ((ch = getopt(argc, argv, "u:g:a:")) != -1) {
		switch (ch) {
		case 'u':
			uname = optarg;
			break;
		case 'g':
			gname = optarg;
			break;
		case 'a':
			access = optarg;
			break;
		default:
			cerr << "command not found" << endl;
			return 1;
		}
	}
	/* check commands */
	if ((gname.empty()) || (uname.empty()) || (argc != 8)) {
		cerr << "command not found" << endl;
		return 1;
	}
	object_name = argv[7];
	/* check user name, group name whether valid */
	if (!check_name_valid(uname)) {
		help();
		return 1;
	}
	if (!check_name_valid(gname)) {
		help();
		return 1;
	}
	/* check the condition that one references other users' objects */
	if (check_reference(object_name)) {
		parse_command(object_name, object_name_parse);
		uname2 = object_name_parse[0];
		object_name = object_name_parse[1];
		acl_name = uname2 + "-" + object_name + "-acl";
	} else {
		acl_name = uname + "-" + object_name + "-acl";
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
	/* check user name, group name whether exist */
	if (!check_user_group(uname, gname))
		return 1;
	/* check allowed or denied */
	if (!check_acl(acl_name, uname, gname, access))
		cerr << "denied" << endl;
	else
		cout << "allowed" << endl;
	return 0;
}


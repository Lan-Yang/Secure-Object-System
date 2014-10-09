#include "header.h"

int main(int argc, char *argv[])
{
	int ch;
	opterr = 0;
	uid_t user_id;
	string uname; /* user name */
	string uname2;
	gid_t group_id;
	string gname; /* group name */
	string access;
	string object_name; /* object name */
	string acl_name;
	vector<string> object_name_parse;

	/* input commands */
	while ((ch = getopt(argc, argv, "a:")) != -1) {
		switch (ch) {
		case 'a':
			access = optarg;
			break;
		default:
			cerr << "command not found" << endl;
			return 1;
		}
	}
	/* check commands */
	if (argc != 4) {
		cerr << "command not found" << endl;
		return 1;
	}
	object_name = argv[3];
	user_id = getuid();
	group_id = getgid();
	uname = to_string(user_id);
	gname = to_string(group_id);
	/* check the condition that one references other users' objects */
	if (check_reference(object_name)) {
		parse_command(object_name, object_name_parse);
		uname2 = object_name_parse[0];
		object_name = object_name_parse[1];
		acl_name = uname2 + "-" + object_name + "-acl";
		/* check referenced user name whether valid?? */
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
	/* check allowed or denied */
	if (!check_acl(acl_name, uname, gname, access))
		cerr << "denied" << endl;
	else
		cout << "allowed" << endl;
	return 0;
}


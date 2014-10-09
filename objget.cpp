#include "header.h"

int main(int argc, char *argv[])
{
	opterr = 0;
	uid_t user_id;
	string uname; 
	string uname2; /* referenced user name */
	gid_t group_id;
	string gname; /* group name */
	string object_name;
	string file_name;
	string acl_name;
	string tmp;
	vector<string> usergroup;
	vector<string> userobject;
	vector<string> acl;
	vector<string> object_name_parse;
	ifstream file;

	/* input commands */
	if (argc != 2) {
		cerr << "command not found" << endl;
		return 1;
	}
	object_name = argv[1];
	user_id = getuid();
	uname = to_string(user_id);
	group_id = getgid();
	gname = to_string(group_id);
	/* check the condition that one references other users' objects */
	if (check_reference(object_name)) {
		parse_command(object_name, object_name_parse);
		uname2 = object_name_parse[0];
		object_name = object_name_parse[1];
		file_name = uname2 + "-" + object_name;
	/* check referenced user name whether valid?? */
	} else {
		file_name = uname + "-" + object_name;
	}
	/* check object name whether valid */
	if (!check_name_valid(object_name)) {
		help();
		return 1;
	}
	/* check user's privilege to this file */
	acl_name = file_name + "-acl";
	if (!check_acl(acl_name, uname, gname, "r")) {
		cerr << "no permission to read" << endl;
		return 1;
	}
	/* if the user has access, then print the object to stdout */
	file.open(file_name.c_str());
	if (!file) {
		cerr << "file can not open" << endl;
		return 1;
	}
	while (!file.eof()) {
		getline(file, tmp);
		if (tmp.length() != 0) /* avoid empty string pushed to vector */
			cout << tmp << endl;
	}
	file.close();
	return 0;
}


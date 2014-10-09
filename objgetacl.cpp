#include "header.h"

int main(int argc, char *argv[])
{
	size_t i;
	opterr = 0;
	uid_t user_id;
	string uname; /* user name */
	string uname2;
	gid_t group_id;
	string gname; /* group name */
	string object_name; /* object name */
	string acl_name;
	string tmp;
	vector<string> acl_parse; /* user.group ops */
	vector<string> object_name_parse;
	vector<string> acl;
	ifstream file;

	/* read command and parse command */
	if (argc != 2) {
		cerr << "command not found" << endl;
		return 1;
	}
	object_name = argv[1];
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
	/* check user whether have permission to view acl */
	file.open(acl_name.c_str());
	if (!file) {
		cerr << "file can not open" << endl;
		return 1;
	}
	while (!file.eof()) {
		getline(file, tmp);
		if (tmp.length() != 0) /* avoid empty string push to vector */
			acl.push_back(tmp);
	}
	file.close();
	for (i = 0; i < acl.size(); i++) {
		parse_command(acl[i], acl_parse);
		if ((acl_parse[0] == uname) &&
		                ((acl_parse[1] == gname) ||
		                 (acl_parse[1] == "*"))) {
			if (acl_parse[2].empty()) {
				cerr << "no permission to view acl" << endl;
				return 1;
			}
			tmp = acl_parse[2];
			if (tmp.find("v") != string::npos) {
				/* print out acl */
				file.open(acl_name.c_str());
				if (!file) {
					cerr << "file can not open" << endl;
					return 1;
				}
				while (!file.eof()) {
					getline(file, tmp);
					if (tmp.length() != 0)
						cout << tmp << endl;
				}
				file.close();
				return 0;
			}
		}
	}
	if (i == acl.size()) {
		cerr << "user has no permission to view acl" << endl;
		return 1;
	}
	return 0;
}


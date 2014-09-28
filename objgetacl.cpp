#include "header.h"

int main(int argc, char *argv[])
{
	int ch;
	size_t i;
	opterr = 0;
	string uname; /* user name */
	string uname2;
	string gname; /* group name */
	string object_name; /* object name */
	string acl_name;
	string tmp;
	vector<string> acl_parse; /* user.group ops */
	vector<string> object_name_parse;
	vector<string> acl;
	ifstream file;

	/* read command and parse command */
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
	/* check username, groupname whether exist */
	if (!check_user_group(uname, gname))
		return 1;
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


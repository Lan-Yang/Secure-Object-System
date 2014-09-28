#include "header.h"
using namespace std;
int main(int argc, char *argv[])
{
	int ch;
	opterr = 0;
	string uname; //user name
	string uname2;
	string gname; //group name
	string access;
	string object_name; //object name
	string acl_name;
	char *object_name_parse[2];
	//input commands
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
			cout << "command not found" << endl;
			return 0;
		}
	}
	//check commands
	if ((gname.empty()) || (uname.empty()) || (argc != 8)) {
		cout << "command not found" << endl;
		return 0;
	}
	object_name = argv[7];
	//check user name, group name whether valid
	if (!check_name_valid(uname)) {
		cout << "user name not valid" << endl;
		cout << "only letters, numbers, underscore are allowed" << endl;
		return 0;
	}
	if (!check_name_valid(gname)) {
		cout << "group name not valid" << endl;
		cout << "only letters, numbers, underscore are allowed" << endl;
		return 0;
	}
	//check the condition that one references other users' objects
	if (check_reference(object_name)) {
		char *input_command = new char[object_name.length() + 1];
		strcpy(input_command, object_name.c_str());
		parse_command(input_command, object_name_parse);
		uname2 = object_name_parse[0];
		object_name = object_name_parse[1];
		acl_name = uname2 + "-" + object_name + "-acl";
	} else {
		acl_name = uname + "-" + object_name + "-acl";
	}
	if (uname == uname2) {
		cout << "command not found" << endl;
		return 0;
	}
	//check object name whether valid
	if (!check_name_valid(object_name)) {
		cout << "object name not valid" << endl;
		cout << "only letters, numbers, underscore are allowed" << endl;
		return 0;
	}
	//check user name, group name whether exist
	if (!check_user_group(uname, gname))
		return 0;

	//check allowed or denied
	if (!check_acl(acl_name, uname, gname, access))
		cout << "denied" << endl;
	else
		cout << "allowed" << endl;
	return 0;
}


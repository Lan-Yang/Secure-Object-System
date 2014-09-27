#include "header.h"
using namespace std;
int main(int argc, char *argv[])
{
	int ch;
	opterr = 0;
	string uname; //user name
	string gname; //group name
	string access;
	string object_name; //object name
	string acl_name;
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
	//check user name, group name, object name whether valid
	if (!check_name_valid(uname)) {
		cout << "user name not valid" << endl;
		cout << "only letters, numbers, underscore are allowed" <<endl;
		return 0;	
	}
	if (!check_name_valid(gname)) {
		cout << "group name not valid" << endl;
		cout << "only letters, numbers, underscore are allowed" <<endl;
		return 0;	
	}
	if (!check_name_valid(object_name)) {
		cout << "object name not valid" << endl;
		cout << "only letters, numbers, underscore are allowed" <<endl;
		return 0;	
	}
	//check user name, group name whether exist
	if (!check_user_group(uname,gname)) 
		return 0;
	//check user whether have "v" permission to acl
	acl_name = uname + "-" + object_name + "-acl";	
	if (!check_acl(acl_name, uname, gname, "v")){
		cout << "no permission to view acl" << endl;
		return 0;
	}
	//check allowed or denied
	if (!check_acl(acl_name, uname, gname, access))
		cout << "denied" << endl;
	else
		cout << "allowed" << endl;
	return 0;
}


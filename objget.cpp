#include "header.h"
using namespace std;
int main(int argc, char *argv[])
{
	int ch;
	opterr = 0;
	string uname; //user name
	string uname2; //referenced user name
	string gname; //group name
	string object_name;
	string file_name;
	string acl_name;
	string tmp;
	vector<string> usergroup;
	vector<string> userobject;
	vector<string> acl;
	char *object_name_parse[2];
	ifstream file;	
	//input commands
	while ((ch = getopt(argc, argv, "u:g:")) != -1) {
		switch (ch) {
		case 'u':
			uname = optarg;
			break;
		case 'g':
			gname = optarg;
			break;
		default:
			cout << "command not found" << endl;
			return 0;
		}
	}
	if ((gname.empty()) || (uname.empty()) || (argc != 6)) {
		cout << "command not found" << endl;
		return 0;
	}
	object_name = argv[5];	
	//check user name, group name whether valid
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
	//check username, groupname whether exist
	if (!check_user_group(uname,gname)) 
		return 0;
	//check the condition that one references other users' objects
	if (check_reference(object_name)){
		char *input_command = new char[object_name.length() + 1];
		strcpy(input_command, object_name.c_str());
		parse_command(input_command, object_name_parse);
		uname2 = object_name_parse[0];
		object_name = object_name_parse[1];
		file_name = uname2 + "-" + object_name;
	}else {
		file_name = uname + "-" + object_name;
	}
	if (uname == uname2) {
		cout << "command not found" << endl;
		return 0;
	}	
	//check object name whether valid
	if (!check_name_valid(object_name)) {
		cout << "object name not valid" << endl;
		cout << "only letters, numbers, underscore are allowed" <<endl;
		return 0;	
	}
	//check user's privilege to this file
	acl_name = file_name + "-acl";	
	if (!check_acl(acl_name, uname, gname, "r")) {
		cout << "no permission to read" << endl;
		return 0;
	}
	// if the user has access, then print the object to stdout
	file.open(file_name.c_str());
	if (!file) {
		cout << "file can not open" << endl;
		return 0;
	}
	while (!file.eof()) {
		getline(file, tmp);
		if (tmp.length() != 0) //avoid empty string pushed to vector
			cout << tmp << endl;
	}
	file.close();
	return 0;
}


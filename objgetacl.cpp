#include "header.h"
using namespace std;

int main(int argc, char *argv[])
{
	int ch;
	size_t i;
	opterr = 0;
	string uname; //user name
	string uname2;
	string gname; //group name
	string object_name; //object name
	string acl_name;
	string tmp;
	char *acl_parse[3]; // user.group ops
	char *object_name_parse[2];
	vector<string> acl;	
	ifstream file;

	// read command and parse command
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
		acl_name = uname2 + "-" + object_name + "-acl";
	}else {
		acl_name = uname + "-" + object_name + "-acl";
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
	//check user whether have permission to view acl
	file.open(acl_name.c_str());
	if (!file) {
		cout << "file can not open" << endl;
		return 0;
	}
	while (!file.eof()) {
		getline(file, tmp);
		if (tmp.length() !=0) // avoid empty string push to vector
			acl.push_back(tmp);
	}
	file.close();
	for (i = 0; i < acl.size(); i++) {
		acl_parse[0] = NULL;
		acl_parse[1] = NULL;
		acl_parse[2] = NULL;
		char *input_command = new char[acl[i].length() + 1];
		strcpy(input_command, acl[i].c_str());
		parse_command(input_command, acl_parse);
		if ((acl_parse[0] == uname) &&
		    ((acl_parse[1] == gname) ||
		     (strcmp(acl_parse[1], "*") == 0))) {
			    if (acl_parse[2] == NULL) {
				    cout<< "no permission to view acl" <<endl;
				    delete[] input_command;
				    return 0;
			    }
			    tmp = acl_parse[2];
			    if (tmp.find("v") != string::npos) {
				    cout<<tmp<<endl;// print out acl
				    delete[] input_command;
				    return 0;
			    }
		}
		delete[] input_command;
	}
	if (i == acl.size())
		cout<< "user has no permission to view acl" <<endl;
	return 0;
}


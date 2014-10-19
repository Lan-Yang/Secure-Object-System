#include "header.h"

int main(int argc, char *argv[])
{
	opterr = 0;
	string uname; 
	string uname2; /* referenced user name */
	string gname; /* group name */
	string object_name;
	string file_name;
	string acl_name;
	char tmp;
	vector<string> usergroup;
	vector<string> userobject;
	vector<string> acl;
	vector<string> object_name_parse;
	ifstream file;
	struct passwd *tmp1 = NULL;
	struct group *tmp2 = NULL;

	/* input commands */
	if (argc != 2) {
		cerr << "command not found" << endl;
		return 1;
	}
	object_name = argv[1];
	tmp1 = getpwuid(getuid());
	tmp2 = getgrgid(getgid());
	if(tmp1 == NULL||tmp2 == NULL){
		cerr<<"error"<<endl;
		return 1;	
	}else {
		uname = tmp1 -> pw_name;
		gname = tmp2 -> gr_name;
	}
	/* check the condition that one references other users' objects */
	if (check_reference(object_name)) {
		parse_command(object_name, object_name_parse);
		uname2 = object_name_parse[0];
		object_name = object_name_parse[1];
		/* check referenced user name whether valid */
		if (check_user(uname2)) {
			file_name = "./lanyang/" + uname2 + "-" + object_name;
		}
		else {
			cerr << "user does not exist" << endl;
			return 1;
		}		
	} else {
		file_name = "./lanyang/" + uname + "-" + object_name;
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
	while (file.good()) {
		tmp = file.get();
		if (file.good())
			cout << tmp;
	}
	/* while (!file.eof()) {
		
		getline(file, tmp);
		if (tmp.length() != 0)  avoid empty string pushed to vector 
			cout << tmp << endl;
	} */
	file.close();
	return 0;
}


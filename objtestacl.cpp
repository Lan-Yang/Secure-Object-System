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
	struct passwd *tmp1 = NULL;
	struct group *tmp2 = NULL;

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
			acl_name = "./lanyang/" + uname2 + "-" + object_name + "-acl";
		}
		else {
			cerr << "user does not exist" << endl;
			return 1;
		}
	} else {
		acl_name = "./lanyang/" + uname + "-" + object_name + "-acl";
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


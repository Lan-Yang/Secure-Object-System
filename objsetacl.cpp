#include "header.h"

int main(int argc, char *argv[])
{
	opterr = 0;
	string uname; /* user name */
	string uname2;
	string gname; /* group name */
	string object_name; /* object name */
	string acl_file_name;
	vector<string> object_name_parse;
	FILE *fout;
	ofstream file;
	char tmp;
	struct passwd *tmp1 = NULL;
	struct group *tmp2 = NULL;

	tmp1 = getpwuid(getuid());
	tmp2 = getgrgid(getgid());
	if(tmp1 == NULL||tmp2 == NULL){
		cerr<<"error"<<endl;
		return 1;	
	}else {
		uname = tmp1 -> pw_name;
		gname = tmp2 -> gr_name;
	}
	/* check commands */
	if (argc != 2) {
		cerr << "command not found" << endl;
		return 1;
	}
	object_name = argv[1];
	/* check the condition that one references other users' objects */
	if (check_reference(object_name)) {
		parse_command(object_name, object_name_parse);
		uname2 = object_name_parse[0];
		object_name = object_name_parse[1];
		/* check referenced user name whether valid */
		if (check_user(uname2)) {
			acl_file_name = "./lanyang/" + uname2 + "-" + object_name + "-acl";
		}
		else {
			cerr << "user does not exist" << endl;
			return 1;
		}
	} else {
		acl_file_name = "./lanyang/" + uname + "-" + object_name + "-acl";
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

	/* check user whether have "p" permission to acl */
	if (!check_acl(acl_file_name, uname, gname, "p")) {
		cerr << "no permission to change acl" << endl;
		return 1;
	}
	/* read file from stdin, write its content to object */
	fout = fopen(acl_file_name.c_str(), "w");


	//file.open(acl_file_name.c_str(), ios::out | ios::trunc);
	if (fout == NULL) {
		cerr << "file can not open" << endl;
		return 1;
	}

	while ((tmp = getchar()) != EOF)
		fputc(tmp, fout);
	fclose(fout);
	return 0;
}


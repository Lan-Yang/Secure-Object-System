#include "header.h"

int main(int argc, char *argv[])
{
	unsigned int i;	
	unsigned int j;
	opterr = 0;
	string uname; /* user name */
	string uname2;
	string gname; /* group name */
	string object_name; /* object name */
	string acl_file_name;
	vector<string> object_name_parse;
	vector<string> acl; /* to check if the read in content is valid */
	vector<string> acl_parse; /* user.group ops */
	ofstream file;
	FILE *fout;
	int tmp;
	string tmp_acl;
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
	/* check user and group whether in userfile */
	if (!check_user_group(uname,gname))
		return 1;
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
	/* check acl content whether valid */
	while ((tmp = getchar()) != EOF) {
		ungetc(tmp, stdin);		
		getline(cin, tmp_acl);
		if (tmp_acl.length() != 0) 
			acl.push_back(tmp_acl);
	}
	for (i = 0; i < acl.size(); i++) {
		parse_command(acl[i].c_str(), acl_parse);
		/* check content */
		if (acl_parse.size() > 3 || acl_parse.size() < 2) {
			cerr << "illegal content for new acl" << endl;
			return 1;
		}
		if (!check_user(acl_parse[0])) {			
			cerr << "illegal user for new acl" << endl;
			return 1;		
		}
		if (!check_user_group(acl_parse[0], acl_parse[1])) {
			cerr << "illegal group for new acl" << endl;
			return 1;
		}
		if (acl_parse.size() == 3) {
			for (j = 0; j < acl_parse[2].size(); j++) {
				if (acl_parse[2][j] != 'r' && 
				acl_parse[2][j] != 'w' &&
				acl_parse[2][j] != 'x' &&
				acl_parse[2][j] != 'p' &&
				acl_parse[2][j] != 'v') {
					cerr << "illegal content for new acl" << endl;
					return 1;
				}
			}
		}
	}
	/* read file from stdin, write its content to object */
	fout = fopen(acl_file_name.c_str(), "w");	
	file.open(acl_file_name.c_str());
	if (fout == NULL) {
		cerr << "file can not open" << endl;
		return false;
	}
	for (i = 0; i < acl.size(); i++) 
	{
		file << acl[i] << endl;
	}
	file.close();
	return 0;
}


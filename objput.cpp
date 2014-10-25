#include "header.h"

int main(int argc, char *argv[])
{
	int flag = 0;
	opterr = 0;
	int init_flag = 0;
	string uname; /* to trans int to string */
	string uname2; /* referenced user name */
	string gname; /* to trans int to string */
	string object_name; /* object name */
	string initial_acl;
	string file_name;
	string obj_user_group;
	string tmp_line;
	vector<string> object_name_parse;
	FILE *fout;
	int tmp; /* for getchar function, it needs to be int type */
	struct passwd *tmp1 = NULL;
	struct group *tmp2 = NULL;

	/* input commands */
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
			file_name = "./lanyang/" + uname2 + "-" + object_name;
		}
		else {
			cerr << "user does not exist" << endl;
			return 1;
		}
	} else {
		uname2 = uname;
		file_name = "./lanyang/" + uname + "-" + object_name;
	}
	/* check object name whether valid */
	if (!check_name_valid(object_name)) {
		help();
		return 1;
	}
	/* check the object whether already exist */
	initial_acl = file_name + "-acl";
	fout = fopen(initial_acl.c_str(), "r");
	if ((fout == NULL) && (uname == uname2))
		init_flag = 1;
	if (fout != NULL) {
	/* if it exists, check acl "w" */
		if (!check_acl(initial_acl, uname, gname, "w")) {
			cerr << "object exists. no permission to write" << endl;
			return 1;
		}
	}
	if ((fout == NULL)&&(uname2 != uname)) {
		cerr << uname <<" has no permission to create doc belongs to " << uname2 << endl;
		return 1;
	}
	/* if user has permission to write */
	fout = fopen(file_name.c_str(), "w");
	if (fout == NULL) {
		cerr << "file can not open" << endl;
		return 1;
	}
	while ((tmp = getchar()) != EOF)
		fputc(tmp, fout);
	fclose(fout);
	/* initiate corresponding acl object */	
	if (init_flag == 1) {
		fout = fopen(initial_acl.c_str(), "w");
		if (fout == NULL) {
			cerr << "file can not open" << endl;
			return 1;
		}
		fprintf(fout, "%s.* rwxpv", uname.c_str());
		fclose(fout);
	}
	/* record to the user object file */
	obj_user_group = object_name + " " + uname2 + " " + gname;
	ifstream file2;
	file2.open("./lanyang/user_object");
	while (!file2.eof()) {
		getline(file2, tmp_line);
		if (tmp_line == obj_user_group)
			flag = 1;
	}
	file2.close();
	ofstream file3;
	if (flag == 0) {
		file3.open("./lanyang/user_object", ios::ate | ios::app);
		if (!file3) {
			cerr << "file can not open" << endl;
			return 1;
		}
		else {
			file3 << object_name << " " << uname2 << " " 
				<< gname << '\n';
		}
		file3.close();
	}
	return 0;
}


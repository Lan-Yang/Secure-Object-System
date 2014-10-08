#include "header.h"

int main(int argc, char *argv[])
{
	int flag = 0;
	opterr = 0;
	uid_t user_id;
	string uname; /* to trans int to string */
	gid_t group_id;
	string gname; /* to trans int to string */
	string object_name; /* object name */
	string initial_acl;
	string file_name;
	string obj_user_group;
	string tmp_line;
	FILE *fout;
	char tmp;

	/* input commands */
	user_id = getuid();
	group_id = getgid();
	/* check commands */
	if (argc != 2) {
		cerr << "command not found" << endl;
		return 1;
	}
	object_name = argv[1];
	/* check object name whether valid */
	if (!check_name_valid(object_name)) {
		help();
		return 1;
	}
	/* read file from stdin, write its content to object */
	uname = to_string(user_id);
	gname = to_string(group_id);
	file_name = uname + "-" + object_name;
	fout = fopen(file_name.c_str(), "w");
	if (fout == NULL) {
		cerr << "file can not open" << endl;
		return 1;
	}
	while ((tmp = getchar()) != EOF)
		fputc(tmp, fout);
	fclose(fout);

	/* initiate corresponding acl object */
	initial_acl = uname + "-" + object_name + "-acl";
	fout = fopen(initial_acl.c_str(), "w");
	if (fout == NULL) {
		cerr << "file can not open" << endl;
		return 1;
	}
	fprintf(fout, "%s.* rwxpv", uname.c_str());
	fclose(fout);
	obj_user_group = object_name + " " + uname + " " + gname;
	ifstream file2;
	file2.open("user_object");
	while (!file2.eof()) {
		getline(file2, tmp_line);
		if (tmp_line == obj_user_group)
			flag = 1;
	}
	file2.close();
	ofstream file3;
	if (flag == 0) {
		file3.open("user_object", ios::ate | ios::app);
		if (!file3) {
			cerr << "file can not open" << endl;
			return 1;
		}
		else {
			file3 << object_name << " " << uname << " " 
				<< gname << '\n';
		}
		file3.close();
	}
	return 0;
}


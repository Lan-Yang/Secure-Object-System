#include "header.h"

void help(void)
{
	cerr << "error: for user name, group name and object name,"
	     << "only letters, numbers, underscore are allowed" << endl;
}

bool check_name_valid(const string &input)
{
	size_t i, LL;

	LL = input.size();
	for (i = 0; i < LL; i++) {
		if (((input[i] >= '0') && (input[i] <= '9'))
		                || ((input[i] >= 'a') && (input[i] <= 'z'))
		                || ((input[i] >= 'A') && (input[i] <= 'Z'))
		                || (input[i] == '_'))
			continue;
		return false;
	}
	return true;
}


int parse_command(const string &input, vector<string> &commands)
{
	commands.clear();
	unsigned int i = 0;
	const char token[] = " .\t+";
	size_t LL = input.size();
	char *buf = new char[LL + 1];
	char *saveptr;
	char *tmp;

	buf[LL] = '\0';
	for (i = 0; i < LL; i++)
		buf[i] = input[i];

	tmp = strtok_r(buf, token, &saveptr);
	for (i = 0; tmp != NULL; i++) {
		commands.push_back(string(tmp));
		tmp = strtok_r(NULL, token, &saveptr);
	}
	delete [] buf;
	return 0;
}

bool check_user_group(const string &uname, const string &gname)
{
	string tmp;
	unsigned int i = 0;
	unsigned int j = 0;
	vector<string> usergroup;
	vector<string> user_group_parse; /* a user can at most in 10 groups */
	ifstream file;

	file.open("userfile.txt");
	if (!file) {
		cerr << "file can not open" << endl;
		return false;
	}
	while (!file.eof()) {
		getline(file, tmp);
		if (!tmp.empty()) /* avoid empty string push to vector */
			usergroup.push_back(tmp);
	}
	for (i = 0; i < usergroup.size(); i++) {
		parse_command(usergroup[i], user_group_parse);
		if (user_group_parse[0] == uname) { /* username matches */
			for (j = 0; j < user_group_parse.size(); j++) {
				if (user_group_parse[j] == gname)
					break;
			}
			if (j == user_group_parse.size()) {
				cerr << "user does not match the group" << endl;
				return false;
			}
			break;
		}
	}
	if (i == usergroup.size()) {
		cerr << "user does not exist" << endl;
		return false;
	}
	return true;
}

bool check_user(const string &uname)
{
	string tmp;
	unsigned int i = 0;
	vector<string> usergroup;
	vector<string> user_group_parse; /* a user can at most in 10 groups */
	ifstream file;

	file.open("userfile.txt");
	if (!file) {
		cerr << "file can not open" << endl;
		return false;
	}
	while (!file.eof()) {
		getline(file, tmp);
		if (!tmp.empty()) /* avoid empty string push to vector */
			usergroup.push_back(tmp);
	}
	for (i = 0; i < usergroup.size(); i++) {
		parse_command(usergroup[i], user_group_parse);
		if (user_group_parse[0] == uname) { /* username matches */
			break;
		}
	}
	if (i == usergroup.size())
		return false;
	else
		return true;
}

bool check_reference(const string &input)
{
	return (input.find('+') != string::npos);
}

bool check_acl(const string &acl_name, const string &uname, const string &gname,
               const string &per)
{
	unsigned int i;
	string tmp;
	vector<string> acl;
	vector<string> acl_parse; /* user.group ops */
	ifstream file;
	file.open(acl_name.c_str());
	if (!file) {
		cerr << "file can not open" << endl;
		return false;
	}
	while (!file.eof()) {
		getline(file, tmp);
		if (tmp.length() != 0) /* avoid empty string push to vector */
			acl.push_back(tmp);
	}
	file.close();
	for (i = 0; i < acl.size(); i++) {
		parse_command(acl[i].c_str(), acl_parse);
		if (((acl_parse[0] == uname) || (acl_parse[0] == "*")) &&
		                ((acl_parse[1] == gname) ||
		                 (acl_parse[1] == "*"))) {
			if (acl_parse.size() < 3)
				return false;
			tmp = acl_parse[2];
			if (tmp.find(per) != string::npos) {
				break;
			}
		}
	}
	if (i == acl.size())
		return false;
	else
		return true;
}

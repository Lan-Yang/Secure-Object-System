#include "header.h"
using namespace std;
bool check_name_valid(string input)
{
	int i;
	for (i = 0; i < input.length(); i++) {
		if ((input[i] >= '0') && (input[i] <= '9')) {
		} else if ((input[i] >= 'a') && (input[i] <= 'z')) {
		} else if ((input[i] >= 'A') && (input[i] <= 'Z')) {
		} else if (input[i] == '_') {
		} else {
			break;
		}
	}
	if (i == input.length())
		return true;
	else
		return false;
}
int parse_command(char *input, char *commands[])
{
	int i = 0;
	const char token[10] = " .\t+";
	commands[0] = strtok(input, token);
	while (commands[i] != NULL) {
		i++;
		commands[i] = strtok(NULL, token);
	}
	return 0;
}

bool check_user_group(string uname, string gname)
{
	string tmp;
	int j = 1;
	int i;
	vector<string> usergroup;
	char *user_group_parse[11]; /* a user can at most in 10 groups */
	ifstream file;
	file.open("user_group");
	if (!file) {
		cerr << "file can not open" << endl;
		return false;
	}
	while (!file.eof()) {
		getline(file, tmp);
		if (tmp.length() != 0) /* avoid empty string push to vector */
			usergroup.push_back(tmp);
	}
	for (i = 0; i < usergroup.size(); i++) {
		char *input_command = new char[usergroup[i].length() + 1];
		strcpy(input_command, usergroup[i].c_str());
		parse_command(input_command, user_group_parse);
		if (user_group_parse[0] == uname) { /* usesname matches */
			while (user_group_parse[j] != NULL) { /* check groupname */
				if (user_group_parse[j] == gname)
					break;
				j++;
			}
			if (user_group_parse[j] == NULL) {
				cerr << "user does not match the group" << endl;
				return false;
			}
			delete[] input_command;
			break;
		}
		delete[] input_command;
	}
	if (i == usergroup.size()) {
		cerr << "user does not exist" << endl;
		return false;
	}
	return true;
}

bool check_reference(string input)
{
	int i;
	for (i = 0; i < input.length(); i++) {
		if (input[i] == '+')
			break;
	}
	if (i == input.length())
		return false;
	else
		return true;
}

bool check_acl(string acl_name, string uname, string gname, string per)
{
	int i;
	string tmp;
	vector<string> acl;
	char *acl_parse[3]; /* user.group ops */
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
	for (i = 0; i < 3; i++)
		acl_parse[i] = NULL;
	for (i = 0; i < acl.size(); i++) {
		char *input_command = new char[acl[i].length() + 1];
		strcpy(input_command, acl[i].c_str());
		parse_command(input_command, acl_parse);
		if ((acl_parse[0] == uname) &&
		                ((acl_parse[1] == gname) ||
		                 (strcmp(acl_parse[1], "*") == 0))) {
			if (acl_parse[2] == NULL) {
				/* cout<< "no permission"<<endl; */
				delete[] input_command;
				return false;
			}
			tmp = acl_parse[2];
			if (tmp.find(per) != string::npos) {
				delete[] input_command;
				break;
			}
		}
		delete[] input_command;
	}
	if (i == acl.size()) {
		/* cout<< "no permission" <<endl; */
		return false;
	} else {
		return true;
	}
}

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string>
#include <cstring>
#include <vector>
using namespace std;

int parse_command(char *input, char *commands[]);
bool check_name_valid(string input);
bool check_user_group(string uname, string gname);
bool check_reference(string input);
bool check_acl(string acl_name, string uname, string gname, string per);

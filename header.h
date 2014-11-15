#include <iostream>
#include <fstream>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <vector>
#include <cstdlib>
#include <sys/types.h>
#include <stdbool.h>
#include <pwd.h>
#include <sys/types.h>
#include <grp.h>
#include <openssl/md5.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>


using namespace std;

void help(void);
int parse_command(const string &input, vector<string> &commands);
bool check_name_valid(const string &input);
bool check_user_group(const string &uname, const string &gname);
bool check_reference(const string &input);
bool check_acl(const string &acl_name, const string &uname, const string &gname,
               const string &per);
bool check_user(const string &uname);
int aesencrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
	    unsigned char *iv, unsigned char *ciphertext);
int aesdecrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
	    unsigned char *iv, unsigned char *plaintext);
void handleErrors(void);

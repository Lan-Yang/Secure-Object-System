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

	if (uname == "*" && gname == "*")
		return true;
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
	/* uname is "*" gname is not "*" */
	if (uname == "*" && gname != "*") {
		for (i = 0; i < usergroup.size(); i++) {
			user_group_parse.clear();
			parse_command(usergroup[i], user_group_parse);
			for (j = 1; j < user_group_parse.size(); j++) {
				if (user_group_parse[j] == gname)
					return true;
			}
		}
		cerr << "group does not exist" << endl;
		return false;
	}
	/* uname is not "*" */
	for (i = 0; i < usergroup.size(); i++) {
		user_group_parse.clear();
		parse_command(usergroup[i], user_group_parse);
		if (user_group_parse[0] == uname) { /* username matches */
			if (gname == "*")
				return true;
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

	if (uname == "*")
		return true;
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

void handleErrors(void)
{
	ERR_print_errors_fp(stderr);
	abort();
}

int aesencrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
	    unsigned char *iv, unsigned char *ciphertext)
{
	EVP_CIPHER_CTX *ctx;
	
	int len;
	
	int ciphertext_len;
	
	/* Create and initialise the context */
	if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();
	
	/* Initialise the encryption operation. IMPORTANT - ensure you use a key
	 * and IV size appropriate for your cipher
	 * Here, we are using 128 bit AES (i.e. a 128 bit key). The
	 * IV size for *most* modes is the same as the block size. For AES this
	 * is 128 bits */
	if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv))
		handleErrors();
	
	/* Provide the message to be encrypted, and obtain the encrypted output.
	 * EVP_EncryptUpdate can be called multiple times if necessary
	 */
	if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
		handleErrors();
	ciphertext_len = len;
	
	/* Finalise the encryption. Further ciphertext bytes may be written at
	 * this stage.
	 */
	if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
	ciphertext_len += len;
	
	/* Clean up */
	EVP_CIPHER_CTX_free(ctx);
	
	return ciphertext_len;
}
int aesdecrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
	    unsigned char *iv, unsigned char *plaintext)
{
	EVP_CIPHER_CTX *ctx;
	
	int len;
	
	int plaintext_len;
	
	/* Create and initialise the context */
	if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();
	
	/* Initialise the decryption operation. IMPORTANT - ensure you use a key
	 * and IV size appropriate for your cipher
	 * In this example we are using 256 bit AES (i.e. a 256 bit key). The
	 * IV size for *most* modes is the same as the block size. For AES this
	 * is 128 bits */
	if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
		handleErrors();
	
	/* Provide the message to be decrypted, and obtain the plaintext output.
	 * EVP_DecryptUpdate can be called multiple times if necessary
	 */
	if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
		handleErrors();
	plaintext_len = len;
	
	/* Finalise the decryption. Further plaintext bytes may be written at
	 * this stage.
	 */
	if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) handleErrors();
	plaintext_len += len;
	
	/* Clean up */
	EVP_CIPHER_CTX_free(ctx);
	
	return plaintext_len;
}

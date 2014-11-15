#include "header.h"

int main(int argc, char *argv[])
{
	int ch;
	opterr = 0;
	string uname;
	string uname2; /* referenced user name */
	string gname; /* group name */
	string object_name;
	string file_name;
	string acl_name;
	string passfile_name;
	string passphrase;
	//char tmp;
	vector<string> usergroup;
	vector<string> userobject;
	vector<string> acl;
	vector<string> object_name_parse;
	ifstream file;
	struct passwd *tmp1 = NULL;
	struct group *tmp2 = NULL;
	FILE *fout;
	const int byte_count = 16; /* generate 128 bits key and IV */
	const int buff_count = 50;
	unsigned char digest[MD5_DIGEST_LENGTH];
	unsigned char randomkey[buff_count];
	unsigned char randomiv1[buff_count];
	unsigned char randomiv2[buff_count];
	unsigned char cipherkey[buff_count];
	unsigned char buff[buff_count];
	unsigned char plaintext[buff_count];
	//int cipherkey_len;
	EVP_CIPHER_CTX *ctx;
	int m = 0;
	int i = 0;
	int len;

	/* input commands */
	while ((ch = getopt(argc, argv, "k:")) != -1) {
		switch (ch) {
		case 'k':
			passphrase = optarg;
			break;
		default:
			cerr << "command not correct" << endl;
			return 1;
		}
	}
	if (argc != 4) {
		cerr << "command not correct" << endl;
		return 1;
	}
	object_name = argv[3];
	tmp1 = getpwuid(getuid());
	tmp2 = getgrgid(getgid());
	if (tmp1 == NULL || tmp2 == NULL) {
		cerr << "error" << endl;
		return 1;
	} else {
		uname = tmp1 -> pw_name;
		gname = tmp2 -> gr_name;
	}
	/* check user and group whether in userfile */
	if (!check_user_group(uname, gname))
		return 1;
	/* check the condition that one references other users' objects */
	if (check_reference(object_name)) {
		parse_command(object_name, object_name_parse);
		uname2 = object_name_parse[0];
		object_name = object_name_parse[1];
		/* check referenced user name whether valid */
		if (check_user(uname2)) {
			file_name = "/home/lanyang/" + uname2 + "-" + object_name;
		} else {
			cerr << "user does not exist" << endl;
			return 1;
		}
	} else {
		file_name = "/home/lanyang/" + uname + "-" + object_name;
	}
	/* check object name whether valid */
	if (!check_name_valid(object_name)) {
		help();
		return 1;
	}
	/* check user's privilege to this file */
	acl_name = file_name + "-acl";
	if (!check_acl(acl_name, uname, gname, "r")) {
		cerr << "no permission to read" << endl;
		return 1;
	}
	/* if the user has access, then print the object to stdout */
	passfile_name = file_name + "-" + "key";
	fout = fopen(passfile_name.c_str(), "r");
	fread(cipherkey, 1, byte_count*2, fout);
	for (i = 0; i < byte_count*2; i++)
		printf("%02x ",cipherkey[i]);
	printf("cipher key\n");
	fread(randomiv1, 1, byte_count, fout);
	for (i = 0; i < byte_count; i++)
		printf("%02x ",randomiv1[i]);
	printf("random iv1\n");
	fread(randomiv2, 1, byte_count, fout);
	for (i = 0; i < byte_count; i++)
		printf("%02x ",randomiv2[i]);
	printf("random iv2\n");
	/* use md5 generate 128 bit key */
	MD5((unsigned char*)(passphrase.c_str()),
		passphrase.length(), (unsigned char*)&digest);
	/* decrypt cipherkey */
	aesdecrypt(cipherkey, byte_count*2, digest, randomiv1, randomkey);
	for (i = 0; i < byte_count; i++)
		printf("%02x ",randomkey[i]);
	printf("randomkey\n");
	
	fout = fopen(file_name.c_str(), "r");
	/* Create and initialise the context */
	if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();
	if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, randomkey, randomiv2))
		handleErrors();
	do{
		m = fread(buff, 1, byte_count, fout);
		if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, buff, byte_count))
			handleErrors();
		fwrite(plaintext, 1, len, stdout);
		if (m < byte_count) {
			if(1 != EVP_DecryptFinal_ex(ctx, plaintext, &len)) 
				handleErrors();
			fwrite(plaintext, 1, len, stdout);
			break;	
		}	
	}while(m == byte_count);
	/* Clean up */
	EVP_CIPHER_CTX_free(ctx);
	fclose(fout);
	return 0;
}


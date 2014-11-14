#include "header.h"

int main(int argc, char *argv[])
{
	int ch;	
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
	string passphrase;
	vector<string> object_name_parse;
	FILE *fout;
	FILE *fp;
	int tmp; /* for getchar function, it needs to be int type */
	struct passwd *tmp1 = NULL;
	struct group *tmp2 = NULL;
	const int byte_count = 16; /* generate 128 bits key and IV */
	const int buff_count = 32;
	unsigned char digest[MD5_DIGEST_LENGTH];
	//unsigned char mdString[33];
	unsigned char randomkey[buff_count];
	unsigned char randomiv1[buff_count];
	unsigned char randomiv2[buff_count];
	unsigned char cipherkey[buff_count];
	int cipherkey_len;
	int ciphertext_len;

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
	object_name = argv[3];
	/* check the condition that one references other users' objects */
	if (check_reference(object_name)) {
		parse_command(object_name, object_name_parse);
		uname2 = object_name_parse[0];
		object_name = object_name_parse[1];
		/* check referenced user name whether valid */
		if (check_user(uname2)) {
			file_name = "./lanyang/" + uname2 + "-" + object_name;
		} else {
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
	if ((fout == NULL) && (uname2 != uname)) {
		cerr << uname << " has no permission to create doc belongs to " << uname2 <<
		     endl;
		return 1;
	}
	/* check if user has permission to write */
	fout = fopen(file_name.c_str(), "w");
	if (fout == NULL) {
		cerr << "file can not open" << endl;
		return 1;
	}	
	
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
		} else {
			file3 << object_name << " " << uname2 << " "
			      << gname << '\n';
		}
		file3.close();
	}
	/* use md5 generate 128 bit key */
	MD5((unsigned char*)(passphrase.c_str()),
		passphrase.length(), (unsigned char*)&digest);
	/* test 
	for(int i = 0; i < 16; i++)
		sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);
	printf("md5 digest: %s\n", mdString);*/
	/* generate 128 bit random number */
	fp = fopen("/dev/urandom", "r");
	fread(&randomkey, 1, byte_count, fp);
	fclose(fp);
	for (int i = 0; i < byte_count; i++)
		printf("%02x ",randomkey[i]);
	printf("random numer\n");
	/* generate 128 bit random iv1 */
	fp = fopen("/dev/urandom", "r");
	fread(&randomiv1, 1, byte_count, fp);
	fclose(fp);
	for (int i = 0; i < byte_count; i++)
		printf("%02x ",randomiv1[i]);
	printf("random iv1\n");
	/* generate 128 bit random iv1 */
	fp = fopen("/dev/urandom", "r");
	fread(&randomiv2, 1, byte_count, fp);
	fclose(fp);
	for (int i = 0; i < byte_count; i++)
		printf("%02x ",randomiv2[i]);
	printf("random iv2\n");
	/* Initialise the library */
	ERR_load_crypto_strings();
	OpenSSL_add_all_algorithms();
	OPENSSL_config(NULL);
	/* encrypt random number using ASE */
	cipherkey_len = aesencrypt(randomkey, byte_count, digest, randomiv1,
				 cipherkey);
	printf("aes key length: %d\n", cipherkey_len);
	/* encrypt plaint text using ASE */

	/* write into file */
	fout = fopen(file_name.c_str(), "w");
	while ((tmp = getchar()) != EOF)
		fputc(tmp, fout);
	fclose(fout);
	
	return 0;
}


#define _GNU_SOURCE

#include <time.h>
#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <openssl/md5.h>

FILE *
fopen(const char *path, const char *mode) 
{

	FILE *original_fopen_ret;
	FILE *(*original_fopen)(const char*, const char*);

	/* call the original fopen function */
	original_fopen = dlsym(RTLD_NEXT, "fopen");
	original_fopen_ret = (*original_fopen)(path, mode);

	char *uid;
	char date[20];
    char timestamp[20];
	uid = getuid();
	
    time_t t;
    t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(date,"%d-%d-%d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);
    sprintf(timestamp,"%d:%d", tm.tm_hour, tm.tm_min);

	
	unsigned char *MD5(const unsigned char *d, unsigned long n, unsigned char *md);

	int MD5_Init(MD5_CTX *c);
	int MD5_Update(MD5_CTX *c, const void *data, unsigned long len);
	int MD5_Final(unsigned char *md, MD5_CTX *c);

	return original_fopen_ret;
}


size_t 
fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) 
{

	size_t original_fwrite_ret;
	size_t (*original_fwrite)(const void*, size_t, size_t, FILE*);

	/* call the original fwrite function */
	original_fwrite = dlsym(RTLD_NEXT, "fwrite");
	original_fwrite_ret = (*original_fwrite)(ptr, size, nmemb, stream);


	char *uid;
	char date[20];
    char timestamp[20];
	uid = getuid();
	
    time_t t;
    t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(date,"%d-%d-%d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);
    sprintf(timestamp,"%d:%d", tm.tm_hour, tm.tm_min);



	return original_fwrite_ret;
}



#define _GNU_SOURCE

#include <time.h>
#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <openssl/md5.h>
#include <stdbool.h>
#include <errno.h>

bool wrote=false;

void compute_md5(char *str, unsigned char digest[32]) {
    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, str, strlen(str));
    MD5_Final(digest, &ctx);
}

// original fopen
FILE*
fopen_FOR_REAL(const char *path, const char *mode) 
{

	FILE *original_fopen_ret;
	FILE *(*original_fopen)(const char*, const char*);

	original_fopen = dlsym(RTLD_NEXT, "fopen");
	original_fopen_ret = (*original_fopen)(path, mode);

	return original_fopen_ret;
}

// fopen on steroids(i mean it's logging...)
FILE*
fopen(const char *path, const char *mode) 
{

	FILE *original_fopen_ret;
	FILE *(*original_fopen)(const char*, const char*);

	original_fopen = dlsym(RTLD_NEXT, "fopen");
	original_fopen_ret = (*original_fopen)(path, mode);

	// Logging the action

	int uid;
	char date[20];
    char timestamp[20];
	uid = getuid();
	int accType;
	
    time_t t;
    t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(date,"%02d-%02d-%d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);
    sprintf(timestamp,"%02d:%02d", tm.tm_hour, tm.tm_min);

	if(strcmp(mode, "r") == 0){
		accType = 1;
	}else if(strcmp(mode, "w") == 0){
		if(original_fopen_ret != NULL){ //the file exists
			accType = 3; 
		}else{ 							//the file does not exist
			accType = 0;
		}
	}else if(strcmp(mode, "a") == 0){
		accType = 2;
	}else if(strcmp(mode, "r+") == 0){
		if(wrote ==  true){ 			//fwrite() was used
			accType = 2; 
		}else{		
			accType = 1;
		}
	}else if(strcmp(mode, "w+") == 0){
		if(original_fopen_ret != NULL){ //the file exists
			if(wrote==true){ 			//fwrite() was used
				accType = 3; 
			}else{
				accType = 1;
			}			
		}else{ 							//the file does not exist
			accType = 0;
		}
	}else if(strcmp(mode, "a+") == 0){
		if(original_fopen_ret != NULL){ //the file exists
			if(wrote==true){ 			//fwrite() was used
				accType = 2; 
			}else{
				accType = 1;
			}			
		}else{ 							//the file does not exist
			accType = 0;
		}
	}

	unsigned char digest[16];
	char *buffer;
	FILE *fptr;
	fptr = (*original_fopen)(path, "r");
	int old_s; 
	int denied = 0;


	if(original_fopen_ret == NULL && errno==EACCES)
	{
		printf("No priviledges!\n"); 
		denied = 1;
		original_fopen_ret = (*original_fopen)(path, "r");  
		// to build Hashing
		old_s = ftell(fptr);
		fseek(fptr,0,SEEK_END);
		int length = ftell(fptr);
		fseek(fptr,0,SEEK_SET);
		buffer=malloc(length);
		fread(buffer,1,length,fptr);
		fseek(fptr,0,old_s);
		fclose(fptr);
		MD5_CTX ctx;
		MD5_Init(&ctx);
		MD5_Update(&ctx, buffer, length);
		MD5_Final(digest, &ctx);

		//
		original_fopen_ret = NULL;
			        
	}else{
		// to build Hashing
		old_s = ftell(fptr);
		fseek(fptr,0,SEEK_END);
		int length = ftell(fptr);
		fseek(fptr,0,SEEK_SET);
		buffer=malloc(length);
		fread(buffer,1,length,fptr);
		fseek(fptr,0,old_s);
		fclose(fptr);
		MD5_CTX ctx;
		MD5_Init(&ctx);
		MD5_Update(&ctx, buffer, length);
		MD5_Final(digest, &ctx);

		//
	}

	fptr = (*original_fopen)("file_logging.log", "a+");
	chmod("file_logging.log", 0777);	

	fprintf(fptr,"%d | %s | %s | %s | %d | %d | ",uid, path, date, timestamp, accType, denied);
	for (int i = 0; i < 16; i++) fprintf(fptr,"%02x", digest[i]);
	fprintf(fptr,"\n");
	fclose(fptr);

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

	FILE *(*original_fopen)(const char*, const char*);

	original_fopen = dlsym(RTLD_NEXT, "fopen");
	
	int MAXSIZE = 0xFFF;
    char proclnk[0xFFF];
    char filename[0xFFF];
    int fno;
    ssize_t r;

	fno = fileno(stream);
	sprintf(proclnk, "/proc/self/fd/%d", fno);
	r = readlink(proclnk, filename, MAXSIZE);
	if (r < 0)
	{
		printf("failed to readlink\n");
		exit(1);
	}
	filename[r] = '\0';

	// Logging the action
	wrote=true;

	unsigned char digest[16];
	char *buffer;
	
	int old_s = ftell(stream);
	fseek(stream,0,SEEK_END);
	int length = ftell(stream);
	fseek(stream,0,SEEK_SET);
	buffer=malloc(length);
	fread(buffer,1,length,stream);
	fseek(stream,0,old_s);
    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, buffer, length);
    MD5_Final(digest, &ctx);

	FILE *fptr;
    fptr = (*original_fopen)("file_logging.log", "a");
	if(fptr == NULL)
	{
		printf("Error!");   
		// exit(1);             
	}

	chmod("file_logging.log", 0777);

	int uid;
	char date[20];
    char timestamp[20];
	uid = getuid();
	
    time_t t;
    t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(date,"%02d-%02d-%d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);
    sprintf(timestamp,"%02d:%02d", tm.tm_hour, tm.tm_min);

	fprintf(fptr,"%d | %s | %s | %s | %d | %d | ",uid, basename(filename), date, timestamp, 2, 0);
	for (int i = 0; i < 16; i++) fprintf(fptr,"%02x", digest[i]);
	fprintf(fptr,"\n");
	fclose(fptr);

	
	return original_fwrite_ret;
}



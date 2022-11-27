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

bool wrote=false;

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
	/* call the original fopen function */
	original_fopen_ret = fopen_FOR_REAL(path, mode);

	// Logging the action

	int uid;
	char date[20];
    char timestamp[20];
	uid = getuid();
	int accType;
	
    time_t t;
    t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(date,"%d-%d-%d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);
    sprintf(timestamp,"%d:%d", tm.tm_hour, tm.tm_min);

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


	
	

	//	Writing to the log file
	FILE *fptr;
	
	fptr = fopen_FOR_REAL("file_logging.log","a");
	

	if(fptr == NULL)
	{
		printf("Error!");   
		exit(1);             
	}

	fprintf(fptr,"%d | %s | %s | %s | %d | ",uid, path, date, timestamp, accType);
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

	// Logging the action
	wrote=true;

    // MD5_Init (&mdContext);
    // while ((bytes = fread (data, 1, 1024, stream)) != 0){
    //     MD5_Update (&mdContext, data, bytes);
	// 	}
    // MD5_Final (c,&mdContext);
	MD5_CTX mdContext;
    int bytes;
    unsigned char data[1024];
	unsigned char c[MD5_DIGEST_LENGTH];

    MD5_Init(&mdContext);
    while ((bytes = fread (data, 1, 1024, stream)) != 0){
        MD5_Update(&mdContext, data, bytes);
		}
    MD5(c,MD5_DIGEST_LENGTH,&mdContext);

	

	
	FILE *fptr;
	fptr = fopen_FOR_REAL("file_logging.log","a");
    
	if(fptr == NULL)
	{
		printf("Error!");   
		exit(1);             
	}	
	for(int i = 0; i < MD5_DIGEST_LENGTH; i++)  fprintf(fptr,"%02x",c[i]);
	fprintf(fptr,"\n");
	
	fclose(fptr);

	
	return original_fwrite_ret;
}



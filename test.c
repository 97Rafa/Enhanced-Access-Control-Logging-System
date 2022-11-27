#define _GNU_SOURCE

#include <time.h>
#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <openssl/md5.h>

int main(){
    // Logging the action
    char *path = "test.txt";
	char uid[100];
	char date[20];
    char timestamp[20];


	// uid = itoa(getuid());
    sprintf(uid,"%d", getuid());
	
    time_t t;
    t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(date,"%02d-%02d-%d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);
    sprintf(timestamp,"%02d:%02d", tm.tm_hour, tm.tm_min);

	
	unsigned char c[MD5_DIGEST_LENGTH];
    
    FILE *inFile = fopen ("malakies.txt", "a");
    MD5_CTX mdContext;
    int bytes;
    unsigned char data[1024];

    MD5_Init(&mdContext);
    while ((bytes = fread (data, 1, 1024, inFile)) != 0){
        MD5_Update(&mdContext, data, bytes);
		}
    MD5_Final(c,&mdContext);
	fclose (inFile);

	

	
	FILE *fptr;
	fptr = fopen("file_logging.log","a");
    
	if(fptr == NULL)
	{
		printf("Error!");   
		exit(1);             
	}
	fprintf(fptr,"%s | %s | %s | %s | ",uid, path, date, timestamp);
	
	for(int i = 0; i < MD5_DIGEST_LENGTH; i++)  fprintf(fptr,"%02x",c[i]);
	fprintf(fptr,"\n");
	
	fclose(fptr);


    return 0;
}
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
    char *path = "randomfile.txt";
	char uid[100];
	char date[20];
    char timestamp[20];
	// uid = itoa(getuid());
    sprintf(uid,"%d", getuid());
	
    time_t t;
    t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(date,"%d-%d-%d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);
    sprintf(timestamp,"%d:%d", tm.tm_hour, tm.tm_min);

	FILE *fptr;
	fptr = fopen("file_logging.log","a");

	if(fptr == NULL)
	{
		printf("Error!");   
		exit(1);             
	}

	fprintf(fptr,"%s | %s | %s | %s",uid, path, date, timestamp);
	fclose(fptr);


    return 0;
}
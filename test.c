#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

struct entry {

	int uid; /* user id (positive integer) */
	int access_type; /* access type values [0-2] */
	int action_denied; /* is action denied values [0-1] */

	char *date; /* file access date */
	char *time; /* file access time */

	char *file; /* filename (string) */
	char *fingerprint; /* file fingerprint */

	/* add here other fields if necessary */
	/* ... */
	/* ... */

};


int main(){

    FILE *file;
    char charbuff[255];
    file = fopen("file_logging.log", "r");
    char c;
    int i=0;
    char terms[7][20];
    int logsCount=0;

    // Extract characters from file and store in character c
    for (c = getc(file); c != EOF; c = getc(file)){
        if (c == '\n'){ // Increment count if this character is newline
            logsCount = logsCount + 1;
        }
    }
    fseek(file, 0, SEEK_SET); //restores file pointer
    struct entry test[logsCount];
    
    i=0;
    int line=0;
    do{
        charbuff[i] = fgetc(file);   
        
        if(charbuff[i] == '\n'){
            // Extract the first token
            i=0;
            char * token = strtok(charbuff, " | ");
            // terms[0] = token;
            int j=0;
            // loop through the string to extract all other tokens
            while(token != NULL) {
                // printf( " %s\n", token ); //printing each token
                strcpy(terms[j],token);
                token = strtok(NULL, " | ");
                j++;
                
            }

            test[line].uid = atoi(terms[0]);
            test[line].file = terms[1];
            test[line].date = terms[2];
            test[line].time = terms[3];
            test[line].access_type = atoi(terms[4]);
            test[line].action_denied = atoi(terms[5]);
            test[line].fingerprint = terms[6];
            // printf("%d\n",test[line].uid);
            // printf("%s\n",test[line].file);
            // printf("%s\n",test[line].date);
            // printf("%s\n",test[line].time);
            // printf("%d\n",test[line].access_type);
            // printf("%d\n",test[line].action_denied);
            // printf("%s\n",test[line].fingerprint);            
            line++;            
        }        
        i++;
    }while(!feof(file));
}
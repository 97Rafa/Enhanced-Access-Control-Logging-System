#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct mal {
		int uid;
		int AttCount;
	};

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

void
usage(void)
{
	printf(
	       "\n"
	       "usage:\n"
	       "\t./monitor \n"
		   "Options:\n"
		   "-m, Prints malicious users\n"
		   "-i <filename>, Prints table of users that modified "
		   "the file <filename> and the number of modifications\n"
		   "-h, Help message\n\n"
		   );

	exit(1);
}

void 
list_unauthorized_accesses(FILE *log)
{
	char c;
	int logsCount=0;

	// Extract characters from file and store in character c
    for (c = getc(log); c != EOF; c = getc(log)){
        if (c == '\n'){ // Increment count if this character is newline
            logsCount = logsCount + 1;
        }
    }
    fseek(log, 0, SEEK_SET); //restores file pointer
    struct entry singleLog[logsCount];

	char charbuff[255];

	int i=0;
    int line=0;
	char terms[7][20];
	size_t len = 0;
	ssize_t read_line;

	do{
        charbuff[i] = fgetc(log);

		if(charbuff[i] == '\n'){
			i=-1;
			charbuff[strlen(charbuff)-1] = '\0';
			
			// Extract the first token		
			char * token = strtok(charbuff, " | ");
			int j=0;
			// loop through the string to extract all other tokens
			while(token != NULL) {
				// printf( "%s\n", token ); //printing each token
				strcpy(terms[j],token);
				token = strtok(NULL, " | ");
				j++;          
			}

			singleLog[line].uid = atoi(terms[0]);
			singleLog[line].file = strdup(terms[1]);
			singleLog[line].date = strdup(terms[2]);
			singleLog[line].time = strdup(terms[3]);
			singleLog[line].access_type = atoi(terms[4]);
			singleLog[line].action_denied = atoi(terms[5]);
			singleLog[line].fingerprint = strdup(terms[6]); 
			line++;    			        
		}
		i++;
	
    }while(!feof(log));
	
	// Creates a new struct to save users
	int usersC=1;
	struct mal users[usersC];
	// Pick all elements one by one
    for (i = 1; i < logsCount; i++) {
        int j = 0;
        for (j = 0; j < i; j++)
            if (singleLog[i].uid == singleLog[j].uid)
                break;
 
        // If not printed earlier, then print it
        if (i == j)
            usersC++;
    }		

	for (i = 0; i < usersC; i++)
	{
		users[i].uid=0;
		users[i].AttCount=0;
	}

	int cnt=0;
	for(int i=0;i<logsCount;i++){
		int e=0;
		for(int j=0;j<usersC;j++){
			if(singleLog[i].uid == users[j].uid){
				e=1;				
			}
		}		
		if (e==0)
		{
			users[cnt].uid=singleLog[i].uid;
			cnt++;
		}		
	}

	for(int j = 0; j < logsCount; j++)
	{
		if (singleLog[j].action_denied == 1)
		{
			for (int k = 0; k < usersC; k++)	
			{		
				if(singleLog[j].uid == users[k].uid){
					users[k].AttCount ++;
				}
			}			
		}
	}	

	for (i = 0; i < usersC; i++)
	{
		if(users[i].AttCount >= 7){
			printf("User: %d\tDenied Actions:%d\n", users[i].uid, users[i].AttCount);
		}
	}
	printf("***Terminating***\n");
	return;

}


void
list_file_modifications(FILE *log, char *file_to_scan)
{
	char c;
	int logsCount=0;

	// Extract characters from file and store in character c
    for (c = getc(log); c != EOF; c = getc(log)){
        if (c == '\n'){ // Increment count if this character is newline
            logsCount = logsCount + 1;
        }
    }
    fseek(log, 0, SEEK_SET); //restores file pointer
    struct entry singleLog[logsCount];

	char charbuff[255];

	int i=0;
    int line=0;
	char terms[7][20];
	size_t len = 0;
	ssize_t read_line;

	do{
        charbuff[i] = fgetc(log);

		if(charbuff[i] == '\n'){
			i=-1;
			charbuff[strlen(charbuff)-1] = '\0';
			
			// Extract the first token		
			char * token = strtok(charbuff, " | ");
			int j=0;
			// loop through the string to extract all other tokens
			while(token != NULL) {
				// printf( "%s\n", token ); //printing each token
				strcpy(terms[j],token);
				token = strtok(NULL, " | ");
				j++;          
			}

			singleLog[line].uid = atoi(terms[0]);
			singleLog[line].file = strdup(terms[1]);
			singleLog[line].date = strdup(terms[2]);
			singleLog[line].time = strdup(terms[3]);
			singleLog[line].access_type = atoi(terms[4]);
			singleLog[line].action_denied = atoi(terms[5]);
			singleLog[line].fingerprint = strdup(terms[6]); 
			line++;    			        
		}
		i++;
	
    }while(!feof(log));

	

	
	printf("***Terminating***\n");
	return;

}


int 
main(int argc, char *argv[])
{

	int ch, logsCount;
	FILE *log;
	char c;

	if (argc < 2)
		usage();

	log = fopen("./file_logging.log", "r");
	if (log == NULL) {
		printf("Error opening log file \"%s\"\n", "./log");
		return 1;
	}


	while ((ch = getopt(argc, argv, "hi:m")) != -1) {
		switch (ch) {		
		case 'i':
			list_file_modifications(log, optarg);
			break;
		case 'm':
			list_unauthorized_accesses(log);
			break;
		default:
			usage();
		}

	}


	/* add your code here */
	/* ... */
	/* ... */
	/* ... */
	/* ... */


	fclose(log);
	argc -= optind;
	argv += optind;	
	
	return 0;
}

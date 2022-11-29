#include <stdio.h>
#include <string.h>

int main() 
{
	int i;
	size_t bytes;
	FILE *file;
	char filenames[10][7] = {"file_0", "file_1", 
			"file_2", "file_3", "file_4",
			"file_5", "file_6", "file_7", 		
			"file_8", "file_9"};


	/* example source code */

	char buffer[]={"banana"};
	char buffer2[10];

	for (i = 0; i < 10; i++) {
			file = fopen(filenames[i], "w");
		
		if (file == NULL) 
			printf("fopen error\n");
		else {
			bytes = fwrite(filenames[i], strlen(filenames[i]), 1, file);
			fclose(file);
		}

	}

	file = fopen(filenames[0], "w");
	if (file == NULL) 
		printf("fopen error\n");
	else {
		bytes = fwrite(buffer, strlen(buffer)+1, 1, file);
		fclose(file);
	}

	file = fopen(filenames[0], "r");
	if (file == NULL) 
		printf("fopen error\n");
	else {
		fread(buffer2, strlen(buffer)+1, 1, file);
		fclose(file);
	}
	
}
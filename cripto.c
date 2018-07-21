#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXLINE 1000

//function declarations:
char *getline(char line[],int max);
void encrypt(char *str);
void decrypt(char *str);
char *readFromFile(char fileName[]);

int main(int argc, char **argv)
{
	int max = MAXLINE;
	char line[MAXLINE];
	char *pline;

	char *inputStream;

	int failedExec = 1;
	if(strcmp(*(argv+1), "-c") == 0)
	{	
		failedExec = 0;
		printf("Encrypting...\n");

		inputStream = readFromFile("numcripto.txt");
		printf("Reading: %s",inputStream);	

		while(strlen(pline = getline(line, max))>0)
		{	
			encrypt(pline);
		}

	}	
	if(strcmp(*(argv+1), "-d") == 0)
	{
		failedExec = 0;
		printf("Decrypting...\n");

		inputStream = readFromFile("numdescripto.txt");
		printf("Reading: %s",inputStream);	

		while(strlen(pline = getline(line, max))>0)
		{	
			decrypt(pline);
		}

	}
	if(failedExec)
	{
		printf("Unexpected subcommand!\n");
	}

	return 0;
}

char *getline(char line[],int max)
{
	int c, i;

	for (i = 0; i < max - 1 && (c=getchar()) != EOF && c != '\n'; ++i)
		line[i] = c;

	if (c == '\n') {
		line[i] = c;
		++i;
	}

	line[i] = '\0';

	return line;
}

void encrypt(char *str)
{
	printf("line: %s",str);
}

void decrypt(char *str)
{
	printf("line: %s",str);
}

char *readFromFile(char fileName[])
{
	FILE *fptr;

	fptr = fopen(fileName,"r");
	if (fptr == NULL)
	{
		printf("Cannot open file!\n");
		exit(0);
	}

	int i = 0;
	char *str;
	str = malloc(100 * sizeof(char));
	int c;	
	c = fgetc(fptr);
	while (c != EOF)
	{
		str[i] = c;
		i++;
		c = fgetc(fptr);
	}
	fclose(fptr);
	
	return str;
}





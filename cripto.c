#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXLINE 1000

typedef struct {
	int key1;
	int key2;
} Keys;

//function declarations:
char *getline(char line[],int max);
void encrypt(char *str);
void decrypt(char *str);
char *readFromFile(char fileName[]);
Keys parseKeys(char* str);

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

		Keys publicKeys;
		publicKeys = parseKeys(inputStream);	
	
		printf("key1: %d\n",publicKeys.key1);
		printf("key2: %d\n",publicKeys.key2);

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

		Keys privateKeys;
		privateKeys = parseKeys(inputStream);	
	
		printf("key1: %d\n",privateKeys.key1);
		printf("key2: %d\n",privateKeys.key2);


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
		printf("Error -	Cannot open file: %s!\n", fileName);
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

Keys parseKeys(char* str)
{
	Keys keys;

	char *key1;
	char *key2;

	key1 = malloc(10 * sizeof(char));
	key2 = malloc(10 * sizeof(char));

	int isFirstKey = 1;

	int max = strlen(str);
	int c, i, j;

	j = 0;
	for (i = 0; i < max -1 && (c=str[i]) != EOF && c != '\n'; ++i)
	{
		if(c == ' ')
		{
			key1[i] = '\0';
			isFirstKey = 0;
		}
		if(isFirstKey)
		{		
			key1[i] = c;
		}
		else
		{
			key2[j] = c;
			j++;
		}
	}
	
	key2[j] = '\0';

	keys.key1 = atoi(key1);
	keys.key2 = atoi(key2);

	return keys;
}



#include <stdio.h>
#include <string.h>
#define MAXLINE 1000

//function declarations:
char *getline(char line[],int max);
void encrypt(char *str);
void decrypt(char *str);

int main(int argc, char **argv)
{
	int max = MAXLINE;
	char line[MAXLINE];
	char *pline;

	int failedExec = 1;
	if(strcmp(*(argv+1), "-c") == 0)
	{	
		failedExec = 0;
		printf("Encrypting...\n");


		while(strlen(pline = getline(line, max))>0)
		{	
			encrypt(pline);
		}

	}	
	if(strcmp(*(argv+1), "-d") == 0)
	{
		failedExec = 0;
		printf("Decrypting...\n");

		
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#define MAXLINE 1000

typedef struct {
	int key1;
	int key2;
} Keys;

//function declarations:
char *getline(char line[],int max);
char *encrypt(Keys publicKeys,char *str);
void decrypt(Keys publicKeys,char *str);
char *readFromFile(char fileName[]);
Keys parseKeys(char* str);
int aToXpowerModY(int a, int x, int y);
void writeEncryptedMessageToFile(char *str);

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

		char *encryptedMessage;

		while(strlen(pline = getline(line, max))>0)
		{	
			encryptedMessage = encrypt(publicKeys,pline);
			printf("encryptedMessage: %s",encryptedMessage);
			//writeEncryptedMessageToFile(encryptedMessage);
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
			decrypt(privateKeys, pline);
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

char *encrypt(Keys publicKeys, char *str)
{

	char *encryptedMessage;
	encryptedMessage = malloc(100 * sizeof(char));

	int x = publicKeys.key1;
	int y = publicKeys.key2;

	int a;
	int i;
	for(i = 0; i < strlen(str) - 1; i++)
	{
		a = str[i];
		encryptedMessage[i] = aToXpowerModY(a,x,y);
	}
	encryptedMessage[i++] = '\n';
	encryptedMessage[i] = '\0';	

	return encryptedMessage;

}

void decrypt(Keys publicKeys, char *str)
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

int aToXpowerModY(int a, int x, int y)
{
	int mask = 1;
	int isBit1 = 0;
	int apow = 0;
	int ac = 1;

	int ac2 = (int)pow(a,2) % y;

	for(int i = 0; i < 31; i++)
	{
		isBit1 = x & mask;
		if(i == 0)
		{
			apow = isBit1*pow(2,i);
			ac *= (int)pow(a,apow)%y;
		}
		if(i > 0 && isBit1)
		{		
			ac *= (int)pow(ac2,i)%y;
			ac = ac % y; 
		}
		mask = mask<<1;
		
	}

	return ac;

}

void writeEncryptedMessageToFile(char *str)
{

	FILE *fptr;

	fptr = fopen("encryptedMessage.txt","w");
	if (fptr == NULL)
	{
		printf("Cannot open file!\n");
		exit(0);
	}
	int i;
	for(i = 0; i < strlen(str) - 1; i++)
		fprintf(fptr, "%d ",str[i]);
	fprintf(fptr, "%d\n",str[i]);
	printf("encryptedMessage.txt criado com sucesso!\n");
	fclose(fptr);
}

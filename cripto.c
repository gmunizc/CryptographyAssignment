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
char *decrypt(Keys publicKeys,char *str);
char *readFromFile(char fileName[]);
Keys parseKeys(char* str);
unsigned long aToXpowerModY(int a, int x, int y);
void writeEncryptedMessageToFile(char **str);
void writeDecryptedMessageToFile(char **str);

int main(int argc, char **argv)
{
	int max = MAXLINE;
	char line[MAXLINE];
	char *pline;

	char *inputStream;

	int failedExec = 1;
	if(argc != 2)
	{
		printf("This program can only run with subcommand -c or -d!\n");
	}
	else
	{
		if(strcmp(*(argv+1), "-c") == 0)
		{	
			failedExec = 0;
			printf("Encrypting...\n");

			inputStream = readFromFile("numcripto.txt");

			Keys publicKeys;
			publicKeys = parseKeys(inputStream);	

			char *encryptedMessage[100];

			int i = 0;
			while(strlen(pline = getline(line, max))>0)
			{	
				encryptedMessage[i] = encrypt(publicKeys,pline);
				i++;

			}
			encryptedMessage[i] = "END";
			writeEncryptedMessageToFile(encryptedMessage);

		}	
		if(strcmp(*(argv+1), "-d") == 0)
		{
			failedExec = 0;
			printf("Decrypting...\n");

			inputStream = readFromFile("numdescripto.txt");

			char *encryptedMessage;
			encryptedMessage = readFromFile("encryptedMessage.txt");
			printf("encryptedMessage: %s",encryptedMessage);

			Keys privateKeys;
			privateKeys = parseKeys(inputStream);	

			char *decryptedMessage[100];
			decryptedMessage[j] = decrypt(privateKeys,encryptedMessage);
			decryptedMessage[j] = "END";

			writeDecryptedMessageToFile(decryptedMessage);


		}
		if(failedExec)
		{
			printf("Unexpected subcommand!\n");
			printf("This program can only run with subcommand -c or -d!\n");
		}
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

char *encrypt(Keys publicKeys, char *message)
{

	char *encryptedMessage;
	encryptedMessage = calloc(150,sizeof(char)	);

	int x = publicKeys.key1;
	int y = publicKeys.key2;

    	char buf[50];

	int a;
	int i;
	for(i = 0; i < strlen(message) - 2; i++)
	{
		a = message[i];
		snprintf(buf, 50, "%d", (int)aToXpowerModY(a,x,y));
		strcat(encryptedMessage,buf);
		strcat(encryptedMessage," ");
	}
	a = message[i];
	snprintf(buf, 50, "%d", (int)aToXpowerModY(a,x,y));
	strcat(encryptedMessage,buf);

	return encryptedMessage;

}

char *decrypt(Keys publicKeys, char *str)
{
	char *test = "Teste";
	printf("line: %s",str);
	return test;
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

unsigned long aToXpowerModY(int a, int x, int y)
{
	int mask = 1;
	int isBit1 = 0;
	int apow = 0;
	unsigned long ac = 1;

	unsigned long ac2 = (unsigned long)pow(a,2) % y;

	for(int i = 0; i < 31; i++)
	{
		isBit1 = x & mask;
		if(i == 0)
		{
			apow = isBit1*pow(2,i);
			ac *= (unsigned long)pow(a,apow)%y;
		}
		if(i > 0 && isBit1)
		{		
			ac *= (unsigned long)pow(ac2,i)%y;
			ac = ac % y; 
		}
		mask = mask<<1;
		
	}

	return ac;

}

void writeEncryptedMessageToFile(char **str)
{

	FILE *fptr;

	fptr = fopen("encryptedMessage.txt","w");
	if (fptr == NULL)
	{
		printf("Cannot open file!\n");
		exit(0);
	}

	int j = 0;
	while(strcmp(str[j],"END") != 0)
	{		
		fprintf(fptr, "%s\n",str[j]);
		j++;
	}

	printf("encryptedMessage.txt criado com sucesso!\n");
	fclose(fptr);
}


void writeDecryptedMessageToFile(char **str)
{

	FILE *fptr;

	fptr = fopen("decryptedMessage.txt","w");
	if (fptr == NULL)
	{
		printf("Cannot open file!\n");
		exit(0);
	}

	int j = 0;
	while(strcmp(str[j],"END") != 0)
	{	
		fprintf(fptr, "%s ",str[j]);
		j++;
	}
	fprintf(fptr, "\n");

	printf("decryptedMessage.txt criado com sucesso!\n");
	fclose(fptr);
}

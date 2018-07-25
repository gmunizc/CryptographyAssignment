#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#define MAXLINE 1000
//
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
long aToXpowerModY(int a, int x, int y);
void writeEncryptedMessageToFile(char **str);
void writeDecryptedMessageToFile(char **str);
long *calcMods(int a,int y);

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

			char *encryptedMessage[1000];

			int i = 0;
			while(strlen(pline = getline(line, max))>0)
			{
				if(pline[0] == '\n')
				{
					//printf("\\n\n");
					continue;
				}
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

			Keys privateKeys;
			privateKeys = parseKeys(inputStream);	

			int j = 0;
			char *decryptedMessage[100];
			decryptedMessage[j++] = decrypt(privateKeys,encryptedMessage);
			decryptedMessage[j] = "END";
			
			printf("Message: %s\n",decryptedMessage[0]);
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
	encryptedMessage = calloc(300,sizeof(char));

	int x = publicKeys.key1;
	int y = publicKeys.key2;

    	char buf[50];

	int a;
	int i;
	for(i = 0; i < strlen(message) - 2; i++)
	{
		a = message[i];
		snprintf(buf, 10, "%d", (int)aToXpowerModY(a,x,y));
		strcat(encryptedMessage,buf);
		strcat(encryptedMessage," ");
	}
	a = message[i];
	snprintf(buf, 10, "%d", (int)aToXpowerModY(a,x,y));
	strcat(encryptedMessage,buf);

	return encryptedMessage;

}

char *decrypt(Keys privateKeys, char *str)
{
	char *decryptedMessage;
	decryptedMessage = calloc(1000,sizeof(char));
	char *encryptedLetter;
	encryptedLetter = calloc(10,sizeof(char));

	int a;
	int x = privateKeys.key1;
	int y = privateKeys.key2;

	int max = strlen(str);
	int c, i;

	int j = 0;
	int k = 0;
	for (i = 0; i < max && (c=str[i]) != EOF; ++i)
	{	
		if(c == ' ')
		{
			encryptedLetter[j] = '\0';
			j = 0;
			a = atoi(encryptedLetter);
			decryptedMessage[k] = (int)aToXpowerModY(a,x,y);
			k++;

			
		}
		else if(c == '\n')
		{	
			encryptedLetter[j] = '\0';
			j = 0;
			a = atoi(encryptedLetter);
			decryptedMessage[k] = (int)aToXpowerModY(a,x,y);
			k++;

			decryptedMessage[k] = c;
			k++;
		}
		else
		{ 	
			encryptedLetter[j] = c;
			j++;
		}

		
	}
	encryptedLetter[j] = '\0';
	a = atoi(encryptedLetter);
	decryptedMessage[k] = (int)aToXpowerModY(a,x,y);
	decryptedMessage[k] = '\0';
	

	return decryptedMessage;
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
	str = malloc(10000 * sizeof(char));
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

long aToXpowerModY(int a, int x, int y)
{
	int mask = 1;
	int isBit1 = 0;
	long ac = 1;

	long *mods = calcMods(a,y);

	for(int i = 0; i < 31; i++)
	{
		isBit1 = x & mask;
		if(isBit1)
		{		
			ac *= mods[i];
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

long *calcMods(int a,int y)
{
	long *mods;
	mods = malloc(32 * sizeof(long));

	mods[0] = (long)a%y;

	for(int i = 1; i < 32; i++)
	{
		mods[i] = (long)pow(mods[i-1],2)%y;
	}

	return mods;	
}

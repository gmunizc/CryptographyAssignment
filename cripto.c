#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

//Constants:
#define MAXLINE 1000
#define MAXINPUTLINENUMBER 1000
#define MAXDIGITNUMBER 10
#define LOWAVGDIGITNUMBER 2
#define MAXCHARNUMBPERFILE 1000000
#define BINARYLENGH 32

//Type Keys:
typedef struct {
	int key1;
	int key2;
} Keys;

//function declarations:
char *getMyLine(char line[],int max);
char *encrypt(Keys publicKeys,char *str);
char *decrypt(Keys publicKeys,char *str);
char *readFromFile(char fileName[]);
Keys parseKeys(char* str);
long aToXpowerModY(int a, int x, int y);
void writeToFile(char fileName[],char **str);
long *calcMods(int a,int y);

//Main:
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

			char *encryptedMessage[MAXINPUTLINENUMBER];

			int i = 0;
			printf("Reading message to be encrypted...\n");
			while(strlen(pline = getMyLine(line, max))>0)
			{
				if(pline[0] == '\n')
				{
					continue;
				}
				encryptedMessage[i] = encrypt(publicKeys,pline);
				i++;
				
			}
			encryptedMessage[i] = "END";

			char fileName[] = "encryptedMessage.txt";
			writeToFile(fileName, encryptedMessage);

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
			char *decryptedMessage[2];
			decryptedMessage[j++] = decrypt(privateKeys,encryptedMessage);
			decryptedMessage[j] = "END";
			
			printf("Decrypted Message:\n");
			printf("%s\n",decryptedMessage[0]);

			char fileName[] = "decryptedMessage.txt";
			writeToFile(fileName, decryptedMessage);

		}
		if(failedExec)
		{
			printf("Unexpected subcommand!\n");
			printf("This program can only run with subcommand -c or -d!\n");
		}
	}

	return 0;
}

//Read line of input message:
char *getMyLine(char line[],int max)
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

//Encrypt line of input message:
char *encrypt(Keys publicKeys, char *message)
{

	char *encryptedMessage;
	encryptedMessage = calloc(MAXDIGITNUMBER * MAXLINE,sizeof(char));
	char newLine[] = {'\n','\0'};

	int x = publicKeys.key1;
	int y = publicKeys.key2;

    	char buf[MAXDIGITNUMBER];

	int a;
	int i;
	for(i = 0; i < strlen(message) - 2; i++)
	{
		a = message[i];
		snprintf(buf, MAXDIGITNUMBER, "%d", (int)aToXpowerModY(a,x,y));
		strcat(encryptedMessage,buf);
		strcat(encryptedMessage," ");
	}
	a = message[i];
	snprintf(buf, MAXDIGITNUMBER, "%d", (int)aToXpowerModY(a,x,y));
	strcat(encryptedMessage,buf);
	strcat(encryptedMessage,newLine);

	return encryptedMessage;

}

//Decrypt the whole encrypted message read from file:
char *decrypt(Keys privateKeys, char *str)
{
	char *decryptedMessage;
	decryptedMessage = calloc(MAXCHARNUMBPERFILE/LOWAVGDIGITNUMBER,sizeof(char));
	char *encryptedLetter;
	encryptedLetter = calloc(MAXDIGITNUMBER,sizeof(char));

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

//Read the content of a file:
char *readFromFile(char fileName[])
{
	FILE *fptr;

	fptr = fopen(fileName,"r");
	if (fptr == NULL)
	{
		printf("Error -	Cannot open file: %s!\n", fileName);
		exit(0);
	}

	char *str;
	str = malloc(MAXCHARNUMBPERFILE * sizeof(char));

	int i = 0;
	int c;	
	c = fgetc(fptr);
	while (c != EOF)
	{	
		if(i >= MAXCHARNUMBPERFILE)
		{	printf("ERROR:\n");
			c = EOF;
			str[i-1] = '\0';
			printf("This file was too long to read it all... It was read up to its %dth character\n",MAXCHARNUMBPERFILE);
		}
		else
		{
			str[i] = c;
			i++;
			c = fgetc(fptr);
		}
	}
	fclose(fptr);
	return str;
}

//Parse a string of digits and populate a Keys type.
Keys parseKeys(char* str)
{
	Keys keys;

	char *key1;
	char *key2;

	key1 = malloc(MAXDIGITNUMBER * sizeof(char));
	key2 = malloc(MAXDIGITNUMBER * sizeof(char));

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

//Calculate (a^x)%y:
long aToXpowerModY(int a, int x, int y)
{
	int mask = 1;
	int isBit1 = 0;
	long accumulated = 1;

	long *mods = calcMods(a,y);

	for(int i = 0; i < BINARYLENGH; i++)
	{
		isBit1 = x & mask;
		if(isBit1)
		{		
			accumulated *= mods[i];
			accumulated %= y; 
		}
		mask = mask<<1;
		
	}

	return accumulated;

}

//Write a message to a file:
void writeToFile(char fileName[],char **str)
{

	FILE *fptr;

	fptr = fopen(fileName,"w");
	if (fptr == NULL)
	{
		printf("Cannot open file!\n");
		exit(0);
	}

	int j = 0;
	while(strcmp(str[j],"END") != 0)
	{		
		fprintf(fptr, "%s",str[j]);
		j++;
	}
	fprintf(fptr, "\n");
	printf("%s criado com sucesso!\n", fileName);
	fclose(fptr);
}

//Calculate all the mods of each binary bit of a to the power of 2:
long *calcMods(int a,int y)
{
	long *mods;
	mods = malloc(BINARYLENGH * sizeof(long));

	mods[0] = (long)a%y;

	for(int i = 1; i < BINARYLENGH; i++)
	{
		mods[i] = (long)pow(mods[i-1],2)%y;
	}

	return mods;	
}

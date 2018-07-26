#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//Constants:
#define MAXDIGITNUMBER 10

//Type Primes:
typedef struct {
	int prime1;
	int prime2;
	int arePrime;
} Primes;

//Type Keys:
typedef struct {
	int key1;
	int key2;
} Keys;

//function declarations:
int isPrime(int n);
Primes getPrimes(void);
int getPrimeWith(int n);
Keys getPublicKeys(Primes primes);
Keys getPrivateKeys(Primes primes);
void writeKeysToFile(char fileName[], Keys keys);


//Main:
int main()
{
	Primes primes = getPrimes();

	if(primes.arePrime)
	{
		Keys publicKeys;
		publicKeys = getPublicKeys(primes);

		Keys privateKeys;
		privateKeys = getPrivateKeys(primes);

		writeKeysToFile("numcripto.txt",publicKeys);
		writeKeysToFile("numdescripto.txt",privateKeys);

	}
	return 0;
}

//Tell if a number n is prime:
int isPrime(int n){

	if((n % 2) == 0)
	{
		return 0;
	}

	int max = floor(sqrt((float)n));
	for (int i = 3; i <= max; i+=2){
		if((n % i) == 0){
			return 0;
		}
	}
	return 1;
}

//Get the prime numbers typed from the keyboard:
Primes getPrimes(void){
	
	Primes primes;
	primes.arePrime = 0;	

	char textPrime1[MAXDIGITNUMBER];
	printf("Digite um número primo:\n");
	fgets(textPrime1, sizeof(textPrime1), stdin);
	
	primes.prime1 = atoi(textPrime1);
	
	if(!isPrime(primes.prime1))
	{
		printf("O número inserido não é primo!\n");
	}
	else
	{
		char textPrime2[MAXDIGITNUMBER];
		printf("Digite outro número primo:\n");
		fgets(textPrime2, sizeof(textPrime2), stdin);
	
		primes.prime2 = atoi(textPrime2);
		
		if(!isPrime(primes.prime2))
		{
			printf("O número inserido não é primo!\n");
		}	
	}

	if(isPrime(primes.prime1) && isPrime(primes.prime2))
	{
		primes.arePrime = 1;
		return primes;
	}
	else
	{
		primes.prime1 = 0;
		primes.prime2 = 0;
		return primes;
	}
}

//Return the number that doesn't have any commom denominators with n:
int getPrimeWith(int n){
	int max;
	max = floor(sqrt((float)n));
	
	int primeWith;
	if((max % 2) == 0)
	{
		primeWith = ++max;
	}
	else
	{
		primeWith = max;
	}

	int count = 0;
	for (int j = primeWith+2; count < 1; j+=2){
		if(isPrime(j))
		{
			primeWith = j;
			count++;
		}
	}
		
	
	return primeWith;

}

//Get populated publicKeys from Primes:
Keys getPublicKeys(Primes primes){

	Keys publicKeys;

	int prime1 = primes.prime1;
	int prime2 = primes.prime2;
	
	int multPrime = prime1 * prime2;
	int multAntPrime = (prime1-1) * (prime2-1);

	publicKeys.key1 = getPrimeWith(multAntPrime);
	publicKeys.key2 = multPrime;

	return publicKeys;
}

//Get populated privateKeys from Primes:
Keys getPrivateKeys(Primes primes){
	
	Keys privateKeys;

	int prime1 = primes.prime1;
	int prime2 = primes.prime2;
	
	int multPrime = prime1 * prime2;
	int multAntPrime = (prime1-1) * (prime2-1);

	int publicKey1 = getPrimeWith(multAntPrime);

	int n = 1;
	int found = 0;
	while(!found)
	{
		if(((n*publicKey1) % multAntPrime) == 1)
		{
			found = 1;
		}
		else
		{
			n++;
		}
	}
	
	int privateKey1 = n;
	int privateKey2 = multPrime;	

	privateKeys.key1 = privateKey1; 
	privateKeys.key2 = privateKey2;

	return privateKeys;
}

//Write the keys to a file:
void writeKeysToFile(char fileName[], Keys keys){

	int key1 = keys.key1;
	int key2 = keys.key2;

	FILE *fptr;

	fptr = fopen(fileName,"w");
	if (fptr == NULL)
	{
		printf("Cannot open file!\n");
		exit(0);
	}
	fprintf(fptr, "%d %d\n",key1, key2);
	printf("%s criado com sucesso!\n",fileName);
	fclose(fptr);
}

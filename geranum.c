#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


typedef struct {
	int prime1;
	int prime2;
	int arePrime;
} Primes;

typedef struct {
	int key1;
	int key2;
} Keys;

//function declarations:
int isPrime(int n);
Primes getPrimes(void);
int getPrimeWith(int n);
Keys getPublicKeys(Primes primes);
void generatePublicKeysFile(Keys publicKeys);

int main()
{
	Primes primes = getPrimes();

	if(primes.arePrime)
	{
		Keys publicKeys;
		publicKeys = getPublicKeys(primes);
		
		generatePublicKeysFile(publicKeys);

	}
	return 0;
}

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

Primes getPrimes(void){
	
	Primes primes;
	primes.arePrime = 0;	

	char textPrime1[10];
	printf("Digite um número primo:\n");
	fgets(textPrime1, sizeof(textPrime1), stdin);
	
	primes.prime1 = atoi(textPrime1);
	
	if(!isPrime(primes.prime1))
	{
		printf("O número inserido não é primo!\n");
	}
	else
	{
		char textPrime2[10];
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

void generatePublicKeysFile(Keys publicKeys){

	int publicKey1 = publicKeys.key1;
	int publicKey2 = publicKeys.key2;

	FILE *fptr;

	fptr = fopen("numcripto.txt","w");
	if (fptr == NULL)
	{
		printf("Cannot open file!\n");
		exit(0);
	}
	fprintf(fptr, "%d %d\n",publicKey1, publicKey2);
	printf("numcripto.txt criado com sucesso!\n");
	fclose(fptr);
}

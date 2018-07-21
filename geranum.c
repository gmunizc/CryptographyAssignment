#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int isPrime(int n);

int main()
{
	int prime1,prime2;

	char textPrime1[10];
	printf("Digite um número primo:\n");
	fgets(textPrime1, sizeof(textPrime1), stdin);
	
	prime1 = atoi(textPrime1);
	
	if(!isPrime(prime1))
	{
		printf("O número inserido não é primo!\n");
	}
	else
	{
		char textPrime2[10];
		printf("Digite outro número primo:\n");
		fgets(textPrime2, sizeof(textPrime2), stdin);
	
		prime2 = atoi(textPrime2);
		
		if(!isPrime(prime2))
		{
			printf("O número inserido não é primo!\n");
		}	
	}

	if(isPrime(prime1) && isPrime(prime2))
	{
		printf("Perfeito!\n");
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

#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
	int failedExec = 1;
	if(strcmp(*(argv+1), "-c") == 0)
	{	
		failedExec = 0;
		printf("We are about to encrypt!\n");
	}	
	if(strcmp(*(argv+1), "-d") == 0)
	{
		failedExec = 0;
		printf("We are about to decrypt!\n");
	}
	if(failedExec)
	{
		printf("Unexpected subcommand!\n");
	}

	return 0;
}

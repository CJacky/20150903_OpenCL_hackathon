#include <stdio.h>
#include <stdlib.h>

#define ARRAY_LEN 4096
#define RUN_COUNT 1000

int max_print=20;
unsigned long long fnd_count=0;

void checker(int round, char* buf)
{
	int i;

	for(i=0; i<ARRAY_LEN; i++)
	{
		switch(buf[i])
		{
			case 'H':
			case 'E':
			case 'L':
			case 'O':
			case 'W':
			case 'R':
			case 'D':
				if(max_print)
				{
					printf("%d\t%d\t%c\n", round, i, buf[i]);
					max_print--;
				}
				fnd_count++;
				break;
			
			default:
				break;
		}
	}
}

int main(int argc, char* argv[])
{
	int i;
	char *buf;

	for(i=0; i<RUN_COUNT; i++)
	{
		buf = (char*)malloc(ARRAY_LEN*sizeof(char));
		checker( i, buf );
		free(buf);
	}
	
	printf("Total find %llu data\n", fnd_count);

	return 0;
}

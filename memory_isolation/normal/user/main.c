#include <stdio.h>
#include <stdlib.h>

#define STR_NUM 1024
#define RUN_COUNT 262144

void func(char *buf)
{
	int i, j;
	for(i=0; i<STR_NUM; i++)
	{
		j = i*10;
		buf[j  ] = 'H';
		buf[j+1] = 'E';
		buf[j+2] = 'L';
		buf[j+3] = 'L';
		buf[j+4] = 'O';
		buf[j+5] = 'W';
		buf[j+6] = 'O';
		buf[j+7] = 'R';
		buf[j+8] = 'L';
		buf[j+9] = 'D';
	}
}

int main(int argc, char* argv[])
{
	int i;
	char *buf;

	for(i=0; i<RUN_COUNT; i++)
	{
		buf = (char*)malloc(STR_NUM*10*sizeof(char));
		func(buf);
		free(buf);
	}

	return 0;
}

#include <stdio.h>

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
			case 'A':
			case 'B':
			case 'C':
			case 'D':
			case 'E':
			case 'F':
			case 'G':
			case 'H':
			case 'I':
			case 'J':
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

int main()
{
	int i;
	size_t buf_size = ARRAY_LEN*sizeof(char);
	char *h_buf, *d_buf;

	cudaSetDevice(0);

	for(i=0; i<RUN_COUNT; i++)
	{
		h_buf = (char*)malloc(buf_size);
		cudaMalloc( (void**)&d_buf, buf_size ); 

		cudaMemcpy( h_buf, d_buf, buf_size, cudaMemcpyDeviceToHost ); 

		checker(i, h_buf);

		free(h_buf);
		cudaFree( d_buf );
	}

	printf("Total find %llu data\n", fnd_count);

	return 0;
}

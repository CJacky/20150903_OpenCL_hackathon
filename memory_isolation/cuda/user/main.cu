#include <stdio.h>

#define STR_NUM 1024 
#define RUN_COUNT 1

__global__ void func(char *buf) 
{
	int idx = gridDim.x*blockIdx.x;
	buf[idx  ] = 'A';
	buf[idx+1] = 'B';
	buf[idx+2] = 'C';
	buf[idx+3] = 'D';
	buf[idx+4] = 'E';
	buf[idx+5] = 'F';
	buf[idx+6] = 'G';
	buf[idx+7] = 'H';
	buf[idx+8] = 'I';
	buf[idx+9] = 'J';
}

int main()
{
	int i;
	size_t buf_size = STR_NUM*10*sizeof(char);
	char *h_buf, *d_buf;

	cudaSetDevice(0);

	for(i=0; i<RUN_COUNT; i++)
	{
		h_buf = (char*)malloc(buf_size);
		cudaMalloc( (void**)&d_buf, buf_size ); 

		func<<<STR_NUM, 10>>>(d_buf);
		cudaMemcpy( h_buf, d_buf, buf_size, cudaMemcpyDeviceToHost ); 

		free(h_buf);
		cudaFree( d_buf );
	}
	return 0;
}

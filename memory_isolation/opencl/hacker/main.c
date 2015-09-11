#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<CL/cl.h>

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

int main(){

	cl_platform_id platform;
	cl_device_id device;
	cl_context context;
	cl_command_queue queue;
	cl_int i, err;

	cl_program program;
	FILE *program_handle;
	char *program_buffer, *program_log;
	size_t program_size, log_size;
	cl_kernel kernel;
	size_t work_units_per_kernel;

	char buf[ARRAY_LEN]={0};

	cl_mem mem_buf;

	char* ext_data;
	size_t ext_size;

	clGetPlatformIDs(1, &platform, NULL);
	clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 1, &device, NULL);
	context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);

	program_handle = fopen("main.cl", "r");
	fseek(program_handle, 0, SEEK_END);
	program_size = ftell(program_handle);
	rewind(program_handle);
	program_buffer = malloc(program_size+1);
	program_buffer[program_size] = '\0';
	fread(program_buffer, 1, program_size, program_handle);
	fclose(program_handle);

	program = clCreateProgramWithSource(context, 1, (const char**)&program_buffer, &program_size, &err);
	free(program_buffer);
	clBuildProgram(program, 0, NULL, NULL, NULL, NULL);

	kernel = clCreateKernel(program, "matvec_mult", &err);
	queue = clCreateCommandQueueWithProperties(context, device, NULL, &err);

	for(i=0; i<RUN_COUNT; i++)
	{
		mem_buf = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(buf) , NULL, &err);
		clSetKernelArg(kernel, 0, sizeof(cl_mem), &mem_buf);

		work_units_per_kernel = ARRAY_LEN;
		clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &work_units_per_kernel, NULL, 0, NULL, NULL);
		clEnqueueReadBuffer(queue, mem_buf, CL_TRUE, 0, sizeof(buf), buf, 0, NULL, NULL);

		checker(i, buf);
		clReleaseMemObject(mem_buf);
	}

	clReleaseKernel(kernel);
	clReleaseCommandQueue(queue);
	clReleaseProgram(program);
	clReleaseContext(context);

	printf("Total find %llu data\n", fnd_count);

	return 0;
}

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<CL/cl.h>

#define STR_NUM 1024
#define RUN_COUNT 262144

int main(){

	cl_platform_id platform;
	cl_device_id device;
	cl_context context;
	cl_command_queue queue;
	cl_int i, j, err;

	cl_program program;
	FILE *program_handle;
	char *program_buffer, *program_log;
	size_t program_size, log_size;
	cl_kernel kernel;
	size_t work_units_per_kernel;

	int buf_size;
	char *buf;

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
	
	buf_size = STR_NUM*10*sizeof(char);

	for(i=0; i<RUN_COUNT; i++)
	{
		buf = (char*)malloc(buf_size);
		mem_buf = clCreateBuffer(context, CL_MEM_WRITE_ONLY, buf_size , NULL, &err);
		clSetKernelArg(kernel, 0, sizeof(cl_mem), &mem_buf);

		work_units_per_kernel = STR_NUM;
		clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &work_units_per_kernel, NULL, 0, NULL, NULL);
		clEnqueueReadBuffer(queue, mem_buf, CL_TRUE, 0, buf_size, buf, 0, NULL, NULL);

		clReleaseMemObject(mem_buf);
		free(buf);
	}

/*
	for(i=0; i<STR_NUM; i++)
	{
		printf("%d: ", i);
		for(j=0; j<10; j++)
		{
			printf("%c", buf[i*10+j]);
		}
		printf("\n");
	}
*/
	printf("number of HELLOWORLD: %d\n", STR_NUM);
	printf("number of round: %d\n", RUN_COUNT);

	clReleaseKernel(kernel);
	clReleaseCommandQueue(queue);
	clReleaseProgram(program);
	clReleaseContext(context);

	return 0;
}

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <CL/cl.h>

#define dim(i,j) ((i)*Mdim+(j))

/////////////////////////////////////////////////////////
// Program main
/////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
	struct timeval  begin, end;
	int device_gpu = argc>2? 0 : 1;
	int Mdim = argc>1? atoi(argv[1]) : 10;
	int i, j;

	unsigned int matrix_size = sizeof(double) * Mdim * Mdim;

	double* h_A = (double*) malloc(matrix_size);
	double* h_B = (double*) malloc(matrix_size);
	double* h_C = (double*) malloc(matrix_size);

	// initialize matrix
	for( i=0; i<Mdim; i++ )
		for( j=0; j<Mdim; j++ )
		{
			h_A[dim(i,j)] = i*10+j;
			h_B[dim(i,j)] = i*10+j;
			h_C[dim(i,j)] = 0;
		}

	// 3. print out A and B
	/*
	if( Mdim <= 10 )
	{
		printf("\n\nMatrix A\n");
		for(int i = 0; i < Mdim*Mdim; i++)
		{
			printf("%02.1f ", h_A[i]);
			if(((i + 1) % Mdim) == 0)
				printf("\n");
		}

		printf("\n\nMatrix B\n");
		for(int i = 0; i < Mdim*Mdim; i++)
		{
			printf("%02.1f ", h_B[i]);
			if(((i + 1) % Mdim) == 0)
				printf("\n");
		}	
	}
	*/
	// 5. Initialize OpenCL
	// OpenCL specific variables

	gettimeofday(&begin, NULL);

	cl_platform_id platform;
	cl_context context;
	cl_device_id device;
	cl_command_queue queue;
	cl_program program;
	cl_kernel kernel;
	cl_int err;
	FILE *program_handle;
	char *program_buffer;
	size_t program_size;
	size_t localWorkSize[2], globalWorkSize[2];

	// OpenCL device memory for matrices
	cl_mem d_A;
	cl_mem d_B;
	cl_mem d_C;

	/*****************************************/
	/* Initialize OpenCL */
	/*****************************************/
	clGetPlatformIDs(1, &platform, NULL);
	
	if( device_gpu )
		clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
	else
		clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &device, NULL);
	
	
	context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);

	program_handle = fopen("ocl_mmul.cl", "r");
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

	kernel = clCreateKernel(program, "matrixMul", &err);
	queue = clCreateCommandQueueWithProperties(context, device, NULL, &err);


	// Setup device memory
	d_A = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, matrix_size, h_A, &err);
	d_B = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, matrix_size, h_B, &err);

	d_C = clCreateBuffer(context, CL_MEM_WRITE_ONLY, matrix_size, NULL, &err);

	clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_A);
	clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_B);
	clSetKernelArg(kernel, 2, sizeof(cl_mem), &d_C);
	clSetKernelArg(kernel, 3, sizeof(int), &Mdim);

	localWorkSize[0] = 16;
	localWorkSize[1] = 16;
	globalWorkSize[0] = Mdim;
	globalWorkSize[1] = Mdim;

	clEnqueueNDRangeKernel(queue, kernel, 2, NULL, globalWorkSize, localWorkSize, 0, NULL, NULL);
	clEnqueueReadBuffer(queue, d_C, CL_TRUE, 0, matrix_size, h_C, 0, NULL, NULL);

	gettimeofday(&end, NULL);

	// 9. print out the results
	/*
	if( Mdim <= 10 )
	{
		for(int i = 0; i < Mdim*Mdim; i++)
		{
			printf("%-5.1f ", h_C[i]);
			if(((i + 1) % Mdim) == 0)
				printf("\n");
		}
		printf("\n");
	}
	*/
	FILE *f;
	if(device_gpu )
	{
		f=fopen("result_ocl_gpu.txt", "a");
		fprintf(f,"%d\t%f\n", Mdim, (double) (end.tv_usec - begin.tv_usec) / 1000000 +  (double) (end.tv_sec  - begin.tv_sec));
	}
	else
	{
		f=fopen("result_ocl_cpu.txt", "a");
		fprintf(f,"%d\t%f\n", Mdim, (double) (end.tv_usec - begin.tv_usec) / 1000000 +  (double) (end.tv_sec  - begin.tv_sec));
	}
	fclose(f);
	
	// 10. clean up memory
	free(h_A);
	free(h_B);
	free(h_C);

	clReleaseMemObject(d_A);
	clReleaseMemObject(d_C);
	clReleaseMemObject(d_B);

	clReleaseKernel(kernel);
	clReleaseCommandQueue(queue);
	clReleaseProgram(program);
	clReleaseContext(context);

	return 0;
}

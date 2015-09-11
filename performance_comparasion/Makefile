
MDIM ?= 10

all: ocl omp

omp: omp_mmul.c
	gcc -fopenmp omp_mmul.c -o openmp

ocl: ocl_mmul.c
	gcc -std=c99 ocl_mmul.c -o opencl -lOpenCL
	

clean:
	rm -rf omp ocl

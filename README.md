# 20150903_OpenCL_hackathon
## HACKATHON ON HETEROGENEOUS COMPUTING

There is group 8 鴿子封包's result.
The project contain 3 parts:

1. Matrix multiplication Benchmark
	- Measure the cost time of doing matrix multiplication on 3 differect type.
		1. OpenCL with HSA CPU
		2. OpenCL with HSA GPU
		3. OpenMP with HSA CPU

2. Memory isolation
	- When we do our computation on the devices, there is high possibility that 
	we can read the data leaved by previous users. As the reason above, we do some experiments about it on 3 platforms.
		1. x86 system memory
		2. cuda GPU memory
		3. HSA GPU memory
		
3. OpenCL exploitation
	- Runtime compilers should allocate some memory	region that is modifiable and executable 
	and we can use the memory to do some BAD things.


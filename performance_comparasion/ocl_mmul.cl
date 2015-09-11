/* kernel.cl 
 * Matrix multiplication: C = A * B.
 * Device code.
 */

// OpenCL Kernel
__kernel void matrixMul(__global double* A, __global double* B, __global double* C, int Mdim)
{

	// 2D Thread ID
	// Old CUDA code
	//int tx = blockIdx.x * TILE_SIZE + threadIdx.x;
	//int ty = blockIdx.y * TILE_SIZE + threadIdx.y;
	int tx = get_global_id(0); 
	int ty = get_global_id(1);

	// value stores the element that is 
	// computed by the thread
	double value = 0;
	for (int k = 0; k < Mdim; ++k)
	{
		double elementA = A[ tx * Mdim + k  ];
		double elementB = B[ k  * Mdim + ty ];
		value += elementA * elementB;
	}

	// Write the matrix to device memory each 
	// thread writes one element
	C[ tx * Mdim + ty] = value;
}

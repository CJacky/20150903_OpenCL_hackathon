#!/bin/bash

rm -f result_omp.txt result_ocl.txt

i=8
while [ $i -lt 4097 ]
do
	for j in $(seq 1 10)
	do  
		./openmp $i
		./opencl $i
		./opencl $i cpu
	done
i=$((i*2))
done

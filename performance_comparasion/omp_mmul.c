#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int main (int argc, char *argv[]) 
{
	int	tid, nthreads, i, j, k, chunk, Mdim;
	double	**a, **b, **c;
	struct timeval  begin, end;
	chunk = 10;                    /* set loop iteration chunk size */

	if(argc>1){
		Mdim = atoi(argv[1]);
	}else{
		Mdim = 10;
	}

	a = (double**)malloc(Mdim*sizeof(double*));
	b = (double**)malloc(Mdim*sizeof(double*));
	c = (double**)malloc(Mdim*sizeof(double*));

	for(i=0; i<Mdim; i++)
	{
		a[i] = (double*)malloc(Mdim*sizeof(double));
		b[i] = (double*)malloc(Mdim*sizeof(double));
		c[i] = (double*)malloc(Mdim*sizeof(double));
	}


	/*** Initialize matrices ***/
	for (i=0; i<Mdim; i++)
		for (j=0; j<Mdim; j++)
		{
			a[i][j]= i*10+j;
			b[i][j]= i*10+j;
			c[i][j]= 0;
		}

	gettimeofday(&begin, NULL);

	#pragma omp parallel shared(a,b,c,Mdim) private(i,j,k)
	{
		#pragma omp for
		for (i=0; i<Mdim; i++)    
		{
			//#pragma omp for schedule (static, chunk)
			for(j=0; j<Mdim; j++)       
			{	
				for (k=0; k<Mdim; k++)
				{
					c[i][j] += a[i][k] * b[k][j];
				}
			}
		}
	}

	gettimeofday(&end, NULL);
	/*** Print results ***/
	/*
	if( Mdim <= 10 )
	{
		printf("******************************************************\n");
		printf("Result Matrix:\n");
		for (i=0; i<Mdim; i++)
		{
			for (j=0; j<Mdim; j++) 
				printf("%6.2f   ", c[i][j]);
			printf("\n"); 
		}
		printf("******************************************************\n");
	}
	*/
	FILE *f = fopen("result_omp.txt", "a");
	fprintf (f,"%d\t%f\n", Mdim, (double) (end.tv_usec - begin.tv_usec) / 1000000 +
			(double) (end.tv_sec  - begin.tv_sec));
	fclose(f);

	return 0;
}

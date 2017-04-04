/* 	Author: Mike Little
	CPU: AMD FX8350 8-core @ 4.5GHz
	VM: virtualbox, 4-cores with 100% execution cap

	
*/

/*
	
                    execution time (sec.) for threads:
code                1       2       3       4
------------------------------------------------------
serial              0.6    -       -       -
my_parallel         0.7    1.3    1.5    1.2    
final_parallel      0.1    0.1    0.1    0.1 
*/

/* Source: http://openmp.org/mp-documents/Intro_To_OpenMP_Mattson.pdf */

#include <stdio.h>
#include <omp.h>
static long num_steps = 100000000;
double step;
int NUM_THREADS = 2;


void final_parallel_pi()
{
	int i;
	double x, pi, sum = 0.0;
	double start_time, run_time;

	start_time = omp_get_wtime();

	step = 1.0/(double) num_steps;
	#pragma omp parallel
	{
		double x;
		#pragma omp for reduction(+:sum)
			for(i=0; i<num_steps;i++)
			{
				x=(i+0.5)*step;
				sum=sum + 4.0/(1.0+x*x);
			}
	}
	pi = step*sum;
	run_time = omp_get_wtime() - start_time;
	printf("%.1f",run_time);
}



void my_parallel_pi()
{
	int i, nthreads;
	double pi, sum[NUM_THREADS];
	double start_time, run_time;
	step = 1.0/(double)num_steps;

	start_time = omp_get_wtime();

	omp_set_num_threads(NUM_THREADS);
	
	#pragma omp parallel
	{
		int i, id, nthrds;
		double x;
		id = omp_get_thread_num();
		nthrds = omp_get_num_threads();
		if (id==0) nthreads = nthrds;
	
	for(i=id,sum[id]=0.0;i<num_steps;i=i+nthrds)
	{
		x = (i+0.5) * step;
		sum[id] += 4.0/(1.0+x*x);
	}
   	
   	}
   	
   	for(i=0, pi=0.0; i<nthreads; i++)
   		pi += sum[i] * step;

	run_time = omp_get_wtime() - start_time;
	printf("%.1f",run_time);

}

void serial_pi()
{

	int i;
	double x, pi, sum = 0.0;
	double start_time, run_time;
	step = 1.0/(double) num_steps;

	start_time = omp_get_wtime();

	for (i=1;i<= num_steps; i++)
	{
		x = (i-0.5)*step;
		sum = sum + 4.0/(1.0+x*x);
	}
	pi = step * sum;
	run_time = omp_get_wtime() - start_time;
	printf("%.1f",run_time);
}  



int main ()
{
	int i;
	
	printf("                    execution time (sec.) for threads:\n");
	printf("code                1       2       3       4\n");
	printf("------------------------------------------------------\n");

	/* seriral */
	printf("serial              ");
	serial_pi();
	printf("    -       -       -\n");

	/* my_parallel */
	printf("my_parallel         ");
	for (i = 1; i <= 4; i++) 
	{
		NUM_THREADS = i;
		my_parallel_pi();
		printf("    ");
	}
	printf("\n");

	/* final_parallel */
	printf("final_parallel      ");
	for (i = 1; i <= 4; i++) 
	{
		NUM_THREADS = i;
		final_parallel_pi();
		printf("    ");
	}
	printf("\n");

}	
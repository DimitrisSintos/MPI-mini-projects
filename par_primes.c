//Dimitrios Sintos A.M.:4012
#include <stdio.h>
#include <mpi.h>
#include <time.h>

#define UPTO 10000000

int main(int argc, char *argv[])
{	
	long int count =0 , counter = 0,      /* number of primes */
        	lastprime, lstprm;  /* the last prime found */
	double starttime, endtime, totaltime, t1, t2, overheads;
	int myid, nproc;
	long int i, num, divisor, quotient, remainder, temp, N;

	starttime = MPI_Wtime();
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&myid);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	

	

	if (UPTO < 2) return 0;

	t1 = MPI_Wtime();
	if(myid == 0){
		printf("Parallel prime number calculations using MPI :\n...\n");

		count = 1;                         /* 2 is the first prime */
		lastprime = 2;
		
	}

	/* The actual computation */ 
	
	for (i = myid; i < (UPTO-1)/2; i+= nproc) {    /* For every odd number */
		num = 2*i + 3;

		divisor = 1;
		do 
		{
			divisor += 2;                  /* Divide by the next odd */
			quotient  = num / divisor;  
			remainder = num % divisor;  
		} while (remainder && divisor <= quotient);  /* Don't go past sqrt */

		if (remainder || divisor == num) /* num is prime */
		{
			count++;
			lastprime = num;
		}
	}
	t2 = MPI_Wtime();
	
	MPI_Reduce(&count, &counter, 1, MPI_LONG,MPI_SUM, 0, MPI_COMM_WORLD);	
	MPI_Reduce(&lastprime,&lstprm,1,MPI_LONG,MPI_MAX,0,MPI_COMM_WORLD);
	//t2 = MPI_Wtime();
	endtime   = MPI_Wtime();
	totaltime = endtime - starttime;
	overheads = totaltime -  (t2 - t1);

	if(myid == 0){
		printf("[mpi] count = %ld, last = %ld  \n", counter, lstprm );
		printf(" Total time = %f || Overheads time = %f || Computations time = %f \n",totaltime, overheads, t2- t1);
	}
	

	MPI_Finalize();

	
	
	return 0;
}

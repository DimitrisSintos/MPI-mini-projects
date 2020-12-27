//Dimitrios Sintos A.M.:4012
#include <stdio.h>
#include <omp.h>
#include <time.h>
#define UPTO 10000000

long int count,      /* number of primes */
         lastprime;  /* the last prime found */


void serial_primes(long int n) {
	long int i, num, divisor, quotient, remainder;

	if (n < 2) return;
	count = 1;                         /* 2 is the first prime */
	lastprime = 2;

	for (i = 0; i < (n-1)/2; ++i) {    /* For every odd number */
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
}




int main()
{	double t1;
	printf("Serial prime number calculations:\n\n");
	
	/* Time the following to compare performance 
	 */
	t1 = (double) clock();
	serial_primes(UPTO);        /* time it */
	printf("[serial] count = %ld, last = %ld (time = %f)\n", count, lastprime,((double) clock() - t1)/CLOCKS_PER_SEC);
	
	return 0;
}

/* Parallel program for matrix-matrix product using MPI.
 *
 * Dimitrios Sintos A.M.:4012
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define N 1024


int A[N][N], B[N][N], C[N][N];
int readmat(char *fname, int *mat, int n), 
    writemat(char *fname, int *mat, int n);

int main(int argc , char *argv[])
{
	int i, j, k, sum, myid, nproc, (*Aa)[N], (*Cc)[N];
	double start,end, t1,t2,w1,w2, total;
	
	start = MPI_Wtime();
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&myid);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);


	int WORK = N /nproc;

	if(myid == 0){
		printf("Parallel matrix multiplication\n...\n");
	/* Read matrices from files: "A_file", "B_file" */
		t1 = MPI_Wtime();
		if (readmat("Amat1024", (int *) A, N) < 0) 
			exit( 1 + printf("file problem\n") );
		if (readmat("Bmat1024", (int *) B, N) < 0) 
			exit( 1 + printf("file problem\n"));
		t2 = MPI_Wtime(); //t2 - t1

		
		
	}
	/*	t3 = MPI_Wtime();
	if (readmat("Bmat1024", (int *) B, N) < 0) 
			exit( 1 + printf("file problem\n"));

	t4 = MPI_Wtime(); */
	
	// MAlloc Aa and Cc

	Aa = (int(*)[N] )malloc(WORK*N*sizeof(int));	
	Cc = (int(*)[N] )malloc(WORK*N*sizeof(int));

	MPI_Scatter(A,WORK*N,MPI_INT,Aa,WORK*N,MPI_INT,0 ,MPI_COMM_WORLD);
	MPI_Bcast(B,N*N,MPI_INT,0,MPI_COMM_WORLD);
	//MPI_Barrier(MPI_COMM_WORLD);
	w1 = MPI_Wtime();
	for (i = 0; i < WORK; i++)
		for (j = 0; j < N; j++)
		{
			for (k = sum = 0; k < N; k++)
				sum += Aa[i][k]*B[k][j];
			Cc[i][j] = sum;
		}
	w2 = MPI_Wtime();

	/* Save result in "Cmat1024"
	 */
	MPI_Gather(Cc,WORK*N,MPI_INT, C, N*WORK,MPI_INT,0,MPI_COMM_WORLD);

	if(myid == 0){
		end = MPI_Wtime();
		total = (end- start) - (t2-t1);
		writemat("Cmat1024", (int *) C, N);
		printf("Total time: %f || Overheads time: %f\nComputations time: %f\n", total , total-(w2-w1), w2-w1);

	}

	//MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
	

	return (0);

}


/* Utilities to read & write matrices from/to files
 * VVD
 */

#define _mat(i,j) (mat[(i)*n + (j)])


int readmat(char *fname, int *mat, int n)
{
	FILE *fp;
	int  i, j;
	
	if ((fp = fopen(fname, "r")) == NULL)
		return (-1);
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
			if (fscanf(fp, "%d", &_mat(i,j)) == EOF)
			{
				fclose(fp);
				return (-1); 
			};
	fclose(fp);
	return (0);
}


int writemat(char *fname, int *mat, int n)
{
	FILE *fp;
	int  i, j;
	
	if ((fp = fopen(fname, "w")) == NULL)
		return (-1);
	for (i = 0; i < n; i++, fprintf(fp, "\n"))
		for (j = 0; j < n; j++)
			fprintf(fp, " %d", _mat(i, j));
	fclose(fp);
	return (0);
}

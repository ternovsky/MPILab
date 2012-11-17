#include <stdio.h> 
#include "mpi.h"
#define DEFAULT_TAG 0

int main(int argc, char **argv) {
    int rank, value, size;
	double t1, t2;

    MPI_Status status;
    MPI_Init( &argc, &argv );
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

	printf("Before: Sum = %d in the process %d\n", rank, rank);
	if (rank == 0) {
		t1 = MPI_Wtime();

		MPI_Send( &rank, 1, MPI_INT, rank + 1, DEFAULT_TAG, MPI_COMM_WORLD);
		MPI_Recv(&value, 1, MPI_INT, rank + 1, DEFAULT_TAG, MPI_COMM_WORLD, &status);	

		t2 = MPI_Wtime();	
		printf("Time = %f seconds.\n", t2 - t1);
	} else { 
		MPI_Recv(&value, 1, MPI_INT, rank - 1, DEFAULT_TAG, MPI_COMM_WORLD, &status);
		value = value + rank;
		if (rank == size - 1) {
			MPI_Send(&value, 1, MPI_INT, rank - 1, DEFAULT_TAG, MPI_COMM_WORLD);
		} else {
			MPI_Send(&value, 1, MPI_INT, rank + 1, DEFAULT_TAG, MPI_COMM_WORLD);
			MPI_Recv(&value, 1, MPI_INT, rank + 1, DEFAULT_TAG, MPI_COMM_WORLD, &status);	
			MPI_Send(&value, 1, MPI_INT, rank - 1, DEFAULT_TAG, MPI_COMM_WORLD);
		}
	}
	MPI_Barrier(MPI_COMM_WORLD);
	printf("After: Sum = %d in the process %d.\n", value, rank);
	
	MPI_Finalize( );
    return 0;
}

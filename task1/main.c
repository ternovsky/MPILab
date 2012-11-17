#include <stdio.h> 
#include "mpi.h"
#define DEFAULT_TAG 0

int main(int argc, char **argv) {
    int rank, value, size;
    MPI_Status status;
    MPI_Init( &argc, &argv );
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (rank == 0) {
		MPI_Send( &rank, 1, MPI_INT, rank + 1, DEFAULT_TAG, MPI_COMM_WORLD);
		MPI_Recv(&value, 1, MPI_INT, rank + 1, DEFAULT_TAG, MPI_COMM_WORLD, &status);	
		printf("Sum: %d\n", value);	
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

	MPI_Finalize( );
    return 0;
}

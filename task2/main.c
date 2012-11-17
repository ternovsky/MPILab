#include <stdio.h> 
#include "mpi.h"
#define DEFAULT_TAG 0

int main(int argc, char **argv) {

	int rank, value, size, sum, depth, power, i;

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	sum = rank;
	printf("Before: Sum = %d in the process %d\n", sum, rank);
	
	depth = ceil(log(size)/log(2));
	for(i = 0; i < depth; i++) {
		power = pow(2, i);
		if (rank % (2 * power) == 0 && rank < size - power) {
			MPI_Recv(&value, 1, MPI_INT, rank + power, DEFAULT_TAG, MPI_COMM_WORLD, &status);
			sum += value;
		}
		if ((rank + power) % (2 * power) == 0) {
			MPI_Send(&sum, 1, MPI_INT, rank - power, DEFAULT_TAG, MPI_COMM_WORLD);
			MPI_Recv(&value, 1, MPI_INT, rank - power, DEFAULT_TAG, MPI_COMM_WORLD, &status);
			sum = value;
		}
	}

	for(i = depth - 1; i >= 0; i--) {
		power = pow(2, i);
		if (rank % (2 * power) == 0 && rank < size - power) {
			MPI_Send(&sum, 1, MPI_INT, rank + power, DEFAULT_TAG, MPI_COMM_WORLD);
		}
	}

	printf("After: Sum = %d in the process %d\n", sum, rank);

	MPI_Finalize();
    return 0;
}

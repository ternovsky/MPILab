#include <stdio.h> 
#include "mpi.h"
#define DEFAULT_TAG 0

int main(int argc, char **argv) {

	int rank, value, size, sum, depth, power, selected, i, j, last, last_sending, last_sending_count, to;

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	sum = rank;
	printf("Before: Sum = %d in the process %d\n", sum, rank);
	
	depth = ceil(log(size)/log(2));
	last = size - 1;
	last_sending = depth - 1;
	last_sending_count = pow(2, depth) - size;
	for(i = 0; i < depth; i++) {
		power = pow(2, i);
		if (rank / power % 2 == 0) {
			selected = rank + power;
		} else {
			selected = rank - power;
		}
		if (selected < size) {
			if (i == last_sending && rank == last) {
				to = selected;
				for (j = 0; j < last_sending_count; j++) {
					to++;
					MPI_Send(&sum,   1, MPI_INT, to, DEFAULT_TAG, MPI_COMM_WORLD); 
				}
			}
			MPI_Send(&sum,   1, MPI_INT, selected, DEFAULT_TAG, MPI_COMM_WORLD);		
			MPI_Recv(&value, 1, MPI_INT, selected, DEFAULT_TAG, MPI_COMM_WORLD, &status);
			sum += value;
        } else if (i == last_sending) {
			if (rank < last) {	
				MPI_Recv(&value, 1, MPI_INT, last, DEFAULT_TAG, MPI_COMM_WORLD, &status);
				sum += value;
			}			
		}
	}

	printf("After: Sum = %d in the process %d\n", sum, rank);

	MPI_Finalize();
    return 0;
}

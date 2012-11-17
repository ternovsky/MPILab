#include <stdio.h> 
#include "mpi.h"
#define DEFAULT_TAG 0

int main(int argc, char **argv) {

	int rank, value, size, sum, depth, power, selected, i, j, last, last_sending, deficit, to, start;
	double t1, t2;

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	sum = rank;
	printf("Before: Sum = %d in the process %d\n", sum, rank);

	if (rank == 0) 
		t1 = MPI_Wtime();
	
	depth = ceil(log(size)/log(2));
	last = size - 1;
	last_sending = depth - 1;
	deficit = pow(2, depth) - size;
	for(i = 0; i < depth; i++) {
		power = pow(2, i);
		if (rank / power % 2 == 0) {
			selected = rank + power;
		} else {
			selected = rank - power;
		}
		if (selected <= last) {
			if (rank == last && selected < last) {
				to = selected;
				for(j = 0; j < deficit; j++) {
					to++;		
					if (to == last) 
						break;			
					MPI_Send(&sum,   1, MPI_INT, to, DEFAULT_TAG, MPI_COMM_WORLD); 
				}
			}
			MPI_Send(&sum,   1, MPI_INT, selected, DEFAULT_TAG, MPI_COMM_WORLD);		
			MPI_Recv(&value, 1, MPI_INT, selected, DEFAULT_TAG, MPI_COMM_WORLD, &status);
			sum += value;
        } else {
			start = last - power;
			if (last / power % 2 != 0 && rank - start <= deficit && rank != last) {
				MPI_Recv(&value, 1, MPI_INT, last, DEFAULT_TAG, MPI_COMM_WORLD, &status);
				sum += value;
			}
		}
	}
	MPI_Barrier(MPI_COMM_WORLD);

	if (rank == 0) {
		t2 = MPI_Wtime();	
		printf("Time = %f seconds.\n", t2 - t1);
	}

	printf("After: Sum = %d in the process %d\n", sum, rank);

	MPI_Finalize();
    return 0;
}

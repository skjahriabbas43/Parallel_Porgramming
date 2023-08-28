#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define ARRAY_SIZE 10

int main(int argc, char** argv) {
    int rank, size;
    int* array = NULL;
    int local_sum = 0;
    int global_sum = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        // Generate the array on the root process
        array = (int*)malloc(ARRAY_SIZE * sizeof(int));
        for (int i = 0; i < ARRAY_SIZE; ++i) {
            array[i] = i + 1;
        }
    }

    // Broadcast the array from the root process to all processes
    MPI_Bcast(array, ARRAY_SIZE, MPI_INT, 0, MPI_COMM_WORLD);

    // Calculate local sum of elements
    for (int i = rank; i < ARRAY_SIZE; i += size) {
        local_sum += array[i];
    }

    // Reduce local sums to get the global sum
    MPI_Allreduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Array: ");
        for (int i = 0; i < ARRAY_SIZE; ++i) {
            printf("%d ", array[i]);
        }
        printf("\nGlobal sum: %d\n", global_sum);

        free(array);
    }

    MPI_Finalize();
    return 0;
}

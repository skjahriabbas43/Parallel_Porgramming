#include <stdio.h>
#include <mpi.h>

#define ARRAY_SIZE 10

int main(int argc, char** argv) {
    int rank, size;
    int array[ARRAY_SIZE];
    int local_sum = 0;
    int global_sum = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        // Generate the array
        for (int i = 0; i < ARRAY_SIZE; ++i) {
            array[i] = i + 1;
        }
    }

    // Calculate the local array size for each process
    int local_array_size = ARRAY_SIZE / size;

    // Scatter the array to local arrays
    MPI_Scatter(array, local_array_size, MPI_INT, &local_sum, local_array_size, MPI_INT, 0, MPI_COMM_WORLD);

    // Calculate local sum of elements
    for (int i = 0; i < local_array_size; ++i) {
        local_sum += local_sum;
    }

    // Reduce local sums to get the global sum
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Array: ");
        for (int i = 0; i < ARRAY_SIZE; ++i) {
            printf("%d ", array[i]);
        }
        printf("\nGlobal sum: %d\n", global_sum);
    }

    MPI_Finalize();
    return 0;
}

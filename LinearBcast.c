#include <stdio.h>
#include <mpi.h>

#define ARRAY_SIZE 20
#define ELEMENT_TO_FIND 7

int main(int argc, char** argv) {
    int rank, size;
    int array[ARRAY_SIZE];
    int local_size;
    int local_elements[ARRAY_SIZE / 5];
    int found_index = -1;
    int local_found_index = -1;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Only the root process initializes the array
    if (rank == 0) {
        for (int i = 0; i < ARRAY_SIZE; ++i) {
            array[i] = i + 1;
        }
    }

    // Broadcast the array to all processes
    MPI_Bcast(array, ARRAY_SIZE, MPI_INT, 0, MPI_COMM_WORLD);

    // Calculate local size for each process
    local_size = ARRAY_SIZE / size;

    // Divide the array among processes
    MPI_Scatter(array, local_size, MPI_INT, local_elements, local_size, MPI_INT, 0, MPI_COMM_WORLD);

    // Perform linear search on local portion of the array
    for (int i = 0; i < local_size; ++i) {
        if (local_elements[i] == ELEMENT_TO_FIND) {
            local_found_index = i + rank * local_size;
            break;
        }
    }

    // Reduce local results to find the global index where element was found
    MPI_Allreduce(&local_found_index, &found_index, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);

    if (rank == 0) {
        if (found_index != -1) {
            printf("Element %d found at index %d.\n", ELEMENT_TO_FIND, found_index);
        } else {
            printf("Element %d not found.\n", ELEMENT_TO_FIND);
        }
    }

    MPI_Finalize();
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define ARRAY_SIZE 10
#define ELEMENT_TO_FIND 7

int main(int argc, char** argv) {
    int rank, size;
    int localArraySize;
    int localArray[ARRAY_SIZE / 5]; // Each process will handle 2 elements
    int globalArray[ARRAY_SIZE] = {0};
    int foundIndex = -1;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 5) {
        printf("This program requires exactly 5 processes.\n");
        MPI_Finalize();
        return 1;
    }

    if (rank == 0) {
        // Initialize the global array with numbers from 1 to 10
        for (int i = 0; i < ARRAY_SIZE; ++i) {
            globalArray[i] = i + 1;
        }
    }

    // Scatter the global array among processes
    localArraySize = ARRAY_SIZE / size;
    MPI_Scatter(globalArray, localArraySize, MPI_INT, localArray, localArraySize, MPI_INT, 0, MPI_COMM_WORLD);

    // Perform linear search for the element 7 in the local portion
    for (int i = 0; i < localArraySize; ++i) {
        if (localArray[i] == ELEMENT_TO_FIND) {
            foundIndex = i;
            break;
        }
    }

    // Reduce results to find the first index where element 7 was found
    int reducedIndex;
    MPI_Reduce(&foundIndex, &reducedIndex, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        if (reducedIndex != -1) {
            printf("Element %d found at index %d.\n", ELEMENT_TO_FIND, reducedIndex);
        } else {
            printf("Element %d not found.\n", ELEMENT_TO_FIND);
        }
    }

    MPI_Finalize();
    return 0;
}

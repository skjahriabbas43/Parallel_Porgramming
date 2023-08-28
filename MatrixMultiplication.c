#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define ROWS 3
#define COLS 3

void printMatrix(int matrix[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    int rank, size;
    int matrixA[ROWS][COLS] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    int matrixB[ROWS][COLS] = {
        {9, 8, 7},
        {6, 5, 4},
        {3, 2, 1}
    };
    int result[ROWS][COLS];
    int local_row[COLS];
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    // Scatter rows of matrixA to different processes
    MPI_Scatter(matrixA, COLS * ROWS / size, MPI_INT, local_row, COLS, MPI_INT, 0, MPI_COMM_WORLD);

    // Broadcast matrixB to all processes
    MPI_Bcast(matrixB, ROWS * COLS, MPI_INT, 0, MPI_COMM_WORLD);
    
    // Perform local matrix multiplication
    for (int i = 0; i < ROWS; i++) {
        result[i][0] = 0;
        for (int j = 0; j < COLS; j++) {
            result[i][j] = local_row[j] * matrixB[j][i];
        }
    }
    
    // Gather results from all processes
    MPI_Gather(result, COLS * ROWS / size, MPI_INT, result, COLS * ROWS / size, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Matrix A:\n");
        printMatrix(matrixA);
        printf("Matrix B:\n");
        printMatrix(matrixB);
        printf("Matrix A * B:\n");
        printMatrix(result);
    }

    MPI_Finalize();

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#define NUM_POINTS 1000000

int main(int argc, char** argv) {
    int rank, size;
    int local_count = 0;
    int global_count = 0;
    double x, y, distance;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Seed the random number generator
    srand(rank);

    for (int i = 0; i < NUM_POINTS / size; ++i) {
        x = (double)rand() / RAND_MAX;
        y = (double)rand() / RAND_MAX;
        distance = x * x + y * y;

        if (distance <= 1.0) {
            local_count++;
        }
    }

    // Reduce local counts to get the global count
    MPI_Reduce(&local_count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double pi_approx = 4.0 * global_count / NUM_POINTS;
        printf("Approximate value of π: %f\n", pi_approx);
    }

    MPI_Finalize();
    return 0;
}

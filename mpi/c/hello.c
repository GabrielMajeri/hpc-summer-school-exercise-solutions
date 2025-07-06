#include <stdio.h>
#include <mpi.h>

int main(int argc, char* argv[])
{
    // Initialize the MPI library
    MPI_Init(&argc, &argv);

    // Determine how many processes will be run
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    printf("Hello world from rank %d out of %d processes\n", world_rank, world_size);

    // Shut down MPI
    MPI_Finalize();

    return 0;
}

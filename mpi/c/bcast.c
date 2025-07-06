#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    printf("Hello from rank #%d\n", world_rank);

    // MPI_Barrier(MPI_COMM_WORLD);

    int data;
    if (world_rank == 0)
    {
        // Root process sets the data
        data = 100;
        printf("Process 0 broadcasting data: %d\n", data);
    }

    // Broadcast the data from process 0 to all processes
    MPI_Bcast(&data, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // All processes now have the data
    printf("Process %d has data: %d\n", world_rank, data);

    MPI_Finalize();
    return 0;
}

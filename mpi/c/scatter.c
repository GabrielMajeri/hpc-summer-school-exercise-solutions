#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int* interval_endpoints = NULL;
    if (world_rank == 0)
    {
        interval_endpoints = (int*)malloc(sizeof(int) * 2 * world_size);

        srand(0);
        for (int i = 0; i < world_size; ++i)
        {
            int start = rand() % 1000;
            int end = start + rand() % 50;
            interval_endpoints[2 * i] = start;
            interval_endpoints[2 * i + 1] = end;
        }
    }

    int endpoints[2] = {-1, -1};

    // Scatter the endpoints to all processes
    MPI_Scatter(interval_endpoints, 2, MPI_INT, endpoints, 2, MPI_INT, 0, MPI_COMM_WORLD);

    printf("Node #%d: interval [%d, %d]\n", world_rank, endpoints[0], endpoints[1]);

    MPI_Finalize();

    return 0;
}

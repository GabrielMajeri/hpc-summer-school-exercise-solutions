#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int send_data = world_rank;

    // Only the root process allocates the receive buffer
    int* recv_data = NULL;
    if (world_rank == 0) {
        recv_data = (int*)malloc(world_size * sizeof(int));
    }

    // Gather data from all processes to the root
    MPI_Gather(&send_data, 1, MPI_INT, recv_data, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Root process prints the gathered data
    if (world_rank == 0) {
        printf("Process 0 gathered data: ");
        for (int i = 0; i < world_size; ++i) {
            printf("%d ", recv_data[i]);
        }
        printf("\n");
        free(recv_data);
    }

    MPI_Finalize();
    return 0;
}

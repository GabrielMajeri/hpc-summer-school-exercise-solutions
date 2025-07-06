#include <stdio.h>
#include <mpi.h>

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    printf("Hello from process #%d!\n", rank);

    int data;
    if (rank == 0) {
        data = 123;
        printf("Sending data from process #%d to #%d...\n", 0, 1);
        MPI_Send(&data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    } else if (rank == 1) {
        printf("Receiving data from process #%d...\n", 0);
        MPI_Recv(&data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 1 received data: #%d\n", data);
    }

    MPI_Finalize();
    return 0;
}

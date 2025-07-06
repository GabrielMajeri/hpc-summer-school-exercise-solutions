#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const size_t num_doubles = 256 * 1024 * 1024;
    printf("Size of buffer: %ld MiB\n", num_doubles / 1024 / 1024);
    double* send_data = (double*)malloc(num_doubles * sizeof(double));
    double* recv_data = (double*)malloc(num_doubles * sizeof(double));

    // Initialize send data
    for (size_t i = 0; i < num_doubles; i++)
    {
        send_data[i] = (double)rank + i * 0.000001;
    }

    MPI_Request req_send, req_recv;
    double t_start_isendrecv, t_end_isendrecv;
    double t_start_wait, t_end_wait;

    // Start non-blocking communication
    t_start_isendrecv = MPI_Wtime();
    if (rank == 0)
    {
        MPI_Isend(send_data, num_doubles, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, &req_send);
        MPI_Irecv(recv_data, num_doubles, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, &req_recv);
    }
    else if (rank == 1)
    {
        MPI_Isend(send_data, num_doubles, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &req_send);
        MPI_Irecv(recv_data, num_doubles, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &req_recv);
    }
    t_end_isendrecv = MPI_Wtime();

    // Wait for communication to complete
    t_start_wait = MPI_Wtime();
    MPI_Wait(&req_send, MPI_STATUS_IGNORE);
    MPI_Wait(&req_recv, MPI_STATUS_IGNORE);
    t_end_wait = MPI_Wtime();


    printf("Process %d: MPI_Isend/Irecv took %.6f seconds\n", rank, t_end_isendrecv - t_start_isendrecv);
    printf("Process %d: MPI_Wait took %.6f seconds\n", rank, t_end_wait - t_start_wait);

    free(send_data);
    free(recv_data);

    MPI_Finalize();

    return 0;
}

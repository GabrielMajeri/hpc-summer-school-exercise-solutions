#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int array_size = 10;
    int data[array_size];

    if (rank == 0) {
        // Initialize data with 0 to 9
        for (int i = 0; i < array_size; i++) {
            data[i] = i;
        }

        // Define vector type: send every second element (i.e., elements at indices 0, 2, 4, 6, 8)
        MPI_Datatype vector_type;
        MPI_Type_vector(5, 1, 2, MPI_INT, &vector_type);
        MPI_Type_commit(&vector_type);

        // Send using custom datatype
        MPI_Send(data, 1, vector_type, 1, 0, MPI_COMM_WORLD);

        MPI_Type_free(&vector_type);
    } else if (rank == 1) {
        // Initialize to sentinel value
        for (int i = 0; i < array_size; i++) {
            data[i] = -1;
        }

        // Receive 5 elements normally into the first 5 positions of `data`
        MPI_Recv(data, 5, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Print the result
        printf("Process 1 received data: ");
        for (int i = 0; i < array_size; i++) {
            printf("%d ", data[i]);
        }
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}

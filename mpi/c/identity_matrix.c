// Constructs and initializes an identity matrix,
// sharded across multiple processes using MPI.

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

const int matrix_size = 8;

int clamp(int value, int min, int max)
{
    if (value < min)
    {
        return min;
    }
    if (value > max)
    {
        return max;
    }
    return value;
}

int min(int a, int b)
{
    return a < b ? a : b;
}

int main(int argc, char *argv[])
{
    // Initialize MPI
    MPI_Init(&argc, &argv);

    // Determine process rank/index
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int root = 0;

    // Determine how many rows each process should store
    int num_processes;
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    if (rank == root)
    {
        printf("Total number of worker processes: %d\n", num_processes);
    }

    int size_div = matrix_size / num_processes;
    int block_size = clamp(
        matrix_size % num_processes == 0 ? size_div : size_div + 1,
        1, matrix_size);
    if (rank == root)
    {
        printf("Matrix size: %d\n", matrix_size);
        printf("Number of rows stored per each process: %d\n", block_size);
    }

    int row_start_index = rank * block_size;
    if (row_start_index > matrix_size)
    {
        // This worker is superfluous, which means we have more processes than rows...
        return 0;
    }

    int row_end_index = min(row_start_index + block_size, matrix_size);

    const int data_size = block_size * matrix_size;
    double *data = calloc(sizeof(double), data_size);
    if (data == NULL)
    {
        printf("Process #%d failed to allocate memory for storing matrix block\n", rank);
        MPI_Abort(MPI_COMM_WORLD, 1);
        return 1;
    }

    for (int i = row_start_index; i < row_end_index; ++i)
    {
        int k = i - row_start_index;

        data[k * matrix_size + i] = 1;
    }

    if (rank == root)
    {
        int matrix_num_elements = matrix_size * matrix_size;
        double *matrix = calloc(sizeof(double), matrix_num_elements);
        if (matrix == NULL)
        {
            printf("Root process (rank %d) failed to allocate memory for storing final assembled matrix\n", root);
            MPI_Abort(MPI_COMM_WORLD, 1);
            return 1;
        }

        MPI_Gather(data, data_size, MPI_DOUBLE, matrix, data_size, MPI_DOUBLE, root, MPI_COMM_WORLD);

        printf("Constructed matrix:\n");

        // Let's print out the matrix
        for (int i = 0; i < matrix_size; ++i)
        {
            for (int j = 0; j < matrix_size; ++j)
            {
                printf("%f ", matrix[i * matrix_size + j]);
            }
            puts("");
        }

        free(matrix);
    }
    else
    {
        // printf("Process #%d sending its %d doubles...\n", rank, data_size);
        MPI_Gather(data, data_size, MPI_DOUBLE, NULL, data_size, MPI_DOUBLE, root, MPI_COMM_WORLD);
    }

    free(data);
    data = NULL;

    MPI_Finalize();

    return 0;
}

#include <mpi.h>
#include <stdio.h>
#include <math.h>

// Returns the minimum of two integers
int min(int a, int b)
{
    return a < b ? a : b;
}

// Evaluate the function to integrate
double f_eval(double x)
{
    return 1.0/(1.0 + x*x);
}

int main(int argc, char* argv[])
{
    // Initialize the MPI library
    MPI_Init(&argc, &argv);

    double time_start = MPI_Wtime();

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    const int num_points = 1e9;
    const double h = (1 - 0) / (double)(num_points - 1);

    const int block_size = num_points / world_size;

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    const int block_start = 0 + world_rank * block_size;
    const int block_end = min(num_points, 0 + (world_rank + 1) * block_size);

    // printf("Process #%d: block from i = %d to i = %d\n", world_rank, block_start, block_end);

    double time_start_compute = MPI_Wtime();
    double result = 0.0;
    for (int i = block_start; i < block_end; ++i)
    {
        double x = 0 + h * i;
        result += h * f_eval(x);
        // printf("x = %f, f(x) = %f\n", x, f_eval(x));
    }
    double time_end_compute = MPI_Wtime();

    const double compute_duration = time_end_compute - time_start_compute;
    printf("Process #%d: Compute took %f seconds\n", world_rank, compute_duration);

    double grand_total;
    double time_start_reduce = MPI_Wtime();
    MPI_Reduce(&result, &grand_total, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    double time_end_reduce = MPI_Wtime();

    const double reduce_duration = time_end_reduce - time_start_reduce;
    printf("Process #%d: Reduce took %f seconds\n", world_rank, reduce_duration);

    grand_total = grand_total - 0.5 * h * f_eval(0.0) - 0.5 * h * f_eval(1.0);
    grand_total *= 4;

    double time_end = MPI_Wtime();

    const double total_duration = time_end - time_start;
    printf("Process #%d: Took %f seconds\n", world_rank, total_duration);

    if (world_rank == 0)
    {
        printf("pi approx equal to %.15f\n", grand_total);
        printf("true value of pi: %.15f\n", M_PI);

        FILE* fout = fopen("output.txt", "a");
        fprintf(fout, "%d %f %f %f\n", world_size, compute_duration, reduce_duration, total_duration);
        fclose(fout);
    }

    MPI_Finalize();

    return 0;
}

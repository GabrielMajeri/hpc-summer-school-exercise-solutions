// Matrix multiplication, parallelized using OpenMP.

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

typedef struct
{
    int dimension;
    double *data;
} SquareMatrix;

void matrix_initialize(SquareMatrix *matrix, int dimension)
{
    assert(matrix && "Must be given a pointer to a valid square matrix structure");
    assert(dimension > 0 && "Dimension must be strictly positive integer");

    matrix->dimension = dimension;

    const int size = dimension * dimension;
    matrix->data = calloc(size, sizeof(double));
}

void matrix_fill_random(SquareMatrix input)
{
    for (int i = 0; i < input.dimension; ++i)
    {
        for (int j = 0; j < input.dimension; ++j)
        {
            input.data[i * input.dimension + j] = ((double)rand()) / RAND_MAX;
        }
    }
}

SquareMatrix matrix_multiply(SquareMatrix A, SquareMatrix B)
{
    const int dimension = A.dimension;
    assert(dimension == B.dimension && "B must have same dimension as A");

    SquareMatrix C;
    matrix_initialize(&C, dimension);

#pragma omp parallel for
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            double acc = 0;
            for (int k = 0; k < dimension; ++k)
            {
                acc += A.data[i * dimension + k] * B.data[k * dimension + j];
            }
            C.data[i * dimension + j] = acc;
        }
    }

    return C;
}

void matrix_print(const SquareMatrix A)
{
    for (int i = 0; i < A.dimension; ++i)
    {
        for (int j = 0; j < A.dimension; ++j)
        {
            printf("%f ", A.data[i * A.dimension + j]);
        }
        puts("");
    }
}

void matrix_free(SquareMatrix *A)
{
    A->dimension = 0;
    free(A->data);
    A->data = NULL;
}

int main()
{
    const int dimension = 512;

    SquareMatrix A, B;
    matrix_initialize(&A, dimension);
    matrix_initialize(&B, dimension);

    srand(17);
    matrix_fill_random(A);
    matrix_fill_random(B);

    const double start = omp_get_wtime();
    SquareMatrix result = matrix_multiply(A, B);
    const double end = omp_get_wtime();
    const double duration = end - start;
    printf("Multiplication took %f seconds\n\n", duration);

    // printf("A:\n");
    // matrix_print(A);
    // printf("\nB:\n");
    // matrix_print(B);
    // printf("\nC = A * B:\n");
    // matrix_print(result);
    assert(result.dimension == A.dimension);

    puts("");

    matrix_free(&A);
    matrix_free(&B);
    matrix_free(&result);

    return 0;
}

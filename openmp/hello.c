#include <omp.h>
#include <stdio.h>

const int num_threads = 8;

int main()
{
    printf("Hello, OpenMP!\n");

#pragma omp parallel for num_threads(num_threads)
    for (int i = 0; i < num_threads; ++i)
    {
        printf("Thread number %d out of %d\n", omp_get_thread_num(), num_threads);
    }

    return 0;
}

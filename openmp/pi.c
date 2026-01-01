// Approximate the value of pi using numerical integration,
// with shared-memory parallelism provided by OpenMP.

#include <stdio.h>
#include <math.h>
#include <omp.h>

double integrand(double x)
{
    return 4.0 / (1.0 + x * x);
}

const double PI = 3.14159265358979323846;

const int num_threads = 16;

int main()
{
    const int num_points = 10000000;
    const double left = 0, right = 1;
    const double dx = (right - left) / num_points;

    double total = 0.0;
#pragma omp parallel for reduction(+ : total) num_threads(num_threads)
    for (int i = 0; i < num_points; ++i)
    {
        const double x_leftpoint = left + dx * i;
        const double x_midpoint = x_leftpoint + dx / 2;
        total += integrand(x_midpoint) * dx;
    }

    printf("Estimated value of pi: %.15f\n", total);
    printf("True(r) value of pi: %.15f\n", PI);

    const double absolute_error = fabs(PI - total);
    printf("Absolute error: %.e\n", absolute_error);
    const double relative_error = absolute_error / PI;
    printf("Relative error: %.e\n", relative_error);

    return 0;
}

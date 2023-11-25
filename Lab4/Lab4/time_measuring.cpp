#include "time_measuring.h"

double serial_solving(double* arr, int size)
{
    auto start = std::chrono::high_resolution_clock::now();
    serial_bubble(arr, size);
    auto end = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.f;
}

double parallel_solving(double* arr, int size)
{
    double start = MPI_Wtime();
    parallel_bubble(arr, size);
    double end = MPI_Wtime();

    return end - start;
}

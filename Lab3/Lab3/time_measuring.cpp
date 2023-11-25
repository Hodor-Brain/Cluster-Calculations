#include "time_measuring.h"

double serial_solving(double* matrix, double* vector, double* result, int size)
{
    auto start = std::chrono::high_resolution_clock::now();
    serial_result_calculation(matrix, vector, result, size);
    auto end = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.f;
}

double parallel_solving(double* matrix, double* vector, double* result, int size)
{
    double start = MPI_Wtime();
    mpi_result_calculation(matrix, vector, result, size);
    double end = MPI_Wtime();

    return end - start;
}

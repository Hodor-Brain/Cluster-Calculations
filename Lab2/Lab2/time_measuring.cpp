#include "time_measuring.h"

double serial_multiplication(int* matrix1, int* matrix2, int* result, int size)
{
    auto start = std::chrono::high_resolution_clock::now();
    multiply(matrix1, matrix2, result, size);
    auto end = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.f;
}

double parallel_multiplication(int* matrix1, int* matrix2, int* result, int size)
{
    double start = MPI_Wtime();
    mpi_multiply(matrix1, matrix2, result, size);
    double end = MPI_Wtime();

    return end - start;
}

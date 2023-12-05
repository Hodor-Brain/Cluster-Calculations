#include "serial_algo.h"
#include "mpi_algo.h"

#include <string>
#include <chrono>


void test_mpi_first(int argc, char** argv, long iterations) {
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double start = MPI_Wtime();
    double res = mpi_first(iterations);
    double end = MPI_Wtime();

    if (rank == 0) {
        std::cout << "PI: " << res << std::endl;
        std::cout << "Time: " << end - start << "s." << std::endl;
    }

    MPI_Finalize();
}
void test_mpi_second(int argc, char** argv, long iterations) {
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double start = MPI_Wtime();
    double res = mpi_second(iterations);
    double end = MPI_Wtime();

    if (rank == 0) {
        std::cout << "PI: " << res << std::endl;
        std::cout << "Time: " << end - start << "s." << std::endl;
    }

    MPI_Finalize();
}
void test_serial(long iterations) {
    auto start = std::chrono::high_resolution_clock::now();
    double res = serial_monte_carlo(iterations);

    auto end = std::chrono::high_resolution_clock::now();

    float time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.f;

    std::cout << "PI: " << res << std::endl;
    std::cout << "Time: " << time << "s." << std::endl;
}

int main(int argc, char** argv)
{
    srand(time(0));

    long iterations = 1000000;

    if (argc > 1) {
        iterations = std::stol(argv[1]);
    }

    int type = 0;// 0 - serial, 1 - mpi first, 2 - mpi second
    if (argc > 2) {
        type = std::stoi(argv[2]);
    }

    switch (type)
    {
    case 1: test_mpi_first(argc, argv, iterations); break;
    case 2: test_mpi_second(argc, argv, iterations); break;
    default: test_serial(iterations); break;
    }
    



    return 0;
}

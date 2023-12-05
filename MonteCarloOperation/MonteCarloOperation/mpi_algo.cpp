#include "mpi_algo.h"

double mpi_first(long iterations) {
    int rank, number;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &number);

    double x, y;
    int count = 0;
    double z;
    double pi = 0;
   
    int* counts = new int[number] {};
    long* iters = new long[number] {};

    srand(11431 * rank);

    for (int i = 0; i < iterations; ++i)
    {
        x = ((double)rand()) / RAND_MAX;
        y = ((double)rand()) / RAND_MAX;
        z = sqrt(x * x + y * y);
        if (z <= 1)
        {
            count++;
        }
    }

    if (rank != 0) {
        MPI_Send(&count,
            1,
            MPI_INT,
            0,
            1,
            MPI_COMM_WORLD);
        MPI_Send(&iterations,
            1,
            MPI_LONG,
            0,
            2,
            MPI_COMM_WORLD);
    }

    if (rank == 0)
    {
        counts[0] = count;
        iters[0] = iterations;

        for (int i = 1; i < number; ++i)
        {
            MPI_Recv(&counts[i],
                1,
                MPI_INT,
                MPI_ANY_SOURCE,
                1,
                MPI_COMM_WORLD,
                MPI_STATUS_IGNORE);
            MPI_Recv(&iters[i],
                1,
                MPI_LONG,
                MPI_ANY_SOURCE,
                2,
                MPI_COMM_WORLD,
                MPI_STATUS_IGNORE);
        }
    }

    if (rank == 0)
    {
        int finalcount = 0;
        long finalniter = 0;
        for (int i = 0; i < number; ++i)
        {
            finalcount += counts[i];
            finalniter += iters[i];
        }

        pi = ((double)finalcount / (double)finalniter) * 4.0;
    }

    delete[] iters;
    delete[] counts;


    return pi;
}

double mpi_second(long iterations) {
    int rank;     
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double x, y;                     

    int count = 0;               
    double z;                       
    double pi = 0;    

    int reducedcount;                  
    long reducedniter;
    srand(time(nullptr) * rank);

    for (int i = 0; i < iterations; i++)                
    {          
        x = (double)rand() / RAND_MAX;         
        y = (double)rand() / RAND_MAX;         
        z = sqrt((x * x) + (y * y));             
        if (z <= 1)
        {
            count++;
        }
    }


    MPI_Reduce(&count,
        &reducedcount,
        1,
        MPI_INT,
        MPI_SUM,
        0,
        MPI_COMM_WORLD);

    MPI_Reduce(&iterations,
        &reducedniter,
        1,
        MPI_INT,
        MPI_SUM,
        0,
        MPI_COMM_WORLD);

    if (rank == 0)
    {
        pi = ((double)reducedcount / (double)reducedniter) * 4.0;
    }

    return pi;
}

#include "serial_algo.h"
double serial_monte_carlo(int iterations) {
    double x, y;
    int count = 0;
    double z;
    double pi;

    for (int i = 0; i < iterations; ++i)
    {
        x = (double)rand() / RAND_MAX;
        y = (double)rand() / RAND_MAX;
        z = sqrt((x * x) + (y * y));

        if (z <= 1) count++;
    }
    pi = ((double)count / (double)iterations) * 4.0;


    return pi;
}
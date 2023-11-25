#include <iostream>
#include <fstream>


int main()
{
    const int PROCESSES = 4;
    int processes[PROCESSES] = { 1, 2, 4, 8 };

    const int SIZE = 8, TIMES = 3;
    int sizes[SIZE] = { 10, 100, 500,1000, 1500, 2000, 2500, 3000 };

    double a = 0.047, b = 53.29, t = 0.00000000193, w = 0.0000038147;

    for (int i = 0; i < 1; i++) {
        int p = processes[i];
        std::cout << "\n\np: " << p << std::endl;
        for (int j = 0; j < SIZE; j++) {
            int n = sizes[j];

            double sum = 0, res;
            for (int k = 2; k < n; k++) {
                sum += 3 * k + 2 * k * k;
            }
            std::cout << sum << std::endl;
            sum *= t;
            sum *= 1.0 / p;

            res = sum + (n - 1) * log2(p) * (3 * a + w * (n + 2) / b);
        }
    }

    return 0;
}

#include <iostream>
#include <chrono>

int main() {
    auto start_time = std::chrono::high_resolution_clock::now();

    int result = 0;
    for (int i = 0; i < 1000000; ++i) {
        result += i;
    }

    auto end_time = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    std::cout << "Result: " << result << std::endl;
    std::cout << "Execution Time: " << duration.count() << " milliseconds" << std::endl;
    std::cout << "Execution Time for +=: " << duration.count()/ 1000000.0 << " milliseconds" << std::endl;

    return 0;
}

#include "time_measuring.h"
#include "utilities.h"
#include <fstream>

bool check_correct(double* arr, int size) {
	for (int i = 1; i < size; i++) {
		if (arr[i - 1] > arr[i]) return false;
	}

	return true;
}

void file_logging(double seconds) {
	std::ofstream file{ "logs.txt", std::ios_base::app };

	file << seconds << "\n";

	file.close();
}

void mpi_run(int argc, char** argv, int size, bool print_all, double* arr) {
	int rank;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0) {
		generate_vector(arr, size);
		std::cout << "Size: " << size << std::endl;
	}
	if (print_all && rank == 0) {
		std::cout << "Array:\n";
		print_vector(arr, size);
	}

	double duration = parallel_solving(arr, size);

	if (print_all && rank == 0) {
		std::cout << "\nResult:\n";
		print_vector(arr, size);
	}

	if (rank == 0) {
		bool correctk = check_correct(arr, size);
		std::cout << (correctk ? "Correct" : "Incorrect") << std::endl;

		std::cout << "\nTime: " << duration << "s\n";
		file_logging(duration);
	}

	MPI_Finalize();
}

void serial_run(int size, bool print_all, double* arr) {
	generate_vector(arr, size);
	std::cout << "Size: " << size << std::endl;

	if (print_all) {

		std::cout << "Array:\n";
		print_vector(arr, size);
	}

	double duration = serial_solving(arr, size);

	if (print_all) {
		std::cout << "\nResult:\n";
		print_vector(arr, size);
	}
	bool correctk = check_correct(arr, size);
	std::cout << (correctk ? "Correct" : "Incorrect") << std::endl;

	std::cout << "\nTime: " << duration << "s\n";
	file_logging(duration);

}
int main(int argc, char** argv) {
	srand(time(0));

	int size = 100;

	if (argc > 1) {
		size = std::stoi(argv[1]);
	}

	bool parallel = false;
	if (argc > 2) {
		parallel = std::stoi(argv[2]);
	}

	bool print_all = false;
	if (argc > 3) {
		print_all = std::stoi(argv[3]);
	}

	double* arr = new double[size] {};

	if (parallel) mpi_run(argc, argv, size, print_all, arr);
	else serial_run(size, print_all, arr);

	delete[] arr;

	return 0;
}
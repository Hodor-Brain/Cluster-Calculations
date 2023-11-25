#include "time_measuring.h"
#include <fstream>


bool check_correct(int* matrix, int* vector, int* result, int size) {
	int* serial_result = new int[size];
	multiply(matrix, vector, serial_result, size);

	for (int i = 0; i < size; i++) {
		if (serial_result[i] != result[i]) return false;
	}

	return true;
}

void file_logging(double seconds) {
	std::ofstream file{ "logs.txt", std::ios_base::app};

	file << seconds << "\n";

	file.close();
}

void mpi_run(int argc, char** argv, int size, bool print_all, int *matrix, int *vector, int *result) {
	int rank;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0) {
		generate_data(matrix, vector, size);
		std::cout << "Size: " << size << std::endl;
	}
	if (print_all && rank == 0) {

		std::cout << "Matrix:\n";
		print_matrix(matrix, size);
		std::cout << "Vector:\n";
		print_vector(vector, size);
	}

	double duration = parallel_multiplication(matrix, vector, result, size);

	if (print_all && rank == 0) {
		std::cout << "\nResult:\n";
		print_vector(result, size);
	}

	if (rank == 0) {
		bool correct = check_correct(matrix, vector, result, size);

		std::cout << "\nMultiplication is " << (correct ? "correct" : "incorrect") << "!\n";
		std::cout << "\nTime: " << duration << "s\n";
		file_logging(duration);
	}

	MPI_Finalize();
}

void serial_run(int size, bool print_all, int* matrix, int* vector, int* result) {
	generate_data(matrix, vector, size);
	std::cout << "Size: " << size << std::endl;

	if (print_all) {

		std::cout << "Matrix:\n";
		print_matrix(matrix, size);
		std::cout << "Vector:\n";
		print_vector(vector, size);
	}

	double duration = serial_multiplication(matrix, vector, result, size);

	if (print_all) {
		std::cout << "\nResult:\n";
		print_vector(result, size);
	}

	std::cout << "\nTime: " << duration << "s\n";
	file_logging(duration);

}
int main(int argc, char** argv) {
	int size = 5;

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

	int* matrix = new int[size * size], * vector = new int[size], * result = new int[size];

	if (parallel) mpi_run(argc, argv, size, print_all, matrix, vector, result);
	else serial_run(size, print_all, matrix, vector, result);

	delete[] matrix;
	delete[] vector;
	delete[] result;
	
	return 0;
}
#include "time_measuring.h"
#include <fstream>


bool check_correct(int* matrix1, int* matrix2, int* result, int size) {
	int* serial_result = new int[size * size] {};
	multiply(matrix1, matrix2, serial_result, size);

	for (int i = 0; i < size*size; i++) {
		if (serial_result[i] != result[i]) return false;
	}

	return true;
}

void file_logging(double seconds) {
	std::ofstream file{ "logs.txt", std::ios_base::app };

	file << seconds << "\n";

	file.close();
}

void mpi_run(int argc, char** argv, int size, bool print_all, int* matrix1, int* matrix2, int* result) {
	int rank;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0) {
		generate_data(matrix1, matrix2, size);
		std::cout << "Size: " << size << std::endl;
	}
	if (print_all && rank == 0) {

		std::cout << "Matrix1:\n";
		print_matrix(matrix1, size);
		std::cout << "Matrix2:\n";
		print_matrix(matrix2, size);
	}

	double duration = parallel_multiplication(matrix1, matrix2, result, size);

	if (print_all && rank == 0) {
		std::cout << "\nResult:\n";
		print_matrix(result, size);
	}

	if (rank == 0) {
		bool correct = check_correct(matrix1, matrix2, result, size);

		std::cout << "\nMultiplication is " << (correct ? "correct" : "incorrect") << "!\n";
		std::cout << "\nTime: " << duration << "s\n";
		file_logging(duration);
	}

	MPI_Finalize();
}

void serial_run(int size, bool print_all, int* matrix1, int* matrix2, int* result) {
	generate_data(matrix1, matrix2, size);
	std::cout << "Size: " << size << std::endl;

	if (print_all) {

		std::cout << "Matrix1:\n";
		print_matrix(matrix1, size);
		std::cout << "Matrix2:\n";
		print_matrix(matrix2, size);
	}

	double duration = serial_multiplication(matrix1, matrix2, result, size);

	if (print_all) {
		std::cout << "\nResult:\n";
		print_matrix(result, size);
	}

	std::cout << "\nTime: " << duration << "s\n";
	file_logging(duration);

}
int main(int argc, char** argv) {
	int size = 3;

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

	int* matrix1 = new int[size * size], * matrix2 = new int[size * size], * result = new int[size * size] {};
	
	if (parallel) mpi_run(argc, argv, size, print_all, matrix1, matrix2, result);
	else serial_run(size, print_all, matrix1, matrix2, result);

	delete[] matrix1;
	delete[] matrix2;
	delete[] result;

	return 0;
}
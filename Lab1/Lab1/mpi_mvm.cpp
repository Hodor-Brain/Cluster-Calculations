#include "mpi_mvm.h"


void init_data(int*& process_rows, int*& process_result, int size, int row_number) {
	process_rows = new int[row_number * size] {};
	process_result = new int[row_number] {};
}

void free_data(int* process_rows, int* process_result) {
	delete[] process_rows;
	delete[] process_result;
}

void distribution(int rank, int number, int* matrix, int* process_rows, int* vector, int size) {

	MPI_Bcast(vector, size, MPI_INT, 0, MPI_COMM_WORLD);

	int* send_indexes = new int[number];
	int* send_number = new int[number];

	int rest = size;
	int row_number = (size / number);

	send_number[0] = row_number * size;
	send_indexes[0] = 0;

	for (int i = 1; i < number; i++) {
		rest -= row_number;
		row_number = rest / (number - i);
		send_number[i] = row_number * size;
		send_indexes[i] = send_indexes[i - 1] + send_number[i - 1];
	}

	MPI_Scatterv(matrix, send_number, send_indexes, MPI_INT, process_rows,
		send_number[rank], MPI_INT, 0, MPI_COMM_WORLD);

	delete[] send_number;
	delete[] send_indexes;
}

void parallel_calculation(int* process_rows, int* vector, int* process_result,
	int size, int row_number) {

	for (int i = 0; i < row_number; i++) {
		process_result[i] = 0;
		for (int j = 0; j < size; j++) {
			process_result[i] += process_rows[i * size + j] * vector[j];
		}
	}
}
void result_replication(int rank, int number, int* process_result, int* result, int size) {

	int* recive_number = new int[number];
	int* recive_indexes = new int[number];

	int rest = size;

	recive_indexes[0] = 0;
	recive_number[0] = size / number;

	for (int i = 1; i < number; i++) {
		rest -= recive_number[i - 1];
		recive_number[i] = rest / (number - i);
		recive_indexes[i] = recive_indexes[i - 1] + recive_number[i - 1];

	}

	MPI_Allgatherv(process_result, recive_number[rank], MPI_INT, result,
		recive_number, recive_indexes, MPI_INT, MPI_COMM_WORLD);

	delete[] recive_number;
	delete[] recive_indexes;
}

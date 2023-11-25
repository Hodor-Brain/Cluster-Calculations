#include "parallel_mvm.h"


void mpi_multiply(int* matrix, int* vector, int* result, int size) {

	int number, rank;

	MPI_Comm_size(MPI_COMM_WORLD, &number);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int row_number = ceil((double)size / number);

	int * process_rows, * process_result;

	int rest = size;
	for (int i = 0; i < rank; i++) rest = rest - rest / (number - i);

	row_number = rest / (number - rank);

	init_data(process_rows, process_result, size, row_number);

	distribution(rank, number, matrix, process_rows, vector, size);
	MPI_Barrier(MPI_COMM_WORLD);
	parallel_calculation(process_rows, vector, process_result, size, row_number);
	result_replication(rank, number, process_result, result, size);

	free_data(process_rows, process_result);
}
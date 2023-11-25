#include "parallel_les.h"

void mpi_result_calculation(double* matrix, double* vector, double* result, int size) {
	int number, rank;

	MPI_Comm_size(MPI_COMM_WORLD, &number);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	double* proc_rows;
	double* proc_vector;
	double* proc_result;
	int* proc_number;
	int* proc_ind;
	int row_number;

	init_data(proc_rows, proc_vector, proc_result, proc_number, proc_ind, size, row_number, rank, number);


	data_distribution(matrix, proc_rows, vector, proc_vector, size, row_number, number, rank, 
		proc_ind, proc_number);

	parallel_result_calculation(proc_rows, proc_vector, proc_result, size, row_number, rank, 
		proc_ind, proc_number, number);

	result_collection(proc_result, result, proc_number, rank, proc_ind);


	free_data(proc_rows, proc_vector, proc_result, proc_number, proc_ind);
}
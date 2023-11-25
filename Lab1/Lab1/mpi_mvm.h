#pragma once

#include "mpi.h"
#include "utilities.h"

//creates matrices and vectors
void init_data(int*& process_rows, int*& process_result, int size, int row_number);

//delete matrices and vectors
void free_data(int* process_rows, int* process_result);

//send data from root process to other
void distribution(int rank, int number, int* matrix, int* process_rows, int* vector, int size);

//multiply some row of matrix in processes
void parallel_calculation(int* process_rows, int* vector, int* process_result,
	int size, int row_number);

//fetch data from other processes to root process
void result_replication(int rank, int number, int* process_result, int* result, int size);
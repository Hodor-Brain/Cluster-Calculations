#pragma once
#include "mpi.h"
#include "utilities.h"
#include "serial_les.h"

void init_data(double*& proc_rows, double*& proc_vector, double*& proce_result, 
	int*& proc_number, int*& proc_ind, int size, int& row_num, int rank, int number);

void free_data(double* proc_rows, double* proc_vector, double* proce_result, 
	int* proc_number, int* proc_ind);

void data_distribution(double* matrix, double* proc_rows, double* vector,
	double* proc_vector, int size, int row_num, int number, int rank, int* proc_ind, int* proc_num);

void parallel_eliminate_columns(double* proc_rows, double* proc_vector,
	double* pivot_row, int size, int row_num, int iter, int* proc_pivot_iter);

void parallel_gaussian_elimination(double* proc_rows, double* proc_vector,
	int size, int row_num, int* pivot_iter, int* proc_ind, int* parallel_pivot_pos, int rank);

void find_back_pivot_row(int row_index, int& iter_proc_rank, int& iter_pivot_pos, int number,
	int* proc_ind);

void parallel_back_substitution(double* proc_rows, double* proc_vector, double* proc_result,
	int size, int row_number, int* parallel_pivot_pos, int rank, int* proc_pivot_iter, 
	int* proc_ind, int number);

void result_collection(double* proc_result, double* result, int* proc_number, int rank, int* proc_ind);

void parallel_result_calculation(double* proc_rows, double* proc_vector,
	double* proc_result, int size, int row_num, int rank, int* proc_ind, int* proc_num, int number);
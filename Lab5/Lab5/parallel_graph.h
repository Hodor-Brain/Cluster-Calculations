#pragma once
#include "mpi.h"
#include <algorithm>
#include "utilities.h"

void process_initialization(int*& process_rows, int& size,
	int& row_number, int rank, int number);

void process_termination(int* process_rows);

void data_distribution(int* matrix, int* process_rows, int size, int row_number, int rank, int number);
void result_collection(int* matrix, int* process_rows, int size, int row_number, int rank, int number);
void row_distribution(int* process_rows, int size, int row_number, int k, int* row, int rabk, int number);

void parallel_floyd_algo(int* process_rows, int size, int row_number, int rank, int number);

void parallel_floyd(int* matrix, int size);
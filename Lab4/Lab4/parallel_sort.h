#pragma once
#include "mpi.h"
#include "serial_sort.h"
#include <algorithm>

enum SplitMode { KeepFirstHalf, KeepSecondHalf };

#include "parallel_sort.h"

void process_initialization(int& size, double*& process_arr, int& block_size,
	int rank, int number);

void process_termination(double* process_data);

void data_distribution(double* arr, int size, double* process_arr, int block_size,
	int number, int rank);

void data_collection(double* arr, int size, double* process_arr, int block_size, int number);

void exchange_data(double* process_arr, int block_size, int dual_rank,
	double* dual_arr, int dual_block_size);

void parallel_bubble_sorting(double* process_arr, int block_size, int number, int rank);

void parallel_bubble(double* arr, int size);
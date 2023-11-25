#pragma once
#include <iostream>

void serial_gaussian_elimination(double* matrix, double* vector, int size,
	int* pivot_iter, int* pivot_pos);

void serial_back_substitution(double* matrix, double* vector,
	double* result, int size, int* pivot_pos);

int find_pivot_row(double* matrix, int size, int iter, int* pivot_iter);

void serial_column_elimination(double* matrix, double* vector, int pivot, int iter,
	int size, int* pivot_iter);

void serial_result_calculation(double* matrix, double* vector, double* result, int size);
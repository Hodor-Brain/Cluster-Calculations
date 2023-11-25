#include "serial_les.h"

void serial_gaussian_elimination(double* matrix, double* vector, int size,
	int* pivot_iter, int* pivot_pos) {
	int pivot_row;

	for (int iter = 0; iter < size; iter++) {

		pivot_row = find_pivot_row(matrix, size, iter, pivot_iter);
		pivot_pos[iter] = pivot_row;
		pivot_iter[pivot_row] = iter;
		serial_column_elimination(matrix, vector, pivot_row, iter, size, pivot_iter);
	}
}
void serial_back_substitution(double* matrix, double* vector,
	double* result, int size, int* pivot_pos) {

	int row_index, row; 
	for (int i = size - 1; i >= 0; i--) {
		row_index = pivot_pos[i];
		result[i] = vector[row_index] / matrix[size * row_index + i];
		for (int j = 0; j < i; j++) {
			row = pivot_pos[j];
			vector[j] -= matrix[row * size + i] * result[i];
			matrix[row * size + i] = 0;
		}
	}
}
int find_pivot_row(double* matrix, int size, int iter, int* pivot_iter) {
	int pivot_row = -1;
	double max = 0;
	for (int i = 0; i < size; i++) {
		if (pivot_iter[i] == -1 && fabs(matrix[i * size + iter]) > max) {
			pivot_row = i;
			max = fabs(matrix[i * size + iter]);
}
	}
	return pivot_row;
}
void serial_column_elimination(double* matrix, double* vector, int pivot, int iter,
	int size, int* pivot_iter) {
	double pivot_value, pivot_factor;
	pivot_value = matrix[pivot * size + iter];

	for (int i = 0; i < size; i++) {
		if (pivot_iter[i] == -1) {
			pivot_factor = matrix[i * size + iter] / pivot_value;
			for (int j = iter; j < size; j++) {
				matrix[i * size + j] -= pivot_factor * matrix[pivot * size + j];
			}
			vector[i] -= pivot_factor * vector[pivot];
		}
	}
}
void serial_result_calculation(double* matrix, double* vector, double* result, int size) {
	int* pivot_pos = new int[size];
	int* pivot_iter = new int[size];
	for (int i = 0; i < size; i++) {
		pivot_iter[i] = -1;
	}
	serial_gaussian_elimination(matrix, vector, size, pivot_iter, pivot_pos);

	serial_back_substitution(matrix, vector, result, size, pivot_pos);

	delete[] pivot_pos;
	delete[] pivot_iter;
}
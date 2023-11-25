#include "serial_mvm.h"

void multiply(int* matrix, int* vector, int* result, int size) {
	for (int i = 0; i < size; i++) {
		result[i] = 0;
		for (int j = 0; j < size; j++)
			result[i] += matrix[i * size + j] * vector[j];
	}
}
#include "serial_mm.h"

void multiply(int* matrix1, int* matrix2, int* result, int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			for (int k = 0; k < size; k++) {
				result[i * size + j] += matrix1[i * size + k] * matrix2[k * size + j];
			}
		}
	}

}
#include "serial_graph.h"

void serial_floyd(int* matrix, int size) {
	int t1, t2;
	for (int k = 0; k < size; k++) {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				if ((matrix[i * size + k] != -1) &&
					(matrix[k * size + j] != -1)) {
					t1 = matrix[i * size + j];
					t2 = matrix[i * size + k] + matrix[k * size + j];
					matrix[i * size + j] = min(t1, t2);
				}
			}
		}
	}
}
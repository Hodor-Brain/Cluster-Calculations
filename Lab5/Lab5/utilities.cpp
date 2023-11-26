#include "utilities.h"

void generate_matrix(int* matrix, int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (i != j) {
				if ((rand() % 100) < 50) matrix[i * size + j] = -1;
				else matrix[i * size + j] = rand() + 1;
			}
			else matrix[i * size + j] = 0;
		}
	}
}

std::string vtos(int* vector, int size) {
	std::stringstream out;
	for (int i = 0; i < size; i++) {
		out << vector[i] << " ";
	}
	out << "\n";

	return out.str();
}

void print_matrix(int* matrix, int size) {
	for (int i = 0; i < size; i++) {
		std::cout << vtos(matrix + i * size, size) << "\n";
	}
}

int min(int item1, int item2) {
	int result = (item1 < item2) ? item1 : item2;
	if ((item1 < 0) && (item2 >= 0)) result = item2;
	if ((item2 < 0) && (item1 >= 0)) result = item1;
	if ((item1 < 0) && (item2 < 0)) result = -1;

	return result;
}
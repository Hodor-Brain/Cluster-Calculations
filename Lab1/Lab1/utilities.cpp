#include "utilities.h"

void generate_vector(int* vector, int size) {
	for (int i = 0; i < size; i++) {
		vector[i] = rand() % 90 + 10;
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

void print_vector(int* vector, int size) {
	std::cout << vtos(vector, size);
}
void print_matrix(int* matrix, int rows, int cols) {
	if (cols == -1) cols = rows;

	std::string res = "";
	for (int i = 0; i < rows; i++) {
		res += vtos(matrix + i * cols, cols);
	}
	std::cout << res + "\n";
}
void generate_data(int* matrix, int* vector, int size) {
	generate_vector(matrix, size * size);
	generate_vector(vector, size);
}
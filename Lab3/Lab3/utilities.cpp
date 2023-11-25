#include "utilities.h"

void generate_vector(double* vector, int size) {
	for (int i = 0; i < size; i++) {
		vector[i] = rand() / double(1000);
	}
}
void generate_matrix(double* matrix, int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (j <= i) matrix[i * size + j] = rand() / double(1000);
			else matrix[i * size + j] = 0;
		}
	}
}

std::string vtos(double* vector, int size) {
	std::stringstream out;
	for (int i = 0; i < size; i++) {
		out << vector[i] << " ";
	}
	out << "\n";

	return out.str();
}

void print_vector(double* vector, int size) {
	std::cout << vtos(vector, size);
}
void print_matrix(double* matrix, int rows, int cols) {
	if (cols == -1) cols = rows;

	std::string res = "";
	for (int i = 0; i < rows; i++) {
		res += vtos(matrix + i * cols, cols);
	}
	std::cout << res + "\n";
}
void generate_data(double* matrix, double* vector, int size) {
	generate_matrix(matrix, size);
	generate_vector(vector, size);
}
#include "utilities.h"

void generate_vector(double* vector, int size) {
	for (int i = 0; i < size; i++) {
		vector[i] = rand() % 90 + 10;
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
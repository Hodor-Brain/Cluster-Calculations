#pragma once
#include <iostream>
#include <string>
#include <sstream>

//creates array with random items
void generate_vector(int* vector, int size);

//convert array to string
std::string vtos(int* vector, int size);

//prints array to console
void print_vector(int* vector, int size);

//print array to console as matrix. Cols = -1 for square matrix
void print_matrix(int* matrix, int rows, int cols = -1);

//generate random matrix and vector for multiplication
void generate_data(int* matrix, int* vector, int size);
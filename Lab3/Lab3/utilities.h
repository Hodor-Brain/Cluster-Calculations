#pragma once
#include <iostream>
#include <string>
#include <sstream>

//creates array with random items
void generate_vector(double* vector, int size);

//creates triangle matrix with random items
void generate_matrix(double* matrix, int size);

//convert array to string
std::string vtos(double* vector, int size);

//prints array to console
void print_vector(double* vector, int size);

//print array to console as matrix. Cols = -1 for square matrix
void print_matrix(double* matrix, int rows, int cols = -1);

//generate random matrix and vector for multiplication
void generate_data(double* matrix, double* vector, int size);
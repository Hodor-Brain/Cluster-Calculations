#pragma once
#include <iostream>
#include <string>
#include <sstream>

//creates array with random items
void generate_vector(double* vector, int size);

//convert array to string
std::string vtos(double* vector, int size);

//prints array to console
void print_vector(double* vector, int size);


#pragma once
#include "serial_mm.h"
#include "parallel_mm.h"
#include <chrono>

double serial_multiplication(int* matrix1, int* matrix2, int* result, int size);

double parallel_multiplication(int* matrix1, int* matrix2, int* result, int size);
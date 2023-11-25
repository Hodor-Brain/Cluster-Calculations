#pragma once
#include "serial_mvm.h"
#include "parallel_mvm.h"
#include <chrono>

double serial_multiplication(int* matrix, int* vector, int* result, int size);

double parallel_multiplication(int* matrix, int* vector, int* result, int size);
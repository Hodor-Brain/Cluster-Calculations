#pragma once
#include "serial_sort.h"
#include "parallel_sort.h"

#include <chrono>

double serial_solving(double* arr, int size);

double parallel_solving(double* arr, int size);
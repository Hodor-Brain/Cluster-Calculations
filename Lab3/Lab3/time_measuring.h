#pragma once
#include "serial_les.h"
#include "parallel_les.h"
#include <chrono>

double serial_solving(double* matrix, double* vector, double* result, int size);

double parallel_solving(double* matrix, double* vector, double* result, int size);
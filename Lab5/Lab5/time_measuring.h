#pragma once
#include "serial_graph.h"
#include "parallel_graph.h"

#include <chrono>

double serial_solving(int* matrix, int size);

double parallel_solving(int* arr, int size);
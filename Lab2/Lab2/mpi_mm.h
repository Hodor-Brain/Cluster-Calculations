#pragma once

#include "mpi.h"
#include "utilities.h"
#include "serial_mm.h"

//creates matrices
void init_data(int*& matrix1_block, int*& block1, int*& block2, int*& block_result,int block_size);

//delete matrices
void free_data(int* matrix1_block, int* block1, int* block2, int* block_result);

void create_grid_communicators(int grid_size, MPI_Comm* grid_comm, MPI_Comm* row_comm,
	MPI_Comm* col_comm, int grid_coords[2], int rank);


void checkerboard_matrix_scatter(int* matrix, int* matrix_block, int size,
	int block_size, int grid_coords[2], MPI_Comm* row_comm, MPI_Comm* col_comm);

void data_distribution(int* matrix1, int* matrix2,
	int* matrix1_block, int* block2, int size, int block_size,
	int grid_coords[2], MPI_Comm* row_comm, MPI_Comm* col_comm);

void block1_communication(int iter, int* block1, int* matrix1_block,
	int block_size, int grid_size, int grid_coords[2], MPI_Comm* row_comm);

void block2_communication(int* block2, int block_size, int grid_size,
	int grid_coords[2], MPI_Comm* col_comm);

void parallel_result_calculation(int* block1, int* matrix1_block,
	int* block2, int* block_result, int block_size, int grid_size, int grid_coords[2],
	MPI_Comm* row_comm, MPI_Comm* col_comm);

void result_collection(int* result, int* block_result, int size,
	int block_size, int grid_coords[2], MPI_Comm* row_comm, MPI_Comm* col_comm);
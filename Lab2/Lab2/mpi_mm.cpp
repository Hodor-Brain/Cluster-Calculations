#include "mpi_mm.h"


void init_data(int*& matrix1_block, int*& block1, int*& block2, int*& block_result, int block_size) {
	int size = block_size * block_size;

	matrix1_block = new int[size];
	block1 = new int[size];
	block2 = new int[size];
	block_result = new int[size] {};
}

void free_data(int* matrix1_block, int* block1, int* block2, int* block_result) {
	delete[] matrix1_block;
	delete[] block1;
	delete[] block2;
	delete[] block_result;
}

void create_grid_communicators(int grid_size, MPI_Comm* grid_comm, MPI_Comm* row_comm,
	MPI_Comm* col_comm, int grid_coords[2], int rank) {
	int dim_size[2];
	int periodic[2];
	int subdims[2];

	dim_size[0] = grid_size;
	dim_size[1] = grid_size; 
	periodic[0] = 0;
	periodic[1] = 0;

	MPI_Cart_create(MPI_COMM_WORLD, 2, dim_size, periodic, 1, grid_comm);
	MPI_Cart_coords(*grid_comm, rank, 2, grid_coords);


	subdims[0] = 0;
	subdims[1] = 1;
	MPI_Cart_sub(*grid_comm, subdims, row_comm);

	subdims[0] = 1;
	subdims[1] = 0;
	MPI_Cart_sub(*grid_comm, subdims, col_comm);
}

void checkerboard_matrix_scatter(int* matrix, int* matrix_block, int size, 
	int block_size, int grid_coords[2], MPI_Comm* row_comm, MPI_Comm* col_comm) {

	int* matrix_row = new int[block_size * size];

	if (grid_coords[1] == 0) {
		MPI_Scatter(matrix, block_size * size, MPI_INT, matrix_row,
			block_size * size, MPI_INT, 0, *col_comm);
	}
	for (int i = 0; i < block_size; i++) {
		MPI_Scatter(&matrix_row[i * size], block_size, MPI_INT,
			&(matrix_block[i * block_size]), block_size, MPI_INT, 0, *row_comm);
	}
	delete[] matrix_row;
}

void data_distribution(int* matrix1, int* matrix2,
	int* matrix1_block, int* block2, int size, int block_size,
	int grid_coords[2], MPI_Comm* row_comm, MPI_Comm* col_comm) {

	checkerboard_matrix_scatter(matrix1, matrix1_block, size, block_size, grid_coords, row_comm, col_comm);
	checkerboard_matrix_scatter(matrix2, block2, size, block_size, grid_coords, row_comm, col_comm);
}

void block1_communication(int iter, int* block1, int* matrix1_block,
	int block_size, int grid_size, int grid_coords[2], MPI_Comm* row_comm) {

	int pivot = (grid_coords[0] + iter) % grid_size;

	if (grid_coords[1] == pivot) {
		for (int i = 0; i < block_size * block_size; i++)
			block1[i] = matrix1_block[i];
	}

	MPI_Bcast(block1, block_size * block_size, MPI_INT, pivot, *row_comm);
}

void block2_communication(int* block2, int block_size, int grid_size,
	int grid_coords[2], MPI_Comm* col_comm) {

	MPI_Status status;
	int next_proc = grid_coords[0] + 1;
	if (grid_coords[0] == grid_size - 1) next_proc = 0;

	int prev_proc = grid_coords[0] - 1;
	if (grid_coords[0] == 0) prev_proc = grid_size - 1;

	MPI_Sendrecv_replace(block2, block_size * block_size, MPI_INT,
		next_proc, 0, prev_proc, 0, *col_comm, &status);
}

void parallel_result_calculation(int* block1, int* matrix1_block,
	int* block2, int* block_result, int block_size, int grid_size, int grid_coords[2], 
	MPI_Comm* row_comm, MPI_Comm* col_comm) {

	for (int i = 0; i < grid_size; i++) {

		block1_communication(i, block1, matrix1_block, block_size, grid_size, grid_coords, row_comm);

		multiply(block1, block2, block_result, block_size);

		block2_communication(block2, block_size, grid_size, grid_coords, col_comm);
	}
}

void result_collection(int* result, int* block_result, int size,
	int block_size, int grid_coords[2], MPI_Comm* row_comm, MPI_Comm* col_comm) {

	int* result_row = new int[size * block_size];
	for (int i = 0; i < block_size; i++) {
		MPI_Gather(&block_result[i * block_size], block_size, MPI_INT,
			&result_row[i * size], block_size, MPI_INT, 0, *row_comm);
	}
	if (grid_coords[1] == 0) {
		MPI_Gather(result_row, block_size * size, MPI_INT, result,
			block_size * size, MPI_INT, 0, *col_comm);
	}
	delete[] result_row;
}
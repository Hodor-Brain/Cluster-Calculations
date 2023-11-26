#include "parallel_graph.h"

void process_initialization(int*& process_rows, int& size, 
	int& row_number, int rank, int number) {

	MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);

	int rest_rows = size;
	for (int i = 0; i < rank; i++) rest_rows = rest_rows - rest_rows / (number - i);
	row_number = rest_rows / (number - rank);

	process_rows = new int[size * row_number];
}
void process_termination(int* process_rows) {
	delete[]process_rows;
}

void data_distribution(int* matrix, int* process_rows, int size, int row_number, int rank, int number) {
	int* send_number;
	int* send_index;
	int rest_rows = size;

	send_index = new int[number];
	send_number = new int[number];

	row_number = size / number;
	send_number[0] = row_number * size;
	send_index[0] = 0;

	for (int i = 1; i < number; i++) {
		rest_rows -= row_number;
		row_number = rest_rows / (number - i);
		send_number[i] = row_number * size;
		send_index[i] = send_index[i - 1] + send_number[i - 1];
	}

	MPI_Scatterv(matrix, send_number, send_index, MPI_INT,
		process_rows, send_number[rank], MPI_INT, 0, MPI_COMM_WORLD);

	delete[]send_number;
	delete[]send_index;
}
void result_collection(int* matrix, int* process_rows, int size, int row_number, int rank, int number) {
	int* pReceiveNum;
	int* pReceiveInd;
	int RestRows = size; 

	pReceiveNum = new int[number];
	pReceiveInd = new int[number];

	row_number = size / number;
	pReceiveInd[0] = 0;
	pReceiveNum[0] = row_number * size;
	for (int i = 1; i < number; i++) {
		RestRows -= row_number;
		row_number = RestRows / (number - i);
		pReceiveNum[i] = row_number * size;
		pReceiveInd[i] = pReceiveInd[i - 1] + pReceiveNum[i - 1];
	}

	MPI_Gatherv(process_rows, pReceiveNum[rank], MPI_INT,
		matrix, pReceiveNum, pReceiveInd, MPI_INT, 0, MPI_COMM_WORLD);

	delete[]pReceiveNum;
	delete[]pReceiveInd;
}
void row_distribution(int* process_rows, int size, int row_number, int k, int* row, int rabk, int number) {
	int process_row_rank;
	int process_row_number;
	int rest_rows = size;
	int index = 0;
	int num = size / number;

	for (process_row_rank = 1; process_row_rank < number + 1; process_row_rank++) {
		if (k < index + num) break;
		rest_rows -= num;
		index += num;
		num = rest_rows / (number - process_row_rank);
	}
	process_row_rank = process_row_rank - 1;
	process_row_number = k - index;

	if (process_row_rank == rabk)
		std::copy(&process_rows[process_row_number * size], &process_rows[(process_row_number + 1) * size], row);

	MPI_Bcast(row, size, MPI_INT, process_row_rank, MPI_COMM_WORLD);
}

void parallel_floyd_algo(int* process_rows, int size, int row_number, int rank, int number) {
	int* row = new int[size];
	int t1, t2;

	for (int k = 0; k < size; k++) {

		row_distribution(process_rows, size, row_number, k, row, rank, number);

		for (int i = 0; i < row_number; i++)
			for (int j = 0; j < size; j++)
				if ((process_rows[i * size + k] != -1) &&
					(row[j] != -1)) {
					t1 = process_rows[i * size + j];
					t2 = process_rows[i * size + k] + row[j];
					process_rows[i * size + j] = min(t1, t2);
				}
	}

	delete[] row;
}

void parallel_floyd(int* matrix, int size) {

	int number, rank;

	MPI_Comm_size(MPI_COMM_WORLD, &number);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int* process_rows, row_number;
	process_initialization(process_rows, size, row_number, rank, number);

	data_distribution(matrix, process_rows, size, row_number, rank, number);

	parallel_floyd_algo(process_rows, size, row_number, rank, number);

	result_collection(matrix, process_rows, size, row_number, rank, number);

	process_termination(process_rows);
}
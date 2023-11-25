#include "mpi_les.h"

void init_data(double*& proc_rows, double*& proc_vector, double*& proce_result,
	int*& proc_number, int*& proc_ind, int size, int& row_num, int rank, int number) {

	int rest_rows = size;
	for (int i = 0; i < rank; i++)
		rest_rows = rest_rows - rest_rows / (number - i);

	row_num = rest_rows / (number - rank);
	proc_rows = new double[row_num * size];
	proc_vector = new double[row_num];
	proce_result = new double[row_num];

	proc_ind = new int[number];
	proc_number = new int[number];
}

void free_data(double* proc_rows, double* proc_vector, double* proce_result,
	int* proc_number, int* proc_ind) {

	delete[] proc_rows;
	delete[] proc_vector;
	delete[] proce_result;
	delete[] proc_number;
	delete[] proc_ind;
}

void data_distribution(double* matrix, double* proc_rows, double* vector,
	double* proc_vector, int size, int row_num, int number, int rank, int* proc_ind, int* proc_num) {
	int* send_num;
	int* send_ind;

	int rest_rows = size;
	send_ind = new int[number];
	send_num = new int[number];

	row_num = (size / number);
	send_num[0] = row_num * size;
	send_ind[0] = 0;

	for (int i = 1; i < number; i++) {
		rest_rows -= row_num;
		row_num = rest_rows / (number - i);
		send_num[i] = row_num * size;
		send_ind[i] = send_ind[i - 1] + send_num[i - 1];
	}

	MPI_Scatterv(matrix, send_num, send_ind, MPI_DOUBLE, proc_rows,
		send_num[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);

	rest_rows = size;
	proc_ind[0] = 0;
	proc_num[0] = size / number;

	for (int i = 1; i < number; i++) {
		rest_rows -= proc_num[i - 1];
		proc_num[i] = rest_rows / (number - i);
		proc_ind[i] = proc_ind[i - 1] + proc_num[i - 1];
	}
	MPI_Scatterv(vector, proc_num, proc_ind, MPI_DOUBLE, proc_vector,
		proc_num[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);

	delete[] send_num;
	delete[] send_ind;
}
void parallel_eliminate_columns(double* proc_rows, double* proc_vector,
	double* pivot_row, int size, int row_num, int iter, int* proc_pivot_iter) {

	double pivot_factor;
	for (int i = 0; i < row_num; i++) {
		if (proc_pivot_iter[i] == -1) {
			pivot_factor = proc_rows[i * size + iter] / pivot_row[iter];
			for (int j = iter; j < size; j++) {
				proc_rows[i * size + j] -= pivot_factor * pivot_row[j];
			}
			proc_vector[i] -= pivot_factor * pivot_row[size];
		}
	}
}
void parallel_gaussian_elimination(double* proc_rows, double* proc_vector,
	int size, int row_num, int* pivot_iter, int* proc_ind, int* parallel_pivot_pos, int rank) {

	double max{};
	int pivot_pos{};
	struct { double max=0; int rank=0; } proc_pivot, pivot;
	
	double* pivot_row = new double[size + 1] {};

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < row_num; j++) {
			if ((pivot_iter[j] == -1) &&
				(max < fabs(proc_rows[j * size + i]))) {
				max = fabs(proc_rows[j * size + i]);
				pivot_pos = j;
			}
		}
		proc_pivot.max = max;
		proc_pivot.rank = rank;

		MPI_Allreduce(&proc_pivot, &pivot, 1, MPI_DOUBLE_INT, MPI_MAXLOC, MPI_COMM_WORLD);
		if (rank == pivot.rank) {
			pivot_iter[pivot_pos] = i;
			parallel_pivot_pos[i] = proc_ind[rank] + pivot_pos;
		}
		MPI_Bcast(&parallel_pivot_pos[i], 1, MPI_INT, pivot.rank, MPI_COMM_WORLD);

		if (rank == pivot.rank) {
			for (int j = 0; j < size; j++) {
				pivot_row[j] = proc_rows[pivot_pos * size + j];
			}
			pivot_row[size] = proc_vector[pivot_pos];
		}
		MPI_Bcast(pivot_row, size + 1, MPI_DOUBLE, pivot.rank, MPI_COMM_WORLD);

		parallel_eliminate_columns(proc_rows, proc_vector, pivot_row, size, row_num, i, pivot_iter);
	}

	delete[] pivot_row;
}

void find_back_pivot_row(int row_index, int& iter_proc_rank, int& iter_pivot_pos, int number, 
	int* proc_ind) {

	for (int i = 0; i < number - 1; i++) {
		if ((proc_ind[i] <= row_index) && (row_index < proc_ind[i + 1]))
			iter_proc_rank = i;
	}
	if (row_index >= proc_ind[number - 1])
		iter_proc_rank = number - 1;
	iter_pivot_pos = row_index - proc_ind[iter_proc_rank];
}

void parallel_back_substitution(double* proc_rows, double* proc_vector, double* proc_result, 
	int size, int row_number, int* parallel_pivot_pos, int rank, int* proc_pivot_iter, 
	int* proc_ind, int number) {
	
	int iter_proc_rank{};
	int iter_pivot_pos{};
	double iter_result{};
	double val{};

	for (int i = size - 1; i >= 0; i--) {
		find_back_pivot_row(parallel_pivot_pos[i], iter_proc_rank, iter_pivot_pos, number, proc_ind);
		if (rank == iter_proc_rank) {
			iter_result = proc_vector[iter_pivot_pos] / proc_rows[iter_pivot_pos * size + i];
			proc_result[iter_pivot_pos] = iter_result;
		}

		MPI_Bcast(&iter_result, 1, MPI_DOUBLE, iter_proc_rank, MPI_COMM_WORLD);
		for (int j = 0; j < row_number; j++)
			if (proc_pivot_iter[j] < i){
				val = proc_rows[j * size + i] *
				iter_result; proc_vector[j] = proc_vector[j] - val;
			}
	}
}
void result_collection(double* proc_result, double* result, int* proc_number, int rank, 
	int* proc_ind) {

	MPI_Gatherv(proc_result, proc_number[rank], MPI_DOUBLE, result, proc_number, 
		proc_ind, MPI_DOUBLE, 0, MPI_COMM_WORLD);
}
void parallel_result_calculation(double* proc_rows, double* proc_vector,
	double* proc_result, int size, int row_num, int rank, int* proc_ind, int* proc_num, int number) {

	int* parallel_pivot_pos = new int[size];
	int* proc_pivot_iter = new int[row_num];

	for (int i = 0; i < row_num; i++)
		proc_pivot_iter[i] = -1;


	parallel_gaussian_elimination(proc_rows, proc_vector, size, row_num, proc_pivot_iter, proc_ind,
		parallel_pivot_pos, rank);

	parallel_back_substitution(proc_rows, proc_vector, proc_result, size,
		row_num, parallel_pivot_pos, rank, proc_pivot_iter, proc_ind, number);

	delete[] parallel_pivot_pos;
	delete[] proc_pivot_iter;
}
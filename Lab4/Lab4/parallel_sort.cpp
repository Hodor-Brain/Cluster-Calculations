#include "parallel_sort.h"

void process_initialization(int& size, double*& process_arr, int& block_size, 
	int rank, int number) {

	MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);

	int rest_data = size;

	for (int i = 0; i < rank; i++)
		rest_data -= rest_data / (number - i);
	block_size = rest_data / (number - rank);
	process_arr = new double[block_size];
}
void process_termination(double* process_data) {
	delete[] process_data;
}
void data_distribution(double* arr, int size, double* process_arr, int block_size, 
	int number, int rank) {

	int* send_ind = new int[number];
	int* send_num = new int[number];
	int rest_data = size;
	int current_size = size / number;
	send_num[0] = current_size;
	send_ind[0] = 0;

	for (int i = 1; i < number; i++) {
		rest_data -= current_size;
		current_size = rest_data / (number - i);
		send_num[i] = current_size;
		send_ind[i] = send_ind[i - 1] + send_num[i - 1];
	}

	MPI_Scatterv(arr, send_num, send_ind, MPI_DOUBLE, process_arr,
		send_num[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);

	delete[] send_num;
	delete[] send_ind;
}
void data_collection(double* arr, int size, double* process_arr, int block_size, int number) {

	int* receive_num = new int[number];
	int* receive_ind = new int[number];
	int rest_data = size;
	receive_ind[0] = 0;
	receive_num[0] = size / number;

	for (int i = 1; i < number; i++) {
		rest_data -= receive_num[i - 1];
		receive_num[i] = rest_data / (number - i);
		receive_ind[i] = receive_ind[i - 1] + receive_num[i - 1];
	}
	MPI_Gatherv(process_arr, block_size, MPI_DOUBLE, arr,
		receive_num, receive_ind, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	delete[] receive_num;
	delete[] receive_ind;
}

void exchange_data(double* process_arr, int block_size, int dual_rank,
	double* dual_arr, int dual_block_size) {

	MPI_Status status;
	MPI_Sendrecv(process_arr, block_size, MPI_DOUBLE, dual_rank, 0,
		dual_arr, dual_block_size, MPI_DOUBLE, dual_rank, 0,
		MPI_COMM_WORLD, &status);
}
template <typename InputIt1, typename InputIt2, typename OutputIt>
void merge(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, OutputIt result) {
	while (first1 != last1 && first2 != last2) {
		if (*first1 < *first2) {
			*result = *first1;
			++first1;
		}
		else {
			*result = *first2;
			++first2;
		}
		++result;
	}

	// Copy the remaining elements, if any, from the first range
	while (first1 != last1) {
		*result = *first1;
		++first1;
		++result;
	}

	// Copy the remaining elements, if any, from the second range
	while (first2 != last2) {
		*result = *first2;
		++first2;
		++result;
	}
}

void parallel_bubble_sorting(double* process_arr, int block_size, int number, int rank) {

	serial_bubble(process_arr, block_size);
	int offset;
	
	SplitMode mode;
	for (int i = 0; i < number; i++) {
		if ((i % 2) == 1) {
			if ((rank % 2) == 1) {
				offset = 1;
				mode = KeepFirstHalf;
			}
			else {
				offset = -1;
				mode = KeepSecondHalf;
			}
		}
		else {
			if ((rank % 2) == 1) {
				offset = -1;
				mode = KeepSecondHalf;
			}
			else {
				offset = 1;
				mode = KeepFirstHalf;
			}
		}

		if ((rank == number - 1) && (offset == 1)) continue;
		if ((rank == 0) && (offset == -1)) continue;
		
		MPI_Status status;
		int dual_block_size;
		MPI_Sendrecv(&block_size, 1, MPI_INT, rank + offset, 0,
			&dual_block_size, 1, MPI_INT, rank + offset, 0,
			MPI_COMM_WORLD, &status);

		double* dual_arr = new double[dual_block_size] {};
		double* merged_arr = new double[block_size + dual_block_size] {};

		exchange_data(process_arr, block_size, rank + offset, dual_arr, dual_block_size);
	

		merge(process_arr, process_arr + block_size, dual_arr, dual_arr + dual_block_size, merged_arr);
		
		if (mode == KeepFirstHalf)
			std::copy(merged_arr, merged_arr + block_size, process_arr);
		else {
			std::copy(merged_arr + block_size, merged_arr + block_size + dual_block_size, process_arr);
		}
		delete[] dual_arr;
		delete[] merged_arr;
	}
}
void parallel_bubble(double* arr, int size)
{
	int number, rank;

	MPI_Comm_size(MPI_COMM_WORLD, &number);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	double* process_arr;
	int block_size;

	process_initialization(size, process_arr, block_size, rank, number);
	data_distribution(arr, size, process_arr, block_size, number, rank);

	parallel_bubble_sorting(process_arr, block_size, number, rank);

	data_collection(arr, size, process_arr, block_size, number);

	process_termination(process_arr);
}

/*
 * Christian Reotutar
 * Michael Miller
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>
#include <ostream>
#include <iostream>
#include <fstream>
#include <string>

#include "simple_ispc.h"

extern void simple_ispc(unsigned int size, unsigned int* ar1, unsigned int* ar2, unsigned int* sol);

// calculate elapsed time
int timeval_subtract (struct timeval * result, struct timeval * x, struct timeval * y)
{
	/* Perform the carry for the later subtraction by updating y. */
	if (x->tv_usec < y->tv_usec) {
		int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
		y->tv_usec -= 1000000 * nsec;
		y->tv_sec += nsec;
	}
	if (x->tv_usec - y->tv_usec > 1000000) {
		int nsec = (x->tv_usec - y->tv_usec) / 1000000;
		y->tv_usec += 1000000 * nsec;
		y->tv_sec -= nsec;
	}

	/* Compute the time remaining to wait.
	 tv_usec is certainly positive. */
	result->tv_sec = x->tv_sec - y->tv_sec;
	result->tv_usec = x->tv_usec - y->tv_usec;

	/* Return 1 if result is negative. */
	return x->tv_sec < y->tv_sec;
}

int main(int argc, char *argv[]) {

	int size = 1000000;
	int iters;

	if (argc != 2) {
		std::cout << "usage: ./simple <dataset multiple>" << std::endl;
		return 1;
	} else {
		iters = atoi(argv[1]);
	}

	// allocate mem for input arrays
	unsigned int **nums = (unsigned int**) malloc(2 * sizeof(unsigned int*));

	// load input from file into array
	std::fstream input[2];
	for (int i = 0; i < 2; i++) {
		std::string filename = "io/rand0" + std::to_string(i + 1) + ".txt";
		input[i].open(filename, std::ios::in);

		nums[i] = (unsigned int*) malloc(size * sizeof(unsigned int));
		for (int j = 0; j < size; j++) {
			input[i] >> nums[i][j];
		}

		input[i].close();
	}

	// allocate mem for solution array
	unsigned int **solu = (unsigned int**) malloc(iters * sizeof(unsigned int*));
	for (int i = 0; i < iters; i++) {
		solu[i] = (unsigned int*) malloc(size * sizeof(unsigned int));
	}

	// determine number of threads to create
	int num_procs = omp_get_num_procs();

	// start time counter
	struct timeval ta, tb, tc, td, te, tf, tresult;

	gettimeofday (&ta, NULL);

	//unvectorized, unparallelized
	for (int i = 0; i < iters; i++) {
		for (int j = 0; j < size; j++) {
			solu[i][j] = nums[0][j] * nums[1][j];
		}
	}

	gettimeofday (&tb, NULL);

	//unvectorized, parallelized
	#pragma omp parallel for num_threads(num_procs)
	for (int i = 0; i < iters; i++) {
		for (int j = 0; j < size; j++) {
			solu[i][j] = nums[0][j] * nums[1][j];
		}
	}

	gettimeofday (&tc, NULL);

	// vectorized(ispc), unparallelized
	for (int i = 0; i < iters; i++) {
			ispc::simple_ispc(size, nums[0], nums[1], solu[i]);
	}

	gettimeofday (&td, NULL);

	// vectorized(ispc), parallelized
	#pragma omp parallel for num_threads(num_procs)
	for (int i = 0; i < iters; i++) {
			ispc::simple_ispc(size, nums[0], nums[1], solu[i]);
	}

	gettimeofday (&te, NULL);

	// nothing

	gettimeofday (&tf, NULL);

	// output
	printf("calcs:%d\n",size*iters);
	printf("num_procs:%d (maybe hyperthreading cores)\n", num_procs);

	double time_temp[5];

	timeval_subtract(&tresult, &tb, &ta);
	printf("unvectorized unparallelized\tsec:%lu;micro:%lu;\n", tresult.tv_sec, tresult.tv_usec);
	time_temp[0] = (tresult.tv_sec * 1000000) + tresult.tv_usec;

	timeval_subtract(&tresult, &tc, &tb);
	printf("unvectorized parallelized\tsec:%lu;micro:%lu;\n", tresult.tv_sec, tresult.tv_usec);
	time_temp[1] = (tresult.tv_sec * 1000000) + tresult.tv_usec;

	timeval_subtract(&tresult, &td, &tc);
	printf("vectorized(ispc) unparallelized\tsec:%lu;micro:%lu;\n", tresult.tv_sec, tresult.tv_usec);
	time_temp[2] = (tresult.tv_sec * 1000000) + tresult.tv_usec;

	timeval_subtract(&tresult, &te, &td);
	printf("vectorized(ispc) parallelized\tsec:%lu;micro:%lu;\n\n", tresult.tv_sec, tresult.tv_usec);
	time_temp[3] = (tresult.tv_sec * 1000000) + tresult.tv_usec;

	printf("speedup(w/vect):%f\n", time_temp[0] / time_temp[2]);
	printf("speedup(w/parr):%f\n", time_temp[0] / time_temp[1]);
	printf("speedup(w/vect+parr):%f\n", time_temp[0] / time_temp[3]);


	// exit successfully
	return 0;
}
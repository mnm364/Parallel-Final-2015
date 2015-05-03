#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <ostream>
#include <iostream>
#include <fstream>
#include <string>

#include "simple_ispc.h"

using namespace ispc;

//extern void simple_ispc(int size, int*, int*, int*);

// measure time
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

	int size;

	if (argc != 2) {
		std::cout << "ERROR in cmd args" << std::endl;
		return 1;
	} else {
		size = atoi(argv[1]);
	}
	
	// allocate mem for input arrays
	int **nums = (int**) malloc(2 * sizeof(int*));

	std::fstream input[2];
	for (int i = 0; i < 2; i++) {
		std::string filename = "io/rand0" + std::to_string(i + 1) + ".txt";
		input[i].open(filename, std::ios::in);

		nums[i] = (int*) malloc(size * sizeof(unsigned int));
		for (int j = 0; j < size; j++) {
			input[i] >> nums[i][j];
		}
	}

	// allocate mem for solution array
	int *solu = (int*) malloc(size * sizeof(unsigned int));

	// start time counter
	struct timeval ta, tb, tresult;
	gettimeofday (&ta, NULL);

	// transformation step
	simple_ispc(size, nums[0], nums[1], solu);
	// for (int i = 0; i < size; i++) {
	// 	for (int j = 0; j < 250; j++) {
	// 		solu[i] = nums[0][i] * nums[1][i];
	// 	}
	// }

	// end time counter
	gettimeofday (&tb, NULL);
	timeval_subtract ( &tresult, &tb, &ta );

	//time output
	printf ("sec:%lu;\nmicro:%lu;\nlen:%d;\n", tresult.tv_sec, tresult.tv_usec, size);

	std::fstream output;
	output.open("io/out_serial.txt", std::ios::out | std::ios::trunc);
	for (int i = 0; i < size; i++) {
		output << solu[i] << std::endl;
	}

	return 0;
}
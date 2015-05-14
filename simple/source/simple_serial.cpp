/*
 * Michael Miller
 * Christian Reotutar
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <ostream>
#include <iostream>
#include <fstream>
#include <string>

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

	int size = 1000000;
	int iters;

	if (argc != 2) {
		std::cout << "usage: ./serial <dataset multiplier>" << std::endl;
		return 1;
	} else {
		iters = atoi(argv[1]);
	}
	
	// allocate mem for input arrays
	unsigned int **nums = (unsigned int**) malloc(2 * sizeof(unsigned int*));

	std::fstream input[2];
	for (int i = 0; i < 2; i++) {
		std::string filename = "io/rand0" + std::to_string(i + 1) + ".txt";
		input[i].open(filename, std::ios::in);

		nums[i] = (unsigned int*) malloc(size * sizeof(unsigned int));
		for (int j = 0; j < size; j++) {
			input[i] >> nums[i][j];
		}
	}

	// allocate mem for solution array
	unsigned int **solu = (unsigned int**) malloc(iters * sizeof(unsigned int*));
	for (int i = 0; i < iters; i++) {
		solu[i] = (unsigned int*) malloc(size * sizeof(unsigned int));
	}

	// start time counter
	struct timeval ta, tb, tresult;
	gettimeofday (&ta, NULL);

	// transformation step
	for (int i = 0; i < iters; i++) {
		for (int j = 0; j < size; j++) {
			solu[i][j] = nums[0][j] * nums[1][j];
		}
	}

	// end time counter
	gettimeofday (&tb, NULL);
	timeval_subtract ( &tresult, &tb, &ta );

	//time output
	printf ("serial(-O3)\tsec:%lu;micro:%lu;\n", tresult.tv_sec, tresult.tv_usec);

	return 0;
}
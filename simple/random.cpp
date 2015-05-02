#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <string>

int main(int argc, char *argv[]) {

	int seed = -1;
	int max_size = 0;
	// std::string filename = "";

	if (argc != 4) {
		std::cout << "usage: ./random <seed(-1)> <size>" << std::endl;
		return 1;
	} else {
		seed = atoi(argv[2]);
		max_size = atoi(argv[3]);
		// filename(argv[1]);
	}

	// be able to pass in a seed	
	if (argv[1] < 0) {
		srand(time(NULL));
	} else {
		srand(seed);
	}

	// std::string s;
	// std::string filename = "io/" + s(argv[1]);
	std::ofstream randFile;
	randFile.open(argv[1], std::ios::out | std::ios::trunc);

	for (int i = 0; i < max_size; i++) {
		randFile << rand()%65535 << "\n"; //
	}

	randFile.close();

	return 0;
}
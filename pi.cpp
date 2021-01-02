#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#pragma comment(lib, "pthreadVC2.lib")

using namespace std;
void* calculatePI(void*);

int main(int argc, char **argv)
{
	double pi_estimate = 0, timeSpend;
	int numOfPerThreadIter;
	unsigned long long  number_of_cpu, number_of_tosses, toss;
	// Child thread's return value
	void* ret;
	// begin time
	clock_t begin = clock();

	if (argc < 2) {
		exit(-1);
	}
	number_of_cpu = atoi(argv[1]);
	number_of_tosses = atoi(argv[2]);
	if ((number_of_cpu < 1) || (number_of_tosses < 0)) {
		exit(-1);
	}

	//Create "number_of_cpu"'s thread
	pthread_t* t = new pthread_t[number_of_cpu];					
	numOfPerThreadIter = (int)(number_of_tosses / number_of_cpu);
	for (int i = 0; i < number_of_cpu; i++) {
		pthread_create(&t[i], NULL, calculatePI, (void*)numOfPerThreadIter);
	}

	// Wait for another threads
	for (int i = 0; i < number_of_cpu; i++) {
		pthread_join(t[i], &ret);
		pi_estimate += (long long)(ret);
	}
	pi_estimate = 4 * pi_estimate / ((double)number_of_tosses);

	clock_t end = clock();
	timeSpend = (double)(end - begin) / CLOCKS_PER_SEC;

	printf("%f\n", pi_estimate);
	pthread_exit(NULL);
}

void* calculatePI(void* num_iter) {
	long long number_in_circle = 0;
	double distance_squared, x, y;

	// Make sure the random value is different in different step
	srand(time(NULL));												
	struct timeval tx;

	gettimeofday(&tx, NULL);
	unsigned int seed = tx.tv_usec;

	for (int toss = 0; toss < (long)num_iter; toss++) {
		// x = random double between -1 and 1;
		x = ((float)(rand_r(&seed)) / (float)(RAND_MAX)) * 2 - 1;
		// y = random double between -1 and 1;
		y = ((float)(rand_r(&seed)) / (float)(RAND_MAX)) * 2 - 1;

		distance_squared = x * x + y * y;
		// In the circle
		if (distance_squared <= 1)
			number_in_circle++;
	}

	pthread_exit((void*)number_in_circle);
	return NULL;
}
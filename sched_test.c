#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* worker(void *data) {
	unsigned int a = 1;
	while (a < INT_MAX/4) {
//	while (a < 10) {
		a++;
	}
	return NULL;
}

int main(int argc, char *argv[])
{
	int threads;
	if (argc != 2) {
		printf("Pass Number of threads %d", argc);
		exit(-1);
	}
	threads = atoi(argv[1]);
	printf("Spawning %d threads\n", threads);
	pthread_t th[threads];
	for (int i = 0; i < threads; i++) {
		pthread_create(&th[i], NULL, worker, NULL);
	}
	printf("Exiting after joining all threads\n");
	for (int i = 0; i < threads; i++) {
		pthread_join(th[i], NULL);
	}
	return 0;
}

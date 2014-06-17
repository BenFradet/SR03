#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>
#include <mpi.h>

#define MAX 100000000

float Random (float coef) {
  return ((float)rand() / RAND_MAX) * coef;
}

int main (int argc,char *argv[]) {
	int rang, size, int_error_handling;
	int val[3];
	float pi = 0.0;

	MPI_Init(&argc, &argv);
	int_error_handling = MPI_Comm_size(MPI_COMM_WORLD, &size);
	int_error_handling = MPI_Comm_rank(MPI_COMM_WORLD, &rang);

    float x, y, elapsed;
    long i, count = 0;
    long t1, t2;

    t1 = clock();
    srand(time (NULL));

    for (i = 0; i < MAX; i++) {
        x = Random(1.0);
        y = Random(1.0);
        if (x * x + y * y < 1) {
            count++;
        }
    }
    t2 = clock();
    elapsed = (float)(t2 - t1) / CLOCKS_PER_SEC;
 
	MPI_Gather(&count, 1, MPI_INT, val, 1, MPI_INT, 1, MPI_COMM_WORLD);

	if(rang == 1) {
		int j;
		for(j = 0; j < 3; j++) {
			pi += val[j];
            printf("\nde %d recu %d \n", j, val[j]);
		}
		pi /= 3 * MAX;
		pi *= 4;
		printf("\npi = %f\n", pi);
	}

	MPI_Finalize();
    return 0;
} 

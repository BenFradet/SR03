#include <stdio.h>
#include <mpi.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int rank, size, int_error_handling;

	char name[MPI_MAX_PROCESSOR_NAME];
	int resultlen;

    // start MPI
    MPI_Init(&argc,&argv);
  
    int_error_handling = MPI_Comm_size(MPI_COMM_WORLD, &size); 

	int_error_handling = MPI_Get_processor_name(name, &resultlen);
	//sleep(10);

    int_error_handling =  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  
    printf("\nhello world from process rank %d of %d, processor name = %s",
            rank, size, name);
    
    int_error_handling =  MPI_Finalize();
    return 0;
}

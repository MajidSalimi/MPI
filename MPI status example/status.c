#include <stdio.h>     
#include <time.h>
#include <string.h> 
#include <mpi.h>
      // -- Use of Parallel MPI_Status status in MPI_Recv   96/06/25
      // -- To compile mpicc status.c, TO RUN:	mpirun -n xx a.out

 int main(int argc, char *argv[]){
	//--Declare variables
	int i, myid, procs, err;
	char msg[50]; 

	MPI_Status status; // MPI_SOURCE, TAG, ERROR

	// initialize MPI_Init
	err = MPI_Init(&argc, &argv);
	if (err != MPI_SUCCESS){
	  printf("\nError initializing MPI.\n");
	  MPI_Abort(MPI_COMM_WORLD, err);
	} // end if

	// Get No. of processors
	MPI_Comm_size(MPI_COMM_WORLD, &procs);
	
	// Get processor id
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	

	if (myid == 0)
	  for ( i = 1;i < procs;i++){
	    MPI_Recv(msg, 50, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, 
	             &status);
	    printf("\nReceiving %s from process %d with tag %d error %d\n", msg,
	         status.MPI_SOURCE, status.MPI_TAG, status.MPI_ERROR);
	  }
	else{
	  //srand(time(NULL));
	  time_t t1;
	  time(&t1); // get system time
	  srand(t1 + myid); // Initilize Random Seed
	  sprintf(msg, "%s %d", " Let's have fun by testing MPI...", myid);
	  int len = strlen(msg) + 1;
	  MPI_Send(msg, len, MPI_CHAR, 0, myid * (rand()% 25), MPI_COMM_WORLD); 
	}
	// end myid == 0

	MPI_Finalize();

	return 0;
  } // end main

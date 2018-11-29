#include <stdio.h>
#include <mpi.h>

    //--	Simulating ring network
    //--	Master generates a token and passess it to first processor
    //-- 	every processors adds 2 to the token and passes it to its right neighbor
    //--	and finally master receives token from last processor 

	int main (int argc, char *argv[]) {
	  int token, procs, myid, err;
	  MPI_Status status;

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

      if (myid == 0) {// to print only once....
			if (argc < 1) {
			  printf("\n\tOOOPS...., INVALID No of Arguements,\n"
						 "\tSimulating ring network\n"
						 "\tMaster generates a token and passess it to first processor\n"
						 "\tevery processors adds 2 to the token and passes it to its right neighbor\n"
						 "\tand finally master receives token from last processor.  94/07/05"
						 "\tTO COMPILE	mpicc fname.c\n"
						 "\tTO RUN	mpirun -n pp a.out \n");
			} // end if
      } // end myid == 0

      if (argc < 1) {MPI_Finalize(); return 0;} // end if



	  if (myid != 0) {
		 // Everyone except master receives from processor one less than its own id.
		 MPI_Recv(&token, 1, MPI_INT, myid - 1, 0, MPI_COMM_WORLD, &status);
		 printf("Process %d received token %d from process %d\n", myid, token, myid - 1);
	  } 
	  else {
		 // Master sets initial value before sending.
		 token = -1;
	  }

	  token += 2;

	  MPI_Send(&token, 1, MPI_INT, (myid + 1) % procs, 0,MPI_COMM_WORLD);

	  // Now process 0 can receive from the last process.

	  if (myid == 0) {
		 MPI_Recv(&token, 1, MPI_INT, procs - 1, 0,MPI_COMM_WORLD, &status);
		 printf("Process %d received token %d from process %d\n",myid, token, procs - 1);
	  }
	  MPI_Finalize();
	  return 0;
	}

#include <stdio.h> 		 
#include <malloc.h>
#include <time.h> 
#include <mpi.h>  		

    //--	A program to generate n number and storing in an array
    //--	and adding these no.'s in Parallel   96/06/25
    //--	TO COMPILE	mpicc fname.c
    //--	TO RUN		mpirun -n pp a.out yy zz
    //--	pp (integer) No.of Processors
    //--	yy (integer) No.of input to generate
    //--	zz (integer) Range of input data to be generated randomly

    int main(int argc, char **argv){
      int i, k, myid, procs;
      int n, range, err;
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
			if (argc < 3) {
			  printf("\n\tOOOPS...., INVALID No of Arguements,\n"
				 "\tA program to generate n number and storing in an array\n"
				 "\tand adding n numbers in parallel using Bcast and Reduce...\n"
				 "\tTO RUN	mpirun -n pp a.out nn rr\n"
				 "\t\tpp (integer) No.of Processors...\n"
				 "\t\tnn (integer) No. of inputs (size of array)...\n"
				 "\t\trr (integer) Range of data to be generated randomly...\n"
				 "\t\tNote: nn should be divisible by pp, otherwise some data will be lost...\n\n");
	      } // end if
      } // end myid == 0

      if (argc < 3) {MPI_Finalize(); return 0;} // end if

      n     = atoi(argv[1]);  // get n
      range = atoi(argv[2]);  // get range
      

      // Compute my startpt and endpt
      k = n / procs; // No. of elements to be computed by each Processor

      // Generate Data
      if (myid == 0) {
      	int sum, finalsum = 0, *arr;
      	
			//srand(time(NULL));
			time_t t1;
			time(&t1); // get system time
			srand(t1); // Initilize Random Seed
			
			// allocate space to generate data
      	arr   = malloc(n * sizeof (int)); 
			for(i = 0; i < n; i++){// generate & print data
			  arr[i] = rand() % range;
			  printf("%6d ", arr[i]); if ((i+1) % 10 == 0) printf("\n");
			} // end for
			
			for(i = 1; i < procs; i++)// send each processor's data
	  			MPI_Send(arr+k * i, k, MPI_INT, i, i, MPI_COMM_WORLD);

      	// compute master share
		   for(i = 0; i < k; i++) 	
		   	finalsum += arr[i];
		   printf("I got %d from %d\n", finalsum, myid);		   
		   
		   // --free allocated spaces
		   free (arr);//free allocated space for array a		   
		   
		   // Recv each processor's sum
			for(i = 1; i < procs; i++){
      		MPI_Recv(&sum, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      		finalsum += sum;
      	}

			printf("\nThe final sum is %d.\n", finalsum);
      } // end myid == 0
      else{
      	int *myarr = malloc(k * sizeof (int)); // allocate space
      	MPI_Recv(myarr, k, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      	     
		   // Add my portion Of data
		   int mysum = 0;
		   for(i = 0; i < k; i++) 	mysum += myarr[i];
	      printf("I got %d from %d\n", mysum, myid);
		   
		   // send sum to master
	  		MPI_Send(&mysum, 1, MPI_INT, 0, myid, MPI_COMM_WORLD);
      	// --free allocated space for myarr
	  		free(myarr);
		   
		} // end else


      MPI_Finalize();
      return 0;
    } // end main

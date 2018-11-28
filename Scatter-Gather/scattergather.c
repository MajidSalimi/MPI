#include <mpi.h>  		
#include <stdio.h> 		 
#include <malloc.h>
#include <time.h>
    //--	A program to generate n number and storing in an array
    //--	and adding these no.'s in Parallel 

    int getsum (int a[], int n, int id){
      int i, sum = 0;

      // Add my portion Of data
      for(i = 0; i < n; i++)
			sum += a[i];

      printf("Sum computed by processor %2d is: %10d\n", id, sum);
      return sum;
    }

    int main(int argc, char **argv){
      int i, k,  procs;
      int myid, mysum, finalsum;
      int *arr, *myarr, *sumarr, n, range, err;

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
      k = n / procs; // size for each processor
      
      arr   = malloc(n     * sizeof (int)); // allocate space for initial data
      myarr = malloc(k     * sizeof (int)); // allocate space for each processor
      sumarr= malloc(procs * sizeof (int)); // allocate space for local sums

      // Generate Data
      if (myid == 0) {
			//srand(time(NULL));
			time_t t1;
			time(&t1); // get system time
			srand(t1); // Initilize Random Seed

			for(i = 0; i < n; i++){// generate & print data
			  	arr[i] = rand() % range;
				printf("%6d ", arr[i]); if ((i+1) % 10 == 0) printf("\n");
			} // end for
	
      } // end myid == 0

      
      //MPI_Scatter automatically moves every k data from source to dest,
      //*source, k, MPI_INT, *dest, k, MPI_INT, 0, MPI_COMM_WORLD);

      MPI_Scatter(arr, k, MPI_INT, myarr, k, MPI_INT, 0, MPI_COMM_WORLD);

      // Add my portion Of data
      mysum = getsum(myarr, k, myid);

      // Gather sum
      //MPI_Gather automatically moves every k data from source to dest
      //*source, k, MPI_INT, *dest, k, MPI_INT, 0, MPI_COMM_WORLD);
      MPI_Gather(&mysum, 1, MPI_INT, sumarr, 1, MPI_INT, 0, MPI_COMM_WORLD);

      if (myid == 0)  {
			finalsum = 0;
			// Master sholud add all the received sums
			for(i = 0; i < procs; i++) 
		  		finalsum += sumarr[i];
			// print final sum
			printf("The finalsum is %d.\n", finalsum);
      }

      // --free allocated spaces
      free (arr);
      free(myarr); 
      free(sumarr);
      
      MPI_Finalize();
      return 0;
    } // end main

# include <cstdlib>
# include <iostream>
# include <iomanip>
#include <mpi.h> 

using namespace std;

    int main(int argc, char **argv){
      int i, j, k,  procs, startpt, endpt;
      int myid, mysum, finalsum;
      int *arr, n, range, err;

      // initialize MPI_Init
      err = MPI_Init(&argc, &argv);
      if (err != MPI_SUCCESS){
			cout<<"\nError initializing MPI.\n";
			MPI_Abort(MPI_COMM_WORLD, err);
      } // end if

      // Get No. of processors
      MPI_Comm_size(MPI_COMM_WORLD, &procs);

      // Get processor id
      MPI_Comm_rank(MPI_COMM_WORLD, &myid);

      if (myid == 0) {// to print only once....
			if (argc < 3) {
			  cout<<"\n \tOOOPS...., INVALID No of Arguements,\n";
	      } // end if
      } // end myid == 0

      if (argc < 3) {MPI_Finalize(); return 0;} // end if

      n     = atoi(argv[1]);  // get n
      range = atoi(argv[2]);  // get range
      
      //arr   = malloc (int) (n * sizeof (int)); // allocate space
      arr = new int[n * sizeof (int)];
      // Generate Data
      if (myid == 0) {
			srand(time(NULL));
			//time_t t1;
			//time(&t1); // get system time
			//srand(t1); // Initilize Random Seed

			for(i = 0; i < n; i++){// generate & print data
			  arr[i] = rand() % range;
			  cout<< arr[i] <<"  "; 
        if ((i+1) % 10 == 0) cout<<"\n";
			} // end for
      } // end myid == 0

      // Broadcast data
      MPI_Bcast(arr, n, MPI_INT, 0, MPI_COMM_WORLD);

      // Compute my startpt and endpt
      k = n / procs;
      startpt  = myid    * k;
      endpt   = startpt  + k;
      
      // Add my portion Of data
      mysum = 0;
      for(i = startpt; i < endpt; i++){
			mysum += arr[i];
      } // end for

      cout<<"I got " << mysum << " from " << myid << endl;

      // Compute global sum
      MPI_Reduce(&mysum, &finalsum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

      if (myid == 0){
			cout<<"\nThe final sum is " << finalsum << endl;
      } // end myid == 0

      // --free allocated spaces
      free (arr);//free allocated space for array a

      MPI_Finalize();
      return 0;
    } // end main
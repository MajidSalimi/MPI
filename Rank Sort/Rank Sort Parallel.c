#include <stdio.h> 		 
#include <malloc.h>
#include <time.h> 
#include <mpi.h>  		
    //--	An implementation of parallel Rank sort algorithm 
    //--    Numbers Generating randomely
    //--	TO RUN	mpirun -np p a.out n a b k
    //--    p (integer) No.of Processors
    //--	n (integer) No.of randomly generated data
    //--	a (integer) range of generated numbers (start of bound)
    //--    b (integer) range of generated numbers (end of bound)
    //--    k (integer) for printing k element in a line

//-----------------------------------------------------------------------------------------print array
void printArray(int *arr, int n, int k) 
{
  int i;
  for (i = 0; i < n; i++)
  {
    printf("%d  ", arr[i]);
    if ((i+1) % k == 0) 
        printf("\n");
  }
  printf("\n"); 
} 
//----------------------------------------------------------------------------------------rank sort
    //devide array between processors
    int Rank_sort (int a[],int b[],int rank[] ,int n, int id, int proc)
    {
      int i, j, k, startpt, endpt;
      // Compute my startpt and endpt
      k = n / proc;
      startpt = id * k;
      endpt   = startpt  + k;
      // Add my portion Of data
      for(i = startpt; i < endpt; i++)
        {   rank[i]=0; //initialize rank array
            for(j=0;j<n;j++)
              if(a[j]<a[i]||(a[i]==a[j]&&i<j)) 
              rank[i]++;
        }
      return 0;
     }
//=========================================================================================Main
    int main(int argc, char **argv)
{     //define variables
	  //FILE *fp;
      int  myid, procs;
      int *A,*rank,*frank,*B, n, rangei,rangej, err,i,k,kp;
      double t1_p,t2_p,t1_s,t2_s;
      
//------------------------------------------------------------------------------initializing
      err = MPI_Init(&argc, &argv);
      if (err != MPI_SUCCESS){
			printf("\nError initializing MPI.\n");
			MPI_Abort(MPI_COMM_WORLD, err);
      } // end if
      MPI_Comm_size(MPI_COMM_WORLD, &procs);
      MPI_Comm_rank(MPI_COMM_WORLD, &myid);

      if (myid == 0) 
      {
			if (argc < 5) 
                        {
			  printf("\n\tOOOPS...., INVALID No of Arguements\n");
                        } 
      } // end myid == 0

      if (argc < 3) {MPI_Finalize(); return 0;} // end if
//------------------------------------------------------------------------------start
      n      = atoi(argv[1]);  // get n
      rangei = atoi(argv[2]);  // get range
      rangej =atoi(argv[3]);   // get range
      kp     =atoi(argv[4]);   //get k (print k element in one row)

      A   = malloc(n * sizeof (int)); // allocate space
      B   = malloc(n * sizeof (int)); // allocate space
      rank= malloc(n * sizeof (int)); // allocate space
      frank= malloc(n * sizeof (int)); // allocate space

      if (myid == 0) 
     {
		  printf("\nInput array:\n");     
//--------------------------------------------------------randomly fill the array		  
	      for(i = 0; i < n; i++)
          {
		      A[i] = (rand() % (rangej + 1 - rangei)) + rangei ;
	      } // end for
	      printArray(A,n,kp);

      } // end myid == 0

      t1_p=MPI_Wtime();//get start time     
       
      MPI_Bcast(A, n, MPI_INT, 0, MPI_COMM_WORLD); // Broadcast data
      Rank_sort(A,B,rank, n, myid, procs);//rank data 
      k = n / procs;
      MPI_Gather(rank+(k*myid), n/procs, MPI_INT, frank, n/procs, MPI_INT, 0, MPI_COMM_WORLD);//gather data
     
      t2_p=MPI_Wtime();//get finish time
      
      if (myid == 0)
     {       // printf("\nRank array\n");
		     // printArray(frank, n,kp);
            for(i=0;i<n;i++)
                 B[frank[i]]=A[i];
              
              printf("\nSorted array:\n");
              printArray(B, n,kp);
              printf("Parallel Elapsed time for this execution is: %f seconds ", t2_p-t1_p); 
              printf("\n*****************************************************************************");
              printf("\nthis program runned for 10000 same random numbers between 1 to 1000 that was saved in a file");
              printf("\nI runned it 5 times for sequential and 5 times for parallel mode with 10 processors.");
              printf("\nThis is the average executed time records for both modes:");
              printf("\nsequential Time:  0.6444338 seconds");
              printf("\nparallel Time:  0.2728532 seconds");
              printf("\nAnd the speedup(Tseq/Tpara) is 2.36183396\n\n\n");
      } // end myid == 0

      // --free allocated spaces
      free (A);//free allocated space for array a
      free (B);
      free(rank);
      free (frank);
      //output comments and reports             

      MPI_Finalize();
      return 0;
} // end main

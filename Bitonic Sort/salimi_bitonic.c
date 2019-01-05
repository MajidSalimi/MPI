//MajidSalimi
//2/11/96
//Bitonic sort
//to compile: -------> mpicc salimi_bitonic.c -lm
//to run:------------> mpirun -np procs a.out n a b
//procs: number of processes
//n: total numbers
//a,b: range of generated numbers
//----------------------------------------------------------------------
#include <mpi.h> 
#include <stdio.h> 		 
#include <malloc.h>
#include <stdlib.h>	
#include <math.h>
#include <time.h> 
//----------------------------------------------------------------------generate array
void GenerateArray(int *arr ,int n,int a,int b)
	{
		int i;
		for(i = 0; i < n; i++)
		{
           arr[i] = (rand() % (b + 1 - a)) + a ;
		}       
	}
//----------------------------------------------------------------------Printarray
void printArray(int *arr, int n) 
	{
	  int i;
	  for (i = 0; i < n; i++)
		printf("%d  ", arr[i]);
	  printf("\n"); 
	 } 

//----------------------------------------------------------------------sequential merge sort
void merge(int arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;

    int L[n1], R[n2];

    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1+ j];
    i = 0; 
    j = 0; 
    k = l; 
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(int arr[], int l, int r)
{
    if (l < r)
    {
        int m = l+(r-l)/2;
        mergeSort(arr, l, m);
        mergeSort(arr, m+1, r);
        merge(arr, l, m, r);
    }
}
	 
//======================================================================main	
	int main(int argc, char * argv[]) 
	{
	   int myid,procs,n,i,j,k,a,b,source,partner2,err,partner, min ,received, max,sent,steps;
	   int *inputarr, *partnerarr ,*myarr;
	   double t1_p,t2_p;
	   MPI_Status status;
	   err = MPI_Init(&argc, &argv);
       if (err != MPI_SUCCESS)
		 {
			printf("\nError initializing MPI.\n");
			MPI_Abort(MPI_COMM_WORLD, err);
         }
       MPI_Comm_size(MPI_COMM_WORLD, &procs);
	   MPI_Comm_rank(MPI_COMM_WORLD, &myid);
//----------------------------------------------------------------------args
	   	n = atoi (argv[1]);
	    a = atoi (argv[2]);
        b = atoi (argv[3]);
        
       if (myid == 0) 
         {
			if (argc < 4)//number of args 
			  {
			    printf("\n\tOOOPS...., INVALID No of Arguements \n");
	          }
	        t1_p=MPI_Wtime();  
         }
      if (argc < 4) //number of args
         {
		  MPI_Finalize(); 
		  return 0;
	     }

		inputarr = malloc(n * sizeof(int));
		partnerarr= malloc((n + 1) * sizeof(int));
		myarr = malloc((n + 1) * sizeof(int));
		GenerateArray(inputarr, n, a, b);
		MPI_Barrier(MPI_COMM_WORLD);
	    steps = (int)(log2(procs));
		mergeSort(inputarr, 0,n);
		steps = 0;
		
		while (i < steps)
		{
			for (j = i; j >= 0; j--) 
			 {
				source=myid >>(i + 1);
				partner2=myid >> j;

	       if ((source % 2 == 0 &&  partner2% 2 == 0) || source % 2 != 0 && partner2 % 2 != 0)
	        {
				sent = 0;
				partner = myid ^ (1 << j);
				MPI_Send(&inputarr[n - 1], 1,MPI_INT, partner, 0, MPI_COMM_WORLD);
				MPI_Recv(&min, 1, MPI_INT, partner, 0,MPI_COMM_WORLD, &status);	
				for (i = 0; i < n; i++) 
			    {
			      if (inputarr[i] > min) 
			      {
				    myarr[sent + 1] = inputarr[i];
				    sent++;
			      }
			     else 
				   break;
			    }
			myarr[0] = sent;
			MPI_Send(myarr, sent, MPI_INT, partner, 0, MPI_COMM_WORLD );
			MPI_Recv(partnerarr,n, MPI_INT, partner, 0, MPI_COMM_WORLD, &status);
			for (i = 1; i < partnerarr[0] + 1; i++) 
			{
				if (inputarr[n-1] < partnerarr[i]) 
				{
					inputarr[n-1] = partnerarr[i];
				} 
				else 
					break;
			}
   	    mergeSort(inputarr, 0, n);					
	}//end if	
		
	else 
	{		
            sent = 0;
			partner = myid ^ (1 << j);
			MPI_Recv(&max,1,MPI_INT, partner, 0, MPI_COMM_WORLD, &status );
			MPI_Send(&inputarr[0], 1, MPI_INT, partner, 0, MPI_COMM_WORLD);
			for (i = 0; i < n; i++) 
			{
				if (inputarr[i] < max) 
				{
					myarr[sent + 1] = inputarr[i];
					sent++;
				}
				else 
					break; 
			}
			MPI_Recv(partnerarr,n, MPI_INT, partner, 0, MPI_COMM_WORLD, &status );		
			received = partnerarr[0];
			myarr[0] = sent;		
			MPI_Send(myarr, sent, MPI_INT, partner,0, MPI_COMM_WORLD );
			for (i = 1; i < received + 1; i++) 
			{
				if (partnerarr[i] > inputarr[0]) 
				{
					inputarr[0] = partnerarr[i];
				} 
				else 
					break; 
			}//end for
			mergeSort(inputarr,0, n);	
	}//end else	
		
	}//end inner for
	i++;
   }//end while
   
		if (myid == 0) 
		{		
		  printArray(inputarr,n);
		  printf("\n");
		  t2_p=MPI_Wtime();//get finish time
          printf("\nParallel Elapsed time for this execution is: %f seconds\n ", t2_p-t1_p); 
		}
				
		free(inputarr);
		free(myarr);
		free(partnerarr);
		
		MPI_Finalize();
		return 0;
	}


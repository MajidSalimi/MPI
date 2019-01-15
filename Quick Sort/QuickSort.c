//written by: Majid Salimi
//quick sort
//to compile: -------> mpicc salimi_Quick.c -lm
//to run:------------> mpirun -np procs a.out n a b
//procs: number of processes
//n: total numbers
//a,b: range of generated numbers
//----------------------------------------------------------------------includes
#include <stdio.h> 		 
#include <malloc.h>
#include <time.h> 
#include <mpi.h> 
#include <stdlib.h>	
#include <string.h>
//----------------------------------------------------------------------print array 	
void printArray(int *arr, int n) 
{
  int i;
  for (i = 0; i < n; i++)
    printf("%d  ", arr[i]);
   // printf("\n");
  
  printf("\n"); 
 } 	
//----------------------------------------------------------------------
void genArray(int *arr ,int n,int rangei,int rangej)
{
	int i;
	for(i = 0; i < n; i++)
       {
           arr[i] = (rand() % (rangej + 1 - rangei)) + rangei ;
       }
       
}
//-----------------------------------------------------------------------
void swap(int* a, int* b)
{
    int t = *a;
    *a = *b;
    *b = t;
}
//------------------------------------------------------------------------
int partition (int arr[], int low, int high,int pivot)
{
	int j;
    int i = (low - 1);  // Index of smaller element
    for (j = low; j <= high- 1; j++)
    {
        if (arr[j] < pivot)
        {
            i++;    // increment index of smaller element
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}
//------------------------------------------------------------------------
int pivoting(int arr[],int n)
{
	int a=arr[0],b=arr[n/2],c=arr[n];
	if( b>a && a>c || c>a && a>b )
       return a;      
    if( a>b && b>c || c>b && b>a )
       return b;
    if( a>c && c>b || b>c && c>a )
       return c;

}
//------------------------------------------------------------------------
void seq_quickSort(int arr[], int low, int high,int pivot)
{
    if (low < high)
    {
        int pi = partition(arr, low, high,pivot);
        seq_quickSort(arr, low, pi - 1,pivot);
        seq_quickSort(arr, pi + 1, high,pivot);
    }
}
//-----------------------------------------------------------------------
 int bitCount(int n) 
 {
    int counter = 0;
    while(n>1) {
		counter++;
        //counter += n % 2;
        n/=2;
       // n >>= 1;
    }
    return counter;
 }
 void makeArray(int *lowarr,int *higharr,int *slavearr,int middle,int n)
 {int i,j;
	 
   for(i=0;i<middle;i++)
              {
                  lowarr[i]=slavearr[i];
			  }
              for(i=middle,j=0;i<=n;i++,j++)
              {
                  higharr[j]=slavearr[i];
		      }	  
 }
//======================================================================main
    int main(int argc, char **argv)
    { 
	  double t1_p,t2_p;
      int err, i, j, myid, procs, n, a, b, pivot,start,middle,d,steps=0,*mergearray,sizeofhigharr=0,highproc,lowproc;
      int *arr,*result,*lowarr,*higharr,*slavearr,*mergearr,lowerproc,sizeofhighproc=0,sizeoflowarr=0,dest;
	  MPI_Status status;
      err = MPI_Init(&argc, &argv);
      if (err != MPI_SUCCESS){
			printf("\nError initializing MPI.\n");
			MPI_Abort(MPI_COMM_WORLD, err);
      }
      MPI_Comm_size(MPI_COMM_WORLD, &procs);
      MPI_Comm_rank(MPI_COMM_WORLD, &myid);
      if (myid == 0) 
         {
			if (argc < 3)//number of args 
			  {
			    printf("\n\tOOOPS...., INVALID No of Arguements,\n");
	          }
         }
      if (argc < 3) //number of args
         {
		  MPI_Finalize(); 
		  return 0;
	     }
//----------------------------------------------------------------------args
      n =atoi(argv[1]);
      a =atoi(argv[2]);
      b =atoi(argv[3]);
      lowarr=malloc( n*sizeof (int));
      higharr=malloc( n*sizeof (int));
      slavearr=malloc(n*sizeof(int));
      result = malloc(procs*n * sizeof (int));
      mergearr=malloc(sizeof(int)*(n));

//----------------------------------------------------------------------master
      if (myid == 0)
         {			            
              arr = malloc( n*procs*sizeof (int)); 
              genArray(arr,(n*procs),a,b);              
              d=bitCount(procs);
              printf("d= %d",d);
         } // end myid == 0
//----------------------------------------------------------------------slaves 
       // slavearr=malloc(n*sizeof(int));
        MPI_Bcast(&d, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Scatter( arr, n, MPI_INT, slavearr, n, MPI_INT, 0, MPI_COMM_WORLD); 
        i=1;
        
        while(steps<d)
        { 
          pivot =pivoting(arr,n);
          MPI_Bcast(&pivot, 1, MPI_INT, 0, MPI_COMM_WORLD);
          printf("\nmy pivot= %d", pivot);  
          middle= partition (slavearr,0,n,pivot);
          printf("\nmidle: %d  \n",middle);
          printf("\nmyid: %d  prtitioned list:\n",myid); 
               
		  printArray(slavearr,n);             
          makeArray(lowarr,higharr,slavearr,middle,n);
		  dest=myid^i;
			 
			 if(myid>dest){

				
				lowerproc=dest;
				  
				MPI_Send (&middle, 1, MPI_INT, lowerproc, 0,MPI_COMM_WORLD);
				 
				MPI_Send (lowarr, middle, MPI_INT, lowerproc, 0,MPI_COMM_WORLD);
				
				MPI_Recv(&sizeoflowarr, 1, MPI_INT, lowerproc, 0, MPI_COMM_WORLD, &status);
				
				
				lowarr=realloc(lowarr, sizeof(int)*sizeoflowarr);
				MPI_Recv(lowarr, sizeoflowarr, MPI_INT, lowerproc, 0,MPI_COMM_WORLD, &status);
				
				mergearr=realloc(mergearr, sizeof(int)*(n-middle+sizeoflowarr));
				
				memcpy(mergearr,lowarr,sizeoflowarr);
				memcpy(mergearr+sizeoflowarr,slavearr,n-middle);
				
				printArray(mergearr,n-middle);
				middle=partition(mergearr,0,n-middle+sizeoflowarr,pivot);
				makeArray(lowarr,higharr,mergearr,middle,n-middle+sizeoflowarr);
			 }
			 else
			 {	
				 
				int middlenew;
			    highproc=dest;
			    
				MPI_Recv(&sizeoflowarr, 1, MPI_INT, highproc, 0,MPI_COMM_WORLD,&status);
				printf("\nim here\n");
				MPI_Recv(lowarr, sizeoflowarr, MPI_INT, highproc, 0,MPI_COMM_WORLD, &status);
				
				
				mergearr=realloc(mergearr, sizeof(int)*(middle+sizeoflowarr));
				
				
				memcpy(mergearr,slavearr,middle);
				memcpy(mergearr+middle,lowarr,sizeoflowarr);
				
				middlenew=partition(mergearr,0,middle+sizeoflowarr,pivot);
				sizeofhighproc=middle+sizeoflowarr-middlenew;
				
			    higharr=realloc(higharr, sizeof(int)*sizeofhighproc);
				
				
				
				makeArray(lowarr,higharr,mergearr,middlenew,middle+sizeoflowarr);
				
				
				MPI_Send (&sizeofhighproc, 1, MPI_INT, highproc, 0,MPI_COMM_WORLD);
				
				MPI_Send (higharr, sizeofhighproc, MPI_INT, highproc, 0,MPI_COMM_WORLD);	
				
			 }
			 
			 i++;
		     
           //  MPI_Send (lowarr, middle, MPI_INT, lowerproc, 0,MPI_COMM_WORLD);
          //   MPI_Send (higharr, n-middle, MPI_INT, upperproc, 0,MPI_COMM_WORLD);
             steps++;
		 
	    }//end while
	    
//----------------------------------------------------------------------master		
MPI_Barrier(MPI_COMM_WORLD);

if(myid>0){
	int slavearrsize=sizeof(slavearr);
	MPI_Send (&slavearrsize, 1, MPI_INT, 0, 0,MPI_COMM_WORLD);
	MPI_Send (slavearr, slavearrsize, MPI_INT, 0, 0,MPI_COMM_WORLD);		
	}
		if (myid==0)
		{
			int temp=0;
			int j=0;
			int slavearrsize;
			for(i=0;i<procs;i++)
			{
			MPI_Recv(&slavearrsize, 1, MPI_INT, MPI_ANY_SOURCE, 0,MPI_COMM_WORLD,&status);
			MPI_Recv(slavearr, slavearrsize, MPI_INT, MPI_ANY_SOURCE, 0,MPI_COMM_WORLD,&status); 
			for(j=0;j<slavearrsize;j++){
				result[temp]=slavearr[j];
				temp++;
			}
		}
		  printArray(result,n*procs);
		  t2_p=MPI_Wtime();//get finish time
          printf("\nParallel Elapsed time for this execution is: %f seconds\n ", t2_p-t1_p); 
	    } //end myid == 0
//------------------------------------------------------------------------end

      free (arr);
      free (result);
      free (higharr);
      free (lowarr);
      free (slavearr);
      MPI_Finalize();
      return 0;
      
    } // end main
//------------------------------------------------------------------------functions


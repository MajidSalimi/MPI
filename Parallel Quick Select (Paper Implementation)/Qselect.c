//Implemented by: Majid Salimi
//Quick select algorithm (Paper implementation)
//This algorithm is a new method for pivot selecting and partitioning for some algorithms like quick sort and quick select
//==================================================================================================
//to compile      mpicc Qselect_salimi.c
//to run          mpirun -np procs a.out n range k
//procs:  number of processores
//n:      number of random numbers
//range:  range of random generated numbers between 1 and range
//k:      we want to find k'th smallest number  
//***hint*** : in this paper, they assume: n==p 
//***hint*** : n (and p) must be Multiples of 3
//==================================================================================================
#include <stdio.h> 		 
#include <malloc.h>
#include <time.h> 
#include <mpi.h> 
#include <stdlib.h>	
#include <math.h>
//----------------------------------------------------------------------print array 	
void printArray(int *arr, int n) 
{
  int i;
  for (i = 0; i < n; i++)
    printf("%d  ", arr[i]);
   // printf("\n");
  
  printf("\n"); 
 } 	
//----------------------------------------------------------------------generate array
void genArray(int *arr ,int n,int rangei,int rangej)
{
	int i;
	for(i = 0; i < n; i++)
       {
           arr[i] = (rand() % (rangej + 1 - rangei)) + rangei ;
       }
       
}

int partition(int x,int pivot,int *s,int *o ,int *v,int xnew,int myid , int kth);//parallel partition function
//======================================================================main
int main(int argc, char **argv)
  { 
	  double t1_p,t2_p;
      int err, i, j, myid, procs, n, a, x, pivot,kth,l1,l2,group,y=0,z=0,Fpivot1,Fpivot2,Fpivot,t=1,d,xnew=0,k=0,color;
      int *arr,*result,*lowarr,*higharr;
      int s[3]={0,0,0},o[3]={0,0,0},v[3]={0,0,0};
	  MPI_Status status;
	  MPI_Request request;
      err = MPI_Init(&argc, &argv);
      if (err != MPI_SUCCESS)
		{
			printf("\nError initializing MPI.\n");
			MPI_Abort(MPI_COMM_WORLD, err);
		}
      MPI_Comm_size(MPI_COMM_WORLD, &procs);
      MPI_Comm_rank(MPI_COMM_WORLD, &myid);    
      if (myid == 0) 
         {
			if (argc < 3)//number of args 
			    printf("\n\tOOOPS...., INVALID No of Arguements,\n");
         }
      if (argc < 3) //number of args
         {
		  MPI_Finalize(); 
		  return 0;
	     }
//----------------------------------------------------------------------args
      n =atoi(argv[1]);//numbers
      a =atoi(argv[2]);//between 1 to a
      kth =atoi(argv[3]);//find kth smallest
//----------------------------------------------------------------------master
      if (myid == 0)
         {
              arr = malloc( n*sizeof (int));
              genArray(arr,n,1,a);//generate random integer array
              group=n/3;//group procs
         } // end myid == 0
//----------------------------------------------------------------------slaves      
	 MPI_Scatter(arr,1, MPI_INT, &x,1, MPI_INT, 0, MPI_COMM_WORLD);
	 t=myid*t;
	 if ( (myid != 1) && (myid !=0) && (myid%2==0) && (myid<(n/3)*3) && (myid%3 !=0))              
		{ 
			MPI_Isend(&x,1,MPI_INT,myid-2,0,MPI_COMM_WORLD,&request); 	 
		}    
	 if ( (myid !=0)  && (myid<(n/3)*3) && ((myid%3 !=0) || myid==1))
		{
			MPI_Isend(&x,1,MPI_INT,myid-1,0,MPI_COMM_WORLD,&request); 	
		}
		 
	  MPI_Barrier(MPI_COMM_WORLD);
	  
	  if ((myid%(3) ==0 || myid==0) && (myid< (n/3)*3))
	  {
	  
	   MPI_Irecv(&y, 1, MPI_INT, myid+1, 0, MPI_COMM_WORLD, &request);
	   MPI_Irecv(&z, 1, MPI_INT, myid+2, 0, MPI_COMM_WORLD, &request);
	  //find median of three numbers
		if( y>x && x>z || z>x && x>y )
		  {
			 pivot=x;
		  }
		if( x>y && y>z || z>y && y>x )
		  {
			pivot=y;
		  }
		if( x>z && z>y || y>z && z>x )
		  {
			pivot=z;
		  }
		}		  
	   MPI_Bcast( &pivot, 1, MPI_INT, 0, MPI_COMM_WORLD );
	   if (myid==0)
	      printf("\npivot is: %d\n", pivot);
	   partition(x,pivot,s,o,v,xnew,myid,kth);              	
//----------------------------------------------------------------------
       //free (arr);
       MPI_Finalize();
       return 0;
  } // end main
  
//----------------------------------------------------------------------partition function
  int partition(int x,int pivot,int *s,int *o ,int *v,int xnew,int myid , int kth)
	{	
		int d;
		//initialize v vector
		if (x<pivot)
		   v[0]=1;
		else if (x==pivot)
		   v[1]=1;
		else if (x>pivot)
		   v[2]=1;
		//calculate s vector    
	    MPI_Allreduce( v, s, 3, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
	    //calculate o vector  
	    MPI_Exscan(v, o, 3, MPI_INT, MPI_SUM, MPI_COMM_WORLD); 	
	    //calculate where target element located based on pivot location    
	    if (x<pivot)
		   d=o[0];
	    else if (x==pivot)
		   d = (s[0] + o[1]);
	    else if (x>pivot)
		   d = (s[0] + s[1] + o[2]);
		//wait for all processors 
		MPI_Barrier(MPI_COMM_WORLD);
		printf("\n\nI'm processor:%d ===%d===> destination=%d",myid,x,d); 
	    if(myid !=d)//if we need to exchange elements
	      { 
			 //send the element to its destination process and receive a new element
			 MPI_Sendrecv(&x, 1, MPI_INT, d, 0, &xnew, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	         x=xnew;
	      }
	   MPI_Barrier(MPI_COMM_WORLD);	  
	   printf("\nmyid=%d  new x: %d\n",myid,x);
	   MPI_Barrier(MPI_COMM_WORLD);	
	   if (kth <= s[0])
			{
				if (myid==0)
				printf("\n{target element is among the smaller elements}\n");
				// MPI_Comm_split(MPI_COMM_WORLD, myid, &myid, &single_server_comm); 
			    //return partition( x, pivot,s,o ,v,xnew,myid , kth);
			}
		else if (kth > (s[0]+s[1]+s[2])-s[2]) 
			{
				if (myid==0)
				printf("\n{target element is among the larger elements}\n");
				//MPI_Comm_split(MPI_COMM_WORLD, myid, &myid, &single_server_comm);
			   //return partition( x, pivot,s,o ,v,xnew,myid , kth);
			}
		else
			{
			 printf ("\nkth element is: %d\n",pivot);
			//{target element is the pivot}
			//return pivot;
			}
	}

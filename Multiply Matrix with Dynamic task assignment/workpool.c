//Majid Salimi Beni
//parallel Matrix multiply with Dynamic task assignment (workpool)

//this program runned for 900*900*900 Matrixes with same random double numbers between 1 to 100 that was saved in a file
//I runned it 5 times for sequential and 5 times for workpool parallel and cyclic parallel mode with 10 processors.(k=10)
//This is the average execution time records for both modes:
//sequential Time:  13.318590 seconds
//parallel workpool Time:  3.121489 seconds
//parallel wtatic Time:  2.404052 seconds
//=======================================================================================================
#include <stdio.h> 		 
#include <malloc.h>
#include <time.h> 
#include <mpi.h>  
#include <stdlib.h>		
    //--	TO COMPILE	mpicc fname.c
    //--	TO RUN		mpirun -n pp a.out m n p k range1 range2
    //--        m,n,p           are the matrix: A(m*n) , B(n*p)
    //--        k               is the number of rows that we send to each processor
    //--        range1,2        are the ranges of generated numbers (double)
//---------------------------------------------------------------------
void genArray(double *arr, int m, int n,int rangei,int rangej) //generate 2D double arrays in form of 1D
{
    int i,j;
    for (i=0;i<(m*n);i++)
    {
         arr[i] = (double)rand() /RAND_MAX * (rangej - rangei) + rangei ;                   
    }   
}
//---------------------------------------------------------------------
void printArray(double *arr,int m,int n)//a function for printing 2D array
{  
	 int i,j;
    for (i=0;i<m;i++)
    {
      for(j=0;j<n;j++)
      {
        printf("%f ",arr[i*n+j]);
      }              
      printf("\n");                   
    }    
}
//---------------------------------------------------------------------

    int main(int argc, char **argv){
     double t1_p,t2_p,t1_s,t2_s;
      int i,u, myid, procs,j,z,trow,temp1,k,t;
      int n,m,p, rangei,rangej, err;
      double  *arr1,*arr2,*result,*temp;
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

      if (myid == 0) {
			if (argc < 7) {
			  printf("\n\tOOOPS...., INVALID No of Arguements,\n");
	      } // end if
      } // end myid == 0

      if (argc < 7) {MPI_Finalize(); return 0;} // end if

      m     = atoi(argv[1]);  // get m,n,p  
      n     = atoi(argv[2]); 
      p     = atoi(argv[3]); 
      k     = atoi(argv[4]);//assign k rows to each processor
      rangei = atoi(argv[5]);  // get range
      rangej = atoi(argv[6]);  // get range
      
      arr1   = malloc(n*m *  sizeof (double)); // allocate space
      arr2   = malloc(n *p* sizeof (double)); // allocate space
      temp   = malloc(p*k *sizeof (double));
      result = malloc(m *p*n* sizeof (double)); // allocate space
//======================================================================
     
      if (myid == 0) {
		   
			srand(time(NULL));
			 
			//srand(t1); // Initilize Random Seed
			printf("\nFirst Matrix: \n");
			genArray(arr1, m, n,rangei,rangej);
			printArray(arr1,m,n);
			printf("\nSecond Matrix: \n");
			genArray(arr2, n, p,rangei,rangej);			
			printArray(arr2,n,p);
			printf("\n");
			
      } // end myid == 0
      int R=m%k;//if np in not divisible by m
      t1_p=MPI_Wtime();//get start time  
      MPI_Bcast(arr1, m*n, MPI_DOUBLE, 0, MPI_COMM_WORLD);//B-cast all data to all processor
      MPI_Bcast(arr2, n*p, MPI_DOUBLE, 0, MPI_COMM_WORLD);
//-------------------------------------------------------------------------------------------master      
      if (myid == 0)
           { 
			for(i=1;i<procs;i++)//send row number to slaves (assign works)
			{   
				temp1=k*(i-1);
			        MPI_Send(&temp1, 1, MPI_INT, i, 1, MPI_COMM_WORLD);//send====send didnt accept &(i-1)
			 }			 
			j=0;
			i= procs-1;
			
			while(j<m)
			{   
				MPI_Recv(temp,p*k , MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);//receive
				 printf("From processor %d I received:\n",status.MPI_SOURCE );  
				 printArray(temp,k,p);
				 printf("\n"); 			
				   
				for(z=0;z<k;z++)
				{
				for(u=0;u<p;u++)
				    result[(status.MPI_TAG+z)*p+u]=temp[z*p+u];//copy temp results that receives from each processor to result
			    } 
				 
				 
                  if(status.MPI_TAG==m)//if np in not divisible by m (tag==m)
                  { 
					
                    for(z=0;z<R;z++)
			           	{
				           for(u=0;u<p;u++)
				             result[(m-R+z)*p+u]=temp[z*p+u];
			         	 } 	   		
      			  }//endif	  	    
				j=j+k;							
				
				if (i<(m/k))//if we have some works yet
				{  t=i*k;
				   MPI_Send(&t, 1, MPI_INT, status.MPI_SOURCE, 1, MPI_COMM_WORLD);	
				   i++;
				}
			    else if (i==(m/k) && R!=0)//if np in not divisible by m (tag==m)
				   {             
			           MPI_Send(&i, 1, MPI_INT, status.MPI_SOURCE, m, MPI_COMM_WORLD);//if the remainder is !=0 		
			       }//end else if
			    else 
			           MPI_Send(&i, 1, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD);//tag=0 Means that work finished		  				          			  
			}//end while
			
      } // end myid == 0
//------------------------------------------------------------------------------------------------------------------slaves      
      else  
      {	  
		  MPI_Recv(&trow,1 , MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);//trow is the number of the trow that we want to assign
		  while (status.MPI_TAG==1)//tag=1 means that some works remaining
		  {  
			  for (z=0;z<k;z++)//do computation
			  {
			    for(i=0;i<p;i++)
			     {
				  temp[z*p+i]=0;
			      for(j=0;j<n;j++)
			         temp[z*p+i]+=(arr1[(z+trow)*n+j]*arr2[j*p+i]);
			         
			     }//end for i=0
			 } //end for z =0
			
			 
			   MPI_Send(temp, p*k, MPI_DOUBLE, 0, trow, MPI_COMM_WORLD);//send temp result (k rows)
			   
		       MPI_Recv(&trow,1 , MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);//receive next row
		     
		  }//end while
		  
		  if(status.MPI_TAG==m)//for remainder
		    {   
				 for(z=0;z<R;z++)
			     {
			      for(i=0;i<p;i++)//computation
			      {
				  temp[z*p+i]=0;
			      for(j=0;j<n;j++)
			         temp[z*p+i]+=(arr1[(z+trow)*n+j]*arr2[j*p+i]); 
			      }//end for i
			    } //end for z 

			   MPI_Send(temp, p*R, MPI_DOUBLE, 0, m, MPI_COMM_WORLD);//send remainder result
			   
			}//end while tag==2			
	   }//end else
//-----------------------------------------------------------------------------------------------------master	   
	     if (myid == 0)
		  {  t2_p=MPI_Wtime();//get finish time  
			 printf("A*B=\n");
			 printArray(result,m,p);
			 printf("\n*****************************************************************************\n");
			 printf("Parallel Elapsed time for this execution is: %f seconds ", t2_p-t1_p); 
			 printf("\nMajid Salimi Beni\n2018\n");
             printf("\n*****************************************************************************");                        
		  }
      // --free allocated spaces
      free (arr1);
      free (arr2);
      free (result);
      free(temp);  
      
      MPI_Finalize();
      
      return 0;
    } // end main

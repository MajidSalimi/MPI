//Written by: Majid Salimi Beni

//A program to read input data from file and sort them with insertion sort
//to compile: mpicc name.c
//to execute: mpirun -np p a.out filename k
//please enter filename without '.txt'
//======================================================================
//this program runned for 100000 random double numbers between 1 to 100 that was saved in a file
//I runned it 5 times for sequential and 5 times for  parallel with 50 processors.(p=50)
//This is the average execute time records for both modes:
//sequential Time:  9.529495 seconds
//parallel Time:  2.179933 seconds
//----------------------------------------------------------------------
#include <stdio.h> 		 
#include <mpi.h>  
#include <malloc.h>
#include <time.h> 
#include <string.h>
#include <stdlib.h>
//----------------------------------------------------------------------
double minimum(double *arr,int n,double current)
{
	int i;
    double min;    
    for(i=0;i<n;i++){
        if(arr[i]<current)
        {
            min=arr[i];
            arr[i]=current;
            current=min;
        }       
    }
    return current;
}
//----------------------------------------------------------------------
void insertionSort(double* arr, int n)//insertion sort is the best algorithm to sort small arrays
{
  int i, j;
  double key;
  for (i = 1; i < n; i++)
  {
    key = arr[i];
    j = i-1;
    while (j >= 0 && arr[j] > key)
    {
        arr[j+1] = arr[j];
        j = j-1;
    }
    arr[j+1] = key;
  }
}
//----------------------------------------------------------------------
void printarray(double *arr, int n,int k) 
{
  int i;
  for (i = 0; i < n; i++)
  {
    printf("%f ", arr[i]);
    if ((i+1) % k == 0) 
        printf("\n");
  }
  printf("\n"); 
} 
//----------------------------------------------------------------------
int main(int argc,char * argv[]) 
{
	double t1_p,t2_p;
    int n,k,portion,processor_num,myid,Start,End,Sentdata=0,WorkRecvd,Slaves,i,j;
    double *array,*slaveportion,*tmp,*result;
    double DataSent,received,c,rangei,rangej;
    MPI_Status status;
    FILE *file1;
    char * filename;
    if (argc < 3) 
    {
        printf("\n\tOOOps, INVALID No of Arguements\n\n");
        exit(1);
    }   
    filename = argv[1];
    k = atoi(argv[2]); 
    strcat(filename,".txt");
    file1 = fopen(filename, "r");  
    if( file1 == NULL)
 	   {
          printf("I couldn't open %s for writing.\n",filename);
          exit(0);
       }
    fscanf(file1,"%d ", &n );//read file information
    fscanf (file1, "%lf ", &rangei);
    fscanf (file1, "%lf ", &rangej);
    int err = MPI_Init(&argc,&argv);
    if (err != MPI_SUCCESS)
    {
        printf("\nError initializing MPI.\n");
        MPI_Abort(MPI_COMM_WORLD, err);
    } // end if
    MPI_Comm_size(MPI_COMM_WORLD, &processor_num);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    t1_p=MPI_Wtime();
    //calculate each processor's portion of data
    Slaves=processor_num-1;
    portion = n / Slaves;
    Start  = myid    * portion;
    End   = Start  + portion;
    array = malloc(n * sizeof (double));
    slaveportion = malloc( portion * sizeof (double));
    tmp = malloc(portion * sizeof (double));
    result = malloc(n * sizeof (double));    
    if(myid==0)//master
   {
    for(i=0;i<n;i++)
    {
        fscanf(file1,"%lf,", &array[i] );
    }
    printf("\n");
    fclose(file1);
    printf("\nInput data before sorting:\n");
    printarray(array, n, k);

        while(Sentdata<n)
        {
            DataSent=array[Sentdata];
            MPI_Send(&DataSent,1,MPI_DOUBLE,1,0,MPI_COMM_WORLD);
            Sentdata=Sentdata+1;
        }
        for(i=0;i<Slaves;i++)
        {
            MPI_Recv(tmp,portion,MPI_DOUBLE,myid+1,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
            for(j=0;j<portion;j++)
                result[portion*(Slaves-status.MPI_TAG)+j]=tmp[j];
        }   
    }
   else
   {       
       double min;
       int DataIndex,RightProcs=processor_num-myid-1,rec_count=0,datanumebrs=(processor_num-myid)*portion;
       while(rec_count<datanumebrs)
       {
           MPI_Recv(&received,1,MPI_DOUBLE,myid-1,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
           if(rec_count>=portion)
           {
               min=minimum(slaveportion, portion, received);
               received=min;
               DataIndex=rec_count+myid*portion;
               MPI_Send(&received,1,MPI_DOUBLE,myid+1,0,MPI_COMM_WORLD);
               rec_count++;
           }
           else
           {  
              slaveportion[rec_count]=received;
              rec_count++;
           }
           
       }
       insertionSort(slaveportion, portion);//sort sub arrays
       MPI_Send(slaveportion,portion,MPI_DOUBLE,myid-1,myid,MPI_COMM_WORLD);
       rec_count=0;
       while(rec_count<RightProcs)
       {
           if(myid==Slaves)
           {
               break;
           }
           MPI_Recv(slaveportion,portion,MPI_DOUBLE,myid+1,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
           MPI_Send(slaveportion,portion,MPI_DOUBLE,myid-1,status.MPI_TAG,MPI_COMM_WORLD);
           rec_count++;           
       }     
   }
   if (myid==0)
   {    t2_p=MPI_Wtime();
	    printf("\nSorted array:\n");
        printarray(result, n, k);   
        printf("Parallel Elapsed time for this execution is: %f seconds \n", t2_p-t1_p);  
        free(tmp);
        free(result);       
        free (array);
        free (slaveportion);  
	}
    MPI_Finalize();
    return 0;
}

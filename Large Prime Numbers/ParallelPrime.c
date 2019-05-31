//Written By: Majid Salimi
// A program to find very larg prime numbers

//to compile  =====>    mpicc ParallelPrime.c -lm
//to run      =====>    mpirun -np procs a.out n 

//procs: number of processes
//n: the number, you want to find the primes between zero and n
//-----------------------------------------------------------------------------------------

#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include<stdbool.h>

int createArray(long *arr, long n,int Pid);
void printarray(long *arr,long size);
void multiplesFinder(long *SqrtArr,long *SlaveArr,int Pid,long sizeSqrtArr,long sizeSlaveArr);
int main(int argc,char * argv[]) {
    int SlaveNum;
    long *arr,Start,t,End,indexOfSqrt,Remaining=0,*SlaveArr,count,n,*SqrtArr,SqrtNum;
    int processor_num,Pid;
    double StartTime, EndTime;
    double cpu_time_used;
    MPI_Status status;
    if (argc < 2) {
        printf("\n\tOOOps, INVALID No of Arguements,\n"
               "\tProgram to generate an integer array[n] with random numbers from 1....range and sorting\n"
               "\tthem  using quick sort, printing array before and after sort k elements per line\n"
               "\texample: a.out  100 10 500.....\n\n");
        exit(1);
    }
    
    //initiate Matrices 
    // initialize MPI_Init
    int err = MPI_Init(&argc,&argv);
    if (err != MPI_SUCCESS){
        printf("\nError initializing MPI.\n");
        MPI_Abort(MPI_COMM_WORLD, err);
    } // end if
    StartTime = MPI_Wtime();
    // Get No. of processors
    MPI_Comm_size(MPI_COMM_WORLD, &processor_num);
    
    
    // Get processor id
    MPI_Comm_rank(MPI_COMM_WORLD, &Pid );
   
    n = atoi(argv[1]);
    count=(n/2)-1;
    t = count/ processor_num;
    SlaveArr=malloc(t*sizeof(long));
    arr = malloc(count*sizeof(long));

        if(Pid==0){
        createArray(arr, n,Pid);
        long i;
        double sqrtN=sqrt(n);
        for(i=0;i<count;i++){
            if(arr[i]>sqrtN){
                indexOfSqrt=i;
                break;
            }    
        }
        SqrtNum=arr[indexOfSqrt+1];
        printf("\nSqrtNum0:%ld \n",SqrtNum);
            
     if (argc < 2) {
            printf("\n\tOOOPS...., INVALID No of Arguements,\n");
        }
        printf("\ncount:%ld\n",count);
  
    }
    if (argc < 2) {
        MPI_Finalize();
        return 0;
    }
    StartTime = MPI_Wtime();
    MPI_Bcast(&SqrtNum,1,MPI_LONG,0,MPI_COMM_WORLD);
    MPI_Bcast(&indexOfSqrt,1,MPI_LONG,0,MPI_COMM_WORLD);
    SqrtArr=malloc((indexOfSqrt+1)*sizeof(long));
    if (Pid==0)
    {
    MPI_Bcast(&count, 1,MPI_LONG, 0, MPI_COMM_WORLD);   
    }
    //-----------------------------------------------------------------scatter
    MPI_Scatter(arr,t,MPI_LONG,SlaveArr,t,MPI_LONG,0,MPI_COMM_WORLD);
    printf("\nProcessor%d Recieved:\n",Pid);
    createArray(SqrtArr, SqrtNum,Pid);  
    Remaining=count % processor_num;//----------------------------------calculation
    multiplesFinder(SqrtArr, SlaveArr,Pid,indexOfSqrt+1,t);
    //------------------------------------------------------------------gather
    MPI_Gather(SlaveArr,t,MPI_LONG,arr,t,MPI_LONG,0,MPI_COMM_WORLD);      
    EndTime = MPI_Wtime();
    cpu_time_used = EndTime - StartTime;
    printf("\nTotal Time:%f\n",cpu_time_used);
   
    if(Pid==0)
    {
        multiplesFinder(SqrtArr, arr+(t*processor_num-1), Pid, indexOfSqrt+1, Remaining);
        //printf("\nArray:\n");
        //printarray(arr, count);
        long cnt=1,k;
        printf("\nPrime Numbers:\n");
        //printf("2 , ");
        for(k=0;k<count;k++)
            if(arr[k]!=0){
                cnt++;
                
                //printf("%ld , ",arr[k]);
            }
        
        printf("\nnumber of primes:%ld\n",cnt);
    }
    free (arr);
    free(SqrtArr);
    free(SlaveArr);
    MPI_Finalize();
    
    return 0;
}


int createArray(long *arr, long n,int Pid) {
    printf("\nProccesor%d CreateArray\n",Pid);
    long k;
    long count=1;
    arr[0]=3;
    for (k = 4; k < n; k++){
        if(k%2==0){
            continue;
        }
        arr[count] = k;
        count++;    
    }
    return count;
}


void printarray(long* arr,long size){
    long i,count=1;
    for(i=0;i<size;i++){
        printf("%ld\t",arr[i]);
        if(count%10==0 ){
            printf("\n");
        }
        count++;
    }
    printf("\n");
    
}
void multiplesFinder(long *SqrtArr,long *SlaveArr,int Pid,long sizeSqrtArr,long sizeSlaveArr){
    long i,j;
    for(i=0;i<sizeSlaveArr;i++){
        for(j=0;j<sizeSqrtArr;j++){
            if(SlaveArr[i]%SqrtArr[j]==0 && SlaveArr[i]!=SqrtArr[j] && SlaveArr[i]!=0){
                SlaveArr[i]=0;
                
            }
            
        }
    }
}





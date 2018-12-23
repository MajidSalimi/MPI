//Written by: Majid Salimi Beni

//A program to read input data from file and sort them with insertion sort
//to compile: gcc name.c -o name
//to execute: ./name filename k
//please enter filename without '.txt'
//======================================================================

#include <stdio.h> 		 
#include <malloc.h>
#include <time.h> 
#include <string.h>
#include <stdlib.h>
//----------------------------------------------------------------------print array
void printArray(double *arr, int n,int k) 
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
//----------------------------------------------------------------------insertion sort
void insertionSort(double *arr, int n)
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
//----------------------------------------------------------------------Main
int main(int argc, char **argv)
 {
	 char *filename;
     int i=0;
     int k,n;
     double rangei,rangej,*a;
     filename = argv[1]; 
     k        = atoi(argv[2]);  // get file name 
//----------------------------------------------------file reading
     strcat(filename,".txt");
     printf("%s\n",filename);
 	 FILE *file1;
 	 file1 = fopen(filename, "r");
 	 if( file1 == NULL)
 	   {
          printf("I couldn't open %s for writing.\n",filename);
          exit(0);
       }
     clock_t begin = clock();
     fscanf(file1,"%d ",&n);
     fscanf(file1,"%lf ", &rangei);
     fscanf(file1,"%lf ", &rangej);
     printf("n=%d a=%f b=%f\n",n,rangei,rangej);
     a= malloc(n * sizeof (double)); // allocate space
     for(i = 0; i < n; i++)
        {
            fscanf(file1,"%lf ", &a[i]); 
        }
     printf("\nbefore sorting data:\n");
     printArray(a,n,k);      
     //sort   
     insertionSort(a,n);
     printf("\nSorted input data:\n");
     printArray(a,n,k); 
     clock_t end = clock();
     double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
     printf("elapsed Time = %f\n",time_spent);
     fclose(file1);  
 }     
	 

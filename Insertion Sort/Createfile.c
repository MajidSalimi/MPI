//Written by: Majid Salimi Beni

//A program to create a txt file and save n double numbers into it
//to compile: gcc name.c -o name
//to execute: ./name n a b filename
//please enter filename without '.txt'

#include <stdio.h> 		 
#include <malloc.h>
#include <time.h> 
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv)
 {
	 char *filename;
     int i=0;
     int n;
     double rangei,rangej;
     n      = atoi(argv[1]);  // get n
     rangei = atoi(argv[2]);  // get range
     rangej = atoi(argv[3]);   // get range
     filename =argv[4];   // get file name
//-----------------------------------------file creating
     strcat(filename,".txt");
     printf("%s\n",filename);
 	 FILE *file1;
 	 file1 = fopen(filename, "w+");
 	 if( file1 == NULL)
 	   {
          printf("I couldn't open %s for writing.\n",filename);
          exit(0);
       }
     fprintf(file1,"%d ",n); 
     fprintf(file1,"%f ",rangei); 
     fprintf(file1,"%f ",rangej); 
//-----------------------------------------------------------writing to file
    for(i = 0; i < n; i++)
        {
             fprintf(file1,"%f ",(double)rand() / RAND_MAX * (rangej - rangei) + rangei);
         }   
//-------------------------------------------------------------reading from file
   // for(i = 0; i < n; i++)
     //   {
       //     fscanf(file1,"%f ", &a[i]); 
       // } 
    fclose(file1);  
 }     

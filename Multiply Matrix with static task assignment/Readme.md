## Multiply 2 Matrix in parallel with static task assignment

An efficient parallel program that accepts four integers m, n, p, k and two float (double) a and b as input. Then generates two float (double) matrices Am×n and Bn×p in the range of a...b (a < b).
Compute C = A × B, using np processors with static (cyclic) task assignments, where every k (k ≥ 1) rows of C is assigned to each processor.


np is not divisible by m. and three matrices A, B and C printed in a user friendly form.


This program runned for 900*900*900 Matrixes with same random double numbers between 1 to 100 that was saved in a file.

I runned it 5 times for sequential and 5 times for cyclic parallel mode with 10 processors.(k=10). This is the average execution time records for both modes:


Sequential Time:  13.318590 seconds

Parallel Time:  2.404052 seconds

TO COMPILE:	
    
      mpicc fname.c

TO RUN:

      mpirun -n pp a.out m n p k range1 range2
    
m,n,p:           are the matrix: A(m*n) , B(n*p)


k:               is the number of rows that we send to each processor


range1,2:        are the ranges of generated numbers (double)

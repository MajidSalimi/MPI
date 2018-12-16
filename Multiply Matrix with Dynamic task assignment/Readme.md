An efficient parallel program that accepts four integers m, n, p, k and two float (double) a and b as input. Then generates two float (double) matrices Am×n and Bn×p in the range of a...b (a < b). Compute C = A × B with Dynamic task assignments, where every k (k ≥ 1) rows of C is assigned to each processor, where every k (k ≥ 1) rows of C is assigned to each processor.

np is not divisible by m. and three matrices A, B and C printed in a user friendly form.

This program runned for 900900900 Matrixes with same random double numbers between 1 to 100 that was saved in a file.

I runned it 5 times for sequential and and 5 times for workpool parallel mode with 10 processors.(k=10). This is the average execution time records for both modes:

Sequential Time: 13.318590 seconds

parallel workpool Time:  3.121489 seconds

TO COMPILE:

    mpicc fname.c

TO RUN:

    mpirun -n pp a.out m n p k range1 range2

m,n,p: are the matrix: A(mn) , B(np)

k: is the number of rows that we send to each processor

range1,2: are the ranges of generated numbers (double)

## Insertion Sort

An efficient modular (using several subprograms) program that accepts a filename and two integers "p" and "k" as input and performs parallel insertion sort using "p" processors in a pipeline network. Initially master reads n (> 107) float numbers from file and stores them in an array. We assume "n" is divisible by "p".
and finally print input data, k elements per line before and after sorting in a user friendly form.
Before you run parallel or sequential code, You should run "Createfile.c" that produces input numbers for sorting. "Createfile.c" is a
program to create a txt file and save n double numbers into that file.


You can run this file using the following commands: (Change names based on the input)

      to compile: gcc name.c -o name
      to execute: ./name n a b filename

Then we can run the parallel or sequential code. To run parallel code:(enter filename without '.txt')

      to compile: mpicc name.c
      to execute: mpirun -np p a.out filename k

And to run the sequential code: (enter filename without '.txt')

      to compile: gcc name.c -o name
      to execute: ./name filename k

#### Results:
This programs runned for 100000 random double numbers between 1 to 100 that was saved in a file
I runned it 5 times for sequential and 5 times for parallel mode with 50 processors.(p=50)

This is the average execution time records for both modes:

sequential Time:  9.529495 seconds

parallel Time:  2.179933 seconds

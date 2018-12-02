An implementation of parallel Rank sort algorithm. Numbers are Generating randomely
    
    TO RUN:	mpirun -np p a.out n a b k
    p (integer): No.of Processors
    n (integer): No.of randomly generated data
    a (integer): range of generated numbers (start of bound)
    b (integer): range of generated numbers (end of bound)
    k (integer): for printing k element in a line




this program runned for 10000 same random numbers between 1 to 1000 that was saved in a file. I runned it 5 times for sequential and 5 times for parallel mode with 10 processors.

This is the average execution time records for both modes:

sequential Time:  0.6444338 seconds

parallel Time:  0.2728532 seconds

And the speedup(Tseq/Tpara) is: 2.36183396

   

An efficient parallel program to implement quick sort on hypercube using `p` processors:
Each processor initially generates `n` integers randomly in the range of `a` and `b`
and sorts them. So over all we want to sort `n×p` integers.

Assume `p`=2k where k ≥ 2.

to compile: 
      
      mpicc salimi_Quick.c -lm

to run:

      mpirun -np procs a.out n a b


`procs`: number of processes

`n`: total numbers

`a,b`: range of generated numbers

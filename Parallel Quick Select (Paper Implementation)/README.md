## Quick Select
#### Siebert Paper implementation

This algorithm is a new method for pivot selecting and partitioning for some algorithms like quick sort and quick select


      to compile:      mpicc Qselect.c
      to run:          mpirun -np procs a.out n range k

`procs`:  number of processores

`n`:      number of random numbers

`range`:  range of random generated numbers between 1 and range

`k`:      we want to find k'th smallest number  

***hint*** : in this paper, they assume: `n==p` 

***hint*** : `n` (and `p`) must be Multiples of 3

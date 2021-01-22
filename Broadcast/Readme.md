These MPI codes generate n numbers and store them in an array. Then partition the array among processors using a broadcast operation.
After finishing the local sum of each processor, all partial sums will return to the master node, using a reduction operation, for final aggregation.


## C version:

```
--	TO COMPILE:	mpicc bcast.c

--	TO RUN:		mpirun -n pp a.out yy zz

```

## C++ version:

```
--	TO COMPILE:	mpic++ bcast.cpp

--	TO RUN:		mpirun -n pp a.out yy zz

```


--	`pp` (integer): The number of Processors

--	`yy` (integer): The number of random input array elements to generate

--	`zz` (integer): The range of input array numbers
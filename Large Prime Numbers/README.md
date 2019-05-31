## Finding Prime Numbers using Parallel Prime Sieve

A program to find very larg prime numbers using Sieve of Eratosthenes with optimizations.
It starts looking for multiples of the prime at that prime squared and stops counting multiples once you've found all primes from 1 to the square root of the given number. 

### Algorithm:
```
Create a list of natural numbers 2, 3, 4, 5, ....., n. None of which is marked. Each process creates its share of lists. 
Set k to 2, the first unmarked number on the list. Each process does this. 
Repeat: Each process marks its share of list 
Mark all multiples of k between k2  and n. 
Find the smallest number greater than k that is unmarked. Set k to this new value 
Process 0 broadcasts k to rest of processes. 
Until k2 > n. 
The unmarked numbers are primes. 

```
### Optimations:
#### 1. Delete Even Integers:

Changing the sieve algorithm so that only odd integers are represented halves the amount of storage required 

doubles the speed at which multiples of particular prime are marked. 

#### 2. Eliminate Broadcasts

Suppose in addition to each task's set of about n/p integers, each task also has a separate array containing integers 3, 5, 7,…, n. Before finding primes 

Through n, each task will use the algorithm to find the primes from 3 through sqrt(n).

Each task has its own private copy of array containing all the primes between 3 and sqrt(n). 

Now the tasks can sieve their portions of the larger array without any broadcast steps. 

#### 3. Reorganize Loops

Think of the Do...While Loop (heart of the program), the outer loop iterates over prime sieve values between 3 and sqrt(n)

while the inner loop iterates over the processes share of the integers between 3 and n. 

If we interchange these loops we can improve the cache hit rate. 

### Running Tutorial

to compile: ```mpicc ParallelPrime.c -lm```

to run: ```mpirun -np procs a.out n``` 

`procs`: number of processes

`n`: the number, you want to find the primes between zero and n

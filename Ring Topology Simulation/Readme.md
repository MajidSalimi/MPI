## Simulating ring network

Master generates a token and passess it to first processor. Every processors adds 2 to the token and passes it to its right neighbor and finally master receives token from last processor.
    
To Compile:

            mpicc fname.c
    
To Run:
            
            mpirun -n pp a.out 
    

`pp` (integer):   No.of Processors

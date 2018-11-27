#include <mpi.h>
#include <math.h>
#include <iostream>
#include <cstdlib>
#include "stdio.h"
#define MIN(a,b) ((a) < (b)) ? (a) : (b))
#define BLOCK_LOW(id,p,n) ((id)*(n)/(p))
#define BLOCK_HIGH(id,p,n) \
        ( BLOCK_LOW((id)+1,p,n)-1)
#define BLOCK_SIZE(id,p,n) \ 
        (BLOCK_LOW( (id)+1,p,n) - BLOCK_LOW((id),p,n))
#define BLOCK_OWNER(index,p,n) \
        ((((p) * (index) + 1) - 1) / (n))
        

//Modify the parallel Sieve of Aratosthenes program in class so that the program does NOT set aside memory for even integers.
using namespace std;

int main(int argc, char*argv[])
{
    int id;
    long long int i;
    long long int index;
    long long int prime;
    long long int first;
    long long int count;
    long long int global_count;
    long long int n;
    int p; //static_cast<int>(sqrt(atoi(argv[1])));
    double elapsed_time;
    long long int size;
    long long int low_value;
    long long int high_value;
    char* marked;
    char* marked2;
    long long int proc0_size;
    MPI_Init (&argc, &argv); // initiaslizes anything if necessary
    MPI_Barrier(MPI_COMM_WORLD);
    elapsed_time = -MPI_Wtime();
    MPI_Comm_rank(MPI_COMM_WORLD, &id); // determines process rank
    MPI_Comm_size(MPI_COMM_WORLD, &p); //determins number of processes
   
    if (argc != 2)
    {
        if(!id)
        {
            printf("Command line: %s <m>\n", argv[0]);
        }
        MPI_Finalize(); 
        exit(1);
    }
    
      n = strtoll (argv[1], NULL, 0);
      n = n/2;
    low_value = 3 + (BLOCK_LOW(id, p, n-1))*2; //had + 2 before
    high_value = 3 + (BLOCK_HIGH(id,p,n-1))*2; //had + 2 before
    size = BLOCK_SIZE(id,p,n-1);
   // cout << "low_value (orig): " << low_value << endl;
   // cout << "high_value (orig): " << high_value << endl;
   // cout << "process id: " << id << endl;
   // cout << "size: " << size << endl;
    proc0_size = (n-1)/p;
    cout << "process 0 size is: " << proc0_size << endl;
        
    if((2 + proc0_size) < (int) sqrt((double) n))
    {
        if (!id) printf ("Too many processes\n");
        MPI_Finalize();
        exit(1);
    }

    marked = (char *) malloc (proc0_size);
    marked2 = (char *) malloc (size);
    if (marked == NULL)
    {
        cout << "Cannot allocate enough memory" << endl;
        MPI_Finalize();
        exit(1);
    }

    for (i = 0; i < proc0_size; i++)
    {
        marked[i] = 0;
    }
    for (i = 0; i < size; i++)
    {
        marked2[i] = 0;
    }
    index = 0;
    prime = 3; // prime used to be 2
    do 
    {
        if (prime*prime > 3)
        {
                first = (prime * prime - 3); //low-value wasn't properly indexed before
        } 
        else
        {
            if (!((3) % prime)) 
            {
                first = 0;
            }
           
            else
            {
                first = prime - ((3) % prime);
            }
        }
           
        for (i = first/2; i < proc0_size; i += prime) //added first - 2 (orig. i = first)
        {  
            if((3 + (i*2)) % prime == 0)
            {
            marked[i] = 1;
            }
            else
            {
                while(!((3 + (i*2)) % prime == 0))
                {
                        i+=1;
                }
                if(i < proc0_size)
                {
                   marked[i] = 1;
                }
            }
        }
            
            while (marked[++index]);
            prime = (index*2) + 3;
                   //    cout << "here is index: " << index << endl;
         //   cout << "prime: " << prime << endl;
     
    }   while (prime * prime <= n*2); //  used to be n
    // break between
    int tempCount = 0;
    tempCount += 1;
/*for(i = 0; i < proc0_size; i++)
    {
            if(!marked[i])
            {
                   tempCount +=1;
            }
    }*/
    prime = 3;
    index = 0;
    
    do 
    {
        if (prime*prime > low_value)
        {
                first = (prime * prime - low_value); //low-value wasn't properly indexed before
        } 
        else
        {
            if (!((low_value) % prime)) 
            {
                first = 0;
            }
           
            else
            {
                first = prime - ((low_value) % prime);
            }
        }
           
        for (i = first/2; i < size; i += prime) //added first - 2 (orig. i = first)
        {  
            if((low_value + (i*2)) % prime == 0)
            {
            marked2[i] = 1;
            }
            else
            {
                while(!((low_value + (i*2)) % prime == 0))
                {
                        i+=1;
                }
                if(i < size)
                {
                   marked2[i] = 1;
                }
            }
        }   
            while (marked[++index]);
            prime = (index*2) + 3;
     
    }   while (prime * prime <= n*2); //  used to be n

    count = 0;
    for (i = 0; i < size; i++)
    {
        if (!marked2[i])
        {
            count++;
        }
    }
        
//    cout << "For " << low_value << "-" << high_value << " there are " << count << " primes" << endl;


    MPI_Reduce (&count, &global_count, 1, MPI_INT, MPI_SUM,0,MPI_COMM_WORLD);
    elapsed_time += MPI_Wtime();
    if (!id)
    {
        cout << (global_count+1) << " primes are less than or equal to " << n*2 << endl;
        printf("Total elapsed time: %10.6f\n", elapsed_time);
    }
    MPI_Finalize();
    return 0;

}

//compile by doing mpiCC -o prog prog.cpp
//execution by mpiexec -v -np no_procs prog
//mpiexec -np 100 --hostfile hosts prog // runs 100 processes on 4 computers

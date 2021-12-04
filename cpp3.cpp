#include <time.h>
#include <iostream>
#include <pthread.h>
#include <stdlib.h>
using namespace std;

#define NUM_THREADS 1000
long count = 0; //counter to count primes
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; //mutex that will protecht count
//defining structure for args
struct range{
    long start;
    long end;
};


//defining prime check function
void* prime_check(void *_args)
{   
    struct range *args = (struct range*) _args;

    long start = args->start;
    long end = args->end;

    //set start as smallest prime 2
    if(start < 2) start=2;
    
    //prime check loop
    for(long i = start; i <= end; i++)
    {
        bool is_prime = true;
        for (long j=2;j*j<=i&&is_prime;j++)
        {
            if(i%j == 0) is_prime = false;
         }

    //count if prime
    if (is_prime)
    {
        pthread_mutex_lock(&mutex);
        count++;
        pthread_mutex_unlock(&mutex);
    }

    }
    pthread_exit (NULL);
}

int main(){
    double startTime = clock();
    pthread_t threads[NUM_THREADS];
    struct range *args[NUM_THREADS];
    int thread;
    
    //defining the range of search
    long start = 0;
    long n = 1000000000000;
    //allocating number for each thread
    long number_per_thread = n/NUM_THREADS;
    for (thread = 0; thread < NUM_THREADS; thread++){
        args[thread] = (range *)calloc(sizeof (struct range), 1);
        args[thread]->start = start + (thread * number_per_thread);
        args[thread]->end = n-(NUM_THREADS-thread-1)*number_per_thread;
        pthread_create (&threads[thread], NULL, prime_check,  args[thread]);
    }
    
    //join thread and free memory
	for (thread = 0; thread < NUM_THREADS; thread ++){
		pthread_join(threads[thread], NULL);
        free(args[thread]);
    }

    cout << "number of prime: " << count << endl;

    double endTime = clock();
    double elapsedTime = (double)(endTime - startTime);
    cout << "Time for the excution: " << elapsedTime << endl;
    return 0;
}


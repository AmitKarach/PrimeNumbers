#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>
#include <sys/sysinfo.h>
#include <semaphore.h>
#include <unistd.h>
#define MY_INT_MAX 2147483647
#define MY_INT_MIN -2147483648
// #define numOfCores get_nprocs_conf(); // # Of Cores

// Global Variables
long sum = 0, primeCounter = 0;
pthread_mutex_t lock;
int everyThreadPart;
int lastThreadPart;


int BetterIsPrime(int n)
{

	if (n < 2) // Possitive Natural Number Below 2 - True
	{
		return 0;
	}
	if (n ==2 || n==3 || n==5 || n==7)
	{
		return 1;
	}

	if ( n%2==0 || n%3==0 || n%5==0) // if the number is divided by 2 3 or 5 then he cant be a prime
	{
		return 0;
	}
	int lim = (int)sqrt((double)n);

	// Iterate from 3 to Sqrt[x] With Jumps of 6 because we cheacked for all the moltiplis of  2 3 and 5 (and 6 because of 2 and 3)
	for (int i = 7; i <= lim; i += 6)
	{
		if (n % i == 0 ||(n % (i + 4) == 0))
			return 0;
	}
	// Return True if yet Returned Value for n:(Prime Number)
	return 1;
}


int myAtoi(char *str)
{
	long result = 0, sign = 1;
	char *tptr = str;
	while (*tptr == ' ')
	{
		tptr++;
	}

	if (*tptr == '-' || *tptr == '+')
	{
		sign = 1 - 2 * (*tptr++ == '-');
	}

	if ((*tptr <= '0') && (*tptr >= '9'))
		return 0;

	for (int i = 0; (tptr[i] != '\0') && (tptr[i] != ' ') && ((tptr[i] >= '0') && (tptr[i] <= '9')); ++i)
	{
		if (result > MY_INT_MAX)
			break;
		result = result * 10 + tptr[i] - '0';
	}

	result = sign * result;
	if (result > MY_INT_MAX)
		return MY_INT_MAX;

	if (result < MY_INT_MIN)
		return MY_INT_MIN;

	return result;
}

void *ThreadsFunction()
{
	// Variables:
	int threadPrimeCounter = 0, random;
	long threadSum = 0;

	// What Thread Should Execute:
	for (int i = 0; i < everyThreadPart ; i++)
	{
		random = rand();
		if (BetterIsPrime(random))
		{
			threadSum += random;
			threadPrimeCounter++;
		}
	}
	// Update Global Sum and Counter:
	pthread_mutex_lock(&lock);
	sum += threadSum;
	primeCounter += threadPrimeCounter;
	pthread_mutex_unlock(&lock);
}

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		printf("Too few arguments ");
		printf("USAGE: ./primeCalc <prime pivot> <num of random numbers>");
		exit(0);
	}
	// Variables:
	int randomPivot = myAtoi(argv[1]), numOfRandomNumbers = myAtoi(argv[2]); // Extraction of Argument Data and Convertion
	long numOfCores = get_nprocs_conf();
    everyThreadPart = numOfRandomNumbers / numOfCores;					 //Value Part Of Thread
	int rest = numOfRandomNumbers % numOfCores;								 // Remainder Of The Value
	lastThreadPart = everyThreadPart + rest;									 // Sum of Both Values

	// Initiation
	pthread_t my_threads[numOfCores]; // Creation
	pthread_mutex_init(&lock, NULL);  // Mutex Initiation
	srand(randomPivot);				  // Feeding Seed Value

	// int *a = malloc(sizeof(*a));
	// if (a == NULL)
	// {
	// 	fprintf(stderr, "Thread Memory Alocation Error\n");
	// 	exit(EXIT_FAILURE);
	// }
	// Thread Creation
	for (int i = 0; i < numOfCores; i++)
	{
		if (i < numOfCores - 1)
		{
			// All Threads but Last - They Take the non-Remainer Value
			// *a = everyThreadPart;
			if (pthread_create(&my_threads[i], NULL, (void *)&ThreadsFunction, NULL) != 0)
				printf("\n Thread Creation Error :[%d]", i);
		}
		else
		{
			// Last Thread Shall Take his Value and the Remainder
			// *a = whatLeft;
			if (pthread_create(&my_threads[numOfCores - 1], NULL, (void *)&ThreadsFunction, NULL) != 0)
				printf("\n Thread Creation Error :[%ld]", numOfCores - 1);
		}
	}
	// Itterate Number of Cores and Command to Join
	for (short i = 0; i < numOfCores; i++)
	{
		pthread_join(my_threads[i], NULL);
	}
	// At End Destroy the Mutex and Print Data
	pthread_mutex_destroy(&lock);
	printf("%ld,%ld\n", sum, primeCounter);
	exit(0);
}

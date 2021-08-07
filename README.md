# PrimeNumbers

in this task we have been asked to beat our teacher time on finding counting and summing random prime numbers numbers
he showed us his time and tould us to beat it but we cant use more then 1 mb mamory (so we dont just save all the first 10000 prime numbers and comper to this)

what i have done is creat threads (as many as the number of cores) and gave each one part of the random numbers to search for the primes in his part
and to add it all up i us lock so there will be no overlap between threads when synchronizing

to run this program you need to rum make first
then run ./solution 6 1000000
when 6 is the seed number so we could comper the results with the teacher and 100000 is the number of random numbers we will cheack 

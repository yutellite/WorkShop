```c
#include<stdio.h>
#include<pthread.h>
#include<time.h>
#include<sys/time.h>

//gcc -lpthread -o counter_without_lock counter_without_lock.c

#define NUM_THREADS 20
#define NUM_INCREMENTS 100000000

int counter;
pthread_mutex_t mutex;
pthread_mutexattr_t attr;

/*
<1013 linux6 [ywx] :/onip/ywx/c_program>./counter_without_lock 
Final value of counter is:1715423
Final execute time is:0 (s)
Final execute time is:135880 (us)
*/
#if 0
void *add_things(void *threadid)
{
    int i = 0;
    for(i=0; i<NUM_INCREMENTS; i++)
        counter++;    
    pthread_exit(NULL);
}
#endif
/*
<1012 linux6 [ywx] :/onip/ywx/c_program>./counter_with_mutex 
Final value of counter is:20000000
Final execute time is:0 (s)
Final execute time is:72582 (us)

<1028 linux6 [ywx] :/onip/ywx/c_program>./counter_with_atomits
Final value of counter is:200000000
Final execute time is:1 (s)
Final execute time is:725005 (us)
*/
#if 0
void *add_things(void *threadid)
{
    int i = 0;
    pthread_mutex_lock(&mutex);
    for(i=0; i<NUM_INCREMENTS; i++)
        counter++;    
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}
#endif
/*
<1016 linux6 [ywx] :/onip/ywx/c_program>./counter_with_mutex
Final value of counter is:20000000
Final execute time is:3 (s)
Final execute time is:3000559 (us)

<1025 linux6 [ywx] :/onip/ywx/c_program>./counter_with_atomits
Final value of counter is:200000000
Final execute time is:33 (s)
Final execute time is:32894127 (us)

<1031 linux6 [ywx] :/onip/ywx/c_program>gcc -lpthread -o counter_with_atomits counter_with_atomits.c ; ./counter_with_atomits
counter_with_atomits.c: In function 'main':
counter_with_atomits.c:129: warning: incompatible implicit declaration of built-in function 'exit'
Final value of counter is:2000000000
Final execute time is:327 (s)
Final execute time is:326351639 (us)
*/
#if 1
void *add_things(void *threadid)
{
    int i = 0;
    for(i=0; i<NUM_INCREMENTS; i++)
    {        
        pthread_mutex_lock(&mutex);
        counter++;    
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}
#endif
/*
<1021 linux6 [ywx] :/onip/ywx/c_program>./counter_with_atomits 
Final value of counter is:20000000
Final execute time is:1 (s)
Final execute time is:967393 (us)

<1023 linux6 [ywx] :/onip/ywx/c_program>./counter_with_atomits
Final value of counter is:200000000
Final execute time is:9 (s)
Final execute time is:9233837 (us)

counter_with_atomits.c:124: warning: incompatible implicit declaration of built-in function 'exit'
Final value of counter is:2000000000
Final execute time is:87 (s)
Final execute time is:86732504 (us)
*/
#if 0
void *add_things(void *threadid)
{
    int i = 0;
    for(i=0; i<NUM_INCREMENTS; i++)
    {        
        __sync_add_and_fetch(&counter, 1);
    }
    pthread_exit(NULL);
}
#endif
int main(int argc, char **argv)
{
    int ret=0;
    long t;
    //time.h seconds
    time_t begin_time;
    //sys/time.h useconds
    struct timeval b_tv;
    struct timeval e_tv;
    pthread_t threads[NUM_THREADS];
        
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
    if(0!=pthread_mutex_init(&mutex, &attr))
    {
        printf("Init pthread mutex failed.\n");
        return -1;
    }
    begin_time=time(NULL);
    gettimeofday(&b_tv, NULL);
    for(t=0; t<NUM_THREADS; t++)
    {
        ret = pthread_create(&threads[t], NULL, add_things, (void *)t);
        if(0!=ret)
        {
            printf("Err: create thread %d failed. ret is: %d.\n", t, ret);
            exit(1);
        }
    }
    
    //wait for threads to finish
    for(t=0; t<NUM_THREADS; t++)
        pthread_join(threads[t], NULL);
    gettimeofday(&e_tv, NULL);    
    printf("Final value of counter is:%d\n", counter);
    printf("Final execute time is:%d (s)\n", time(NULL)-begin_time);
    printf("Final execute time is:%d (us)\n", e_tv.tv_sec*1000*1000+e_tv.tv_usec-b_tv.tv_sec*1000*1000-b_tv.tv_usec);
    pthread_exit(NULL);
}
```

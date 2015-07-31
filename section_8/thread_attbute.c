#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>


pthread_once_t once = PTHREAD_ONCE_INIT;

void run(void)
{
    printf("function run is running in thread %u\n", (unsigned)pthread_self());
}

void* thread1(void *arg)
{
    pthread_t thid = pthread_self();
    printf("current thread's ID is %u\n", (unsigned)thid);
    pthread_once(&once, run);
    printf("thread1, ends\n");
}

void* thread2(void* arg)
{
    pthread_attr_t * s1;
    int de;
    s1 = (pthread_attr_t *)malloc(sizeof(pthread_attr_t));
    pthread_attr_init(s1);
    pthread_t thid = pthread_self();
    printf("current thread's ID is %u\n", (unsigned)thid);
    pthread_once(&once, run);
    pthread_attr_getdetachstate(s1, &de);
    //printf("%d\n", s1->schedpolicy);
    printf("%d\n", de);
    printf("thread2, ends\n");
}

int main()
{
    pthread_t thid1, thid2;

    pthread_create(&thid1, NULL, thread1, NULL);
    pthread_create(&thid2, NULL, thread2, NULL);
    sleep(3);
    printf("main thread exit!\n");
    exit(0);
}

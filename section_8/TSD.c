/*************************************************************************
	> File Name: TSD.c
	> Author: zhuxinquan
	> Mail: zhuxinquan61@gmail.com
	> Created Time: 2015年08月01日 星期六 17时54分29秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

pthread_key_t key;

void * thread2(void *arg)
{
    int tsd = 5;
    printf("thread2 %d is running\n", (int)pthread_self());
    pthread_setspecific(key, (void *) &tsd);
    printf("thread %d return %d\n", (int)(pthread_self()), *(int *)pthread_getspecific(key));
}

void *thread1(void *arg)
{
    int tsd = 0;
    pthread_t thid2;

    printf("thread %u is running\n", (unsigned)pthread_self());
    pthread_setspecific(key, (void *)&tsd);
    pthread_create(&thid2, NULL, thread2, NULL);
    sleep(5);
    printf("thread %d returns %d\n", (int)pthread_self(), *(int *)pthread_getspecific(key));
}

int main(void)
{
    pthread_t thid1;
    printf("main thread begins running\n");
    pthread_key_create(&key, NULL);
    pthread_create(&thid1, NULL, thread1, NULL);
    //sleep(1);
    pthread_join(thid1, NULL);
    pthread_key_delete(key);
    printf("main thread exit\n"); return 0;
}

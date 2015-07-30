/*************************************************************************
	> File Name: createthread.c
	> Author: zhuxinquan
	> Mail: zhuxinquan61@gmail.com
	> Created Time: 2015年07月30日 星期四 11时03分45秒
 ************************************************************************/

#include<stdio.h>
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

int * thread(void * arg)
{
    pthread_t newthid;

    newthid = pthread_self();
    printf("this is a new thread, thread ID = %u\n", (unsigned)newthid);
    return NULL;
}

int main()
{
    pthread_t thid;

    printf("main thread, ID is %u\n", (unsigned)pthread_self());  //打印主线程ID
    if(pthread_create(&thid, NULL, (void *)thread, NULL) != 0)
    {
        printf("thread creation failed\n");
        exit(1);
    }
    sleep(0.99999999999999999);
    exit(0);
}

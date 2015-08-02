/*************************************************************************
	> File Name: mutex_test.c
	> Author: zhuxinquan
	> Mail: zhuxinquan61@gmail.com
	> Created Time: 2015年08月02日 星期日 11时04分51秒
 ************************************************************************/

#include<stdio.h>
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

static pthread_mutex_t testlock;
pthread_t test_thread;

void *test()
{
    pthread_mutex_lock(&testlock);
    printf("thread Test() \n");
    pthread_mutex_unlock(&testlock);

}

int main()
{
    pthread_mutex_init(&testlock, NULL);
    pthread_mutex_lock(&testlock); 

    printf("Main lock \n");

    pthread_create(&test_thread, NULL, test, NULL);
    sleep(1);   //更加明显的观察到是否执行了创建线程的互斥锁
    printf("Main unlock \n");
    pthread_mutex_unlock(&testlock); 

    sleep(1);

    pthread_join(test_thread,NULL); 
    pthread_mutex_destroy(&testlock); 
    return 0;

}

/*************************************************************************
	> File Name: many_thread_program.c
	> Author: zhuxinquan
	> Mail: zhuxinquan61@gmail.com
	> Created Time: 2015年08月03日 星期一 21时26分23秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<errno.h>


int a = 0;      //用作要改变的全局变量
pthread_mutex_t mutex;  //互斥锁变量

void thread1(void *arg)
{
    //pthread_mutex_lock (&mutex);
    printf("thread1 a = %d\n", a);
    a++;
    //pthread_mutex_unlock (&mutex);
}


void thread2(void *arg)
{
    //pthread_mutex_lock (&mutex);
    printf("thread2 a = %d\n", a);
    a++;
    //pthread_mutex_unlock (&mutex);
}

void thread3(void *arg)
{
    //pthread_mutex_lock (&mutex);
    printf("thread3 a = %d\n", a);
    a++;
    //pthread_mutex_unlock (&mutex);
}

int main(void)
{
    pthread_t tid1, tid2, tid3;   //用于接收线程ID
    int       err;                //出错码
    void *    tret;               //返回值

    pthread_create (&tid1, NULL, (void *)thread1, NULL);
    pthread_create (&tid2, NULL, (void *)thread2, NULL);
    pthread_create (&tid3, NULL, (void *)thread3, NULL);

    err = pthread_join(tid1, &tret);
    if(err != 0)
    {
        perror("join");
    }
    err = pthread_join(tid2, &tret);
    if(err != 0)
    {
        perror("join");
    }
    err = pthread_join(tid3, &tret);
    if(err != 0)
    {
        perror("join");
    }

    exit(0);
}


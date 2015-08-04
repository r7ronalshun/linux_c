/*************************************************************************
	> File Name: error_test.c
	> Author: zhuxinquan
	> Mail: zhuxinquan61@gmail.com
	> Created Time: 2015年08月04日 星期二 08时41分41秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

//测试error一键多值，全局的error在不同的线程中依旧代表不同的值，典型的一键多值
void thread1(void * arg)
{
    FILE *fp;
    
    if(fopen("test", "r") == NULL)
    {
        perror("open");
    }
    pthread_exit(0);
}

void thread2(void * arg)
{
    perror("thread_error");
    pthread_exit(0);
}

int main(void)
{
    pthread_t tid1, tid2;
    
    pthread_create(&tid1, NULL, (void *)thread1, NULL);
    pthread_create(&tid2, NULL, (void *)thread2, NULL);

    sleep(10);
    pthread_exit(0);
}

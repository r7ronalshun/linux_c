/*************************************************************************
	> File Name: mutex_global.c
	> Author: zhuxinquan
	> Mail: zhuxinquan61@gmail.com
	> Created Time: 2015年08月02日 星期日 13时39分55秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>


pthread_mutex_t number_mutex;
int             globalnumber = 5;

void write_globalnumber()
{
    printf("1:%d\n", globalnumber);
    pthread_mutex_lock(&number_mutex);
    globalnumber++;
    pthread_mutex_unlock(&number_mutex);
    printf("2:%d\n", globalnumber);
}

int read_globalnumber()
{
    int temp;
    pthread_t thid;
    pthread_mutex_init(&number_mutex, NULL);
    printf("3:%d\n", globalnumber);
    pthread_mutex_lock(&number_mutex);
    temp = globalnumber;
    pthread_mutex_unlock(&number_mutex);
    pthread_create(&thid, NULL, (void *)write_globalnumber, NULL);
    pthread_join(thid, NULL);
    printf("4:%d\n", globalnumber);
    return temp;
}

int main(void)
{
    int  temp;
    pthread_t thid;
    if(pthread_create(&thid, NULL, (void *)read_globalnumber, NULL) != 0)
    {
        printf("thread create failed!\n");
        exit(1);
    }
    sleep(1);
    pthread_join(thid, (void *)&temp);
}

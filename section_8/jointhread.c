/*************************************************************************
	> File Name: jointhread.c
	> Author: zhuxinquan
	> Mail: zhuxinquan61@gmail.com
	> Created Time: 2015年07月31日 星期五 10时52分07秒
 ************************************************************************/

#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

void assisthread(void * arg)
{
    printf("I'm help to do something!\n");
    sleep(3);
    pthread_exit(0);
}

int main(void)
{
    pthread_t assistthid;
    int status;

    printf("%d\n", status);
    pthread_create(&assistthid, NULL, (void *) assisthread, NULL);
//    pthread_join(assistthid, (void *)&status);
    sleep(5);
    printf("\n\n");
    printf("assistthid's exit is caused %d\n", status);
    //sleep(5);

    return 0;
}

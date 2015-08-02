/*************************************************************************
	> File Name: print_thread_id.c
	> Author: zhuxinquan
	> Mail: zhuxinquan61@gmail.com
	> Created Time: 2015年08月02日 星期日 15时50分02秒
 ************************************************************************/

#include<stdio.h>
#include<stdio.h>
#include<unistd.h>
#include<apue.h>
#include<pthread.h>

pthread_t ntid;
void printids(const char *s)
{
    pid_t     pid;
    pthread_t tid;

    pid = getpid();
    tid = pthread_self();
    printf("%s pid %u tid %u (0x%x)\n", s, (unsigned int)pid, (unsigned int)tid, (unsigned int)tid);
}

void *thr_fn(void *arg)
{
    printids("new thread: ");
    return ((void *)0);
}

int main(void)
{
    int err;

    err = pthread_create(&ntid, NULL, (void *)thr_fn, NULL);
    if(err != 0)
    {
        err_quit("can't create thread :%s\n", strerror(err));
    }
    printids("main thread :");
    sleep(1);
    exit(0);
}

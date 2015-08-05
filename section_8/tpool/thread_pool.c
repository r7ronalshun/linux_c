/*************************************************************************
	> File Name: thread_pool.c
	> Author: zhuxinquan
	> Mail: zhuxinquan61@gmail.com
	> Created Time: 2015年08月05日 星期三 08时57分39秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include"tpool.h"

static tpool_t * tpool = NULL;

static void * thread_routine(void * arg)
{
    tpool_work_t *work;
    
    while(1)
    {
        pthread_mutex_lock(&tpool->queue_lock);             //若线程池没有被销毁并且没有任务执行则等待
        while(!tpool->queue_head && tpool->shutdown)        //shutdown：销毁标志， queue_head:若为空则代表没有任务
        {
            pthread_cond_wait(&tpool->queue_ready, &tpool->queue_lock);
        }
        if(tpool->shutdown)
        {
            pthread_mutex_unlock(&tpool->queue_lock);
            pthread_exit(NULL);
        }
        
        work = tpool->queue_head;
        tpool->queue_head = tpool->queue_head->next;
        pthread_mutex_unlock(&tpool->queue_lock);

        work->routine(work->arg);
        free(work);
    }
    return NULL;
}


int tpool_create(int max_thr_num)           //创建线程池
{
    int i;
    
    tpool = calloc(1, sizeof(tpool_t));
    if(!tpool)
    {
        perror("calloc");
        exit(1);
    }
    
    tpool->max_thr_num = max_thr_num;       //初始化
    tpool->shutdown = 0;
    tpool->queue_head = NULL;
    if(pthread_mutex_init(&tpool->queue_lock, NULL) != 0)
    {
        perror("pthread_mutex_init");
        exit(1);
    }
    
    tpool->thr_id = calloc(max_thr_num, sizeof(pthread_t));
    if( !tpool->thr_id )
    {
        perror("calloc failed");
        exit(1);
    }
    for( i = 0; i < max_thr_num; ++i )
    {
        if(pthread_create(&tpool->thr_id[i], NULL, (void *)&thread_routine, NULL) != 0)
        {
            perror("pthread_create failed");
            exit(1);
        }
    }
    return 0;
}

void tpool_destroy()
{
    int i;
    tpool_work_t *member;
    
    if(tpool->shutdown)
    {
        return ;
    }
    tpool->shutdown = 1;

    pthread_mutex_lock(&tpool->queue_lock);
    pthread_cond_broadcast(&tpool->queue_ready);    //通知所有正在等待的线程
    pthread_mutex_unlock(&tpool->queue_lock);
    for(i = 0; i < tpool->max_thr_num; ++i)
    {
        pthread_join(tpool->thr_id[i], NULL);
    }
    free(tpool->thr_id);

    while(tpool->queue_head)
    {
        member = tpool->queue_head;
        tpool->queue_head =  tpool->queue_head->next;
        free(member);
    }

    pthread_mutex_destroy(&tpool->queue_lock);
    pthread_cond_destroy(&tpool->queue_ready);

    free(tpool);
}

int tpool_add_work(void * (*routine)(void *), void * arg)       //向线程池添加任务
{
    tpool_work_t *work, *member;

    if(!routine)
    {
        perror("invalid argument");
        return -1;
    }

    work = malloc(sizeof(tpool_work_t));
    if(!work)
    {
        perror("malloc failed");
        return -1;
    }
    work->routine = routine;
    work->arg = arg;
    work->next = NULL;

    pthread_mutex_lock(&tpool->queue_lock);
    member = tpool->queue_head;
    if(!member)
    {
        tpool->queue_head = work;
    }
    else
    {
        while(member->next)
        {
            member = member->next;
        }
        member->next = work;
    }

    pthread_cond_signal(&tpool->queue_ready);
    pthread_mutex_unlock(&tpool->queue_lock);

    return 0;
}


void * fun(void *arg)
{
    int int_arg;
    int_arg = *(int *)&arg;
    printf("thread %d\n", int_arg);
    return NULL;
}

int main(int argc, char ** argv)
{
    if(tpool_create(5) != 0) 
    {
        printf("tpool_create failed\n");
        exit(1);
    }

    int i;
    for( i = 0; i < 10; ++i )
    {
        tpool_add_work(fun, (void *)&i);
    }

    sleep(2);
    tpool_destroy();
    return 0;
    
}

/*************************************************************************
	> File Name: /home/zhuxinquan/linux_c/section_8/tpool.h
	> Author: zhuxinquan
	> Mail: zhuxinquan61@gmail.com
	> Created Time: 2015年08月05日 星期三 09时02分40秒
 ************************************************************************/

#ifndef _TPOOL_H
#define _TPOOL_H

#include<pthread.h>
typedef struct tpool_work           //要执行的任务结构体
{
    void * (*routine)(void *);      //任务函数
    void * arg;                     //传入任务的参数
    struct tpool_work * next;
}tpool_work_t;

typedef struct tpool                //线程结构体，保存线程状态
{
    int shutdown;                   //线程池的销毁状态
    int max_thr_num;                //最大的线程数量
    pthread_t *thr_id;              //线程id数组，动态分配，保存所有线程的id
    tpool_work_t *queue_head;       //线程链表的头指针
    pthread_mutex_t queue_lock;     //互斥锁
    pthread_cond_t queue_ready;     //条件变量
}tpool_t;

int tpool_create(int max_thr_num);  //线程池创建函数

void tpool_destory();               //线程池销毁函数

int tpool_add_work(void *(*routine)(void *), void * arg);   
/*任务添加函数，
routine为任务函数指针，
arg为任务函数参数，
return 0：success，other:failed*/

#endif

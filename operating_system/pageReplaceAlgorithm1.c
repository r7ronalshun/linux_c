/*************************************************************************
> File Name: pageReplaceAlgorithm1.c
> Author: zhuxinquan
> Mail: zhuxinquan61@xiyoulinux.org
> Created Time: 2016年05月10日 星期二 11时45分10秒
************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0
#define INVALID -1
#define NUL 0

#define total_instruction 320  /*指令流长*/
#define total_vp 32          /*虚页长*/
#define clear_period 50     /*清零周期*/


typedef struct{            /*页面结构*/
    int pn,pfn,counter,time;
}pl_type;

pl_type pl[total_vp];  /*页面结构数组*/

struct pfc_struct{      /*页面控制结构*/
    int pn,pfn;
    struct pfc_struct *next;
};

typedef struct pfc_struct pfc_type;
pfc_type pfc[total_vp],*freepf_head,*busypf_head,*busypf_tail;
int diseffect,a[total_instruction];
int page[total_instruction],  offset[total_instruction];
void initialize();
void FIFO();
void LRU();
void OPT();
void LFU();
void NUR();

int main()
{
    int S,i;
    srand((int)getpid());

    S=(int)rand()%390;

    for(i=0;i<total_instruction;i+=1)          /*产生指令队列*/
    {
        a[i]=S;                               /*任选一指令访问点*/
        a[i+1]=a[i]+1;                         /*顺序执行一条指令*/
        a[i+2]=(int)rand()%390;          /*执行前地址指令m’*/
        a[i+3]=a[i+2]+1;                         /*执行后地址指令*/
        S=(int)rand()%390;
    }
    for(i=0;i<total_instruction;i++)               /*将指令序列变换成页地址流*/
    {       
        page[i]=a[i]/10;
        offset[i]=a[i]%10;
    }
    for(i=4;i<=32;i++)                        /*用户内存工作区从4个页面到32个页面*/
    {
        printf("%2d page frames",i);
        FIFO(i);
        LRU(i);
        OPT(i);
        LFU(i);
        NUR(i);
        printf("\n");
    }
    return 0;
}

void FIFO(total_pf)                        /*FIFO(First in First out)ALGORITHM*/
int total_pf;                               /*用户进程的内存页面数*/
{
    int i;
    pfc_type *p, *t;
    initialize(total_pf);                      /*初始化相关页面控制用数据结构*/
    busypf_head=busypf_tail=NUL;            /*忙页面队列头，对列尾链接*/
    for(i=0;i<total_instruction;i++)
    {
        if(pl[page[i]].pfn==INVALID) /*页面失效*/
        {
            diseffect+=1;        /*失效次数*/
            if(freepf_head==NUL) /*无空闲页面*/
            {
                p=busypf_head->next;
                pl[busypf_head->pn].pfn=INVALID; /*释放忙页面队列中的第一个页面*/
                freepf_head=busypf_head;
                freepf_head->next=NUL;
                busypf_head=p;
            }

            p=freepf_head->next; /*按方式调新页面入内存页面*/
            freepf_head->next=NUL;
            freepf_head->pn=page[i];
            pl[page[i]].pfn=freepf_head->pfn;
            if(busypf_tail==NUL)
            busypf_head=busypf_tail=freepf_head;
            else
            {
                busypf_tail->next=freepf_head;
                busypf_tail=freepf_head;
            }
            freepf_head=p;
        }
    }
    printf("FIFO:%6.4F",1-(float)diseffect/320);
}

void LRU(total_pf)
int total_pf;
{
    int min,minj,i,j,present_time;

    initialize(total_pf);present_time=0;
    for(i=0;i<total_instruction;i++)
    { 
        if(pl[page[i]].pfn==INVALID)     /*页面失效*/
        { 
            diseffect++;
            if(freepf_head==NUL) /*无空闲页面*/
            {
                min=32767;
                for(j=0;j<total_vp;j++)
                if(min>pl[j].time&&pl[j].pfn!=INVALID)
                {
                    min=pl[j].time;
                    minj=j;
                }
                freepf_head=&pfc[pl[minj].pfn];
                pl[minj].pfn=INVALID;
                pl[minj].time=-1;
                freepf_head->next=NUL;
            }
            pl[page[i]].pfn=freepf_head->pfn;
            pl[page[i]].time=present_time;
            freepf_head=freepf_head->next;
        }
        else
        pl[page[i]].time=present_time;
        present_time++;
    }
    printf("LRU:%6.4f",1-(float)diseffect/320);
}


void NUR(total_pf)
int total_pf;
{
    int i,j,dp,cont_flag,old_dp;
    pfc_type *t;

    initialize(total_pf);
    dp=0;
    for(i=0;i<total_instruction;i++)
    {
        if(pl[page[i]].pfn==INVALID) /*页面失效*/
        {
            diseffect++;
            if(freepf_head==NUL) /*无空闲页面*/
            {
                cont_flag=TRUE;old_dp=dp;
                while(cont_flag)
                if(pl[dp].counter==0&&pl[dp].pfn!=INVALID)
                cont_flag=FALSE;
                else
                {
                    dp++;
                    if(dp==total_vp)						
                    dp=0;
                    if(dp==old_dp)
                    for(j=0;j<total_vp;j++)
                    pl[j].counter=0;
                }
                freepf_head=&pfc[pl[dp].pfn];
                pl[dp].pfn=INVALID;
                freepf_head->next=NUL;
            }
            pl[page[i]].pfn=freepf_head->pfn;
            freepf_head=freepf_head->next;
        }
        else
        pl[page[i]].counter=1;
        if(i%clear_period==0)
        for(j=0;j<total_vp;j++)
        pl[j].counter=0;
    }
    printf("NUR:%6.4f",1-(float)diseffect/320);
}

void OPT(total_pf) /*OPT(Optimal Replacement)ALGORITHM*/
int total_pf;
{
    int i,j,max,maxpage,d,dist[total_vp];
    pfc_type *t;
    initialize(total_pf);
    for(i=0;i<total_instruction;i++)
    {
        if(pl[page[i]].pfn==INVALID)
        {
            diseffect++;
            if(freepf_head==NUL)
            {
                for(j=0;j<total_vp;j++)
                if(pl[j].pfn!=INVALID)
                dist[j]=32767;
                else 
                dist[j]=0;
                d=1;

                for(j=i+1;j<total_instruction;j++)
                {
                    if(pl[page[j]].pfn!=INVALID)
                    dist[page[j]]=d;
                    d++;
                }
                max=-1;

                for(j=0;j<total_vp;j++)
                if(max<dist[j])
                {
                    max=dist[j];maxpage=j;}
                freepf_head=&pfc[pl[maxpage].pfn];
                freepf_head->next=NUL;
                pl[maxpage].pfn=INVALID;
            }
            pl[page[i]].pfn=freepf_head->pfn;
            freepf_head=freepf_head->next;
        }
    }
    printf("OPT:%6.4f",1-(float)diseffect/320);
}

void LFU(total_pf)                      /*LFU(leat Frequently Used) ALGORITHM*/
int total_pf;
{
    int i,j,min,minpage;
    pfc_type *t;

    initialize(total_pf);
    for(i=0;i<total_instruction;i++)
    {
        if (pl[page[i]].pfn==INVALID)
        {
            diseffect++;
            if(freepf_head==NUL)
            {
                min=32767;
                for(j=0;j<total_vp;j++)
                {
                    if(min>pl[j].counter&&pl[j].pfn!=INVALID)
                    { 
                        min=pl[j].counter; minpage=j;
                    }
                    pl[j].counter=0;
                }
                freepf_head=&pfc[pl[minpage].pfn];
                pl[minpage].pfn=INVALID;
                freepf_head->next=NUL;
            }
            pl[page[i]].pfn=freepf_head->pfn;
            freepf_head=freepf_head->next;
        }
        else
        pl[page[i]].counter++;
    }
    printf("LFU:%6.4f",1-(float)diseffect/320);

}
void  initialize(total_pf)                /*初始化相关数据结构*/
int total_pf;                         /*用户进程的内存页面数*/
{
    int i;
    diseffect=0;

    for(i=0;i<total_vp;i++)
    {
        pl[i].pn=i;pl[i].pfn=INVALID;          /*置页面控制结构中的页号，页面为空*/
        pl[i].counter=0;pl[i].time=-1;        /*页面控制结构中的访问次数为0，时间为-1*/
    }

    for(i=1;i<total_pf;i++)
    {
        pfc[i-1].next=&pfc[i];pfc[i-1].pfn=i-1;/*建立pfc[i-1]和pfc[i]之间的连接*/
    }
    pfc[total_pf-1].next=NUL;pfc[total_pf-1].pfn=total_pf-1;
    freepf_head=&pfc[0];    /*页面队列的头指针为pfc[0]*/
}

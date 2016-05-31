/*************************************************************************
> File Name: mutilprocess.c
> Author: zhuxinquan
> Mail: zhuxinquan61@xiyoulinux.org
> Created Time: 2016年04月19日 星期二 10时21分30秒
************************************************************************/

#include<stdio.h>
#include <stdlib.h>
#define MAXPRI 100

#define NIL -1
#define True 1
#define False 0
static int s1=0,s2=0;
typedef struct messagetp *message;
struct messagetp{
    int num;
    int pro;
    message next;
};

//进程控制块
struct procb{
    int id;             //进程标识数，id = 0， 1， 2
    int nextwr;         //等待链指针,指示在同一信号量上等待的下一个进程的标识数
    int priority;       //进程优先级
    int inum;
    char status;        //进程状态
    char addr;
    message mess;
};

//信号量(互斥)
struct semaphorel{
    int value;          //信号量，初值为1
    int firstwr;        //等待链首指针,指示在同一信号量上等待的下一个进程的标识数
};

struct procb pcb[3];
struct semaphorel sem[2];
char addr;
int i,seed,exe;             //exe为当前执行进程的指针


/*int style;*/
void send(int sender,int receiver,int snum)
{
    message q,p;
    p=(message)malloc(sizeof(struct messagetp));
    p->num=snum;
    p->next=NULL;
    p->pro=sender;
    printf("send message to process %d\n",receiver);
    printf("process %d already run %d times\n",sender,snum);
    q=pcb[receiver].mess;
    if(q==NULL) pcb[receiver].mess=p;
    else {
        while(q->next!=NULL)
        q=q->next;
        q->next=p; 
    }
}
void receive(int receiver)
{ 
    message p;
    p=pcb[receiver].mess;
    if (p!=NULL){
        while (p!=NULL){
            printf("receive message from process %d\n",p->pro);
            printf("process %d is already run %d time\n",p->pro,p->num);
            printf("\n");
            p=p->next;
        }
        pcb[receiver].mess=NULL; 
    }
}
void init()
{ 
    int j;
    for(j=0;j<=2;j++){ 
        /*initiate pcb and sem,current process is set to -1*/
        pcb[j].id=j;
        pcb[j].status='r';
        pcb[j].nextwr=NIL;
        printf("process %d priority?",j);
        scanf("%d",&i);
        pcb[j].priority=i;
        pcb[j].mess=NULL;
        pcb[j].inum=0;
        pcb[j].addr='0';
    }
    sem[0].value=1;
    sem[0].firstwr=NIL;
    sem[1].value=1;
    sem[1].firstwr=NIL;
    exe=NIL;
}

float random1()
{ 
    int m;
    if(seed<0) m=-seed;
    else m=seed;
    seed=(25173*seed+13849)%65536;
    return (m/32767.0);
}
int find()
{ 
    int j,pd,w;
    pd=NIL;
    w=MAXPRI;
    for(j=0;j<=2;j++)
    if (pcb[j].status=='r')
    if (pcb[j].priority<w){
        w=pcb[j].priority;
        pd=j; 
    }
    if (pd==NIL)
    for(j=0;j<=2;j++)
    if (pcb[j].status=='t')
    if(pcb[j].priority<w){
        w=pcb[j].priority;
        pd=j; 
    }
    return pd;
}

//调度程序
int scheduler()
{ 
    int pd;
    pd=find();
    if ((pd==NIL)&&(exe==NIL)) return NIL;
    else{
        if (pd!=NIL){
            if(exe==NIL) { 
                /*no current is executing*/
                pcb[pd].status='e';
                exe=pd;
                printf("process %d is executing\n",exe);
            }
            else if (pcb[pd].priority<pcb[exe].priority){
                pcb[exe].status='r';
                printf("process %d enter into ready\n",exe);
                pcb[pd].status='e';
                exe=pd;
                printf("proecess %d is executing\n",exe);
            }
        } /*if*/
        i=pcb[exe].inum;
        addr=pcb[exe].addr;
        return exe;
    }
}
void block(int se)
{
    int w;
    printf("process %d is blocked\n",exe);
    pcb[exe].status='w';
    pcb[exe].nextwr=NIL;
    w=sem[se].firstwr;
    if (w==NIL) {
        sem[se].firstwr=exe;
    }
    else {
        while (pcb[w].nextwr!=NIL)
        w=pcb[w].nextwr;
        pcb[w].nextwr=exe; 
    }

}
int p(int se,char ad)
{ 
    sem[se].value=sem[se].value-1;
    if(sem[se].value>=0) return False;
    else {
        block(se);
        pcb[exe].inum=i;
        pcb[exe].addr=ad;
        exe=NIL;
        return True;
    }
}
void wakeup(int se)
{ 
    int w;
    w=sem[se].firstwr;
    if (w!=NIL){
        sem[se].firstwr=pcb[w].nextwr;
        pcb[w].status='r';
        printf("process %d is wakeup\n",w);
    }
}
int v(int se,char ad)
{
    sem[se].value=sem[se].value+1;
    if (sem[se].value>0) return False;
    else{
        wakeup(se);
        pcb[exe].inum=i;
        pcb[exe].addr=ad;
        return True;
    }

}
int timeint(char ad)
{ 
    float x;
    x=random();
    if((x<0.33)&&(exe==0)) return False;
    else if((x<0.66)&&(exe==1)) return False;
    else if((x<1.00)&&(exe==2)) return False;
    else {
        pcb[exe].inum=i;
        pcb[exe].addr=ad;
        pcb[exe].status='t';
        printf("time slice interrupt.\n");
        printf("process %d enter into ready\n",exe);
        exe=NIL;
        return True;
    }
}
void eexit(int n)
{
    pcb[n].status='c';
    printf("process %d is completed\n",n);
    exe=NIL;

}
void process0()
{
    if(addr=='a') goto a0;
    if(addr=='b') goto b0;
    if(addr=='c') goto c0;
    if(addr=='d') goto d0;
    if(addr=='e') goto e0;
    if(addr=='f') goto f0;
    while(i<5) {
        receive(0);
        printf("process 0 calls P on semaphore0.\n");
        /*do p operation to sem[0],if no critical resoure,process block at address 'a'*/
        if(p(0,'a')) goto stop0;
        a0: printf("process 0 is executing on its cretical section0.\n");
        if (timeint('b')) goto stop0;
        b0: printf("seed=%d\n",seed);
        s1=s1+1;
        printf("s1=%d",s1);
        printf("process 0 calls V on semaphore 0 and quit critical section 0.\n");
        if(v(0,'c')) goto stop0;
        c0: printf("process 0 calls P on semaphore1.\n");
        if(p(1,'d')) goto stop0;
        d0: printf("process 0 is excecuting section1.\n");
        if(timeint('e')) goto stop0;
        e0: printf("seed=%d\n",seed);
        s2=s2+1;
        printf("s2=%d",s2);
        printf("pcocess 0 calls V on semaphore1 and quit cretical section1.\n");
        if(v(1,'f')) goto stop0;
        f0: printf("process 0 cyclen count=%d\n",i+1);
        i=i+1;
        send(0,1,i); send(0,2,i);
        printf("\n");

    }
    stop0:
    if(i<5) goto end0;
    eexit(0);
    end0:;

}
void process1()
{
    if(addr=='a') goto a1;
    if(addr=='b') goto b1;
    if(addr=='c') goto c1;
    if(addr=='d') goto d1;
    if(addr=='e') goto e1;
    if(addr=='f') goto f1;
    while(i<5) {
        receive(1);
        printf("process1 calls P on semaphore1.\n");
        if(p(1,'a')) goto stop1;
        a1: printf("process1 is executing on its cretical section1.\n");
        if (timeint('b')) goto stop1;
        b1: s2=s2+1;
        printf("s2=%d",s2);
        printf("process 1 calls V on semaphore1 and quit cretical section1.\n");
        if(v(1,'c')) goto stop1;
        c1: printf("process 1 calls P on semaphore0.\n");
        if(p(0,'d')) goto stop1;
        d1: printf("process 1 is excecuting section0.\n");
        if(timeint('e')) goto stop1;
        e1: s1=s1+1;
        printf("s1=%d",s1);
        printf("pcocess 1 calls V on semaphore0 and quit section0.\n");
        if(v(0,'f')) goto stop1;
        f1: printf("process 1 cyclen count=%d\n",i+1);
        i=i+1;
        send(1,0,i); send(1,2,i);
        printf("\n");

    }
    stop1:
    if(i<5) goto end1;
    eexit(1);
    end1:;

}
void process2()
{
    if(addr=='a') goto a2;
    if(addr=='b') goto b2;
    if(addr=='c') goto c2;
    while(i<5) {
        receive(2);
        printf("process 2 calls P on semaphore 1.\n");
        if(p(2,'a')) goto stop2;
        a2: printf("process 2 is executing on its cretical section 1.\n");
        if (timeint('b')) goto stop2;
        b2: s2=s2+1;
        printf("s2=%d",s2);
        printf("process 2 calls V on semaphore2 and quit cretical section 0.\n");
        if(v(2,'c')) goto stop2;
        c2: printf("process 2 cyclen count=%d\n",i+1);
        i=i+1;
        send(2,0,i); send(2,1,i);
        printf("\n");

    }
    stop2:
    if(i<5) goto end2;
    eexit(2);
    end2:;

}
int main()
{
    int k;
    seed=0;
    printf("C.O.S Example one");
    printf("\n");
    init();
    printf("s1=%d,s2=%d\n",s1,s2);
    printf("process1,process2,and process3 are all ready!\n");
    lable:
    k=scheduler();
    if(k!=NIL)
    switch (k)
    {
        case 0: process0();
        goto lable;
        case 1: printf("s1=%d,s2=%d\n",s1,s2);
        process1();
        goto lable;
        case 2: printf("s1=%d,s2=%d\n",s1,s2);
        process2();
        goto lable;
        default: printf("process identifer error.\n");
    }
    printf("\n");
    printf("\n");
    printf("s1=%d,s2=%d",s1,s2);
    printf("\n");
    printf("COMPLETED!\n");
    getchar();
    return 0;
}

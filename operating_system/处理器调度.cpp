//时间片轮转法处理机调度
#include <iostream>
#include <cstdlib>
using namespace std;

typedef struct PNode { // PCB
   struct PNode *next; // 定义指向下一个节点的指针
   char name[10];    // 定义进程名，并分配空间
   int All_Time;    // 定义总运行时间
   int Runed_Time;    // 定义已运行时间
   char state;        // 定义进程状态 Ready / End
}* Proc; // 指向该PCB的指针

int ProcNum; // 总进程个数

// 初始化就绪队列
void InitPCB(Proc &H) { 
    cout<<"请输入总进程个数: ";
    cin>>ProcNum; // 进程总个数
    int Num=ProcNum;
    H=(Proc)malloc(sizeof(PNode)); // 建立头节点    
    H->next=NULL;
    Proc p=H;  //定义一个指针
    cout<<"总进程个数为 "<<ProcNum<<" 个,请依次输入相应信息\n\n";
    
    while (Num--) {
        p=p->next=(Proc)malloc(sizeof(PNode));   
        cout<<"进程名 总运行时间 已运行时间 :";
        cin>>p->name>>p->All_Time>>p->Runed_Time;  
        p->state='R';
        p->next=NULL;
    }
    p->next=H->next;  
}

//输出运行中的进程信息
void DispInfo(Proc H) { 
    Proc p=H->next;
    do {
        if (p->state != 'E')   //如果该进程的状态不是End的话
        {
            cout<<"进程名:"<<p->name<<"\t总运行时间:"<<p->All_Time
                <<"\t已运行时间:"<<p->Runed_Time
                <<"\t状态:"<<p->state<<endl;
            p=p->next;
        }
        else p=p->next;
    } while (p != H->next); // 整个进程链条始终完整，只是状态位有差异
}

// 时间片轮转法
void SJP_Simulator(Proc &H) { 
    cout<<endl<<"-------------------START--------------------\n";
    int flag=ProcNum; // 记录剩余进程数
    int round=0; // 记录轮转数
    Proc p=H->next;
    while (p->All_Time > p->Runed_Time) {  // 即未结束的进程
            round++;   
            cout<<endl<<"Round "<<round<<"--正在运行 "<<p->name<<" 进程"<<endl;
            p->Runed_Time++;   // 更改正在运行的进程的已运行时间
            DispInfo(H);      // 输出此时为就绪状态的进程的信息
            if (p->All_Time == p->Runed_Time) {   // 并判断该进程是否结束
                p->state='E';  
                flag--;
                cout<<p->name<<" 进程已运行结束,进程被删除!\n";
            }
    p=p->next;
    while (flag && p->All_Time == p->Runed_Time)
        p=p->next; // 跳过先前已结束的进程

    }
    cout<<endl<<"--------------------END---------------------\n";
}


int  main() {
    Proc H;
    InitPCB(H); // 数据初始化
    DispInfo(H); // 输出此刻的进程状态
    SJP_Simulator(H); // 时间片轮转法
    system("pause");
    return 0;
}

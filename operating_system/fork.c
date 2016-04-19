/*************************************************************************
	> File Name: fork.c
	> Author: zhuxinquan
	> Mail: zhuxinquan61@xiyoulinux.org
	> Created Time: 2016年04月12日 星期二 10时06分32秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>

int main(int argc, char* argv[]){
    int p1, p2;
    //fork含有两个返回值，父进程返回子进程pid，子进程返回0
    /*switch(p1 = fork()){
        case 0:
        printf("当前为子进程\n");
        break;
        case -1:
        printf("进程创建失败\n");
        break;
        default:
        printf("当前为父进程\n");
        break;
    }*/
    
    if(p1 = fork()){
        //父进程创建成功返回非0值，子进程创建成功
        //printf("%d", p1);
        putchar('b');
    }else{
        if(p2 = fork()){
            putchar('c');
        }else{
            putchar('a');
        }
    }
}

/*************************************************************************
> File Name: who_list.c
> Author: zhuxinquan 
> Mail: zhuxinquan61@xiyoulinux.org 
> Created Time: 2016年11月21日 星期一 11时13分03秒
************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/wait.h>

int main(int argc,char *argv[]){
    int pid,fd;
    int fds[2];
    if(pipe(fds)==-1){
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    if(fork()==0){
        sleep(1);
        char buf[128];
        dup2(fds[0],0);
        close(fds[1]);
        fd=creat("list.txt",0644);
        dup2(fd,1);
        execlp("sort","sort",(char *)0);
    }
    else
    {
        if(fork()==0){
            dup2(fds[1],1);
            close(fds[0]);
            execlp("who","who",NULL);    
        }
        else{
            close(fds[0]);
            close(fds[1]);
            close(fd);
            wait(NULL);
            wait(NULL);
        }
    }
    return 0;
}

/*************************************************************************
> File Name: pipe.c
> Author: zhuxinquan
> Mail: zhuxinquan61@xiyoulinux.org
> Created Time: 2016年11月21日 星期一 11时23分15秒
************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/wait.h>

int main(int argc, char * argv[]){
    int pid = -1;
    int fd = -1;
    char buf[10];
    int pfd[2];

    if(pipe(pfd)==-1){
        perror("pipe");
        exit(1);    
    }


    //mkfifo("pipe", 0644);
    if((pid = fork()) > 0){
        int stat;
        //fd = open("pipe", O_WRONLY);
        dup2(pfd[1], 1);
        execlp("ls", "ls", "-l", NULL);
        //close(fd);
        wait(&stat);
    }else if(pid == 0){
//        fd = open("pipe", O_RDONLY);
        dup2(pfd[0], 0);
        printf("more:\n");
        execlp("more", "more", NULL);
//        close(fd);
    }else{
        perror("FORK ERROR");
    }
    return 0;
}

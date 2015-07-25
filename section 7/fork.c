/*************************************************************************
	> File Name: fork.c
	> Author: zhuxinquan
	> Mail: zhuxinquan61@gmail.com
	> Created Time: 2015年07月25日 星期六 09时21分26秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>


int main(void)
{
    pid_t pid;
    
    printf("process creation study\n");
    pid = fork();
    switch(pid)
    {
        case 0:
        printf("child is run ,current pid is %d, parent pid is %d\n", pid, getppid());
        break;

        case -1:
        perror("process creat failed:");
        break;

        default:
        printf("parent process is run, childpid is %d, parent pid is %d\n", pid, getpid());
        break;
    }

    exit(0);
}

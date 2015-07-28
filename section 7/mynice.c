/*************************************************************************
	> File Name: mynice.c
	> Author: zhuxinquan
	> Mail: zhuxinquan61@gmail.com
	> Created Time: 2015年07月28日 星期二 09时04分40秒
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/resource.h>
#include<sys/wait.h>
#include<stdlib.h>

int main(void)
{
    pid_t pid;
    int stat_val = 0;
    int oldpri, newpri;

    printf("nice study!\n");

    pid = fork();
    switch(pid)
    {
        case 0:
        printf("child is running, curpid is %d, parentpid is %d\n", pid, getppid());
        oldpri = getpriority(PRIO_PROCESS, 0);
        printf("old priority = %d\n", oldpri);
        newpri = nice(2);
        printf("new priority = %d\n", newpri);
        exit(0);

        case -1:
        perror("process creation failed!\n");
        break;

        default:
        printf("parent is rnning, childpid is %d, parentpid is %d\n", pid, getpid());
        break;
    }

    wait(&stat_val);
    exit(0);
}

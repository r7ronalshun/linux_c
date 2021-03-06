/*************************************************************************
	> File Name: execve.c
	> Author: 
	> Mail: 
	> Created Time: 2015年06月17日 星期三 17时43分53秒
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
int main(int argc, char **argv, char **environ)
{
    pid_t pid;
    int stat_val;
    printf("Exec example!\n");
    pid = fork();
    switch(pid)
    {
    case -1:
        perror("process creation failed");
        exit(1);
    case 0:
        printf("child process is running!\n");
        printf("My pid = %d,parentpid = %d\n",getpid(), getppid());
        printf("uid = %d, gid = %d\n",getuid(), getgid());
        execve("processimage", argv, environ);
        printf("process never go to here!\n");
        exit(0);
    default:
        printf("parent process is running");
        break;
    }
    wait(&stat_val);
    exit(0);
}

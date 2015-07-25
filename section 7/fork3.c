/*************************************************************************
	> File Name: fork3.c
	> Author: zhuxinquan
	> Mail: zhuxinquan61@gmail.com
	> Created Time: 2015年07月25日 星期六 10时46分03秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>

int main(void)
{
    pid_t pid;

    pid = fork();
    switch(pid)
    {
        case 0:
        {
            while(1)
            {
                printf("child process is run, pid is %d, parent id is %d.\n", getpid(), getppid());
                sleep(3);
            }
            break;
        }
        case -1:
        {
            perror("process creat failed:");
            exit(-1);
            break;
        }
        default:
        {
            printf("parent process is run!");
            exit(0);
        }
    }
    return 0;
}

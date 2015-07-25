/*************************************************************************
	> File Name: test_orphan_process.c
	> Author: zhuxinquan
	> Mail: zhuxinquan61@gmail.com
	> Created Time: 2015年07月25日 星期六 13时31分24秒
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>

int main(void)
{
    pid_t pid;
    int i = 0;
    pid = fork();

    switch(pid)
    {
        case 0:
        {
            while(i++ < 2)  
            {
                printf("child id is %d, parent id is %d\n", getpid(), getppid());
                getchar();
                sleep(3);
            }
            break;
        }
        case -1:
        {
            perror("creation process failed:");
            exit(0);
            break;
        }
        default:
        {
            while(i++ < 5)
            { 
                printf("I'm parent process, my pid is %d\n", getpid());
                getchar();
                sleep(2);
            }
        }
    }
    return 0;
}

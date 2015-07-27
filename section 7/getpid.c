/*************************************************************************
	> File Name: getpid.c
	> Author: zhuxinquan
	> Mail: zhuxinquan61@gmail.com
	> Created Time: 2015年07月27日 星期一 21时11分03秒
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>

int main(void)
{
    pid_t pid;

    if((pid = fork()) == -1)
    {
        printf("fork error!\n");
        exit(1);
    }

    if(pid == 0)
    {
        printf("getpid return %d\n", getpid());
    }
    exit(0);
}

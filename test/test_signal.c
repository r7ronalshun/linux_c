/*************************************************************************
	> File Name: test_signal.c
	> Author: zhuxinquan
	> Mail: zhuxinquan61@gmail.com
	> Created Time: 2015年07月27日 星期一 15时08分48秒
 ************************************************************************/

#include<stdio.h>
#include<signal.h>
int main(int argc, char **argv)
{
    signal(SIGINT, SIG_IGN);
    for(;;);
    return 0;
}

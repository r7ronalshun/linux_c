/*************************************************************************
	> File Name: array_test.c
	> Author: zhuxinquan
	> Mail: zhuxinquan61@gmail.com
	> Created Time: 2015年07月28日 星期二 21时25分44秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>

int main()
{
    char **arg;
    int i;

    //申请二维字符数组的内存空间
    arg = (char **)malloc(100*sizeof(char *));
    for(i = 0; i < 100; i++)    
    {
        arg[i] = (char *)malloc(256 * sizeof(char ));
    }

    for(i = 0; i < 100; i++)
    {
        arg[i] = "hello";
    }
    for(i = 0; i < 10; i++)
    {
        puts(arg[i]);
    }
}

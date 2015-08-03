/*************************************************************************
	> File Name: checkerrno.c
	> Author: zhuxinquan
	> Mail: zhuxinquan61@gmail.com
	> Created Time: 2015年08月03日 星期一 21时02分37秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>

int main(void)
{
    FILE   *stream;
    char   *filename = "test";

    errno = 0;  //初始化错误码

    stream = fopen (filename, "r");
    if(stream == NULL){
        printf ("open file %s failed, errno is %d: %s\n", filename, errno, strerror(errno));
    }
    else {
        printf ("open file %s successfully\n", filename);
    }
}

/*************************************************************************
	> File Name: my_perror.c
	> Author: zhuxinquan
	> Mail: zhuxinquan61@xiyoulinux.org
	> Created Time: 2016年09月18日 星期日 09时35分30秒
 ************************************************************************/

#include<stdio.h>
#include<errno.h>
#include<string.h>
#include<stdlib.h>

void my_perror( char * str){
    printf("%s:%s\n", str, strerror(errno));
    exit(-1);
}

int main(int argc, char ** argv){
    errno = E2BIG;
    my_perror(argv[0]);
    return 0;
}

/*************************************************************************
	> File Name: t1.c
	> Author: zhuxinquan
	> Mail: zhuxinquan61@xiyoulinux.org
	> Created Time: 2016年07月20日 星期三 17时30分18秒
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>

void my_err(const char * err_string, int line){
    fprintf(stderr, "line : %d", line);
    perror(err_string);
    exit(1);
}

int main(void){
    int fd;
    char buf[32];

    if((fd = open("temp", O_RDWR|O_CREAT|O_TRUNC)) < 0){
        my_err("open", __LINE__);
    }

    getchar();
    if(write(fd, "temp", 5) < 0)
    {
        my_err("write", __LINE__);

    }
    if((lseek(fd, 0, SEEK_SET)) == -1)
    {
        my_err("lseek", __LINE__);

    }
    if(read(fd, buf, 5) < 0)
    {
        my_err("read", __LINE__);

    }
    printf("%s\n", buf);
    getchar();

    return 0;
}

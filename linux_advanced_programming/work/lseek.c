/*************************************************************************
	> File Name: lseek.c
	> Author: zhuxinquan
	> Mail: zhuxinquan61@xiyoulinux.org
	> Created Time: 2016年09月28日 星期三 20时37分44秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

int main(int argc, char * argv[]){
    int fd;
    int len;
    struct stat buf;
    if((fd = open("test.txt", O_RDWR)) < 0){
        perror("open");
    }
    if(lseek(fd, 0, SEEK_END) == -1){
        perror("lseek");
    }
    if((len = lseek(fd, 0, SEEK_CUR)) == -1){
        perror("lseek");
    }

    printf("源文件长度：%d Byte\n", len);
    ftruncate(fd, len / 2);
    close(fd);
    stat("test.txt", &buf);
    printf("截断后文件大小为：%lu\n", buf.st_size);
    return 0;
}

/*************************************************************************
> File Name: cat.c
> Author: zhuxinquan
> Mail: zhuxinquan61@xiyoulinux.org
> Created Time: 2016年09月28日 星期三 10时32分06秒
************************************************************************/

#include<stdio.h>
#include <stdlib.h>  
#include <unistd.h>  
#include <fcntl.h>  

#define BUFFERSIZE 4096  

int main(int argc, char *argv[])  
{  
    int fd, fd_tty, n_chars;  
    char buf[BUFFERSIZE];  

    if (argc != 2)  
    {     
        fprintf(stderr, "Usage: %s filename\n", *argv);  
        exit(1);  
    }     

    if ((fd = open(argv[1], O_RDONLY)) == -1)   
    {     
        perror("Open error!");  
        exit(1);  
    }     

    /* /dev/tty 表示输入终端 */  
    /*if (fd_tty = open("/dev/tty", O_RDWR) == -1)   
    {     
        perror("Open error!");  
        exit(1);  
    }*/     

    while ((n_chars = read(fd, buf, BUFFERSIZE)) > 0)  
    {     
        if (write(1, buf, n_chars) != n_chars)  
        {     
            perror("Write error!");  
            exit(1);  
        }     
    }     

    if (close(fd) == -1)   
    perror("Error closing files!");  

    return 0;  
}  

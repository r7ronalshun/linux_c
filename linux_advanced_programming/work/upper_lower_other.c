/*************************************************************************
	> File Name: upper_lower_other.c
	> Author: zhuxinquan
	> Mail: zhuxinquan61@xiyoulinux.org
	> Created Time: 2016年09月28日 星期三 10时56分37秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>


int main(int argc, char * argv[]){
    
    int fds, fdup, fdlow, fdoth;
    char buf;

    if((fds = open("temp.txt", O_RDONLY)) == -1){
        perror("open:");
    }
    if((fdup = open("upper.txt", O_RDWR|O_CREAT|O_TRUNC, 0644)) == -1){
        perror("open:");
    }
    if((fdlow = open("lower.txt", O_RDWR|O_CREAT|O_TRUNC, 0644)) == -1){
        perror("open:");
    }
    if((fdoth = open("other.txt", O_RDWR|O_CREAT|O_TRUNC, 0644)) == -1){
        perror("open:");
    }

    while(read(fds, &buf, sizeof(buf)) != 0){
        if(buf >= 'A' && buf <= 'Z'){
            if(write(fdup, &buf, sizeof(buf)) == -1){
                perror("write:");
            }
        }else if(buf >= 'a' && buf <= 'z'){
            if(write(fdlow, &buf, sizeof(buf)) == -1){
                perror("write:");
            }
        }else{
            if(write(fdoth, &buf, sizeof(buf)) == -1){
                perror("write:");
            }
        }
    }
    
    return 0;
}

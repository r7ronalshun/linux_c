/*************************************************************************
	> File Name: rm.c
	> Author: zhuxinquan
	> Mail: zhuxinquan61@xiyoulinux.org
	> Created Time: 2016年09月28日 星期三 10时37分31秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main(int argc, char * argv[]){
    if(argc != 2){
        printf("please use: \'command filename\'");
        exit(EXIT_FAILURE);
    }

    if(unlink(argv[1]) < 0){
        printf("%s\n", argv[1]);
        perror("unlink:");
    }

    return 0;
}

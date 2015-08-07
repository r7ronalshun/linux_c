/*************************************************************************
	> File Name: getsockopt.c
	> Author: zhuxinquan
	> Mail: zhuxinquan61@gmail.com
	> Created Time: 2015年08月07日 星期五 09时46分03秒
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
int main(void)
{
    int s;
    int optval;
    int optlen = sizeof(int);
    if((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    perror("socket");
    getsockopt(s, SOL_SOCKET, SO_TYPE, &optval, &optlen);
    printf("optval = %d\n", optval);
    close(s);
}

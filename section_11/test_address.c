/*************************************************************************
	> File Name: test_address.c
	> Author: zhuxinquan
	> Mail: zhuxinquan61@gmail.com
	> Created Time: 2015年08月05日 星期三 15时25分14秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int main(void)
{
    char        buffer[32];
    int         ret = 0;
    int         host = 0;
    int         network = 0;
    unsigned int address = 0;
    struct in_addr in;

    in.s_addr = 0;

    printf("input your ip address:");           //输入一个以.分隔的ip地址
    fgets(buffer, 31, stdin);
    buffer[31] = '\0';

    if((ret = inet_aton(buffer, &in)) == 0)             //inet_aton
    {
        printf("inet_aton :\t invalid address\n");
    }
    else
    {
        printf("inet_aton: \t0x%x\n", in.s_addr);
    }

    if((address = inet_addr(buffer)) == INADDR_NONE)    //inet_addr
    {
        printf("inet_addr: \t invalid address\n");
    }
    else 
    {
        printf("inet_addr:\t 0x%x\n", address);
    }
    
    if((address = inet_network(buffer)) == -1)          //inet_network
    {
        printf("inet_network:\t invalid address\n");
    }
    else 
    {
        printf("inet_network:\t0x%x\n", address);
    }

    if(inet_ntoa(in) == NULL)                           //inet_ntoa
    {
        printf("inet_ntoa:\t invalid address\n");
    }
    else 
    {
        printf("inet_ntoa: \t%s\n", inet_ntoa(in));
    }

    host = inet_lnaof(in);
    network = inet_netof(in);
    printf("inet+lnaof:\t0x%x\n", host);
    printf("inet_netof:\t0x%x\n", network);

    in = inet_makeaddr(network, host);
    printf("inet_makeaddr:0x%x\n", in.s_addr);

    return 0;
}

/*************************************************************************
	> File Name: my_client.c
	> Author: zhuxinquan
	> Mail: zhuxinquan61@gmail.com
	> Created Time: 2015年08月06日 星期四 22时24分20秒
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include "my_recv.h"

#define INVALID_USERINFO    'n'     //用户信息无效
#define VALID_USERINFO      'y'     //用户信息有效

int get_userinfo(char *buf, int len)                //获取用户输入存入buf，长度len，以'\n'结束
{
    int i;
    int c;

    if(buf == NULL)
    {
        return -1;
    }

    i = 0;
    while(((c = getchar()) != '\n') && (c != EOF) && (i < len - 2))
    {
        buf[i++] = c;
    }
    buf[i++] = '\n';
    buf[i++] = '\0';

    return 0;
}


void input_userinfo(int conn_fd, const char *string)                //输入用户名，然后通过fd发送出去
{
    char input_buf[32];
    char recv_buf[BUFSIZE];
    int  flag_userinfo;

    do
    {                                                             //输入用户信息知道正确为止 
        printf("%s:", string);
        if(get_userinfo(input_buf, 32) < 0)
        {
            printf("error return from get_userinfo\n");
            exit(1);
        }
        
        if(send(conn_fd, input_buf,strlen(input_buf), 0) < 0)
        {
            my_err("send", __LINE__);
        }
        
        if(my_recv(conn_fd, recv_buf, sizeof(recv_buf)) < 0)
        {
            printf("data is too long\n");
            exit(1);
        }
        
        if(recv_buf[0] == VALID_USERINFO)
        {
            flag_userinfo = VALID_USERINFO; 
        }
        else
        {
            printf("%s error, input again,", string);
            flag_userinfo = INVALID_USERINFO;
        }
    }while(flag_userinfo == INVALID_USERINFO);
}

int main(int argc, char ** argv)
{
    int                  i;
    int                  ret;
    int                  conn_fd;
    int                  serv_port;
    struct sockaddr_in   serv_addr;
    char                 recv_buf[BUFSIZE];

    if(argc != 5)                                   //检查参数个数
    {
        printf("usage : [-p] [serv_port] [-a] [serv_address]\n");
        exit(1);
    }

    memset(&serv_addr, 0, sizeof(struct sockaddr_in));        //初始化服务器端地址结构
    serv_addr.sin_family = AF_INET;
    for(i = 1; i < argc; i++)                                   //从命令行获取服务器的端口与地址
    {
        if(strcmp("-p", argv[i]) == 0)
        {
            serv_port = atoi(argv[i+1]);
            if(serv_port < 0 || serv_port > 65535)
            {
                printf("invalid serv_addr.sin_port\n");
                exit(1);
            }
            else 
            {
                serv_addr.sin_port = htons(serv_port);
            }
            continue;
        }
        if(strcmp("-a", argv[i]) == 0)
        {
            if(inet_aton(argv[i+1], &serv_addr.sin_addr) == 0)
            {
                printf("invalid server ip address\n");
                exit(1);
            }
            continue;
        }
    }
    
    if(serv_addr.sin_port == 0 || serv_addr.sin_addr.s_addr == 0)           //检测是否少输入了某项参数
    {
        printf("usage: [-p] [serv_addr.sin_port] [-a] [serv_address]\n");
        exit(1);
    }

    conn_fd = socket(AF_INET, SOCK_STREAM, 0);                              //创建一个tcp套接字
    if(conn_fd < 0){
        my_err("socket", __LINE__);
    }

    if(connect(conn_fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) < 0){    //向服务器端发送连接请求
        my_err("connect", __LINE__);
    }
    
    input_userinfo(conn_fd, "username");                    //输入用户名和密码
    input_userinfo(conn_fd, "password");

    if((ret = my_recv(conn_fd, recv_buf, sizeof(recv_buf))) < 0){   //读取欢迎信息并打印
        printf("data is too long\n");
        exit(1);
    }
    for(i = 0; i < ret; i++){
        printf("%c", recv_buf[i]);
    }
    printf("\n");

    close(conn_fd);
    return 0;
    
}

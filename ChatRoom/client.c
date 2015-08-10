/*************************************************************************
	> File Name: client.c
	> Author: zhuxinquan
	> Mail: zhuxinquan61@gmail.com
	> Created Time: 2015年08月08日 星期六 17时15分06秒
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdlib.h>
#include "inputkey_getch.h"

#define BUFSIZE 1024 

void cls(void);                         //清除输入缓存函数
void my_err(const char *, int);         //错误处理函数
void print_menu(void);                  //打印主界面菜单函数
void quit(void);                        //退出函数
int register_num();                    //注册函数


struct info                             //个人信息
{
    char    username[10];
};

struct group                            //群组信息
{
    char            group_name[20];
    char            admin_name[10];
    struct info     member[10];
};

struct users                            //用户链表结构体
{
    char            password[10];
    struct info     user;
    struct info     friends[10];        //好友
    struct group    group[5];
    struct users*   next;
};

/*在客户端只有一个用户在使用
 *所以可以将所有的用户的好友
 *数、群组数等设置为全局变量
 */
int                 conn_fd;
int                 friend_num;
int                 group_num;
int                 member_num;
char                password[10];
struct info         user;
struct info         friend[10];
struct info         member[10];
struct info         group[3];


struct chat                             //聊天消息结构体
{
    int     flag;
    char    from[10];
    char    to[10];
    char    ask[10];
    char    time[30];
    char    news[500];
};

int             message_num  = 0;               //消息数量
struct chat     message[100];                   //消息队列


void my_err(const char * err_string, int line)
{
    fprintf(stderr, "line:%d,", line);
    perror(err_string);
    exit(1);
}

void cls()
{
    char c;
    do{
        c = fgetc(stdin);
    }while(c != 10 && c != EOF);
}

int main(int argc, char ** argv)
{
    int                  i;
    int                  ret;
    int                  serv_port;
    struct sockaddr_in   serv_addr;

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
            printf("%s\n", argv[i+1]);
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
            printf("%s\n", argv[i+1]);
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

    print_menu();                                                           //打印主菜单

}


void print_menu(void)
{
    char c;                                             //接收选项
    while(1)
    {
        system("clear");
        printf("\n");
        printf("\n\n\n");
        printf("\t\t            1.注册\n\n");
        printf("\t\t            2.登陆\n\n");
        printf("\t\t            3.退出\n\n");
        printf("\t\t----------------------------\n\n");
        printf("\t\t         请选择<1 ~ 3>：");
        c= getchar();
        if(c == 1)
        {
            if(register_num())                  //注册返回值1,注册成功
            {
                break;
            }
        }
        else if(c == 2)
        {
            //登陆函数
        }
        else if(c == 3)
        {
            quit();
        }
        else{
            continue;
        }
    }
}


void quit()
{
    char quitbuf[1024];                         //接受退出时服务器的返回结果
    send(conn_fd, "q", 1024, 0);
    recv(conn_fd, quitbuf, 1024, 0);
    if(strcmp(quitbuf, "y") == 0)
    {
        close(conn_fd);
        system("clear");
        exit(0);
    }
    else{
        printf("quit error!\n");
    }
}

int register_num()
{
    int         i;
    char        register_buf[1024] = "0";
    struct info user_temp;                      //暂存当前申请的用户信息
    char        key[10], key1[10];
    struct log
    {
        char    flag;
        char    name[10];
        char    passwd[10];
    }apply;

    memset(&apply, 0, sizeof(apply));
    printf("请输入用户名：");
    cls();                                      //清除输入缓存
    scanf("%s", apply.name);
    while(1)
    {
        printf("请输入密码：");
        inputkey(key);
        printf("请再次输入密码：");
        inputkey(key1);
        if(strcmp(key, key1) != 0)
        {
            printf("两次输入密码不相同，按任意键返回重新输入！\n");
            getchar();
        }
        else 
        {
            strcpy(apply.passwd, key);
            break;
        }
    }

    apply.flag = 'a';
    memcpy(register_buf, &apply, sizeof(apply));
    send(conn_fd, register_buf, 1024, 0);

    memset(register_buf, 0, sizeof(register_buf));
    recv(conn_fd, register_buf, 1024, 0);
    if(strcmp(register_buf, "n") == 0)
    {
        printf("\n 用户名已被使用!\n");
        getchar();
        return (0);
    }
    memcpy(&user_temp, register_buf, sizeof(register_buf));
    printf("user:%s\n", user_temp.username);
    strcpy(password, apply.passwd);
    user = user_temp;

    return 1;
}

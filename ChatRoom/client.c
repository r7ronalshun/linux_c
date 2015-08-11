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
int register_num();                     //注册函数
int login();                            //登陆函数
void print_login_menu();                //登陆后菜单

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

struct log                              //用户登陆注册、退出时使用
{
    char    flag;
    char    name[10];
    char    pwd[10];
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

    print_menu();                                                           //打印主菜单
    print_login_menu();
    printf("任意键退出！\n");
    getchar();

}


void print_menu(void)
{
    int   ret;
    int   c;                                         //接收选项
    while(1)
    {
        //system("clear");
        printf("\n");
        printf("\n\n\n");
        printf("\t\t            1.注册\n\n");
        printf("\t\t            2.登陆\n\n");
        printf("\t\t            3.退出\n\n");
        printf("\t\t----------------------------\n\n");
        printf("\t\t         请选择<1 ~ 3>：");
        scanf("%d", &c);
        
        if(c == 1)
        {
            if(register_num())                  //注册返回值1,注册成功
            {
                break;
            }
        }
        else if(c == 2)
        {
            ret = login();//登陆函数
            if(ret == 1)
            {
                break;  
            }
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
    struct log log;
    int ret;
    char quitbuf[1024];                         //接受退出时服务器的返回结果
    memset(&log, 0, sizeof(struct log));
    log.flag = 'q';
    memcpy(quitbuf, &log, sizeof(struct log));
    ret = send(conn_fd, quitbuf, 1024, 0);
    if(ret != 1024)
    {
        perror("send error");
        exit(1);
    }
    ret = recv(conn_fd, quitbuf, 1024, 0);
    if(ret < 0)
    {
        printf("recv failed\n");
        exit(0);
    }
    memset(&log, 0, sizeof(struct log));
    memcpy(&log, quitbuf, sizeof(struct log));
    if( log.flag == 'y')
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
    struct log  apply;

    memset(&apply, 0, sizeof(apply));
    printf("请输入用户名：");
    scanf("%s", apply.name);
    while(1)
    {
        printf("\n\t请输入密码：");
        inputkey(key);
        printf("\n\t请再次输入密码：");
        inputkey(key1);
        if(strcmp(key, key1) != 0)
        {
            printf("两次输入密码不相同，按任意键返回重新输入！\n");
            getchar();
        }
        else 
        {
            strcpy(apply.pwd, key);
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
    printf("\n申请成功用户名:%s\n", user_temp.username);
    strcpy(password, apply.pwd);
    user = user_temp;
    return 1;
}

int login()
{
    struct log log;
    char       login_buf[1024];
    int        ret;

    memset(&log, 0, sizeof(struct log));

    while(1)
    {
        printf("请输入用户名：");
        scanf("%s", log.name);
        if(strcmp(log.name, "") != 0)
        {
            break;
        }
    }
    while(1)
    {
        printf("请输入密码：");
        inputkey(log.pwd);
        if(strcmp(log.pwd, "") != 0)
        {
            break;
        }
    }
    log.flag = 'l';
    memcpy(login_buf, &log, sizeof(struct log));
    ret = send(conn_fd, login_buf, 1024, 0);

    if(ret < 0)
    {
        perror("send error");
        exit(0);
    }
    memset(login_buf, 0, 1024);
    ret = recv(conn_fd, login_buf, 1024, 0);
    if(ret < 0)
    {
        perror("recv");
        exit(0);
    }
    if(strcmp(login_buf, log.name) == 0)
    {
        printf("\n登陆成功！\n");
        return 1;
    }
    else
    {
        printf("\n登陆失败！\n");
        return 0;
    }

}

void print_login_menu()
{
    while(1)
    {
        int             ret;
        struct chat     login_user;
        char            login_user_buf[1024];
        int             select;
        
        printf("\n");
        printf("---------------------\n");
        printf("       1.好友管理\n");
        printf("       2.查找好友\n");
        printf("       3.消息管理\n");
        printf("       0.退出\n");
        printf("---------------------\n");
        printf("       请选择：");
    }
}

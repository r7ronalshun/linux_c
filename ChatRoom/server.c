/*************************************************************************
	> File Name: server.c
	> Author: zhuxinquan
	> Mail: zhuxinquan61@gmail.com
	> Created Time: 2015年08月08日 星期六 09时34分39秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<errno.h>
#include<pthread.h>

#define SERV_PORT       4507                        //服务器端的端口
#define LISTENQ         20                          //连接请求队列的最大长度
#define BUFSIZE         1024                        //缓冲区大小       

struct conn                                         //每个客户端有一个连接套接字
{
    int     fd;                                     //连接套接字
    char    name[10];                               //连接的客户端用户名
}conn[20];                                          //直接定义最大请求连接队列数个结构体，保证每个客户端使用不同的连接套接字

struct info                                         //个人信息结构体
{
    char username[10];                              //用户名
};

struct group                                        //群组结构体
{
    char        group_name[10];                     //群名称
    char        admin_name[10];                     //群管理员
    int         member_num;                         //群成员数量
    struct info member[10];                         //群成员
};

struct users 
{
    char        password[10];                       //用户密码
    int         friends_num;                        //好友数
    int         group_num;                          //群组数
    struct info user;                               //用户个人信息
    struct info friend[10];                         //好友信息
    struct group group[5];                          //群组
    struct users * next;
};

struct chat
{
    int         mark;                               //消息标志
    char        from[10];                           //消息来自
    char        to[10];                             //消息发往
    char        ask[10];
    char        time[30];                           //时间
    char        news[500];                          //聊天信息
};

struct log
{
    char        flag;                               //登陆和注册的标志
    char        name[10];
    char        pwd[10];
};

void my_err(const char *, int);                     //错误处理函数
void quit(void *);                                  //服务器退出函数
void * client (void *);                             //单独的处理客户端处理线程
void save();                                        //用户数据保存
struct users * readuser();                          //用户数据读取
struct users * apply(struct log, int);              //申请账号

pthread_mutex_t         mutex;                      //锁
struct users * head = NULL;                         //用户数据链表的头指针
struct users * current_user;                        //指向当前用户的指针

int main(void)
{
    int                 i;
    int                 sock_fd, conn_fd;           //TCP套接字， 连接套接字
    int                 optval;                     //保存待设定的套接字选项的值
    socklen_t           cli_len;                    //保存struct sockaddr_in 的字节长度
    struct sockaddr_in  cli_addr, serv_addr;        //客户端和服务器端的地址结构
    pthread_t           thid, quit_thid;            //线程ID，quit_thid用来服务器退出

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);      //创建一个TCP套接字
    if(sock_fd < 0)
    {
        my_err("socket", __LINE__);
        exit(1);
    }

    optval = 1;                                     //设置该套接字使之可以重新绑定端口，optval中存有待设定的值
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (void *)&optval, sizeof(int)) < 0)
    {
        my_err("setsockopt", __LINE__);
        exit(1);
    }

    memset(&serv_addr, 0, sizeof(struct sockaddr_in));      //初始化服务器端口地址
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port =htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if(bind(sock_fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in)) < 0)    //将套接字绑定到本地端口
    {
        my_err("bind", __LINE__);
        exit(1);
    }

    if(listen(sock_fd, LISTENQ) < 0)                //将套接字转化为监听字
    {
        my_err("listen", __LINE__);
        exit(1);
    }
    
    for(i = 0; i < LISTENQ; i++ )                   //初始化连接套接字
    {
        conn[i].fd = -1;                            //将所有的客户端的套接字初始化为-1,表示未连接
        strcpy(conn[i].name, " ");
    }
    
    pthread_create(&quit_thid, NULL, (void *)quit, NULL);       //创建一个线程，用来服务器退出
    cli_len = sizeof(struct sockaddr_in);                       //客户端套接字长度
    while(1)
    {
        for(i = 0; i < LISTENQ; i++)                            //得到未使用的一个连接套接字
        {
            if(conn[i].fd == -1){                               //当前连接套接字未使用，找到未使用的则退出循环
                break;
            }
        }
        conn_fd = accept(sock_fd, (struct sockaddr *)&cli_addr, &cli_len);            //通过accept接受客户端连接请求，返回连接套接字
        if(conn_fd < 0)                                                             //出错处理
        {
            my_err("accept", __LINE__); 
            exit(1);
        }
        
        conn[i].fd = conn_fd;                                   //若该连接套接字可用，将其使用赋值给连接套接字队列中未被使用的一个
        printf("accrpt a new client, IP :%s\n", inet_ntoa(cli_addr.sin_addr));      //新客户端连接，服务器显示客户端连接ip
        
        pthread_create(&thid, NULL, (void *)client, (void *)&i);                     //创建一个新的线程处理客户端请求
    }
    return 0;
}

void * client (void * arg)
{
    int             i = *(int *)arg;
    char            flag[1024] ;
    char            recv_buf[1024];
    struct users*   p_user;
    int             ret;

    while(1)
    {
        struct log  log;
        memset(flag, 0, sizeof(flag));

        if((ret = recv(conn[i].fd, flag, 1024, 0)) < 0)
        {
            my_err("apply", __LINE__);
            pthread_exit(0);
        }
        
        pthread_mutex_lock(&mutex);         //加锁
        memcpy(&log, flag, 1024);
        
        head = readuser();

        if(log.flag == 'a')                 //注册请求
        {
            p_user = apply(log, i);
            pthread_mutex_unlock(&mutex);   //解锁
            if(p_user != NULL)
            {
                break;
            }
        }

        if(log.flag == 'b')                 //登陆请求
        {
            p_user = login(log, i);
            pthread_mutex_unlock(&mutex);
            if(p_user != NULL)
            {
                break;
            }
        }

        if(log.flag == 'q')
        {
            pthread_mutex_unlock(&mutex);
            ret = send(conn[i].fd, "y", 1024, 0);
            close(conn[i].fd);
            conn[i].fd = -1;
            pthread_exit(0);
        }
    }

    while(1)
    {
        struct chat         chat;
        memset(&chat, 0, sizeof(chat));
        memset(recv_buf, 0, sizeof(recv_buf));
        ret = recv(conn[i].fd, recv_buf, 1024, 0);
        memcpy(&chat, recv_buf, sizeof(recv_buf));
        switch(chat.mark)
        {
            
        }
    }
}

void quit(void *arg)                        //服务器退出函数
{
    char shutdown[10];                          //接受服务器退出时输入
    int  i;                                 //关闭所有的连接套接字
    while(1)
    {
        printf("输入exit退出！！\n");
        scanf("%s", shutdown);
        if(strcmp(shutdown, "exit") == 0)
        {
            for(i = 0; i < LISTENQ; i++)
            {
                if(conn[i].fd != -1) 
                {
                    close(conn[i].fd);
                    conn[i].fd = -1;
                    strcpy(conn[i].name, " ");
                }
            }
            exit(1);
        }
    }
}


void save()                                             //用户数据保存
{
    FILE *          fp;
    struct users*   p = head->next;
    if((fp = fopen("usersdata.db", "wb")) == NULL)
    {
        printf("Cann't open usersdata.db\n");
        exit(0);
    }
    while(p != NULL)
    {
        fwrite(p, sizeof(struct users), 1, fp);
        p = p->next;
    }
    fclose(fp);
}

struct users * readuser()
{
    FILE *          fp;
    struct users    *p1, *p2;
    
    if((fopen("usersdata.db", "rb")) == NULL)               //先判断文件是否存在，不存在则新建
    {
        fp = fopen("usersdata.db", "wb");                   //不存在，新建
        fclose(fp);
    }
    
    if((fp = fopen("usersdata.db", "rb")) == NULL)
    {
        printf("Cann't open usersdata.db\n");
        exit(0);
    }
    head = p1 = (struct users*)malloc(sizeof(struct users));
    p2 = (struct users *)malloc(sizeof(struct users));
    fread(p2, sizeof(struct users), 1, fp);
    while(!feof(fp))
    {
        p1->next = p2;
        p1 = p2;
        p2 = (struct users *)malloc(sizeof(struct users));
        fread(p2, sizeof(struct users), 1, fp);
    }
    p1->next = NULL;
    free(p2);
    fclose(fp);
    return head;
}

void my_err(const char * err_string, int line)      //自定义出错处理函数
{
    fprintf(stderr, "line: %d", line);
    perror(err_string);
}

struct users * apply(struct log log, int i)
{
    char            enroll_buf[1024];
    struct log      log;
    struct users
}

/*************************************************************************
	> File Name: select_example.c
	> Author: zhuxinquan
	> Mail: zhuxinquan61@gmail.com
	> Created Time: 2015年08月07日 星期五 19时40分42秒
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/ioctl.h>
#include<unistd.h>
#include<sys/time.h>
#include<time.h>


int main()
{
    char    buffer[128];                            //输入缓存
    int     result, nread;          
    fd_set  inputs, testfds;
    struct  timeval timeout;

    FD_ZERO(&inputs);                               //用select函数之前先把集合清零
    FD_SET(0, &inputs);                             //将要检测的句柄标准输入(0),加入到集合了里
    while(1)
    {
        testfds = inputs;
        timeout.tv_sec = 2;
        timeout.tv_usec = 50000;
        result = select(FD_SETSIZE, &testfds, (fd_set *)0, (fd_set *)0, &timeout);
        switch(result)
        {
        case 0:                                     //在阻塞时间内所有文件描述符都没有事件发生，函数select返回0
            printf("timeout\n");
            break;
        case -1:                                    //出错
            perror("select");
            exit(1);
        default:                                   //返回发生事件的文件描述符的个数
            if(FD_ISSET(0, &testfds))               //FD_ISSET函数用来判断描述符fd是否在给定的描述符集中   
            {
                ioctl(0, FIONREAD, &nread);         //ioctl函数用来得到缓冲区有多少字节待读取，并将字节数存入nread(包括回车)
                if(nread == 0)
                {
                    printf("keyboard done\n");
                    exit(0);
                }
                nread = read(0, buffer, nread);
                buffer[nread] = 0;
                printf("read %d from keyboard: %s\n", nread, buffer);
            }
            break;
        }
    }
    return 0;
}

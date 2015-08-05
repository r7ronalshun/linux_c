/*************************************************************************
	> File Name: select_test.c
	> Author: zhuxinquan
	> Mail: zhuxinquan61@gmail.com
	> Created Time: 2015年08月05日 星期三 20时55分26秒
 ************************************************************************/

#include<stdio.h>
#include<sys/time.h>
#include<sys/types.h>
#include<unistd.h>
#include<time.h>
#include<stdlib.h>

void display_time(const char *string)
{
    int seconds;

    seconds = time((time_t*)NULL);
    printf("%s,%d\n", string, seconds);
}

int main(void)
{
    fd_set          readfds;
    struct timeval  timeout;
    int             ret;

    FD_ZERO(&readfds);              //开始使用一个描述符集合前将其清空
    FD_SET(0, &readfds);            //监视文件描述符0(标准键盘输入)
    
    timeout.tv_sec = 10;            //设置阻塞时间为10秒
    timeout.tv_usec = 0;

    while(1)
    {
        display_time("before select");
        ret = select(1, &readfds, NULL, NULL, &timeout);
        display_time("after select");
        
        switch(ret){
            case 0:
                printf("No data in ten seconds. \n");
                exit(0);
                break;
            case -1:
                perror("select");
                exit(1);
                break;
            default :
            getchar();              //将数据读入，否则标准输入上将一直为读就绪
            printf("Data is available now.\n");
        }
    }
    return 0;
}

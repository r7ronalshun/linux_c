/*************************************************************************
	> File Name: my_ls.c
	> Author: 
	> Mail: 
	> Created Time: 2015年07月16日 星期四 10时52分28秒
 ************************************************************************/

#include<stdio.h>
#include<stdib.h>
#include<string.h>
#include<time.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/types.h>
#include<Linux/limits.h>
#include<dirent.h>
#include<grp.h>
#include<pwd.h>
#include<errno.h>


#define PARAM_NONE 0    //无参数
#define PARAM_A 1   //-a :显示所有文件
#define PARAM_L 2 //-l ：一行显示一个文件的详细信息
#define MAXROWLEN 80 //一行最多显示的字符数

int g_leave_len = MAXROWLEN;    //一行剩余长度，用于输出对齐
int g_maxlen;   //存放某目录下最长的文件名的长度

//错误打印函数
void my_err(const char *err_string, int line)
{
    fprintf(stderr, "line:%d ", line);
    perror(err_string);
    exit(1);
}

//获取文件属性
void display_attribute(struct stat buf, char * name)
{
    
}

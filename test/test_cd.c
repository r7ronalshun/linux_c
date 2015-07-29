/*************************************************************************
	> File Name: my_cdvc.c
	> Author: 
	> Mail: 
	> Created Time: 2015年07月19日 星期日 14时55分18秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<linux/limits.h>

//自定义错误处理函数
void my_err(const char * err_string, int line)
{
    fprintf(stderr, "line:%d", line);
    perror(err_string);
    exit(1);
}

int main(int argc, char **argv)
{
    char buf[PATH_MAX + 1];

    if (argc < 2)
    {
        printf("my_cd <target path>\n");
        exit(1);
    }
    getcwd(buf, 512);
    printf("%s\n", buf);
    getchar();
    if(chdir(argv[1]) < 0)
    {
        my_err("getcwd", __LINE__);
    }
    if(getcwd(buf, 512) < 0)
    {
        my_err("getcwd", __LINE__);
    }

    printf("%s\n", buf);
    return 0;
}

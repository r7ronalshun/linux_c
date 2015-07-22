/*************************************************************************
	> File Name: my_ls.c
	> Author: 
	> Mail: 
	> Created Time: 2015年07月16日 星期四 10时52分28秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/types.h>
#include<linux/limits.h>
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

void sort(char str[][PATH_MAX + 1], int left, int right)
{
    if(left >= right)   //如果左边索引位置大于右边的索引就代表已整理完成当前的一个组了
    {
        return ;
    }
    int i = left;
    int j = right;
    char key[PATH_MAX + 1];
    strcpy(key, str[left]);

    while(i < j)
    {
        while(i < j && (strcmp(key, str[j]) < 0))     //寻找条件为找到一个大于（小于）key的值
        {
            j--;
        }
        strcpy(str[i], str[j]);    //找到这样的值之后就把它赋值给前面的被拿走的i的值，如果一直循环至i == j,则本身赋值
        
        while(i < j && (strcmp(key, str[i]) > 0))     //在这一组向前寻找，如果遇到比key值大的停止循环
        {
            i++;
        }

        strcpy(str[j], str[i]);
    }
    
    strcpy(str[i], key);     //在当前组找完成一遍之后将中间数key回归
    
    //完成上面循环后，原组已经分为两组，使用递归将左边的组与右边的组进行排序
    
    sort(str, left, i - 1);
    
    sort(str, i + 1, right);//a[i]的位置已经是两组的分界线不必再次进行排序
}

//获取文件属性并打印
void display_attribute(struct stat buf, char * name)
{
    char buf_time[32];
    struct passwd *psd;     //从该结构体获取文件所有者的用户名
    struct group *grp;      //从该结构体获取文件所有者所属组的组名

    //获取并打印文件类型
    if (S_ISLNK(buf.st_mode))
    {
        printf("l");
    }
    else if (S_ISREG(buf.st_mode))
    {
        printf("-");
    }
    else if (S_ISDIR(buf.st_mode))
    {
        printf("d");
    }
    else if(S_ISCHR(buf.st_mode))
    {
        printf("c");
    }
    else if (S_ISBLK(buf.st_mode))
    {
        printf("b");
    }
    else if (S_ISFIFO(buf.st_mode))
    {
        printf("f");
    }
    else if(S_ISSOCK(buf.st_mode))
    {
        printf("s");
    }

    //获取并打印文件所有者的权限
    if(buf.st_mode & S_IRUSR)
    {
        printf("r");
    }
    else
    {
        printf("-");
    }
    if(buf.st_mode & S_IWUSR)
    {
        printf("w");
    }
    else
    {
        printf("-");
    }
    if(buf.st_mode & S_IXUSR)
    {
        printf("x");
    }
    else
    {
        printf("-");
    }

    //获取并打印与文件所有者同组的用户对该文件的操作权限
    if(buf.st_mode & S_IRGRP)
    {
        printf("r");
    }
    else
    {
        printf("-");
    }
    if(buf.st_mode & S_IWGRP)
    {
        printf("w");
    }
    else
    {
        printf("-");
    }
    if(buf.st_mode & S_IXGRP)
    {
        printf("x");
    }
    else
    {
        printf("-");
    }

    //获取并打印其他用户对该文件的操作权限
    if (buf.st_mode & S_IROTH)
    {
        printf("r");
    }
    else
    {
        printf("-");
    }
    if (buf.st_mode & S_IWOTH)
    {
        printf("w");
    }
    else
    {
        printf("-");
    }
    if (buf.st_mode & S_IXOTH)
    {
        printf("x");
    }
    else
    {
        printf("-");
    }

    printf("    ");

    //根据uid与gid获取文件所有者的用户名与组名
    psd = getpwuid(buf.st_uid);
    grp = getgrgid(buf.st_gid);
    printf("%4d ", (int)buf.st_nlink);   //打印文件的链接数
    printf("%-8s", psd->pw_name);
    printf("%-8s", grp->gr_name);

    printf("%6d", (int)buf.st_size);     //打印文件大小
    strcpy(buf_time, ctime(&buf.st_mtime));
    buf_time[strlen(buf_time) - 1] = '\0';  //取掉换行符
    printf("  %s", buf_time);   //打印文件的时间信息
}

//在没有使用-l选项时，打印一个文件名，打印时保证上下对齐
void display_single(char *name)
{
    int i,len;

    //如果本行不足以打印一个文件名则换行
    if(g_leave_len < g_maxlen)
    {
        printf("\n");
        g_leave_len = MAXROWLEN;
    }

    len = strlen(name);
    len = g_maxlen - len;
    printf("%-s", name);

    for (i = 0; i < len; i++)
    {
        printf(" ");
    }
    printf("  ");
    g_leave_len -= (g_maxlen + 2);  //2指示两空格
}

//根据命令行参数和完整路径名显示目标文件
//参数flag：命令行参数
//参数pathname：包含了文件名的路径名

void display(int flag, char *pathname)
{
    int i,j;
    struct stat buf;
    char name[NAME_MAX + 1];

    //从路径中解析出文件名
    for(i = 0, j = 0; i < strlen(pathname); i++)
    {
        if(pathname[i] == '/')
        {
            j = 0;
            continue;
        }
        name[j++] = pathname[i];
    }
    name[j] = '\0';

    //用lstat而不是stat以方便解析链接文件
    if(lstat(pathname, &buf) == -1)
    {
        my_err("stat", __LINE__);
    }

    switch(flag)
    {
        case PARAM_NONE:    //没有-l和-a选项
        if(name[0] != '.')
        {
            display_single(name);
        }
        break;
       
        case PARAM_A:   //-a:显示包括隐藏文件在内的所有文件
        display_single(name);
        break;
        
        case PARAM_L:   //-l:每个文件单独占一行
        if(name[0] != '.')
        {
            display_attribute(buf, name);
            printf(" %-s\n", name);
        }
        break;

        case PARAM_A + PARAM_L:     //同时有-a和-l选项的情况
        display_attribute(buf, name);
        printf(" %-s\n", name);
        break;

        defualt:
        break;
    }
}

void display_dir(int flag_param, char *path)
{
    DIR *dir;
    struct dirent *ptr;
    int count = 0;
    char filename[256][PATH_MAX+1],temp[PATH_MAX + 1];

    //获取该目录下文件总数和最长文件名
    dir = opendir(path);
    if (dir == NULL)
    {
        my_err("opendir", __LINE__);
    }
    while((ptr =readdir(dir)) != NULL)
    {
        if(g_maxlen < strlen(ptr->d_name))
        {
            g_maxlen = strlen(ptr->d_name);
        }
        count++;
    }
    closedir(dir);

    if(count > 256)
    {
        my_err("too many file under this dir", __LINE__);
    }

    int i, j, len = strlen(path);
    dir = opendir(path);    //获取该目录下所有的文件名
    for(i = 0; i < count; i++)
    {
        ptr = readdir(dir);
        if(ptr == NULL)
        {
            my_err("readdir", __LINE__);
        }
        strncpy(filename[i], path, len);
        filename[i][len] = '\0';
        strcat(filename[i], ptr->d_name);
        filename[i][len + strlen(ptr->d_name)] = '\0';
    }

    //使用冒泡法对文件名进行排序，排序后文件名按字母顺序存储与filename
    /*for(i = 0; i < count - 1; i++)
    {
        for(j = 0; j < count - 1 - i; j++)
        {
            if(strcmp(filename[j], filename[j+1]) > 0)
            {
                strcpy(temp, filename[j + 1]);
                temp[strlen(filename[j+1])] = '\0';
                strcpy(filename[j+1], filename[j]);
                filename[j+1][strlen(filename[j])] = '\0';
                strcpy(filename[j], temp);
                filename[j][strlen(temp)] = '\0';
            }
        }
    }*/
    sort(&filename[0], 0, count-1);
    
    
    for(i = 0; i < count; i++)
    {
        display(flag_param, filename[i]);
    }

    closedir(dir);

    //如果命令行没有-l选项，打印一个换行符          ？？？？
    if((flag_param & PARAM_L) == 0)
    {
        printf("\n");
    }
}

int main(int argc, char **argv)
{
    int i, j, k, num;
    //char path[PARAM_NONE + 1];
    char param[32];     //保存命令行参数，目标文件和目录名不在此例
    int flag_param = PARAM_NONE;    //参数种类，即是否有-l和-a选项
    struct stat buf;

    char *path = (char*)malloc(PARAM_NONE+1);
    //命令行参数的解析，分析-l,-a,-al,-la选项
    j = 0;
    num = 0;
    for(i = 1; i < argc; i++)
    {
        if(argv[i][0] == '-')
        {
            for(k = 1; k < strlen(argv[i]); k++, j++)
            {
                param[j] = argv[i][k];  //获取'-'后面的参数保存到数组param中
            }
            num++;      //保存'-'的个数
        }
    }

    //只支持a和l，如果含有其它选项就报错
    for(i = 0; i < j; i++)
    {
        if(param[i] == 'a')
        {
            flag_param |= PARAM_A;
            continue;
        }
        else if(param[i] == 'l')
        {
            flag_param |= PARAM_L;
            continue;
        }
        else
        {
            printf("my_ls: invalid opention -%c\n", param[i]);
            exit(1);
        }
    }
    param[j] = '\0';

    //如果没有输入文件名或目录，就显示当前目录
    if((num + 1) == argc)
    {
        strcpy(path, "./");
        path[2] = '\0';
        display_dir(flag_param, path);
        return 0;
    }

    i = 1;
    do
    {
        //如果不是目标文件名或目录，解析下一个命令行参数
        if(argv[i][0] == '-')
        {
            i++;
            printf("%d\n", i);
            continue;
        }
        else 
        {
            //printf("%d\n", i);
            //strcpy(path, argv[i]);
            strncpy(path, argv[i],strlen(argv[i])+1);
            //如果目标文件或目录不存在，报错并退出程序
            //printf("%d\n", i);
            //puts(path);
            if(stat(path, &buf) == -1)
            {
                my_err("stat", __LINE__);
            }
            if(S_ISDIR(buf.st_mode))    //argv[i]是一个目录
            {
                if(path[strlen(argv[i])-1] != '/')  //如果目录的最后一个字符不是'/',就加上'/'
                {
                    path[strlen(argv[i])] = '/';
                    path[strlen(argv[i]) + 1] = '\0';
                }
                else
                {
                    path[strlen(argv[i])] = '\0';
                }
                display_dir(flag_param, path);
                i++;
            }
            else    //argv[i]是一个文件
            {
                display(flag_param, path);
                i++;
            }
        }
    }while(i < argc);
    return 0;
}

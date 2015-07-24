/*************************************************************************
	> File Name: my_chmod.c
	> Author: 
	> Mail: 
	> Created Time: 2015年07月16日 星期四 11时35分54秒
 ************************************************************************/

//功能：改变文件的访问权限

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>

mode_t get_file_mode(const char *path)
{
    struct stat buffer;

    if(stat(path , &buffer) == -1)
    {
        printf("The function stat error\n");
        perror("stat error");
        return (mode_t)0;
    }

    return buffer.st_mode;
}

mode_t all_file_access(mode_t access , const char *argument)
{
    mode_t mode[3][3] = {{S_IRUSR , S_IWUSR , S_IXUSR} , 
    {S_IRGRP , S_IWGRP , S_IXGRP} , 
    {S_IROTH , S_IWOTH , S_IXOTH}};
    int user;
    int user_access;
    mode_t user_mode;

    switch(argument[0])
    {
    case 'U':
    case 'u':
        user = 0;
        break;

    case 'g':
    case 'G':
        user = 1;
        break;

    case 'o':
    case 'O':
        user = 2;
        break;

    default:
        printf("argument error !");
        break;
    }

    switch(argument[2])
    {
    case 'r':
    case 'R':
        user_access = 0;
        break;

    case 'w':
    case 'W':
        user_access = 1;
        break;

    case 'x':
    case 'X':
        user_access = 2;
    }

    user_mode = mode[user][user_access];

    if(argument[1] == '+')
        return access | user_mode;

    return access ^ user_mode;
}

int my_chmod_function(const char *path , const char *argument)
{
    mode_t mode;

    mode = all_file_access(get_file_mode(path) , argument);
    if(chmod(path , mode) == -1)
    {
        printf("Change file access failed !\n");
        perror("Change file access failed ");
        return -1;
    }

    return 0;
}

int main(int argc , char **argv)
{
    if(my_chmod_function(argv[2] , argv[1]) == -1)
    {
        printf("Access change failed !\n");
        perror("Access change failed !");

        return -1;
    }

    return 0;
}

/*************************************************************************
	> File Name: test_read_write.c
	> Author: zhuxinquan
	> Mail: zhuxinquan61@gmail.com
	> Created Time: 2015年07月25日 星期六 15时37分12秒
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<errno.h>
#include<sys/stat.h>
#include<fcntl.h>


struct student
{
    int num;
    char id[10];
};

int main(void)
{
    int fd, i;
    int count;

    //struct student stu[3] = {{1, "01"}, {2, "02"}, {3, "03"}};
    struct student stu[3] ;
    fd = open("example.txt", O_CREAT | O_RDWR , S_IRWXU);
    for(i = 0; i < 3; i++)
    {
        count = read(fd, &stu[i].num, sizeof(stu[i].num));
        printf("%d\n", stu[i].num);
        count = read(fd, stu[i].id, sizeof(stu[i].id));
        printf("%s\n", stu[i].id);
        /*count = write(fd, &stu[i].num, sizeof(stu[i].num));
        printf("%d\n", stu[i].num);
        count = write(fd, stu[i].id, sizeof(stu[i].id));
        printf("%s\n", stu[i].id);*/
    }
}

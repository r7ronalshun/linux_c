/*************************************************************************
	> File Name: qsort.c
	> Author: 
	> Mail: 
	> Created Time: 2015年07月22日 星期三 09时23分23秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>

void sort(char str[][10], int left, int right)
{
    if(left >= right)   //如果左边索引位置大于右边的索引就代表已整理完成当前的一个组了
    {
        return ;
    }
    int i = left;
    int j = right;
    char key[10];

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

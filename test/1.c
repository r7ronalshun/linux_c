/*************************************************************************
	> File Name: 1.c
	> Author: zhuxinquan
	> Mail: zhuxinquan61@gmail.com
	> Created Time: 2015年09月08日 星期二 19时55分29秒
 ************************************************************************/

#include<stdio.h>
void buble_sort(int a[], int n)
{
    int change;
    int w, i, j;
    for(i = 0; i < n-1; i++)
    {
        for(j = 0; j < n-i-1; j++)
        {
            if(a[j] < a[j+1])
            {
                w = a[j];
                a[j] = a[j+1];
                a[j+1] = w;
            }
        }
    }
}

int main(void)
{
    int a[3]={1, 2, 3};
    int i;
    buble_sort(a, 3);
    for(i = 0; i < 3; i++)
    {
        printf("%d  ", a[i]);
    }
}

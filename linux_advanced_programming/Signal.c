#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<signal.h>

void function1(int signum) 
{
    if(signum == SIGINT)
    {
        printf("uid = %d\n",getuid());
        signal(SIGINT, SIG_DFL);
    }
    else if(signum == SIGTERM)
    {
        printf("pid = %d\n",getpid());
        signal(SIGTERM, SIG_DFL);
    }
}/*
void function2(int signum)
{
	printf("pid = %d\n",getpid());
	signal(SIGTERM, SIG_DFL);
}
*/
int main(int args, char *argc[])
{
	signal(SIGINT, function1);
	signal(SIGTERM, function1);
	while(1) {}
	return 1;	
}

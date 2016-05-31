/*************************************************************************
> File Name: pageReplaceAlgorithm.c
> Author: zhuxinquan
> Mail: zhuxinquan61@xiyoulinux.org
> Created Time: 2016年05月10日 星期二 11时13分50秒
************************************************************************/

#include<stdio.h>
#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>

#define k 1024
#define length 256
#define vmsize 32
#define page 32
#define sizemin 1
#define assignmin 4
#define vmplus 34
#define TRUE 1
#define FALSE 0

FILE *f,*fdata;
int size,called, assigned,pagefault ;//0 or 1
int now,count,used,seed;
int address[length+1];
int pageno[length+1];
int table[page+1];
char alg[10];

/*int trunc(float f)
{
    int i;
    i=(int)abs(f);
    i-=2;
    while(i<f) i++;
    i--;
    if (f>0) return(i);
    else return(-i);
}*/
int getrandom(int *seed)
{
    int temp=*seed;
    *seed=(197*temp+109)%32767;
    if (*seed<0) *seed+=32767;
    return(temp);
}
void produce_addrstream()
/*create a address stream with size "length" in which 25% forward,
25% backward,50% in order*/
{
    int p,rnd;
    FILE *fdata;
    fdata=fopen("data","r");
    /* if "data" is producted
    then we do only read the file "data" and get address stream.
    produting the adress stream need very much time.*/
    if (fdata==NULL)
    {
        fclose(fdata);
        fdata=fopen("data","w");
        if (fdata==NULL) {printf("file data cann not be written. \n");exit(1);}
        seed=511;
        address[0]=16*k+getrandom(&seed)%513;
        for(p=1;p<length;p++)
        {
            printf("%5d",p);
            rnd=getrandom(&seed)%513;
            if (rnd<=128)
            address[p]=trunc((address[p-1]-1)*((getrandom(&seed)%513)/512.00));
            else
            if (rnd<=256)
            address[p]=address[p-1]+trunc((vmsize*k-address[p-1])*((getrandom(&seed)%513)/512.00));
            else
            address[p]=(address[p-1]+1)%32767;
        }
        fprintf(f,"the virtual address stream as follows:\n");
        for (p=0;p<length;p++)
        {
            fprintf(fdata,"%7d",address[p]);
            if ((p+1)%4==0) fprintf(f,"a[%3d]=%5d\n",p,address[p]);
            else fprintf(f,"a[%3d]=%5d",p,address[p]);
        }
        fclose(fdata);
    }
    else /*the file data is exist*/
    {
        for (p=0;p<length;p++)
        {
            fscanf(fdata,"%d",&address[p]);
            if((p+1)%4==0) fprintf(f," a[%3d]=%5d\n",p,address[p]);
            else fprintf(f,"a[%3d]=%5d",p,address[p]);
        }
        fclose(fdata);
    }
}/*produce_addrstream*/
void opt_replacement()
{
    int i,pl,replaced,late,pp;
    fprintf(f," BEGIN OF OPT.\n");
    do{
        /*calulate the page number for each address */
        for(i=0;i<length;i++)
        pageno[i]=((address[i]-1)/k)/size+1;
        fprintf(f,"PAGE NUMBER WITHSIZE %dK FOR EACH ADDRESSIS:\n\n",size);
        for(i=0;i<length;i++)
        {
            fprintf(f,"pageno[%3d]=%2d",i,pageno[i]);
            if((i+1)%4==0) fprintf(f,"\n");
        }
        fprintf(f,"\n\nvmsize=%dkpagesize=%dk\n",vmsize,size);
        fprintf(f,"------------------------------\n\n");
        fprintf(f,"page assigned pages_in total references \n");
        assigned=4;
        do{
            count=0;now=0;used=0;
            do{
                pagefault=TRUE;
                called=pageno[now];pl=1;now++;
                /*search in pagetable for the page being called*/
                while(pl<=used&&pagefault)
                if (table[pl]==called) pagefault=FALSE;
                else pl++;
                if (pagefault)
                if(used<assigned)
                {
                    used++;table[used]=called;/*fill a free page in */
                }
                else{
                    /*replacement using opt algorithm*/
                    late=0;replaced=0;pl=0;
                    do{pp=now;pl++;
                       while(table[pl]!=pageno[pp]&&pp<length) pp++;
                       if(pp>late) {replaced=pl;late=pp;}
                      }
                    while(pl!=used&&late<length);
                    table[replaced]=called;
                }
                if(!pagefault) count++;/* count for one page fault */
            }
            while (now!=length);
            fprintf(f,"%2d%f\n",assigned,(float)count/(float)length);
            if(size<=2) assigned+=2;
            else assigned++;
        }while(assigned<=vmsize/size);
        fprintf(f,"\n\n");
        size*=2;
    }while(size<=vmsize/4);
}
void lru_replacement()
{
    int i,pl;
    fprintf(f,"BEGIN OF LRU.\n");
    do{
        /*calculate the page number for each address*/
        for(i=0;i<length;i++)
        pageno[i]=((address[i]-1)/k)/size+1;
        fprintf(f,"\n\n vmsize=%d k pagesize=%d k\n",vmsize,size);
        fprintf(f,"-------- --------\n\n");
        fprintf(f,"page assigned pages_in total references\n");
        assigned=4;
        do{
            count=0;now=0;used=0;
            do{
                pagefault=TRUE;
                called=pageno[now];pl=1,now++;
                while(pl<=used)
                {
                    if(table[pl]==called)
                    {
                        pagefault=FALSE;
                        while(pl<used) table[pl++]++;
                        table[pl]=called;
                    }
                    pl++;
                }
                if(pagefault)
                if(used<assigned) table[++used]=called;
                else{
                    for(pl=1;pl<assigned;pl++)
                    table[pl]=table[pl+1];
                    table[used]=called;
                }
                if(!pagefault) count++;
            }while(now!=length);
            fprintf(f,"%2d%f\n",assigned,(float)count/(float)length);
            if(size<=2) assigned+=2;
            else assigned++;
        }while(assigned<=vmsize/size);
        fprintf(f,"\n\n");
        size*=2;
    }while(size<=vmsize/4);
}
int main()
{
    int start,end=0;
    f=fopen("output","w");
    produce_addrstream();
    fprintf(f,"\n\n");
    size=1;
    do{start=1;
        do{
            if(!start) printf("\nWRONG INPUT!\n");
            start=0;
            printf("\nINPUT ALGRITHM(opt or lru or end):");
            scanf("%s",alg);
        }while(strcmp(alg,"end")&&strcmp(alg,"opt")&&strcmp(alg,"lru"));
        if(!strcmp(alg,"opt")) opt_replacement();
        else if(!strcmp(alg,"lru")) lru_replacement();
        else if(!strcmp(alg,"end")) end=1;
        else{
           printf("WRONG INPUT! \nINPUT opt or lru,TRY AGAIN.\n");exit(1);
        }
        fprintf(f,"end the result for %s\n",alg);
        fprintf(f,"* * * * * * * * * * * *\n");
        fprintf(f,"\n\n");
    }while(!end);
    fclose(f);
}

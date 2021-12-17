// clock计时函数
// 来自 - Mooc - 数据结构（陈越）

#include <stdio.h>
#include <time.h>
int main(int argc, char* argv[])
{
    time_t start,stop;
    double duration;
    start=clock();
    for(int i=0;i<100000000;i++)
    {
        i++;
        i--;
    }
    stop=clock();
    duration=(double)(stop-start)/CLK_TCK;
    printf("duration = \t%.3fs\n",duration);
    printf("CLK_TCK  = \t%ds\n",CLK_TCK);
}